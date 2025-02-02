/*
 * Peony-Qt
 *
 * Copyright (C) 2020, Tianjin LINGMO Information Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Authors: Yue Lan <lanyue@kylinos.cn>
 *
 */

#include "../explorer-application.h"

#include "main-window.h"
#include "operation-menu.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QWidgetAction>
#include <QStandardPaths>
#include <KWindowSystem>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>
#include <QVariant>
#include <QMessageBox>
#include <QInputDialog>

#include "global-settings.h"
#include "clipboard-utils.h"
#include "file-operation-utils.h"
#include "file-operation-manager.h"
#include "directory-view-widget.h"
#include "directory-view-container.h"
#include "file-meta-info.h"
#include "file-utils.h"

OperationMenu::OperationMenu(MainWindow *window, QWidget *parent) : QMenu(parent)
{
    m_window = window;

    connect(this, &QMenu::aboutToShow, this, &OperationMenu::updateMenu);

    //FIXME: implement all actions.

    auto editWidgetContainer = new QWidgetAction(this);
    auto editWidget = new OperationMenuEditWidget(window, this);
    m_edit_widget = editWidget;
    editWidgetContainer->setDefaultWidget(editWidget);
    addAction(editWidgetContainer);

    connect(m_edit_widget, &OperationMenuEditWidget::operationAccepted, this, &QMenu::hide);

    addSeparator();

    //addAction(tr("Conditional Filter"));
//    auto advanceSearch = addAction(tr("Advance Search"), this, [=]()
//    {
//       m_window->advanceSearch();
//    });

//    addSeparator();

    auto keepAllow = addAction(tr("Keep Allow"), this, [=](bool checked) {
        //use kf5 interface to fix set on top has no effect issue
        if (checked)
            KWindowSystem::setState(m_window->winId(), KWindowSystem::KeepAbove);
        else
            KWindowSystem::clearState(m_window->winId(), KWindowSystem::KeepAbove);
    });
    keepAllow->setCheckable(true);

    auto showHidden = addAction(tr("Show Hidden"), this, [=](bool checked) {
        //window set show hidden
        m_window->setShowHidden(checked);
    });
    m_show_hidden = showHidden;
    showHidden->setCheckable(true);


    m_showFileExtension = addAction(tr("Show File Extension"), this, [=](bool checked) {
        m_window->setShowFileExtensions(checked);
    });
    m_showFileExtension->setCheckable(true);

    auto forbidThumbnailing = addAction(tr("Forbid thumbnailing"), this, [=](bool checked) {
        //FIXME:
        Peony::GlobalSettings::getInstance()->setValue(FORBID_THUMBNAIL_IN_VIEW, checked);
        //m_window->refresh();
    });
    m_forbid_thumbnailing = forbidThumbnailing;
    forbidThumbnailing->setCheckable(true);
    forbidThumbnailing->setChecked(Peony::GlobalSettings::getInstance()->getValue(FORBID_THUMBNAIL_IN_VIEW).toBool());

    auto residentInBackend = addAction(tr("Resident in Backend"), this, [=](bool checked) {
        //FIXME:
        Peony::GlobalSettings::getInstance()->setValue(RESIDENT_IN_BACKEND, checked);
        qApp->setQuitOnLastWindowClosed(!checked);
    });
    m_resident_in_backend = residentInBackend;
    residentInBackend->setCheckable(true);
    residentInBackend->setChecked(Peony::GlobalSettings::getInstance()->getValue(RESIDENT_IN_BACKEND).toBool());

    auto allowFileOpParallel = addAction(tr("Parallel Operations"), this, [=](bool checked){
        Peony::FileOperationManager::getInstance()->setAllowParallel(checked);
    });
    allowFileOpParallel->setCheckable(true);
    allowFileOpParallel->setChecked(Peony::FileOperationManager::getInstance()->isAllowParallel());

    addAction(tr("Set samba password"), this, [=]() {
        QDBusInterface *interFace = new QDBusInterface("org.lingmo.samba.share.config",
                                                           "/org/lingmo/samba/share",
                                                           "org.lingmo.samba.share.config",
                                                           QDBusConnection::systemBus());
        QString username = g_get_user_name();
        int pid = getpid();
        int uid = getuid();
        QDBusReply<bool> initReply = interFace->call("init", username, pid, uid);
        if (initReply.isValid()) {
            if (initReply.value()) {
                 QDBusReply<bool> hasPasswdReply = interFace->call("hasPasswd");
                 if (hasPasswdReply.isValid()) {
                    if (hasPasswdReply.value()) {
                        auto result = QMessageBox::question(nullptr, tr("Tips"), tr("The user already has a samba password, do you need to reset the samba password?"),
                                                            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                        if (result == QMessageBox::Yes) {
                              goto setPasswd;
                        }
                    } else {
setPasswd:
                        bool ok = false;
                        QInputDialog dlg;
                        dlg.setLabelText(tr("Samba password:"));
                        dlg.setTextEchoMode(QLineEdit::Password);
                        dlg.setWindowTitle(tr("Samba set user password"));
                        dlg.setFixedSize(470,150);
                        ok = dlg.exec();
                        QString text = dlg.textValue();
                        if (ok && !text.isNull() && !text.isEmpty()) {
                            QDBusReply<bool> setPasswdReply = interFace->call("setPasswd", text);
                            if (setPasswdReply.isValid()) {
                                if (!setPasswdReply.value()) {
                                     QMessageBox::warning(nullptr, tr("Warning"), tr("Samba set password failed, Please re-enter!"));
                                }
                            } else {
                                qDebug() << "setPasswd call failed!";
                            }
                        }
                    }
                 } else {
                     qDebug() << "hasPasswd call failed!";
                 }
            } else {
                 QMessageBox::warning(nullptr, tr("Warning"), tr("Shared configuration service exception, please confirm if there is an ongoing shared configuration operation, or please reset the share!"), QMessageBox::Ok);
            }
        } else {
           qDebug() << "init call failed!";
        }
        interFace->call("finished");
    });

    //task#147390  设置是否新建窗口打开文件夹
    auto showFoldersInNewWindow = addAction(tr("Open each folder in a new window"), this, [=](bool checked) {
        Peony::GlobalSettings::getInstance()->setValue(SHOW_IN_NEW_WINDOW, checked);
    });
    showFoldersInNewWindow->setCheckable(true);
    showFoldersInNewWindow->setChecked(Peony::GlobalSettings::getInstance()->getValue(SHOW_IN_NEW_WINDOW).toBool());

    addSeparator();

    //comment icon to design request
    addAction(/*QIcon::fromTheme("gtk-help"),*/ tr("Help"), this, [=]() {
        PeonyApplication::help();
    });

    addAction(/*QIcon::fromTheme("gtk-about"),*/ tr("About"), this, [=]() {
        //PeonyApplication::about();
        AboutDialog dlg(m_window);
        dlg.setWindowModality(Qt::WindowModal);
        dlg.exec();
    });
}

void OperationMenu::updateMenu()
{
    if (Peony::GlobalSettings::getInstance()->getValue(USE_GLOBAL_DEFAULT_SORTING).toBool()) {
        m_show_hidden->setChecked(Peony::GlobalSettings::getInstance()->isExist(SHOW_HIDDEN_PREFERENCE)?
                                  Peony::GlobalSettings::getInstance()->getValue(SHOW_HIDDEN_PREFERENCE).toBool():
                                  false);
    } else {
        auto uri = m_window->getCurrentUri();
        auto metaInfo = Peony::FileMetaInfo::fromUri(uri);
        if (metaInfo) {
            bool checked = metaInfo->getMetaInfoVariant(SHOW_HIDDEN_PREFERENCE).isValid()? metaInfo->getMetaInfoVariant(SHOW_HIDDEN_PREFERENCE).toBool(): false;
            m_show_hidden->setChecked(checked);
        } else {
            m_show_hidden->setChecked(false);
        }
    }

    m_showFileExtension->setChecked(Peony::GlobalSettings::getInstance()->isExist(SHOW_FILE_EXTENSION)?
                              Peony::GlobalSettings::getInstance()->getValue(SHOW_FILE_EXTENSION).toBool():
                              true);

    m_forbid_thumbnailing->setChecked(Peony::GlobalSettings::getInstance()->isExist(FORBID_THUMBNAIL_IN_VIEW)?
                                      Peony::GlobalSettings::getInstance()->getValue(FORBID_THUMBNAIL_IN_VIEW).toBool():
                                      false);

    m_resident_in_backend->setChecked(Peony::GlobalSettings::getInstance()->isExist(RESIDENT_IN_BACKEND)?
                                      Peony::GlobalSettings::getInstance()->getValue(RESIDENT_IN_BACKEND).toBool():
                                      false);

    //get window current directory and selections, then update ohter actions.
    m_edit_widget->updateActions(m_window->getCurrentUri(), m_window->getCurrentSelections());
}

OperationMenuEditWidget::OperationMenuEditWidget(MainWindow *window, QWidget *parent) : QWidget(parent)
{
    auto vbox = new QVBoxLayout;
    setLayout(vbox);

    auto title = new QLabel(this);
    title->setText(tr("Edit"));
    title->setAlignment(Qt::AlignCenter);
    vbox->addWidget(title);

    auto hbox = new QHBoxLayout;
    auto copy = new QToolButton(this);
    m_copy = copy;
    copy->setFixedSize(QSize(40, 40));
    copy->setIcon(QIcon::fromTheme("edit-copy-symbolic"));
    copy->setIconSize(QSize(16, 16));
    copy->setAutoRaise(false);
    copy->setToolTip(tr("copy"));
    hbox->addWidget(copy);

    auto paste = new QToolButton(this);
    m_paste = paste;
    paste->setFixedSize(QSize(40, 40));
    paste->setIcon(QIcon::fromTheme("edit-paste-symbolic"));
    paste->setIconSize(QSize(16, 16));
    paste->setAutoRaise(false);
    paste->setToolTip(tr("paste"));
    hbox->addWidget(paste);

    auto cut = new QToolButton(this);
    m_cut = cut;
    cut->setFixedSize(QSize(40, 40));
    cut->setIcon(QIcon::fromTheme("edit-cut-symbolic"));
    cut->setIconSize(QSize(16, 16));
    cut->setAutoRaise(false);
    cut->setToolTip(tr("cut"));
    hbox->addWidget(cut);

    auto trash = new QToolButton(this);
    m_trash = trash;
    trash->setFixedSize(QSize(40, 40));
    trash->setIcon(QIcon::fromTheme("edit-delete-symbolic"));
    trash->setIconSize(QSize(16, 16));
    trash->setAutoRaise(false);
    trash->setToolTip(tr("trash"));
    hbox->addWidget(trash);

    vbox->addLayout(hbox);

    connect(m_copy, &QToolButton::clicked, this, [=]() {
        if (!window->getCurrentSelections().isEmpty()) {
//            if (window->getCurrentSelections().first().startsWith("trash://", Qt::CaseInsensitive)) {
//                return ;
//            }

            Peony::ClipboardUtils::setClipboardFiles(window->getCurrentSelections(), false);
            Q_EMIT operationAccepted();
        }
    });

    connect(m_cut, &QToolButton::clicked, this, [=]() {
        if (!window->getCurrentSelections().isEmpty()) {
//            if (window->getCurrentSelections().first().startsWith("trash://", Qt::CaseInsensitive)) {
//                return ;
//            }
            Peony::ClipboardUtils::setClipboardFiles(window->getCurrentSelections(), true);
            window->getCurrentPage()->getView()->repaintView();
            Q_EMIT operationAccepted();
        }
    });

    connect(m_paste, &QToolButton::clicked, this, [=]() {
        auto op = Peony::ClipboardUtils::pasteClipboardFiles(window->getCurrentUri());
        if (op) {
            connect(op, &Peony::FileOperation::operationFinished, window, [=](){
                auto opInfo = op->getOperationInfo();
                auto targetUirs = opInfo->dests();
                window->setCurrentSelectionUris(targetUirs);
            }, Qt::BlockingQueuedConnection);
        }
        Q_EMIT operationAccepted();
    });

    connect(m_trash, &QToolButton::clicked, this, [=]() {
        if (window->getCurrentUri() == "trash:///") {
            Peony::FileOperationUtils::executeRemoveActionWithDialog(window->getCurrentSelections());
        } else {
            Peony::FileOperationUtils::trash(window->getCurrentSelections(), true);
        }
        Q_EMIT operationAccepted();
    });

    copy->setProperty("useIconHighlightEffect", true);
    copy->setProperty("iconHighlightEffectMode", 1);
    copy->setProperty("fillIconSymbolicColor", true);
    paste->setProperty("useIconHighlightEffect", true);
    paste->setProperty("iconHighlightEffectMode", 1);
    paste->setProperty("fillIconSymbolicColor", true);
    cut->setProperty("useIconHighlightEffect", true);
    cut->setProperty("iconHighlightEffectMode", 1);
    cut->setProperty("fillIconSymbolicColor", true);
    trash->setProperty("useIconHighlightEffect", true);
    trash->setProperty("iconHighlightEffectMode", 1);
    trash->setProperty("fillIconSymbolicColor", true);
}

void OperationMenuEditWidget::updateActions(const QString &currentDirUri, const QStringList &selections)
{
    //FIXME:
    bool isSelectionEmpty = selections.isEmpty();
    QString desktopPath = "file://" +  QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString desktopUri = Peony::FileUtils::getEncodedUri(desktopPath);
    QString homeUri = "file://" +  QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    bool isDesktop = selections.contains(desktopUri);
    bool isHome = selections.contains(homeUri);
    bool isSearch = currentDirUri.startsWith("search://");
    bool isRecent = currentDirUri.startsWith("recent://");
    bool isTrash = currentDirUri.startsWith("trash://");
    bool isComputer = currentDirUri.startsWith("computer:///");
    bool isFileBox = currentDirUri == "filesafe:///";
    bool hasLongFileName = false;
    for (auto uri : selections) {
        if(Peony::FileUtils::isLongNameFileOfNotDel2Trash(uri)){/* 在家目录/下载/扩展目录下存放的长文件名文件使用永久删除,所以该菜单置灰，link bug#188864 */
            hasLongFileName = true;
            break;
        }
    }

    //fix bug#183268, not allow paste in mtp, gphoto2 path or can not write path
    bool isDirectoryCanWrite = true;
    auto info = Peony::FileInfo::fromUri(currentDirUri);
    if (!info->isEmptyInfo()) {
        isDirectoryCanWrite = info->canWrite();
    }
    //comment to fix bug#191108, huawei phone can paste file success
//    if (currentDirUri.startsWith("mtp://") || currentDirUri.startsWith("gphoto2://")){
//        isDirectoryCanWrite = false;
//    }

    m_copy->setEnabled(!isSelectionEmpty && !isSearch && !isRecent && !isTrash && !isComputer);
    m_cut->setEnabled(!isSelectionEmpty && !isDesktop && !isHome && !isSearch && !isRecent && !isTrash && !isComputer && isDirectoryCanWrite);
    m_trash->setEnabled(!isSelectionEmpty && !isDesktop && !isHome && !isSearch && !isComputer && isDirectoryCanWrite && !hasLongFileName);

    Peony::ClipboardUtils::getInstance()->updateClipboardManually();
    bool isClipboradHasFile = Peony::ClipboardUtils::isClipboardHasFiles();
    m_paste->setEnabled(isClipboradHasFile && !isSearch && !isRecent && !isTrash && !isComputer && !isFileBox && isDirectoryCanWrite);
}
