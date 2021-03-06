
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

// support functions for FortressCrackers
// Functions which basically don't fit in
// events.cpp or interface.cpp

#include <fortresscrackers.h>
#include <iostream>
#include <sstream>

FortressCrackers::FortressCrackers() : cardWidth(100), cardHeight(100), cardMiddle(cardWidth / 2),
                xpadding(28), ypadding(28), cardOffset(cardMiddle + xpadding / 2), 
                levelOne(0), levelTwo(4), levelThree(16), levelFour(32), bonusWidth(150), bonusHeight(63),
                takingScoreWidth(236), takingScoreHeight(66) {
    
}

std::string FortressCrackers::name() const {
    return "fortresscrackers";
}

std::string FortressCrackers::imageName() const {
    return "fortresscrackers.png";
}

// This function doesn't even do anything anymore...really
// The background should be a modified bg.png
GdkColor *FortressCrackers::backgroundColor() {
    return &bgcolor;
}

void FortressCrackers::shuffle(int *deck) {
    int deckcopy[52];
    int lh, rh(51), middle;
    int pos(51);
    
    middle = getRand(52);
    lh = middle;
    
    while (rh != middle && lh != -1) {
        deckcopy[pos--] = deck[lh--];
        deckcopy[pos--] = deck[rh--];
    }
    while (rh != middle) {
        deckcopy[pos--] = deck[rh--];
    }
    while (lh != -1) {
        deckcopy[pos--] = deck[lh--];
    }
    // now cut the deck
    
    middle = getRand(52);
    lh = middle;
    pos = 51;
    while (lh >= 0) {
        deck[pos--] = deckcopy[lh--];
    }
    rh = 51;
    while (rh > middle) {
        deck[pos--] = deckcopy[rh--];
    }

}

void FortressCrackers::startRound() {
    std::stringstream sstr;
    
    if (round + 1 == 4) {
        gameState = GAME_OVER;
        return;
    }
    round += 1;
//     if (++round < 3) {
//         maxColumns = 6;
//         maxRows = 6;
//         perXSector = 3;
//         perYSector = 2;
//         maxInt = 6;
//     }
//     
    for (int x = 0; x < 13; ++x) {
        for (int y = 0; y < 4; ++y) {
            deck[x * 4 + y] = x;
        }
    }
    for (int i = 0; i < 20; ++i) {
        shuffle(deck);
    }
    
    for (int i = 0; i < 36; ++i) {
        std::stringstream sstr1;
        sstr1 << deck[i] << ".png";
        reloadImage(cards[i], "back.png");
        gtk_widget_show_all(cards[i]->parent);
    }
    
    sstr << round;
    gtk_label_set_text((GtkLabel *)roundLabel, sstr.str().c_str());
    
    gameState = INIT_1;
    timeLeft = 600;
    
    cardPosition = 36;
    currentBonus = 0;
    sstr.str("");
    sstr << deck[cardPosition++] + 1 << ".png";
    reloadImage(dealCard, sstr.str().c_str());
    roundScore = 0;
    gtk_widget_show(userCard);
    gtk_widget_show(userCard->parent);
    
    for (int i = 0; i < 5; ++i) {
        gtk_widget_hide(bonus[i]);
    }
    gtk_widget_hide(takingScore);
    
    gtk_label_set_text((GtkLabel *)streakLabel, "0");
    gtk_label_set_text((GtkLabel *)totalScoreLabel, "0");
    gtk_label_set_text((GtkLabel *)cardScoreLabel, "0");
    gtk_label_set_text((GtkLabel *)roundScoreLabel, "0");
    gtk_label_set_text((GtkLabel *)timeScoreLabel, "0");
    
    progress.percent(100);
    progress.text("2:00");
    
    gtk_label_set_text((GtkLabel *)cardsLeftLabel, "15");
    
    streak = 0;
    cardBonus = 0;
    timeBonus = 0;
}


GtkWidget *FortressCrackers::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    // Keep this color
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "fortresscrackers is a game where you try to get\n"
            << "rid of all of the cards.  This is done by selecting\n"
            << "cards from the middle that are either the next number\n"
            << "above or below the card in the lower left of the\n"
            << "game.  The card in the lower right can be used at\n"
            << "anytime.  When used, you can select any card from\n"
            << "the middle to start with.\n\n"
            << "Points are scored for a correct card picked from\n"
            << "the middle.  The first card in a streak is worth\n"
            << "100, and each card increasing that by 100, i.e.\n"
            << "100, 200, 300, 400, etc.  Bonuses are awarded at\n"
            << "the bottom of each stack.";
        
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int FortressCrackers::gameType() const {
    return STRATEGY_GAME;
}



char FortressCrackers::roundOver() {
    if (round == 2) {
        FortressCrackers *s(this);
        exitGame(NULL, (void **)&s);
        return 0;
    }
    return 0;
}

