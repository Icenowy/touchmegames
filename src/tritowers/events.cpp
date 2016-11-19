
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

// events for TriTowers

#include <tritowers.h>
#include <sstream>
#include <iomanip>

void TriTowers::exitGame(GtkWidget *widget, gpointer *ptr) {
    TriTowers *tritowers(reinterpret_cast<TriTowers *>(*ptr));
    SCORE_STRUCT newScore;
    
    newScore.name = "";
    newScore.score = tritowers->totalScore;
    newScore.date = "";
    
    
    tritowers->GAME_PTR::addScore(tritowers->scores, newScore);
    
    tritowers->GAME_PTR::saveScores(tritowers->scores);
    
    g_source_remove(tritowers->timer);
    gtk_widget_hide(tritowers->topFixed);
}

void TriTowers::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    TriTowers *tritowers(reinterpret_cast<TriTowers *>(ptr));
    
    if (tritowers->quitDialog()) {
        g_source_remove(tritowers->timer);
        gtk_widget_hide(tritowers->topFixed);
    }
    
}


gboolean TriTowers::timeout(gpointer ptr) {
    TriTowers *tt(reinterpret_cast<TriTowers *>(ptr));
    std::stringstream sstr;
    
    switch (tt->gameState) {
        case PLAY_ROUND:
            if (!tt->waitingForFirstClick) {
                tt->timeLeft -= 1.0 / 1200.0;
                if (tt->timeLeft < 0.0) {
                    tt->timeLeft = 0.0;
                    tt->gameState = OUT_OF_TIME;
                }
                else {
                    gtk_progress_bar_set_fraction((GtkProgressBar *)tt->timeDealie, tt->timeLeft);
                }
            }
        break;
        case BETWEEN_ROUNDS:
            tt->timeLeft -= 1.0 / 1200.0;
            if (tt->timeLeft < 0.0) {
                tt->gameState = COUNT_CARDS;
                g_source_remove(tt->timer);
                tt->timer = g_timeout_add(300, tt->timeout, tt);
            }
            else {
                gtk_progress_bar_set_fraction((GtkProgressBar *)tt->timeDealie, tt->timeLeft);
                tt->totalScore += 1;
                sstr << tt->addCommas(tt->totalScore);
                gtk_label_set_text((GtkLabel *)tt->scoreLabel, sstr.str().c_str());
            }
                    
            break;
        case COUNT_CARDS:
            if (52 - tt->cardLocation > 0) {
                ++tt->cardLocation;
                sstr.str("");
                sstr << std::setw(2) << std::setfill('0') 
                                << 52 - tt->cardLocation << " Cards Remaining";
                gtk_label_set_text((GtkLabel *)tt->remainingLabel, sstr.str().c_str());
                tt->totalScore += 1000;
                sstr.str("");
                sstr << tt->addCommas(tt->totalScore);
                gtk_label_set_text((GtkLabel *)tt->scoreLabel, sstr.str().c_str());
            }
            else if (tt->round < 3) {
                tt->timeLeft = 0.0;
                ++tt->round;
                g_source_remove(tt->timer);
                tt->timer = g_timeout_add(100, tt->timeout, tt);
                tt->startRound();
            }
            else {
                // call it game over?
                tt->gameState = GAME_OVER;
                tt->exitGame(NULL, (void **)&tt);
            }
        break;
        case DROPPING_SWORD:
            //if (tt->swordy > 50) {
                switch (tt->bonusLevel - 1) {
                    case 0:
                        gtk_fixed_move((GtkFixed *)tt->centerFixed,
                                        tt->bonusImages[0], tt->swordx - 77, 150);
                        break;
                    case 1:
                        gtk_fixed_move((GtkFixed *)tt->centerFixed,
                                        tt->bonusImages[1], tt->swordx - 77, 150);
                        break;
                    case 2:
                        gtk_fixed_move((GtkFixed *)tt->centerFixed,
                                        tt->bonusImages[2], tt->swordx - 77, 150);
                        break;
                }
                if (tt->checkWinner()) {
                    // automagic here?
                    tt->gameState = BETWEEN_ROUNDS;
                    gtk_fixed_move((GtkFixed *)tt->centerFixed,
                                            tt->addingBonus, 50, 220);
                    g_source_remove(tt->timer);
                    tt->timer = g_timeout_add(10, tt->timeout, tt);
                }
                else {
                    tt->gameState = PLAY_ROUND;
                    // slow the timer back to the old rate
                    g_source_remove(tt->timer);
                    tt->timer = g_timeout_add(100, tt->timeout, tt);
                }
            //}
            //else {
            //        tt->swordy += 10;
            //        gtk_fixed_move((GtkFixed *)tt->centerFixed, tt->swordImage,
            //                        tt->swordx, tt->swordy);
            //}
        break;
        case OUT_OF_TIME:
            
            // need to get this out of time going...perhaps a penalty.
            tt->takeScoreReleased(NULL, NULL, tt);
        break;
        case GAME_OVER:
        break;
    }
    return TRUE;
}

