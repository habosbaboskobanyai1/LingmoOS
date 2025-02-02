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
#include "abstracttwopointtool.h"
#include <cmath>
namespace {

const double ADJ_UNIT = std::atan(1.0);
const int DIRS_NUMBER = 4;

enum UNIT {
    HORIZ_DIR = 0,
    DIAG1_DIR = 1,
    VERT_DIR = 2,
    DIAG2_DIR = 3
};

const double ADJ_DIAG_UNIT = 2 * ADJ_UNIT;
const int DIAG_DIRS_NUMBER = 2;

enum DIAG_UNIT {
    DIR1 = 0,
    DIR2 = 1
};

}

AbstractTwoPointTool::AbstractTwoPointTool(QObject *parent) :
    CaptureTool(parent), m_thickness(0), m_padding(0)
{

}

bool AbstractTwoPointTool::isValid() const {
    return (m_points.first != m_points.second);
}

bool AbstractTwoPointTool::closeOnButtonPressed() const {
    return false;
}

bool AbstractTwoPointTool::isSelectable() const {
    return true;
}

bool AbstractTwoPointTool::showMousePreview() const {
    return true;
}

void AbstractTwoPointTool::undo(QPixmap &pixmap) {
    QPainter p(&pixmap);
    p.drawPixmap(backupRect(pixmap.rect()).topLeft(), m_pixmapBackup);
}

void AbstractTwoPointTool::drawEnd(const QPoint &p) {
    Q_UNUSED(p);
}

void AbstractTwoPointTool::drawMove(const QPoint &p) {
    m_points.second = p;
}

void AbstractTwoPointTool::drawMoveWithAdjustment(const QPoint &p) {
    m_points.second = m_points.first + adjustedVector(p - m_points.first);
}

void AbstractTwoPointTool::colorChanged(const QColor &c) {
    m_color = c;
}

void AbstractTwoPointTool::thicknessChanged(const int th) {
    m_thickness = th;
}
void AbstractTwoPointTool::textthicknessChanged(const int th) {
    Q_UNUSED(th);
}

void AbstractTwoPointTool::updateBackup(const QPixmap &pixmap) 
{
    QRect r = QRect(backupRect(pixmap.rect()));
    m_pixmapBackup = pixmap.copy(QRect(r.x()*pixelRatio,
				    r.y()*pixelRatio,
				    r.width()+m_thickness + m_padding +2,
				    r.height()+m_thickness + m_padding +2));
}

QRect AbstractTwoPointTool::backupRect(const QRect &limits) const {
    QRect  r;
    if(m_points.first.x() < m_points.second.x()){
	if (m_points.first.y() < m_points.second.y()){
            r = QRect(m_points.first.x(),m_points.first.y(),
	             (m_points.second.x()-m_points.first.x())*pixelRatio,
		     (m_points.second.y()-m_points.first.y())*pixelRatio)
	            .normalized();
	}
	else
	{
           r = QRect(m_points.first.x(),m_points.second.y(),
                     (m_points.second.x()-m_points.first.x())*pixelRatio,
                     (m_points.first.y()-m_points.second.y())*pixelRatio)
                    .normalized();
	}
    }
    else
    {
	 if (m_points.first.y() < m_points.second.y()){
            r = QRect(m_points.second.x(),m_points.first.y(),
                     (m_points.first.x()-m_points.second.x())*pixelRatio,
                     (m_points.second.y()-m_points.first.y())*pixelRatio)
                    .normalized();
         }
	 else
	 {
            r = QRect(m_points.second.x(),m_points.second.y(),
                     (m_points.first.x()-m_points.second.x())*pixelRatio,
                     (m_points.first.y()-m_points.second.y())*pixelRatio)
                    .normalized();
	 }

    }
    const int val = m_thickness + m_padding;
    r += QMargins(val, val, val, val);
    return r.intersected(limits);
}

QPoint AbstractTwoPointTool::adjustedVector(QPoint v) const {
    if (m_supportsOrthogonalAdj && m_supportsDiagonalAdj) {
        int dir = ( static_cast<int>(round(atan2(-v.y(), v.x()) / ADJ_UNIT)) + DIRS_NUMBER ) % DIRS_NUMBER;
        if (dir == UNIT::HORIZ_DIR) {
            v.setY(0);
        } else if (dir == UNIT::VERT_DIR) {
            v.setX(0);
        } else if (dir == UNIT::DIAG1_DIR) {
            int newX = (v.x() - v.y()) / 2;
            int newY = -newX;
            v.setX(newX);
            v.setY(newY);
        } else {
            int newX = (v.x() + v.y()) / 2;
            int newY = newX;
            v.setX(newX);
            v.setY(newY);
        }
    } else if (m_supportsDiagonalAdj) {
        int dir = ( static_cast<int>(round((atan2(-v.y(), v.x()) - ADJ_DIAG_UNIT / 2) / ADJ_DIAG_UNIT))
                    + DIAG_DIRS_NUMBER ) % DIAG_DIRS_NUMBER;
        if (dir == DIAG_UNIT::DIR1) {
            int newX = (v.x() - v.y()) / 2;
            int newY = -newX;
            v.setX(newX);
            v.setY(newY);
        } else {
            int newX = (v.x() + v.y()) / 2;
            int newY = newX;
            v.setX(newX);
            v.setY(newY);
        }
    }
    return v;
}

void AbstractTwoPointTool::textChanged(const CaptureContext &context) {
    Q_UNUSED(context);
}