bool FortressCrackers::isOnTop(GtkWidget *widget) {
    int offset(0), idx(0);
    GdkRectangle rect;
    
    // First find out which widget this is
    for (offset = 0; offset < 36; ++offset) {
        if (widget == cards[offset]) {
            break;
        }
    }
    
    // Now, check to see if the there are cards above it
    // check the 4 corners
    // left side
    rect.x = widget->allocation.x;
    // half way
    rect.y = widget->allocation.y + cardHeight / 2;
    rect.width = 1;
    rect.height = 1;
    for (idx = offset + 1; idx < 36; ++idx) {
        if (gtk_widget_intersect(cards[idx], &rect, NULL) && gtk_widget_get_visible(cards[idx]->parent) == TRUE) {
            return false;
        }
    }
    
    // right side
    rect.x = widget->allocation.x + cardWidth - 1;
    // half way
    rect.y = widget->allocation.y + cardHeight / 2;
    rect.width = 1;
    rect.height = 1;
    for (idx = offset + 1; idx < 36; ++idx) {
        if (gtk_widget_intersect(cards[idx], &rect, NULL) && gtk_widget_get_visible(cards[idx]->parent) == TRUE) {
            return false;
        }
    }
    
    // middle
    rect.x = widget->allocation.x + cardWidth / 2;
    // top
    rect.y = widget->allocation.y;
    rect.width = 1;
    rect.height = 1;
    for (idx = offset + 1; idx < 36; ++idx) {
        
        if (gtk_widget_intersect(cards[idx], &rect, NULL) && gtk_widget_get_visible(cards[idx]->parent) == TRUE) {
            return false;
        }
    }
    
    // middle
    rect.x = widget->allocation.x + cardWidth / 2;
    // bottom
    rect.y = widget->allocation.y + cardHeight - 1;
    rect.width = 1;
    rect.height = 1;
    for (idx = offset + 1; idx < 36; ++idx) {
        
        if (gtk_widget_intersect(cards[idx], &rect, NULL) && gtk_widget_get_visible(cards[idx]->parent) == TRUE) {
            return false;
        }
    }
    
    return true;
}

bool FortressCrackers::isValid(GtkWidget *widget) {
    int offset;
    int x, y;
    int tmp;
    std::stringstream sstr;
    for (offset = 0; offset < 36; ++offset) {
        if (widget == cards[offset]) {
            break;
        }
    }
    
    
    // - 1 because cardPosition is ready for the next card
    if (abs(deck[offset] - deck[cardPosition - 1]) != 1 && abs(deck[offset] - deck[cardPosition - 1]) != 12
            && deck[cardPosition - 1] != -1) {
        //streak = 0;
        return false;
    }
    
    // Add to score here!!
    roundScore += (streak + 1) * 100;
    gtk_label_set_text((GtkLabel *)roundScoreLabel, addCommas(roundScore).c_str());
            
    
    sstr.str("");
    deck[cardPosition - 1] = deck[offset];
    sstr << deck[offset] + 1 << ".png";
    reloadImage(dealCard, sstr.str().c_str());
    
    sstr.str("");
    sstr << streak + 1;
    gtk_label_set_text((GtkLabel *)streakLabel, sstr.str().c_str());
    
    
    if (offset < levelTwo) { // bonus time!!
        // Add the bonus to the score
        
        roundScore += (currentBonus + 1) * 5000;
        gtk_label_set_text((GtkLabel *)roundScoreLabel, addCommas(roundScore).c_str());
        
        
        gtk_widget_show(bonus[currentBonus]);
        // get the x and y coords of the current card
        x = widget->allocation.x;
        y = widget->allocation.y;
        // now get the center
        x += cardWidth / 2;
        y += cardHeight / 2;
        
        // now place the bonus in the proper location
        x -= bonusWidth / 2;
        y -= bonusHeight / 2;
        gtk_fixed_move((GtkFixed *)topFixed, bonus[currentBonus++], x, y);
        
        if (currentBonus == 4) {
            // User has finished the round
            gameState = TAKE_WILD_SCORE;
            stageCountDown = 5;
            
            y = ypadding;
            x = xpadding + cardOffset * 3 + cardWidth / 2 - takingScoreWidth / 2;
            
            // Show the taking score image
            gtk_widget_show(takingScore);
            gtk_fixed_move((GtkFixed *)topFixed, takingScore, x, y);
        }
    }
    ++streak;
    return true;
}

void FortressCrackers::showTop() {
    for (int i = 0; i < 36; ++i) {
        if (isOnTop(cards[i])) {
            std::stringstream sstr;
            sstr << deck[i] + 1 << ".png";
            reloadImage(cards[i], sstr.str().c_str());
        }
    }
}
