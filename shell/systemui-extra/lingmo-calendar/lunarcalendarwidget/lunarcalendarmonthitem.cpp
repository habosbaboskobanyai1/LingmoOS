﻿/*
 * Copyright (C) 2019 Tianjin LINGMO Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU  Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */
#pragma execution_character_set("utf-8")

#include "lunarcalendarmonthitem.h"
#include "calendarcolor.h"

#include "qpainter.h"
#include "qevent.h"
#include "qdatetime.h"
#include "qdebug.h"
#include <QGSettings>

LunarCalendarMonthItem::LunarCalendarMonthItem(QString fontName, QWidget *parent) : QWidget(parent),m_font(fontName)
{
    hover = false;
    pressed = false;
    select = false; 

    select = false;
    showLunar = true;
    bgImage = ":/image/bg_calendar.png";
    selectType = SelectType_Rect;

    date = QDate::currentDate();
    lunar = "初一";
    dayType = DayType_MonthCurrent;

    //实时监听主题变化
    const QByteArray id("org.lingmo.style");
    QGSettings * fontSetting = new QGSettings(id, QByteArray(), this);
    connect(fontSetting, &QGSettings::changed,[=](QString key) {
        if((key.compare("style-name") == 0) || (key.compare("theme-color") == 0)|| (key.compare("widget-theme-name")==0) ||
            (key.compare("styleName") == 0) || (key.compare("themeColor") == 0)|| (key.compare("widgetThemeName")==0) ){
            weekColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::TEXT);
            currentTextColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::TEXT);
            otherTextColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::OTHER_TEXT);
            otherLunarColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::OTHER_TEXT);
            currentLunarColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::TEXT);
            lunarColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::TEXT);
            selectBgColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::CLICKED);
            //hoverBgColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::CLICKED);
            qreal mix = 0.2;
            hoverBgColor = CalendarColor::CalendarColor::mixColor(CalendarColor::CalendarColor::getThemeColor(CalendarColor::CLICKED),mix);
            this->update();
        }
    });

    weekColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::TEXT);
    currentTextColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::TEXT);
    otherTextColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::OTHER_TEXT);
    otherLunarColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::OTHER_TEXT);
    currentLunarColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::TEXT);
    lunarColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::TEXT);

    borderColor = QColor(180, 180, 180);
    superColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::WORK);
    selectLunarColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::WORK);
    hoverLunarColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::WORK);
    selectBgColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::CLICKED);
    //hoverBgColor = CalendarColor::CalendarColor::getThemeColor(CalendarColor::CLICKED);
    qreal mix = 0.2;
    hoverBgColor = CalendarColor::CalendarColor::mixColor(CalendarColor::CalendarColor::getThemeColor(CalendarColor::CLICKED),mix);
    setProperty("useStyleWindowManager",false);
}

void LunarCalendarMonthItem::onEnter()
{
        hover = true;
        this->update();
}
void LunarCalendarMonthItem::onLeave()
{
        hover = false;
        this->update();
}
void LunarCalendarMonthItem::onSelected()
{
    Q_EMIT monthMessage(date, dayType);
    update();
}


void LunarCalendarMonthItem::paintEvent(QPaintEvent *)
{
    QDate dateNow = QDate::currentDate();
    //绘制准备工作,启用反锯齿
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    //绘制背景和边框
    drawBg(&painter);
    //对比当前的时间，画选中状态
    if(dateNow.month() == date.month() && dateNow.year() == date.year()) {
        drawBgCurrent(&painter, selectBgColor);
        if(hover) {
            drawBgHover(&painter, hoverBgColor);
        }
        currentTextColor =CalendarColor::CalendarColor::getThemeColor(CalendarColor::CLICKED_TEXT);
    } else {
        currentTextColor =CalendarColor::CalendarColor::getThemeColor(CalendarColor::TEXT);
    }
    //绘制悬停状态
    if (hover || select) {
        drawBgHover(&painter, hoverBgColor);
    }
    //绘制日期
    drawMonth(&painter);
}

void LunarCalendarMonthItem::drawBg(QPainter *painter)
{
    painter->save();

    //根据当前类型选择对应的颜色
    QColor bgColor = currentBgColor;
    if (dayType == DayType_MonthPre || dayType == DayType_MonthNext) {
        bgColor = otherBgColor;
    }
    painter->restore();
}

