
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

// support functions for Consume21

#include <consume21.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <sys/time.h>


Consume21::Consume21() {
    bgcolor.red = 0x0000;
    bgcolor.green = 0x9e00;
    bgcolor.blue = 0x3700;
}

std::string Consume21::name() const {
    return "consume21";
}

std::string Consume21::imageName() const {
    return "consume21.gif";
}

GdkColor *Consume21::backgroundColor() {
    return &bgcolor;
}


void Consume21::startRound() {
    std::stringstream sstr;
    
    if (round + 1 == 4) {
        gameState = GAME_OVER;
        return;
    }
    
    totalMugImages = 0;
    createMug();
    for (int i = 0; i < 5; ++i) {
        columnMugs[i] = 0;
        columnTotal[i] = 0;
        gtk_label_set_text((GtkLabel *)columnTotalLabels[i], "0");
        for (int j = 0; j < 7; ++j) {
            columnMugValues[i][j] = 0;
        }
    }
    gtk_label_set_text((GtkLabel *)columnsTotal, "0");
    gameState = WAITING;
    timeLeft = 300;
    progress.percent(100);
    setTotalWin(0);
    
//     gtk_image_set_from_file((GtkImage *)spill, "/usr/local/share/touchmegames/consume21/images/spillup.png");
    reloadImage(spill, "spillup.png");
    gtk_widget_show(spill);
    
//     sstr << "/usr/local/share/touchmegames/consume21/images/round" << ++round << ".png";
    sstr << "round" << ++round << ".png";
//     gtk_image_set_from_file((GtkImage *)roundImage, sstr.str().c_str());
    reloadImage(roundImage, sstr.str().c_str());
    
    roundScore = 0;
}

void Consume21::getCurrentScore() {
    int values[9] = { 1000, 500, 400, 300, 250, 200, 150, 100, 50 };
    int total(0);
    for (int i = 0; i < 5; ++i) {
        total += columnTotal[i];
    }
    if (total >= 97) {
        currentScore += values[8 - (total - 97)];
    }
    else {
        std::cout << "Hey, you forgot to show the bad player message" << std::endl;
    }
}

int Consume21::getMultiplier() {
    int values[9] = { 1000, 500, 400, 300, 250, 200, 150, 100, 50 };
    int total(0);
    multiplier = -1;
    for (int i = 0; i < 5; ++i) {
        total += columnTotal[i];
    }
    
    if (total >= 97) {
        multiplier = values[8 - (total - 97)];
    }
    return multiplier;
}

GtkWidget *Consume21::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "consume21 is a game which you\n"
            << "have 5 columns in which to place mugs\n"
            << "into.  Each mug has a value on it and\n"
            << "you need to get up to 21 in each column\n"
            << "by adding the values of the mugs in them.\n"
            << "The score for each round is based off of\n"
            << "the total of the columns and the amount\n"
            << "of time left in the round.";
    
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int Consume21::gameType() const {
	return STRATEGY_GAME;
}



char Consume21::roundOver() {
    for (int i = 0; i < totalMugImages; ++i) {
        gtk_widget_destroy(mugImages[i]);
    }
    
}
