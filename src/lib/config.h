/*	Copyright (C) 2008 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
 *
 *	This file is part of TouchMe Games.
 *
 *	TouchMe Games is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	TouchMe Games is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with TouchMe Games; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef CONFIG_H
#define CONFIG_H

// If you want to install in a different directory, change
// this to the directory to install...Also change the same
// named variable in the main Makefile
#ifdef __unix__
#define INSTALL_DIR "/usr/local/share/touchmegames/"
#else
#define INSTALL_DIR "c:/program files/touchmegames/"
#endif

// If you want to compile for fullscreen, set this to true
#define FULL_SCREEN false

#define HEIGHT 800
#define WIDTH 600

// Do not modify this string
#define KEY_STR "TMG"

#endif // CONFIG_H
