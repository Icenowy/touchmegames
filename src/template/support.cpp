
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
 *	Foundation, Inc., 59 Temple Place, Suite 328, Boston, MA  02111-1287  USA
 */

// support functions for Template
// Functions which basically don't fit in
// events.cpp or interface.cpp

#include <template.h>
#include <sstream>

Template::Template() {
}

std::string Template::name() const {
    return "template";
}

std::string Template::imageName() const {
    return "template.gif";
}

// This function doesn't even do anything anymore...really
// The background should be a modified bg.png
GdkColor *Template::backgroundColor() {
    return &bgcolor;
}

void Template::startRound() {
//     std::stringstream sstr;
//     
//     if (round + 1 == 4) {
//         gameState = GAME_OVER;
//         return;
//     }
//     
//     if (++round < 3) {
//         maxColumns = 6;
//         maxRows = 6;
//         perXSector = 3;
//         perYSector = 2;
//         maxInt = 6;
//     }
//     
//     if (round == 1) {
//         generatePuzzle(10, 20, 20);
//         
//     }
//     else if (round == 2) {
//         generatePuzzle(10, 20, 24);
//     }
//     else {
//         return;
//     }
//     sstr << round;
//     gtk_label_set_text((GtkLabel *)roundLabel, sstr.str().c_str());
//     
//     gameState = RUNNING;
//     timeLeft = 600;
//     
//     roundScore = 0;
//     errors = 0;
//     currentInt = 0;
}


GtkWidget *Template::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    // Keep this color
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "template is a template which you\n"
            << "have to fill out the rest of the code.";
        
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int Template::gameType() const {
    return UNKNOWN_GAME;
}



char Template::roundOver() {
    if (round == 2) {
        Template *s(this);
        exitGame(NULL, (void **)&s);
        return 0;
    }
    return 0;
}
