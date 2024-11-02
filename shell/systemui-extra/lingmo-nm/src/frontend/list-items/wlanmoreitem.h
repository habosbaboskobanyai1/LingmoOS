/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin LINGMO Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#ifndef WLANMOREITEM_H
#define WLANMOREITEM_H

#include <QObject>
#include "listitem.h"

const QString WMI_OB_NAME = "WlanMoreItemObjName";

class WlanMoreItem : public ListItem
{
    Q_OBJECT

protected:
    void onRightButtonClicked();

public:
    WlanMoreItem(QWidget *parent = nullptr);
    ~WlanMoreItem();

    void onNetButtonClicked();
    void onMenuTriggered(QAction *action);
Q_SIGNALS:
    void hiddenWlanClicked();
};

#endif // WLANMOREITEM_H
