
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

// support functions for Memory
// Functions which basically don't fit in
// events.cpp or interface.cpp

#include <memory.h>
#include <algorithm>
#include <sstream>

Memory::Memory() : cardHeight(90), cardWidth(90), xpadding(35), ypadding(145), columns(5), rows(4), totalMatches(columns * rows / 2) {
}

std::string Memory::name() const {
    return "memory";
}

std::string Memory::imageName() const {
    return "memory.png";
}

// This function doesn't even do anything anymore...really
// The background should be a modified bg.png
GdkColor *Memory::backgroundColor() {
    return &bgcolor;
}

void Memory::startRound() {
    std::stringstream sstr;
    int numbers[columns * rows];
    
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < (columns * rows) / 2; ++j) {
            numbers[i * ((columns * rows) / 2) + j] = j;
        }
    }
    
    for (int i = 0; i < 100; ++i) {
        std::swap(numbers[getRand(columns * rows)], numbers[getRand(columns * rows)]);
    }
    
    
    for (int j = 0; j < rows; ++j) {
        for (int i = 0; i < columns; ++i) {
            g_object_set_data((GObject *)images[i][j]->parent, KEY_STR, (void *)numbers[i + j * columns]);
            reloadImage(images[i][j], "back.png");
        }
    }
    
    gtk_label_set_text((GtkLabel *)matchScoreLabel, "0");
    gtk_label_set_text((GtkLabel *)attemptScoreLabel, "0");
    gtk_label_set_text((GtkLabel *)timeScoreLabel, "0");
    gtk_label_set_text((GtkLabel *)totalRoundLabel, "0");
    
    attempts = 0;
    

    if (round + 1 == 4) {
        gameState = GAME_OVER;
        return;
    }
    ++round;

    sstr << round;
    gtk_label_set_text((GtkLabel *)roundLabel, sstr.str().c_str());
     
    gtk_label_set_text((GtkLabel *)attemptLabel, "0");
    gameState = RUNNING;
    
    cardsPressed[0] = 0;
    cardsPressed[1] = 0;
    
    timeLeft = 600;
    progress.percent((int)timeLeft / 6);
    

    progress.text("2:00");
    
    
    matches = 0;
//     
    roundScore = 0;
    timeBonus = 0;
    started = false;
    matchScore = 0;
//     errors = 0;
//     currentInt = 0;
}


GtkWidget *Memory::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    // Keep this color
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "memory is a game in which you have to \n"
            << "select pairs of cardsthe cards \n"
            << "start turned over and you can only\n"
            << "look at two at a time.  Points are\n"
            << "awarded when a pair is flipped over.\n"
            << "Points are lost when a pair is\n"
            << "not made.  Bonus points for time left\n"
            << "after clearing the board.";
        
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int Memory::gameType() const {
    return UNKNOWN_GAME;
}



char Memory::roundOver() {
    if (round == 2) {
        Memory *s(this);
        exitGame(NULL, (void **)&s);
        return 0;
    }
    return 0;
}


void Memory::updateScores() {
    std::string str;
    int i;
    gtk_label_set_text((GtkLabel *)matchScoreLabel, addCommas(matchScore).c_str());
    str = "-";
    str += addCommas(attempts * 250);
    gtk_label_set_text((GtkLabel *)attemptScoreLabel, str.c_str());
    
    gtk_label_set_text((GtkLabel *)timeScoreLabel, addCommas(timeBonus).c_str());
    
    i = matchScore - (attempts * 250) + timeBonus;
    gtk_label_set_text((GtkLabel *)totalRoundLabel, addCommas(i).c_str());
}
