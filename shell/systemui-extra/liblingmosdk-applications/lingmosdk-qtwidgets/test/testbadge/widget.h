/*
 * 
 *
 * Copyright (C) 2023, KylinSoft Co., Ltd.
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
 * Authors: Zhen Sun <sunzhen1@kylinos.cn>
 *
 */

#ifndef WIDGET_H
#define WIDGET_H

#include "kwidget.h"
#include "kbadge.h"
#include "kpixmapcontainer.h"

using namespace kdk;

class Widget : public KWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    KPixmapContainer *m_pContainer1;
    KPixmapContainer *m_pContainer2;
    KPixmapContainer *m_pContainer3;
    KPixmapContainer *m_pContainer4;
    KBadge *m_pBadge1;
    KBadge *m_pBadge2;
    KBadge *m_pBadge3;
    KBadge *m_pBadge4;
};
#endif // WIDGET_H
