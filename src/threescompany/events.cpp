
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

// events for ThreesCompany

#include <threescompany.h>
#include <sstream>
#include <iomanip>

void ThreesCompany::exitGame(GtkWidget *widget, gpointer *ptr) {
    ThreesCompany *threescompany(reinterpret_cast<ThreesCompany *>(*ptr));
    SCORE_STRUCT newScore;
    
    newScore.name = "";
    newScore.score = threescompany->totalScore;
    newScore.date = "";
    
    
    threescompany->GAME_PTR::addScore(threescompany->scores, newScore);
    
    threescompany->GAME_PTR::saveScores(threescompany->scores);
    
    g_source_remove(threescompany->timer);
    gtk_widget_hide(threescompany->topEventBox);
}

void ThreesCompany::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    ThreesCompany *threescompany(reinterpret_cast<ThreesCompany *>(ptr));
    
    if (threescompany->quitDialog()) {
        g_source_remove(threescompany->timer);
        gtk_widget_hide(threescompany->topEventBox);
    }
}

int ThreesCompany::checkTrip(int hold) {
    int trip[3];
    
    for (int i = 0; i < 3; ++i) {
        trip[i] = (intptr_t)g_object_get_data((GObject *)cardHoldImages[hold][i], KEY_STR);

        // adjust the number for the actual value
        trip[i] = 13 - ((trip[i] - 1) / 4);
    }
    
    if (trip[0] + 1 == trip[1] && trip[1] + 1 == trip[2]) {
        return 1;
    }
    else if (trip[0] - 1 == trip[1] && trip[1] - 1 == trip[2]) {
        return 1;
    }
    else if (trip[0] == 13 && trip[1] == 1 && trip[2] == 2) {
        return 1;
    }
    else if (trip[0] == 2 && trip[1] == 1 && trip[2] == 13) {
        return 1;
    }
    else if (trip[0] == trip[1] && trip[1] == trip[2]) {
        return 3;
    }
    
    return 0;
}

gboolean ThreesCompany::timeout(gpointer ptr) {
    ThreesCompany *vp(reinterpret_cast<ThreesCompany *>(ptr));
    switch (vp->gameState) {
        case PLAY_ROUND:
            if (vp->timeLeft) {
                vp->timeLeft -= 1.0 / (10.0 * 120.0);
                if (vp->timeLeft < 0.0) {
                    vp->timeLeft = 0;
                }
                vp->progress.percent(vp->timeLeft * 100);
            }
            else {
                vp->gameState = OUT_OF_TIME;
            }
        break;
        case BETWEEN_ROUNDS:
        break;
        case OUT_OF_TIME:
            // need to get this out of time going...perhaps a penalty.
            vp->takeScoreReleased(vp->takeScore, NULL, vp);
        break;
        case GAME_OVER:
        break;
    }
    return TRUE;
}

void ThreesCompany::cardSelected(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    ThreesCompany *vp(reinterpret_cast<ThreesCompany *>(ptr));
    int i = 0;
}

void ThreesCompany::dealPressed(GtkWidget *widget, gpointer *ptr) {
    ThreesCompany *vp(reinterpret_cast<ThreesCompany *>(ptr));
}

void ThreesCompany::cardReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    ThreesCompany *tc(reinterpret_cast<ThreesCompany *>(ptr));
    
    if (widget == tc->cards[2] && !tc->shown) {
        tc->cardMasksCard = tc->cards[2];
        tc->shown = true;
        tc->createMask();
        gtk_fixed_put((GtkFixed *)tc->fixed, tc->cardMask, widget->allocation.x + 23, widget->allocation.y + 40);
    }
    else if (widget == tc->cards[2] && tc->shown) {
        gtk_widget_destroy(tc->cardMask);
        tc->cardMask = NULL;
        tc->shown = false;
    }
    else if (widget == tc->cards[0] || widget == tc->cards[1]) {
        return;
    }
    else if (!tc->shown) {
        tc->shown = true;
        tc->createMask();
        gtk_fixed_put((GtkFixed *)tc->fixed, tc->cardMask, widget->allocation.x + 23, widget->allocation.y + 40);
        tc->cardMasksCard = widget;
        return;
    }
    
    if (widget == tc->cardHoldImages[0][0] || widget == tc->cardHoldImages[0][1]) {
        tc->holdReleased(tc->cardHolds[0], NULL, tc);
    }
    else if (widget == tc->cardHoldImages[1][0] || widget == tc->cardHoldImages[1][1]) {
        tc->holdReleased(tc->cardHolds[1], NULL, tc);
    }
    else if (widget == tc->cardHoldImages[2][0] || widget == tc->cardHoldImages[2][1]) {
        tc->holdReleased(tc->cardHolds[2], NULL, tc);
    }
    else if (widget == tc->cardHoldImages[3][0] || widget == tc->cardHoldImages[3][1]) {
        tc->holdReleased(tc->cardHolds[3], NULL, tc);
    }
}


