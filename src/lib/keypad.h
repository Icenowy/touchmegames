
/*	Copyright (C) 2005 - 2008 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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

#ifndef KEYPAD_H
#define KEYPAD_H

#include <string>
#include <gtk/gtk.h>

class KEYPAD {
private:
    GtkWidget *dialog;
    GtkWidget *entry;
    
    
    void changeEntry(int keyval);
    static void buttonPressed(GtkWidget *widget, gpointer ptr);
    static gboolean keyPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static gboolean keyReleased(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
public:
    KEYPAD(std::string str = "");
    void init();
    std::string run();
};

#endif

