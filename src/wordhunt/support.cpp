
/*	Copyright (C) 2005, 2006 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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

// support functions for WordHunt

#include <wordhunt.h>
#include <algorithm>
#include <sys/time.h>
#include <sstream>

WordHunt::WordHunt() {
    bgcolor.red = 0xFFFF;
    bgcolor.green = 0xFFFF;
    bgcolor.blue = 0xFFFF;
}

std::string WordHunt::name() const {
    return "wordhunt";
}

std::string WordHunt::imageName() const {
    return "wordhunt.gif";
}

GdkColor *WordHunt::backgroundColor() {
    return &bgcolor;
}

GtkWidget *WordHunt::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "WordHunt is like any typical word hunt game\n"
            << "you just need to search and find the words from a \n"
            << "list on the right.  The words can be placed horizontally\n"
            << "or vertically, forwards and backwards.  You are timed,\n"
            << "which will give you more points the faster you finish.\n"
            << "The round bonus is completely dependent on how fast you\n"
            << "find all of the words.";
    
    
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int WordHunt::gameType() const {
	return WORD_GAME;
}