void ThreesCompany::holdReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    ThreesCompany *tc(reinterpret_cast<ThreesCompany *>(ptr));
    int card0(-1), card1(-1), holdCard(0), previousCard(-1), previousHold(-1), hold;
    GtkWidget *widget1;
    
    if (!tc->shown) {
        return;
    }
    
    gtk_widget_destroy(tc->cardMask);
    tc->cardMask = NULL;
    tc->shown = false;
    for (int i = 0; i < 4; ++i) {
        
        if (widget == tc->cardHolds[i]) {
            hold = i;
            i = 4;
        }
    }
    if (tc->cardHoldImages[hold][0] == NULL) {
        holdCard = 0;
    }
    else if (tc->cardHoldImages[hold][1] == NULL) {
        holdCard = 1;
    }
    else if (tc->cardHoldImages[hold][2] == NULL) {
        holdCard = 2;
    }
    else {
        return;
    }
    if (tc->cardMasksCard == tc->cards[2]) {
        tc->cardHoldImages[hold][holdCard] 
                        = tc->createCard((intptr_t)g_object_get_data((GObject *)tc->cards[2], KEY_STR));
            
        gtk_fixed_put((GtkFixed *)tc->fixed, tc->cardHoldImages[hold][holdCard], 160 + (131 * hold), 
                        200 + (30 * holdCard));
            
        gtk_widget_show_all(tc->cardHoldImages[hold][holdCard]);
        
        if (tc->cards[0] != NULL) {
            card0 = (intptr_t)g_object_get_data((GObject *)tc->cards[0], KEY_STR);
        }
        if (tc->cards[1] != NULL) {
            card1 = (intptr_t)g_object_get_data((GObject *)tc->cards[1], KEY_STR);
        }
        
        if (tc->cards[0] != NULL) {
            gtk_widget_destroy(tc->cards[0]);
        }
        if (tc->cards[1] != NULL) {
            gtk_widget_destroy(tc->cards[1]);
        }
        gtk_widget_destroy(tc->cards[2]);
        if (tc->cardLocation < 52) {
            tc->cards[0] = tc->createCard(tc->cardDeck[tc->cardLocation++] + 1);
            gtk_fixed_put((GtkFixed *)tc->fixed, tc->cards[0], 200, 445);
            gtk_widget_show_all(tc->cards[0]);
        }
        else {
            tc->cards[0] = NULL;
        }
        if (card0 != -1) {
            tc->cards[1] = tc->createCard(card0);
            gtk_fixed_put((GtkFixed *)tc->fixed, tc->cards[1], 236, 445);
            gtk_widget_show_all(tc->cards[1]);
        }
        else {
            tc->cards[1] = NULL;
        }
        if (card1 != -1) {
            tc->cards[2] = tc->createCard(card1);
            gtk_fixed_put((GtkFixed *)tc->fixed, tc->cards[2], 272, 445);
            gtk_widget_show_all(tc->cards[2]);
        }
        else {
            tc->cards[2] = NULL;
        }
        
        if (tc->cardHoldImages[hold][2] != NULL) {
            int ret = tc->checkTrip(hold);
            if (ret) {
                tc->clearHold(hold, ret);
            }
        }
        return;
    }
    // first check if this card is in the top holds
    if (tc->cardMasksCard == tc->topCardHoldImages[0]) {
        previousCard = 0;
    }
    else if (tc->cardMasksCard == tc->topCardHoldImages[1]) {
        previousCard = 1;
    }
    else if (tc->cardMasksCard == tc->topCardHoldImages[2]) {
        previousCard = 2;
    }
    if (previousCard != -1) {
        card0 = (intptr_t)g_object_get_data((GObject *)tc->topCardHoldImages[previousCard], KEY_STR);
        gtk_widget_destroy(tc->topCardHoldImages[previousCard]);
        tc->topCardHoldImages[previousCard] = NULL;
        tc->cardHoldImages[hold][holdCard] = tc->createCard(card0);
        gtk_fixed_put((GtkFixed *)tc->fixed, tc->cardHoldImages[hold][holdCard], 160 + (131 * hold), 200 + (30 * holdCard));
                
        gtk_widget_show_all(tc->cardHoldImages[hold][holdCard]);
        
        if (tc->cardHoldImages[hold][2] != NULL) {
            int ret = tc->checkTrip(hold);
            if (ret) {
                tc->clearHold(hold, ret);
            }
        }
        return;
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (tc->cardMasksCard == tc->cardHoldImages[i][j] && i != hold) {
                previousCard = j;
                previousHold = i;
            }
        }
    }
    if (previousCard != -1) {
        card0 = (intptr_t)g_object_get_data((GObject *)tc->cardHoldImages[previousHold][previousCard], KEY_STR);
        gtk_widget_destroy(tc->cardHoldImages[previousHold][previousCard]);
        tc->cardHoldImages[previousHold][previousCard] = NULL;
        tc->cardHoldImages[hold][holdCard] = tc->createCard(card0);
        gtk_fixed_put((GtkFixed *)tc->fixed, tc->cardHoldImages[hold][holdCard], 160 + (131 * hold), 200 + (30 * holdCard));
                
        gtk_widget_show_all(tc->cardHoldImages[hold][holdCard]);
        
        if (tc->cardHoldImages[hold][2] != NULL) {
            int ret = tc->checkTrip(hold);
            if (ret) {
                tc->clearHold(hold, ret);
            }
        }
        return;
    }
}