void LunarCalendarMonthItem::drawBgCurrent(QPainter *painter, const QColor &color)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(color);

    QRect rect = this->rect();
    rect = QRect(rect.x()+1,rect.y()+1,rect.width()-2,rect.height()-2);
    painter->drawRoundedRect(rect,6,6);

    painter->restore();
}

void LunarCalendarMonthItem::drawBgHover(QPainter *painter, const QColor &color)
{
    painter->save();
    QRect rect = this->rect();
    rect = QRect(rect.x()+1,rect.y()+1,rect.width()-2,rect.height()-2);
    painter->setPen(QPen(color,3));
    painter->drawRoundedRect(rect,6,6);
    painter->restore();
}

void LunarCalendarMonthItem::drawMonth(QPainter *painter)
{
    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);

    painter->save();

    //根据当前类型选择对应的颜色
    QColor color = currentTextColor;
    if (dayType == DayType_MonthPre || dayType == DayType_MonthNext) {
        color = otherTextColor;
    } else if (dayType == DayType_WeekEnd) {
        color = weekColor;
    }

    painter->setPen(color);

    QFont font = m_font;
    font.setPixelSize(side * 0.2);
    //设置文字粗细
    font.setBold(true);
    painter->setFont(font);

    QRect dayRect = QRect(0, 0, width, height / 1.7);
    QString arg = QString::number(date.month()) /*+ "月"*/;
    switch(date.month()){
    case 1:
        arg = tr("Janurary");
        break;
    case 2:
        arg = tr("Februray");
        break;
    case 3:
        arg = tr("March");
        break;
    case 4:
        arg = tr("April");
        break;
    case 5:
        arg = tr("May");
        break;
    case 6:
        arg = tr("June");
        break;
    case 7:
        arg = tr("July");
        break;
    case 8:
        arg = tr("August");
        break;
    case 9:
        arg = tr("September");
        break;
    case 10:
        arg = tr("October");
        break;
    case 11:
        arg = tr("November");
        break;
    case 12:
        arg = tr("December");
        break;
    default :
        break;
    }
    painter->drawText(dayRect, Qt::AlignHCenter | Qt::AlignBottom, arg);
    painter->restore();
}

bool LunarCalendarMonthItem::getSelect() const
{
    return this->select;
}

bool LunarCalendarMonthItem::getShowLunar() const
{
    return this->showLunar;
}

QString LunarCalendarMonthItem::getBgImage() const
{
    return this->bgImage;
}

LunarCalendarMonthItem::SelectType LunarCalendarMonthItem::getSelectType() const
{
    return this->selectType;
}

QDate LunarCalendarMonthItem::getDate() const
{
    return this->date;
}

QString LunarCalendarMonthItem::getLunar() const
{
    return this->lunar;
}

LunarCalendarMonthItem::DayType LunarCalendarMonthItem::getDayType() const
{
    return this->dayType;
}

QColor LunarCalendarMonthItem::getBorderColor() const
{
    return this->borderColor;
}

QColor LunarCalendarMonthItem::getWeekColor() const
{
    return this->weekColor;
}

QColor LunarCalendarMonthItem::getSuperColor() const
{
    return this->superColor;
}

QColor LunarCalendarMonthItem::getLunarColor() const
{
    return this->lunarColor;
}

QColor LunarCalendarMonthItem::getCurrentTextColor() const
{
    return this->currentTextColor;
}

QColor LunarCalendarMonthItem::getOtherTextColor() const
{
    return this->otherTextColor;
}

QColor LunarCalendarMonthItem::getSelectTextColor() const
{
    return this->selectTextColor;
}

QColor LunarCalendarMonthItem::getHoverTextColor() const
{
    return this->hoverTextColor;
}

QColor LunarCalendarMonthItem::getCurrentLunarColor() const
{
    return this->currentLunarColor;
}

QColor LunarCalendarMonthItem::getOtherLunarColor() const
{
    return this->otherLunarColor;
}

QColor LunarCalendarMonthItem::getSelectLunarColor() const
{
    return this->selectLunarColor;
}

QColor LunarCalendarMonthItem::getHoverLunarColor() const
{
    return this->hoverLunarColor;
}

