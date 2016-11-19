
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

// support functions for GORILLA_THRILLA

#include <gorillathrilla.h>
#include <sstream>

GORILLA_THRILLA::GORILLA_THRILLA() : round(1),
		totalScore(0), gameOver(false), dealCard(NULL) {
	cardMap = new std::map<GtkWidget *, int>;
	bgcolor.red = 0x9000;
	bgcolor.green = 0x9000;
	bgcolor.blue = 0x9000;
}

GORILLA_THRILLA::~GORILLA_THRILLA() {
	delete cardMap;
}

std::string GORILLA_THRILLA::name() const {
    return "gorillathrilla";
}

std::string GORILLA_THRILLA::imageName() const {
	return "gorillathrilla.gif";
}
GdkColor *GORILLA_THRILLA::backgroundColor() {
	return &bgcolor;
}

GtkWidget *GORILLA_THRILLA::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    
    std::stringstream sstr;
    GdkColor color;
    color.red = 0xffff;
    color.green = 0xffff;
    color.blue = 0xffff;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "GorillaThrilla consists of three rounds where\n"
            << "you have to clear the barrels to receive a\n"
            << "bonus of 10,000 points.  In order to clear\n"
            << "the barrels, you have to choose two barrels\n"
            << "which are either the same number, or consecutive\n"
            << "numbers.  The numbers do wrap around so that you\n"
            << "can select 1 and 13 to clear them.\n\n"
            << "If you select a 9-9 you will get 3 times the\n"
            << "score and if you select 1-2 you will get\n"
            << "5 times the score.";
    
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int GORILLA_THRILLA::gameType() const {
	return STRATEGY_GAME;
}