void ThreesCompany::topHoldReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    ThreesCompany *tc(reinterpret_cast<ThreesCompany *>(ptr));
    int card0(-1), card1(-1), holdCard(0), previousCard(-1), previousHold(-1), hold;
    GtkWidget *widget1;
    
    if (!tc->shown) {
        return;
    }
    
    gtk_widget_destroy(tc->cardMask);
    tc->cardMask = NULL;
    tc->shown = false;
    
    for (int i = 0; i < 3; ++i) {
        if (widget == tc->topCardHolds[i]) {
            hold = i;
            i = 4;
        }
    }
    
    if (tc->topCardHoldImages[hold] != NULL) {
        return;
    }
    
    if (tc->cardMasksCard == tc->cards[2]) {
        tc->topCardHoldImages[hold] = tc->createCard((intptr_t)g_object_get_data((GObject *)tc->cards[2], KEY_STR));
        
        gtk_fixed_put((GtkFixed *)tc->fixed, tc->topCardHoldImages[hold], 185 + (173 * hold), 40);
        
        gtk_widget_show_all(tc->topCardHoldImages[hold]);
        
        if (tc->cards[0] != NULL) {
            card0 = (intptr_t)g_object_get_data((GObject *)tc->cards[0], KEY_STR);
        }
        if (tc->cards[1] != NULL) {
            card1 = (intptr_t)g_object_get_data((GObject *)tc->cards[1], KEY_STR);
        }
        
        if (tc->cards[0] != NULL) {
            gtk_widget_destroy(tc->cards[0]);
        }
        if (tc->cards[1] != NULL) {
            gtk_widget_destroy(tc->cards[1]);
        }
        if (tc->cards[2] != NULL) {
            gtk_widget_destroy(tc->cards[2]);
        }
        
        if (tc->cardLocation < 52) {
            tc->cards[0] = tc->createCard(tc->cardDeck[tc->cardLocation++] + 1);
            gtk_fixed_put((GtkFixed *)tc->fixed, tc->cards[0], 200, 445);
            gtk_widget_show_all(tc->cards[0]);
        }
        else {
            tc->cards[0] = NULL;
        }
        if (card0 != -1) {
            tc->cards[1] = tc->createCard(card0);
            gtk_fixed_put((GtkFixed *)tc->fixed, tc->cards[1], 236, 445);
            gtk_widget_show_all(tc->cards[1]);
        }
        else {
            tc->cards[1] = NULL;
        }
        if (card0 != -1) {
            tc->cards[2] = tc->createCard(card1);
            gtk_fixed_put((GtkFixed *)tc->fixed, tc->cards[2], 272, 445);
            gtk_widget_show_all(tc->cards[2]);
        }
        else {
            tc->cards[2] = NULL;
        }
            
    }
    if (tc->cardMasksCard == tc->topCardHoldImages[0]) {
        previousCard = 0;
    }
    else if (tc->cardMasksCard == tc->topCardHoldImages[1]) {
        previousCard = 1;
    }
    else if (tc->cardMasksCard == tc->topCardHoldImages[2]) {
        previousCard = 2;
    }
    if (previousCard != -1) {
        card0 = (intptr_t)g_object_get_data((GObject *)tc->topCardHoldImages[previousCard], KEY_STR);
        gtk_widget_destroy(tc->topCardHoldImages[previousCard]);
        tc->topCardHoldImages[previousCard] = NULL;
        tc->topCardHoldImages[hold] = tc->createCard(card0);
        gtk_fixed_put((GtkFixed *)tc->fixed, tc->topCardHoldImages[hold], 185 + (173 * hold), 40);
                
        gtk_widget_show_all(tc->topCardHoldImages[hold]);
        return;
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (tc->cardMasksCard == tc->cardHoldImages[i][j]) {
                previousCard = j;
                previousHold = i;
            }
        }
    }
    if (previousCard != -1) {
        card0 = (intptr_t)g_object_get_data((GObject *)tc->cardHoldImages[previousHold][previousCard], KEY_STR);
        gtk_widget_destroy(tc->cardHoldImages[previousHold][previousCard]);
        tc->cardHoldImages[previousHold][previousCard] = NULL;
        tc->topCardHoldImages[hold] = tc->createCard(card0);
        gtk_fixed_put((GtkFixed *)tc->fixed, tc->topCardHoldImages[hold], 185 + (173 * hold), 40);
                
        gtk_widget_show_all(tc->topCardHoldImages[hold]);
        return;
    }
}

