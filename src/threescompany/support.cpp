
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

// support functions for ThreesCompany

#include <threescompany.h>
#include <sstream>
#include <algorithm>
#include <sys/time.h>

ThreesCompany::ThreesCompany() : toggleOccuring(false),
            deckLocation("cards/"),
            deckBack("cards/backs/back-1.png") {
    bgcolor.red = 0x0000;
    bgcolor.green = 0x0000;
    bgcolor.blue = 0x0000;
}

std::string ThreesCompany::name() const {
    return "threescompany";
}

std::string ThreesCompany::imageName() const {
    return "threescompany.gif";
}

GdkColor *ThreesCompany::backgroundColor() {
    return &bgcolor;
}

void ThreesCompany::shuffle(int *deck) {
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

void ThreesCompany::startRound() {
    std::stringstream sstr;
    GtkWidget *label;
    // shuffle
    for (int i = 0; i < 10; ++i) {
        shuffle(cardDeck);
    }
    
    if (cardMask != NULL) {
        gtk_widget_destroy(cardMask);
        cardMask = NULL;
    }
    
    sstr << round;
    gtk_label_set_text((GtkLabel *)roundLabel, sstr.str().c_str());
    
    cardLocation = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (cardHoldImages[i][j] != NULL) {
                gtk_widget_destroy(cardHoldImages[i][j]);
            }
        }
    }
    for (int i = 0; i < 3; ++i) {
        if (topCardHoldImages[i] != NULL) {
            gtk_widget_destroy(topCardHoldImages[i]);
        }
    }
    
    for (int i = 0; i < 3; ++i) {
        cardHoldImages[0][i] = NULL;
        cardHoldImages[1][i] = NULL;
        cardHoldImages[2][i] = NULL;
        cardHoldImages[3][i] = NULL;
        topCardHoldImages[i] = NULL;
    }
    
    if (cards[0] != NULL) {
        gtk_widget_destroy(cards[0]);
    }
    
    if (cards[1] != NULL) {
        gtk_widget_destroy(cards[1]);
    }
    
    if (cards[2] != NULL) {
        gtk_widget_destroy(cards[2]);
    }
    cards[0] = createCard(cardDeck[cardLocation++] + 1);
    cards[1] = createCard(cardDeck[cardLocation++] + 1);
    cards[2] = createCard(cardDeck[cardLocation++] + 1);
    
    
    gtk_fixed_put((GtkFixed *)fixed, cards[0], 200, 445);
    gtk_fixed_put((GtkFixed *)fixed, cards[1], 236, 445);
    gtk_fixed_put((GtkFixed *)fixed, cards[2], 272, 445);
    
    gtk_widget_show_all(cards[0]);
    gtk_widget_show_all(cards[1]);
    gtk_widget_show_all(cards[2]);
    
    timeLeft = 1.0;
    gameState = PLAY_ROUND;
    
    shown = false;
}

GtkWidget *ThreesCompany::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "threescompany is a game which you\n"
                    << "have 52 cards, and need to either get three in\n"
                    << "a row or three of a kind to clear the cards.\n"
                    << "There are three holding cells at the top which\n"
                    << "you can place one card in at a time.  The four\n"
                    << "cells in the middle are for matching up the three\n"
                    << "cards.  6000 points are awarded if the you get\n"
                    << "three of a kind and 3000 points are awarded for\n"
                    << "three in a row.";
    
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int ThreesCompany::gameType() const {
    return CARD_GAME;
}
