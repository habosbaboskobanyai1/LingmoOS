/* Copyright(c) 2017-2019 Alejandro Sirgo Rica & Contributors

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

#pragma once

#include <QObject>

struct TerminalApp {
    QString name;
    QString arg;
};

class TerminalLauncher : public QObject {
    Q_OBJECT
public:
    explicit TerminalLauncher(QObject *parent = nullptr);

    static bool launchDetached(const QString &command);
private:
    static TerminalApp getPreferedTerminal();
};
