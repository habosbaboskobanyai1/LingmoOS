/*
 * Copyright (C) 2024, KylinSoft Co., Ltd.
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
 * Authors: youdiansaodongxi <guojiaqi@kylinos.cn>
 *
 */

#ifndef CALENDARITEM_H
#define CALENDARITEM_H

#include <QObject>
#include <QDate>

class CalendarItemPrivate;
class CalendarItem : public QObject
{
    Q_OBJECT
public:
    explicit CalendarItem(QDate date, QObject* parent = nullptr);
    QDate getItemDate();
    QString getItemLunar();

private:
    CalendarItemPrivate *d = nullptr;
};

#endif // CALENDARITEM_H
