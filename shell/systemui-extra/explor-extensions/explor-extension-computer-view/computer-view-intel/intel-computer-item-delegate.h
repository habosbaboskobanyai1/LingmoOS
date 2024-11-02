/*
 * Peony-Qt's Library
 *
 * Copyright (C) 2020, Tianjin LINGMO Information Technology Co., Ltd.
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

#ifndef INTEL_COMPUTERITEMDELEGATE_H
#define INTEL_COMPUTERITEMDELEGATE_H

#include <QStyledItemDelegate>

class QListView;
class AbstractComputerItem;

namespace Intel {

class ComputerItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ComputerItemDelegate(QObject *parent = nullptr);
    ~ComputerItemDelegate();

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void paintVolumeItem(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, AbstractComputerItem *item) const;
    void paintRemoteItem(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, AbstractComputerItem *item) const;
    void paintNetworkItem(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, AbstractComputerItem *item) const;

    void drawTab(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, AbstractComputerItem *item) const;
    void drawStyledItem(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QListView *m_styleIconView;
};

}

#endif // COMPUTERITEMDELEGATE_H
