/*
 * Peony-Qt's Library
 *
 * Copyright (C) 2019, Tianjin LINGMO Information Technology Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Authors: Yue Lan <lanyue@kylinos.cn>
 *
 */

#include "directory-view-extensions-test-plugin.h"
#include "directory-view-widget.h"

#include <QLabel>

#include <QVBoxLayout>

#include <QDebug>

using namespace Peony;

DirectoryViewExtensionsTestPlugin::DirectoryViewExtensionsTestPlugin(QObject *parent) : QObject(parent)
{

}

//void DirectoryViewExtensionsTestPlugin::fillDirectoryView(DirectoryViewWidget *view)
//{
//    auto layout = new QVBoxLayout(view);
//    layout->addWidget(new QLabel("test icon view", nullptr));
//    view->setLayout(layout);
//}

DirectoryViewWidget *DirectoryViewExtensionsTestPlugin::create()
{
    qDebug()<<"create directory view";
    auto w = new DirectoryViewWidget;
    auto layout = new QVBoxLayout(w);
    layout->addWidget(new QLabel("test directory view", w));
    w->setLayout(layout);

    return w;
}
