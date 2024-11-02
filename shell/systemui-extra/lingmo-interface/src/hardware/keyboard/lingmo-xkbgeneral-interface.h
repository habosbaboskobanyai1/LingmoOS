/*
 * Copyright (C) 2019 Tianjin LINGMO Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
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


#ifndef __LINGMOXKBGENERALINTERFACE_H__
#define __LINGMOXKBGENERALINTERFACE_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the DBus Communication
 */
bool InitDBusXkbgeneral(void);

/**
 * Delete the DBus Communication
 */
bool DeInitDBusXkbgeneral(void);

/**
 * Keep and manage separate group per window.
 * @param in_arg is the value you want to set.
 * @return true if setting successfully.
 */
bool lingmo_hardware_keyboard_set_groupperwindow(const bool in_arg);

/**
 * Get the value of key.
 * @return the value of key.
 */
bool lingmo_hardware_keyboard_get_groupperwindow();

#ifdef __cplusplus
}
#endif

#endif













