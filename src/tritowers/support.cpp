
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

// support functions for TriTowers

#include <tritowers.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <sys/time.h>

TriTowers::TriTowers() : deckLocation("cards/default/"),
            deckBack("cards/backs/back-2.gif") {
    bgcolor.red = 0x0000;
    bgcolor.green = 0x0000;
    bgcolor.blue = 0x0000;
}

std::string TriTowers::name() const {
    return "tritowers";
}

std::string TriTowers::imageName() const {
    return "tritowers.gif";
}

GdkColor *TriTowers::backgroundColor() {
    return &bgcolor;
}

void TriTowers::shuffle(int *deck) {
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

void TriTowers::startRound() {
    std::stringstream sstr;
    GtkWidget *label;
    GdkColor color;
    int xpos, ypos;
    run = 0;
    highrun = 0;
    bonusLevel = 0;
    waitingForFirstClick = true;
    gtk_label_set_text((GtkLabel *)runLabel, "0");
    
    // create the card deck
    for (int i = 0; i < 52; ++i) {
        cardDeck[i] = i;
    }
    
    // shuffle
    for (int i = 0; i < 10; ++i) {
        shuffle(cardDeck);
    }
    
    // lose the cards from last time.
    for (int i = 0; i < 28; ++i) {
        
        if (dispCards[i] != NULL) {
            gtk_widget_destroy(dispCards[i]);
        }
        if (dispCardBoxes[i] != NULL) {
            gtk_widget_destroy(dispCardBoxes[i]);
        }
    }
    // now create the cards
    cardLocation = 0;
    for (int row = 0; row < 3; ++row) {
        xpos = (3 - row) * 28;
        ypos = row * 50 + 100;
        for (int c = 0; c < 3; ++c) {
            for (int i = 0; i < row + 1; ++i) {
                sstr.str("");
                sstr << deckLocation 
                                << cardDeck[cardLocation] + 1
                                << ".png";
                

                dispCards[cardLocation] = loadMainImage(sstr.str().c_str());
                
                
                // create the event box
                dispCardBoxes[cardLocation] = gtk_event_box_new();
                // just a little cheat
                g_object_set_data((GObject *)dispCardBoxes[cardLocation], KEY_STR,
                                (void *)cardDeck[cardLocation]);
                
                gtk_widget_add_events(dispCardBoxes[cardLocation],
                                GDK_BUTTON_RELEASE_MASK);
                gtk_widget_add_events(dispCardBoxes[cardLocation], 
                                GDK_BUTTON_PRESS_MASK);
                gtk_widget_add_events(dispCardBoxes[cardLocation], 
                                GDK_BUTTON1_MOTION_MASK);
                
                g_signal_connect(G_OBJECT(dispCardBoxes[cardLocation]), 
                                "button-press-event",
                                G_CALLBACK(cardSelected), (gpointer)this);
                
                gtk_container_add((GtkContainer *)dispCardBoxes[cardLocation], dispCards[cardLocation]);

                gtk_fixed_put(GTK_FIXED(centerFixed), dispCardBoxes[cardLocation], 
                                xpos, ypos);
                gtk_widget_show_all(dispCardBoxes[cardLocation]);
                ++cardLocation;
                xpos += 72;
            }
            xpos += (2 - row) * 72;
        }
    }
    
    ypos = 250;
    xpos = 0;
    for (int i = 0; i < 10; ++i) {
        GtkWidget *image;
        sstr.str("");
        sstr << deckLocation 
                        << cardDeck[cardLocation] + 1
                        << ".png";
        image = loadMainImage(sstr.str().c_str());
        dispCards[cardLocation] = image;

        // create the event box
        dispCardBoxes[cardLocation] = gtk_event_box_new();
        // just a little cheat
        g_object_set_data((GObject *)dispCardBoxes[cardLocation], KEY_STR, (void *)cardDeck[cardLocation]);
        
        gtk_widget_add_events(dispCardBoxes[cardLocation],
                                GDK_BUTTON_RELEASE_MASK);
        gtk_widget_add_events(dispCardBoxes[cardLocation], 
                                GDK_BUTTON_PRESS_MASK);
        gtk_widget_add_events(dispCardBoxes[cardLocation], 
                                GDK_BUTTON1_MOTION_MASK);
        
        g_signal_connect(G_OBJECT(dispCardBoxes[cardLocation]), 
                            "button-press-event",
                            G_CALLBACK(cardSelected), (gpointer)this);
        
        gtk_container_add((GtkContainer *)dispCardBoxes[cardLocation], image);
        
        
        gtk_fixed_put(GTK_FIXED(centerFixed), dispCardBoxes[cardLocation], 
                        xpos, ypos);
        
        gtk_widget_show_all(dispCardBoxes[cardLocation]);
        ++cardLocation;
        xpos += 72;
    }
    
    // ok, now setup the first deal card
    if (dealCard != NULL) {
        gtk_widget_destroy(dealCard);

    }
    sstr.str("");
    sstr << deckLocation << cardDeck[cardLocation++] + 1 << ".png";
    
    dealCard = loadMainImage(sstr.str().c_str());
    
    sstr.str("");
    sstr << std::setw(2) << std::setfill('0') << 52 - cardLocation << " Cards Remaining";
    gtk_label_set_text((GtkLabel *)remainingLabel, sstr.str().c_str());
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    gtk_widget_modify_fg(remainingLabel, (GtkStateType)0, &color);

    gtk_container_add((GtkContainer *)dealCardBox, dealCard);
    // just a little cheat
    g_object_set_data((GObject *)dealCardBox, KEY_STR, (void *)cardDeck[cardLocation - 1]);
    
    g_signal_connect(G_OBJECT(dealCardBox), "button-press-event",
                        G_CALLBACK(dealPressed), (gpointer)this);
    gtk_widget_show_all(dealCardBox);
    
    sstr.str("");
    sstr << round;
    gtk_label_set_text((GtkLabel *)roundLabel, sstr.str().c_str());
    
    timeLeft = 1.0;
    gameState = PLAY_ROUND;
    gtk_progress_bar_set_fraction((GtkProgressBar *)timeDealie, timeLeft);
    for (int i = 0; i < 3; ++i) {
//         gtk_fixed_move((GtkFixed *)centerFixed, swordImages[i], 0, -300);
    }
    gtk_fixed_move((GtkFixed *)centerFixed, bonusImages[0], 0, -300);
    gtk_fixed_move((GtkFixed *)centerFixed, bonusImages[1], 0, -300);
    gtk_fixed_move((GtkFixed *)centerFixed, bonusImages[2], 0, -300);
    gtk_fixed_move((GtkFixed *)centerFixed, addingBonus, 0, -300);
}

GtkWidget *TriTowers::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "tritowers is a game which you\n"
            << "start with 3 towers of cards. You have\n"
            << "to remove the cards from the bottom to\n"
            << "the top and cannot remove any card which\n"
            << "is underneth other cards.\n"
            << "Cards which are either one higher or one\n"
            << "lower than the card at the bottom of the\n"
            << "screen can be clicked on to remove.\n"
            << "For each card removed in a single run, the\n"
            << "points per card removed will be increased.";
    
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int TriTowers::getRow(int i) {
    if (i < 3) {
        return 0;
    }
    if (i < 9) {
        return 1;
    }
    if (i < 18) {
        return 2;
    }
    if (i < 30) {
        return 3;
    }
    return 0;
}

int TriTowers::getCard(int i) {
    if (i < 3) {
        return i;
    }
    if (i < 9) {
        return i - 3;
    }
    if (i < 18) {
        return i - 9;
    }
    if (i < 30) {
        return i - 18;
    }
    return 0;
}

int TriTowers::checkWinner() {
    for (int i = 0; i < 28; ++i) {
        if (dispCards[i] != NULL) {
                return 0;
        }
    }
    return 1;
}

void TriTowers::checkCards(GtkWidget *lh, GtkWidget *rh) {
    int absval(0);
    int lhcard((int)g_object_get_data((GObject *)lh, KEY_STR)); 
    int rhcard((int)g_object_get_data((GObject *)rh, KEY_STR));
    absval = abs(lhcard / 4 - 
                rhcard / 4);
    
    if (absval == 1 || absval == 12) {
        std::stringstream sstr;
        GtkWidget **location;
        
        if (!run) {
            totalScore += 100;
            
            gtk_label_set_text((GtkLabel *)cardScore, "100");
            gtk_fixed_move((GtkFixed *)centerFixed, cardScore, 
                            rh->allocation.x - 32, rh->allocation.y + 20);
            gtk_widget_show(cardScore);
        }
        else {
            totalScore += run * 200;
            
            sstr.str("");
            sstr << run * 200;
            gtk_label_set_text((GtkLabel *)cardScore, sstr.str().c_str());
            gtk_fixed_move((GtkFixed *)centerFixed, cardScore, 
                            rh->allocation.x - 32, rh->allocation.y + 20);
            gtk_widget_show(cardScore);
        }
        
        ++run;
        if (run > highrun) {
            highrun = run;
            sstr.str("");
            sstr << highrun;
            gtk_label_set_text((GtkLabel *)highRunLabel, sstr.str().c_str());
        }
        sstr.str("");
        sstr << run;
        gtk_label_set_text((GtkLabel *)runLabel, sstr.str().c_str());
        
        sstr.str("");
        
        if (((GtkBin *)rh)->child == dispCards[0] || 
                            ((GtkBin *)rh)->child == dispCards[1] || 
                            ((GtkBin *)rh)->child == dispCards[2]) {
            int xpos;
            gameState = DROPPING_SWORD;
            GtkWidget *image;
            
            switch (bonusLevel) {
                    case 0:
                            totalScore += 5000;
                    break;
                    case 1:
                            totalScore += 10000;
                    break;
                    case 2:
                            totalScore+= 15000;
                    break;
            }
            //sstr << setw(12) << setfill(' ') << score;
            //gtk_label_set_text((GtkLabel *)scoreLabel, sstr.str().c_str());
            //sstr.str("");
            ++bonusLevel;
            if (((GtkBin *)rh)->child == dispCards[0]) {
                    swordImage = swordImages[0];
                    swordx = 110;
            }
            else if (((GtkBin *)rh)->child == dispCards[1]) {
                    swordImage = swordImages[1];
                    swordx = 310;
            }
            else {   // we know it's the last card here
                    swordImage = swordImages[2];
                    swordx = 510;
            }
            swordy = -237;
            //gtk_fixed_move((GtkFixed *)centerFixed, swordImage, swordx, swordy);
//             gtk_widget_show(swordImage);
            gtk_widget_hide(cardScore);
            
            // speed up the timer
            g_source_remove(timer);
            timer = g_timeout_add(10, timeout, this);
        }

        sstr.str("");
        sstr << addCommas(totalScore);
        gtk_label_set_text((GtkLabel *)scoreLabel, sstr.str().c_str());
        
        if (dealCard != NULL) {
                gtk_widget_destroy(dealCard);
        }
        sstr.str("");
        sstr << deckLocation 
                << (int)g_object_get_data((GObject *)rh, KEY_STR) + 1 << ".png";
//         dealCard = gtk_image_new_from_file(sstr.str().c_str());
        dealCard = loadMainImage(sstr.str().c_str());
        g_object_set_data((GObject *)dealCardBox, KEY_STR, g_object_get_data((GObject *)rh, KEY_STR));
        
        gtk_container_add((GtkContainer *)dealCardBox, dealCard);
        gtk_widget_show_all(dealCard);
        
        location = std::find(dispCards, &dispCards[28], ((GtkBin*)rh)->child);
        if (location != &dispCards[28]) {
                dispCards[location - dispCards] = NULL;
                dispCardBoxes[location - dispCards] = NULL;
        }
        gtk_widget_destroy(((GtkBin *)rh)->child);

        location = std::find(dispCards, &dispCards[28], ((GtkBin*)rh)->child);

        if (location != &dispCards[28]) {
                dispCards[location - dispCards] = NULL;
                dispCardBoxes[location - dispCards] = NULL;
        }
        gtk_widget_destroy(rh);

        if (checkWinner() && round == 3) {
        
        }
        else if (checkWinner()) {
                if (gameState != DROPPING_SWORD) {
                        gameState = BETWEEN_ROUNDS;
                        g_source_remove(timer);
                        timer = g_timeout_add(10, timeout, this);
                }
        }
        
        else {

        }

    }

}

int TriTowers::gameType() const {
    return CARD_GAME;
}

