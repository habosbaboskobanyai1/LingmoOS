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

#include "content-preview-page.h"

#include <QVBoxLayout>
#include <QLabel>

ContentPreviewPage::ContentPreviewPage(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    m_label = new QLabel(tr("This page is a part of explor-qt extension"), this);
    layout->addWidget(m_label);
}

ContentPreviewPage::~ContentPreviewPage()
{

}

void ContentPreviewPage::startPreview()
{
    m_label->setText(m_current_uri);
}

void ContentPreviewPage::cancel()
{
    m_label->setText(tr("This page is a part of explor-qt extension"));
}