int TriTowers::getArrayPosition(int row, int card) {
    switch(row) {
        case 0: return card;
        case 1: return 3 + card;
        case 2: return 9 + card;
        case 3: return 18 + card;
    }
    return -1;
}

int TriTowers::cardIn(int row, int card) {
    int location(getArrayPosition(row, card));
    
    if (dispCards[location] != NULL) {
        return 1;
    }
    
    return 0;
}

int TriTowers::checkValidSelect(GtkWidget *widget) {
    int row, card;
    GtkWidget **location;
    location = std::find(dispCards, &dispCards[30], ((GtkBin *)widget)->child);
    if (location == NULL) {
        return 0;
    }
    card = location - dispCards;
    if (card < 0) {
        return 0;
    }
    row = getRow(card);

    
    card = getCard(card);
    
    if (row == 3) {
        return 1;
    }
    
    ++row;
    if (row < 3) {
        if (cardIn(row, card + card / row) || cardIn(row, card + card / row + 1)) {
            return 0;
        }
    }
    else {
        if (cardIn(row, card) ||  cardIn(row, card + 1)) {
            return 0;
        }
    }
    return 1;
}

void TriTowers::cardSelected(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    TriTowers *tt(reinterpret_cast<TriTowers *>(ptr));
    GtkWidget **location;
    int row, card;
    tt->waitingForFirstClick = 0;
    gtk_widget_hide(tt->cardScore);
    
    if (tt->gameState != PLAY_ROUND) {
        return;
    }
    
    location = std::find(tt->dispCards, &tt->dispCards[30], ((GtkBin*)widget)->child);
    if (location != NULL) {
        row = tt->getRow(location - tt->dispCards);
        card = tt->getCard(location - tt->dispCards);
    }
    
    if (tt->checkValidSelect(widget) == 0) {
        return;
    }
    
    tt->checkCards(tt->dealCardBox, widget);
}

void TriTowers::dealPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    TriTowers *tt(reinterpret_cast<TriTowers *>(ptr));
    tt->waitingForFirstClick = 0;
    tt->run = 0;
    gtk_widget_hide(tt->cardScore);
    
    if (tt->gameState != PLAY_ROUND) {
            return;
    }
    
    // grab the next card, if there is one
    if (tt->cardLocation < 52) {
        // ok, so you want a new card, huh?
        // delete the old card
        gtk_widget_destroy(tt->dealCard);

        std::stringstream sstr;
        sstr << tt->deckLocation << tt->cardDeck[tt->cardLocation++] + 1 << ".png";
        tt->dealCard = tt->loadMainImage(sstr.str().c_str());
        gtk_container_add((GtkContainer *)tt->dealCardBox, tt->dealCard);
        
        // just a little cheat
        g_object_set_data((GObject  *)tt->dealCardBox, KEY_STR, (void *)tt->cardDeck[tt->cardLocation - 1]);
        gtk_widget_show(tt->dealCard);
        
        // update the cards remaining
        sstr.str("");
        sstr << std::setw(2) << std::setfill('0') << 52 - tt->cardLocation << " Cards Remaining";
        gtk_label_set_text((GtkLabel *)tt->remainingLabel, sstr.str().c_str());
        
    }
}

void TriTowers::cardReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    TriTowers *tc(reinterpret_cast<TriTowers *>(ptr));
}

void TriTowers::takeScorePressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
// gtk_image_set_from_file((GtkImage *)gtk_bin_get_child((GtkBin *)widget), 
//  "/usr/local/share/touchmegames/tritowers/images/takescoredown.xpm");
	
}

void TriTowers::takeScoreReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    TriTowers *tc(reinterpret_cast<TriTowers *>(ptr));
    if (widget != NULL) {
// 	gtk_image_set_from_file((GtkImage *)gtk_bin_get_child((GtkBin *)widget), 
// 			 "/usr/local/share/touchmegames/tritowers/images/takescoreup.xpm");
    }
    if (tc->gameState == PLAY_ROUND || widget == NULL) {
        ++tc->round;
        if (tc->round <= 3) {
            tc->startRound();
        }
        else {
            tc->gameState = GAME_OVER;
            tc->exitGame(NULL, (void **)&tc);
        }
    }
    
}

