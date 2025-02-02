/* Copyright(c) 2017-2019 Alejandro Sirgo Rica & Contributors
*
* This file is part of Lingmo-Screenshot.
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
*/

#include "flameshotdbusadapter.h"
#include "src/utils/confighandler.h"
#include "src/utils/screengrabber.h"
#include "src/core/controller.h"
#include "src/utils/screenshotsaver.h"
#include "src/utils/systemnotification.h"
#include <QBuffer>
#include <QGSettings/qgsettings.h>

FlameshotDBusAdapter::FlameshotDBusAdapter(QObject *parent) :
    QDBusAbstractAdaptor(parent)
{
    auto controller = Controller::getInstance();
    connect(controller, &Controller::captureFailed,
            this, &FlameshotDBusAdapter::captureFailed);
    connect(controller, &Controller::captureTaken,
            this, &FlameshotDBusAdapter::handleCaptureTaken);
    connect(controller, &Controller::captureExit,
            this, &FlameshotDBusAdapter::captureExit);
    connect(controller, &Controller::captureCopy,
            this, &FlameshotDBusAdapter::captureCopy);
}

FlameshotDBusAdapter::~FlameshotDBusAdapter()
{
}

void FlameshotDBusAdapter::graphicCapture(QString path, int delay, uint id)
{
    CaptureRequest req(CaptureRequest::GRAPHICAL_MODE, delay, path);
// if (toClipboard) {
// req.addTask(CaptureRequest::CLIPBOARD_SAVE_TASK);
// }
    req.setStaticID(id);
    Controller::getInstance()->requestCapture(req);
}

void FlameshotDBusAdapter::fullScreen(
    QString path, QString type, bool toClipboard, int delay, uint id)
{
    CaptureRequest req(CaptureRequest::FULLSCREEN_MODE, delay, path, type);
    if (toClipboard) {
        req.addTask(CaptureRequest::CLIPBOARD_SAVE_TASK);
    }
    if (!path.isEmpty()) {
        req.addTask(CaptureRequest::FILESYSTEM_SAVE_TASK);
    }
    req.setStaticID(id);
    Controller::getInstance()->requestCapture(req);
}

void FlameshotDBusAdapter::openLauncher()
{
    Controller::getInstance()->openLauncherWindow();
}

void FlameshotDBusAdapter::captureScreen(int number, QString path, QString type, bool toClipboard,
                                         int delay, uint id)
{
    CaptureRequest req(CaptureRequest::SCREEN_MODE, delay, path, type, number);
    if (toClipboard) {
        req.addTask(CaptureRequest::CLIPBOARD_SAVE_TASK);
    }
    if (!path.isEmpty()) {
        req.addTask(CaptureRequest::FILESYSTEM_SAVE_TASK);
    }
    req.setStaticID(id);
    Controller::getInstance()->requestCapture(req);
}

void FlameshotDBusAdapter::openConfig()
{
    Controller::getInstance()->openConfigWindow();
}

void FlameshotDBusAdapter::trayIconEnabled(bool enabled)
{
    auto controller = Controller::getInstance();
    if (enabled) {
        controller->enableTrayIcon();
    } else {
        controller->disableTrayIcon();
    }
}

void FlameshotDBusAdapter::autostartEnabled(bool enabled)
{
    ConfigHandler().setStartupLaunch(enabled);
    auto controller = Controller::getInstance();
    // Autostart is not saved in a .ini file, requires manual update
    controller->updateConfigComponents();
}

void FlameshotDBusAdapter::handleCaptureTaken(uint id, const QPixmap &p)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    p.save(&buffer, "PNG");
    emit captureTaken(id, byteArray);
}

QVariantMap FlameshotDBusAdapter::getSaveInfo()
{
    QVariantMap map;
    QGSettings *ScreenshotGsettings = new QGSettings("org.lingmo.screenshot");
    map.insert("screenshot-path",
               QVariant::fromValue(ScreenshotGsettings->get("screenshot-path").toString()));
    map.insert("screenshot-name",
               QVariant::fromValue(ScreenshotGsettings->get("screenshot-name").toString()));
    map.insert("screenshot-type",
               QVariant::fromValue(ScreenshotGsettings->get("screenshot-type").toString()));
    return map;
}
