/*
* Copyright (C) 2020 Tianjin LINGMO Information Technology Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
*
*/

#ifndef SETFONTCOLOR_H
#define SETFONTCOLOR_H

#include "custom_color_panel.h"

class SetFontColor : public CustomColorPanel
{
    Q_OBJECT

public:
    explicit SetFontColor(QWidget *parent = nullptr);
    ~SetFontColor();

    static QString LINGMO_BLUE;
    static QString LINGMO_RED;
    static QString LINGMO_GREEN;
    static QString LINGMO_ORANGE;
    static QString LINGMO_PURPLE;
    
private:
    void initSetup();
};

#endif // SETFONTCOLORE_H
