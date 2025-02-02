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

#ifndef STABLEMAINWINDOWFACTORY_H
#define STABLEMAINWINDOWFACTORY_H

#include <QObject>
#include "main-window-factory-iface.h"
#include "windows_global.h"
#include <QMainWindow>

/*!
 * \brief The MainWindowFactory class
 * \deprecated
 */
class DEVELCONTROL_EXPORT StableMainWindowFactory : public QObject, public Peony::MainWindowFactoryIface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID MainWindowFactoryIface_iid FILE "common.json")
    Q_INTERFACES(Peony::MainWindowFactoryIface)
public:
    const QString name()
    {
        return "lingmo3.0";
    }
    static Peony::MainWindowFactoryIface *getInstance();
    Peony::FMWindowIface *create(const QString &uri);
    Peony::FMWindowIface *create(const QStringList &uris);
    QWidget *createWindow(const QString &uri = nullptr);
    QWidget *createWindow(const QStringList &uris);
    QWidget *createWindow(const QString &uri, QStringList selectUris);

    explicit StableMainWindowFactory(QObject *parent = nullptr);
};

#endif // STABLEMAINWINDOWFACTORY_H
