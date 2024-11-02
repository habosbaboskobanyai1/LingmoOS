/*
 * Qt5-LINGMO's Library
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
 * Authors: xibowen <lanyue@kylinos.cn>
 *
 */


#include "config-checkbox-parameters.h"
using namespace LINGMOConfigStyleSpace;

ConfigCheckBoxParameters::ConfigCheckBoxParameters()
{
    radius = 0;

    checkBoxDefaultBrush = QBrush(Qt::NoBrush);
    checkBoxHoverBrush = QBrush(Qt::NoBrush);
    checkBoxClickBrush = QBrush(Qt::NoBrush);
    checkBoxDisableBrush = QBrush(Qt::NoBrush);
    checkBoxOnDefaultBrush = QBrush(Qt::NoBrush);
    checkBoxOnHoverBrush = QBrush(Qt::NoBrush);
    checkBoxOnClickBrush = QBrush(Qt::NoBrush);
    checkBoxPathBrush = QBrush(Qt::NoBrush);
    checkBoxPathDisableBrush = QBrush(Qt::NoBrush);

    checkBoxDefaultPen = QPen(Qt::NoPen);
    checkBoxHoverPen = QPen(Qt::NoPen);
    checkBoxClickPen = QPen(Qt::NoPen);
    checkBoxDisablePen = QPen(Qt::NoPen);
    checkBoxOnDefaultPen = QPen(Qt::NoPen);
    checkBoxOnHoverPen = QPen(Qt::NoPen);
    checkBoxOnClickPen = QPen(Qt::NoPen);
    checkBoxDefaultPen.setWidth(0);
    checkBoxHoverPen.setWidth(0);
    checkBoxClickPen.setWidth(0);
    checkBoxDisablePen.setWidth(0);
    checkBoxOnDefaultPen.setWidth(0);
    checkBoxOnHoverPen.setWidth(0);
    checkBoxOnClickPen.setWidth(0);
}
