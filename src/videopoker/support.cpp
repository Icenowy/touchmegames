
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

// support functions for VideoPoker

#include <videopoker.h>
#include <algorithm>
#include <sys/time.h>
#include <sstream>

VideoPoker::VideoPoker() : toggleOccuring(false),
        deckBack(std::string(INSTALL_DIR) + std::string("images/cards/backs/back-1.png")),
        deckLocation(std::string(INSTALL_DIR) + std::string("images/cards/")) {
    
    color.red = 0xA000;
    color.green = 0xA000;
    color.blue = 0xA000;
}

std::string VideoPoker::name() const {
    return "videopoker";
}

std::string VideoPoker::imageName() const {
    return "videopoker.gif";
}

GdkColor *VideoPoker::backgroundColor() {
    return &color;
}

void VideoPoker::shuffle(int *deck) {
    timeval tv;
    struct timezone tz;
    int deckcopy[52];
    int lh, rh(51), middle;
    int pos(51);
    
    tz.tz_minuteswest = 0;
    tz.tz_dsttime = 0;

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

void VideoPoker::startHand() {
    // shuffle
    for (int i = 0; i < 10; ++i) {
        shuffle(cardDeck);
    }
    cardLocation = 0;
    
    
    gameState = SELECT_BET;
}

bool VideoPoker::isFlush() {
    int suit(cardNumber[0] % 4);
    for (int i = 1; i < 5; ++i) {
        if (cardNumber[i] % 4 != suit) {
                return false;
        }
    }
    return true;
}

bool VideoPoker::isStraight() {
    int cards[5];
    bool firstTest(true);
    for (int i = 0; i < 5; ++i) {
        cards[i] = cardNumber[i] / 4;
    }
    for (int i = 0; i < 4; ++i) {
        if (cards[i] != cards[i + 1] + 1) {
            firstTest = false;
        }
    }
    if (firstTest) {
        return true;
    }
    if (cards[0] == 0) {// ace
        cards[0] = 12;
    }
    
    for (int i = 0; i < 4; ++i) {
        if (cards[i] != cards[i + 1] - 1) {
            return  false;
        }
    }
    return true;
}

int VideoPoker::pairs() {
    int card;
    int numPairs(0);
    for (int i = 0; i < 5; ++i) {
        int numOfCard(1);
        int idx;
        card = cardNumber[i] / 4;
        for (idx = i + 1; idx < 5; ++idx) {
            if (cardNumber[idx] / 4 == card) {
                ++numOfCard;
            }
        }
        if (numOfCard == 2) {
            ++numPairs;
        }
        i += numOfCard - 1;
    }
    return numPairs;
}

int VideoPoker::trips() {
    int card;
    int numTrips(0);
    for (int i = 0; i < 5; ++i) {
        int numOfCard(1);
        int idx;
        card = cardNumber[i] / 4;
        for (idx = i + 1; idx < 5; ++idx) {
            if (cardNumber[idx] / 4 == card) {
                ++numOfCard;
            }
        }
        if (numOfCard == 3) {
            ++numTrips;
        }
        i += numOfCard - 1;
    }
    return numTrips;
}

int VideoPoker::getHand() {
    std::sort(cardNumber, &cardNumber[5], std::greater<int>());
    if (std::count(cardNumber, &cardNumber[5], cardNumber[2]) == 4) {
        return FOUR_OF_A_KIND;
    }
    if (isStraight()) {
        if (isFlush()) {
            if (cardNumber[4] < 4) {
                return ROYAL_FLUSH;
            }
            return STRAIGHT_FLUSH;
        }
        return STRAIGHT;
    }
    if (isFlush()) {
        return FLUSH;
    }
    if (trips()) {
        if (pairs()) {
            return FULL_HOUSE;
        }
        return TRIPS;
    }
    if (pairs() == 2) {
        return TWO_PAIR;
    }
    if (pairs()) {
        return PAIR;
    }
    return HIGH_CARD;
}


int VideoPoker::gameType() const {
    return CARD_GAME;
}

GtkWidget *VideoPoker::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "videopoker is the same as any other video poker\n"
            << "game, except you have 20 rounds to get a high score and\n"
            << "ANY pair count as a pair, not just Jacks and higher.\n";
    
    
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}