void ThreesCompany::takeScorePressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    ThreesCompany *tc(reinterpret_cast<ThreesCompany *>(ptr));
//     gtk_image_set_from_file((GtkImage *)gtk_bin_get_child((GtkBin *)widget), 
//                                 "/usr/local/share/touchmegames/threescompany/images/takescoredown.xpm");
    tc->reloadImage(gtk_bin_get_child((GtkBin *)widget), "takescoredown.xpm");

}

void ThreesCompany::takeScoreReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    ThreesCompany *tc(reinterpret_cast<ThreesCompany *>(ptr));
    
//     gtk_image_set_from_file((GtkImage *)gtk_bin_get_child((GtkBin *)widget), 
//                             "/usr/local/share/touchmegames/threescompany/images/takescoreup.xpm");
//     tc->reloadImage(gtk_bin_get_child((GtkBin *)widget), "takescoreup.xpm");
    ++tc->round;
    if (tc->round <= 3) {
//         tc->gameState = BETWEEN_ROUNDS;
        tc->startRound();
    }
    else {
        tc->gameState = GAME_OVER;
        tc->exitGame(NULL, (void **)&tc);
    }

}


void ThreesCompany::maskReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    ThreesCompany *tc(reinterpret_cast<ThreesCompany *>(ptr));
    gtk_widget_destroy(tc->cardMask);
    tc->cardMask = NULL;
    tc->shown = false;
}