QColor LunarCalendarMonthItem::getCurrentBgColor() const
{
    return this->currentBgColor;
}

QColor LunarCalendarMonthItem::getOtherBgColor() const
{
    return this->otherBgColor;
}

QColor LunarCalendarMonthItem::getSelectBgColor() const
{
    return this->selectBgColor;
}

QColor LunarCalendarMonthItem::getHoverBgColor() const
{
    return this->hoverBgColor;
}

QSize LunarCalendarMonthItem::sizeHint() const
{
    return QSize(100, 100);
}

QSize LunarCalendarMonthItem::minimumSizeHint() const
{
    return QSize(20, 20);
}

void LunarCalendarMonthItem::setSelect(bool select)
{
    if (this->select != select) {
        this->select = select;
        this->update();
    }
}

void LunarCalendarMonthItem::setShowLunar(bool showLunar)
{
        this->showLunar = showLunar;
        this->update();
}

void LunarCalendarMonthItem::setBgImage(const QString &bgImage)
{
    if (this->bgImage != bgImage) {
        this->bgImage = bgImage;
        this->update();
    }
}

void LunarCalendarMonthItem::setSelectType(const LunarCalendarMonthItem::SelectType &selectType)
{
    if (this->selectType != selectType) {
        this->selectType = selectType;
        this->update();
    }
}

void LunarCalendarMonthItem::setDate(const QDate &date)
{
    if (this->date != date) {
        this->date = date;
        this->update();
    }
}

void LunarCalendarMonthItem::setLunar(const QString &lunar)
{
    if (this->lunar != lunar) {
        this->lunar = lunar;
        this->update();
    }
}

void LunarCalendarMonthItem::setDayType(const LunarCalendarMonthItem::DayType &dayType)
{
    if (this->dayType != dayType) {
        this->dayType = dayType;
        this->update();
    }
}

void LunarCalendarMonthItem::setDate(const QDate &date, const QString &lunar, const DayType &dayType)
{
    this->date = date;
    this->lunar = lunar;
    this->dayType = dayType;
    this->update();
}

void LunarCalendarMonthItem::setBorderColor(const QColor &borderColor)
{
    if (this->borderColor != borderColor) {
        this->borderColor = borderColor;
        this->update();
    }
}

void LunarCalendarMonthItem::setWeekColor(const QColor &weekColor)
{
    if (this->weekColor != weekColor) {
        this->weekColor = weekColor;
        this->update();
    }
}

void LunarCalendarMonthItem::setSuperColor(const QColor &superColor)
{
    if (this->superColor != superColor) {
        this->superColor = superColor;
        this->update();
    }
}

void LunarCalendarMonthItem::setLunarColor(const QColor &lunarColor)
{
    if (this->lunarColor != lunarColor) {
        this->lunarColor = lunarColor;
        this->update();
    }
}

void LunarCalendarMonthItem::setCurrentTextColor(const QColor &currentTextColor)
{
    if (this->currentTextColor != currentTextColor) {
        this->currentTextColor = currentTextColor;
        this->update();
    }
}

void LunarCalendarMonthItem::setOtherTextColor(const QColor &otherTextColor)
{
    if (this->otherTextColor != otherTextColor) {
        this->otherTextColor = otherTextColor;
        this->update();
    }
}

void LunarCalendarMonthItem::setSelectTextColor(const QColor &selectTextColor)
{
    if (this->selectTextColor != selectTextColor) {
        this->selectTextColor = selectTextColor;
        this->update();
    }
}

void LunarCalendarMonthItem::setHoverTextColor(const QColor &hoverTextColor)
{
    if (this->hoverTextColor != hoverTextColor) {
        this->hoverTextColor = hoverTextColor;
        this->update();
    }
}

void LunarCalendarMonthItem::setCurrentLunarColor(const QColor &currentLunarColor)
{
    if (this->currentLunarColor != currentLunarColor) {
        this->currentLunarColor = currentLunarColor;
        this->update();
    }
}

void LunarCalendarMonthItem::setOtherLunarColor(const QColor &otherLunarColor)
{
    if (this->otherLunarColor != otherLunarColor) {
        this->otherLunarColor = otherLunarColor;
        this->update();
    }
}



