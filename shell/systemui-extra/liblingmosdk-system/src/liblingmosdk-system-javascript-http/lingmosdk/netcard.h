/*
 * liblingmosdk-system's Library
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
 * Authors: Yunhe Liu <liuyunhe@kylinos.cn>
 *
 */

#ifndef NETCARD_H
#define NETCARD_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include "lingmosdkdbus.h"

class NetCard : public QWidget
{
    Q_OBJECT
public:
    explicit NetCard(QWidget *parent = nullptr);
    NetCard(QString urlPath);
    int dbusConnect;
    Kysdkdbus kdbus;
    QString ServerName ;
    QString ServerInterface;
    QString ServerPath;

    QJsonObject deal_netcard();
    QJsonObject deal_netcard_path();
    QJsonObject deal_netcard_path_para();
private:
    QStringList path_list;
    QString path;

public slots:
    QStringList getNetCardName();
    int getNetCardType(QString NetCardName);
    QStringList getNetCardProduct(QString NetCardName);

    int getNetCardUp(QString NetCardName);
    QStringList getNetCardUpcards();
    QString getNetCardPhymac(QString NetCardName);
    QString getNetCardPrivateIPv4(QString NetCardName);
    QStringList getNetCardIPv4(QString NetCardName);
    QString getNetCardPrivateIPv6(QString NetCardName);
//    int getNetCardType(QString NetCardName);
    QStringList getNetCardIPv6(QString NetCardName);

signals:

};

#endif // NETCARD_H
