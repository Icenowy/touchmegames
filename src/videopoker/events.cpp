
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

// events for VideoPoker

#include <videopoker.h>
#include <sstream>
#include <iomanip>

void VideoPoker::exitGame(GtkWidget *widget, gpointer *ptr) {
    VideoPoker *videopoker(reinterpret_cast<VideoPoker *>(*ptr));
    SCORE_STRUCT newScore;
    
    newScore.name = "";
    newScore.score = videopoker->currentScore;
    newScore.date = "";
    
    
    videopoker->GAME_PTR::addScore(videopoker->scores, newScore);
    
    videopoker->GAME_PTR::saveScores(videopoker->scores);
    
    g_source_remove(videopoker->timer);
    gtk_widget_hide(videopoker->topfixed);
}

void VideoPoker::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    VideoPoker *v(reinterpret_cast<VideoPoker *>(ptr));
        
        if (v->quitDialog()) {
            g_source_remove(v->timer);
            gtk_widget_hide(v->topfixed);
        }
}

gboolean VideoPoker::timeout(gpointer ptr) {
    VideoPoker *vp(reinterpret_cast<VideoPoker *>(ptr));
    
    switch (vp->gameState) {
        case SELECT_BET:
        break;
        case DEAL_PRESSED_1: {
                std::stringstream sstr;
                
                vp->cardFront[vp->cardLocation] = true;
                vp->cardNumber[vp->cardLocation] = vp->cardDeck[vp->cardLocation];
                sstr << vp->deckLocation << vp->cardDeck[vp->cardLocation++] + 1 << ".gif";
                gtk_image_set_from_file((GtkImage *)vp->cardImages[vp->currentFlipCard], sstr.str().c_str());
                gtk_widget_show(vp->cardImages[vp->currentFlipCard]);
                
                if (++vp->currentFlipCard == 5) {
                        vp->gameState = SELECT_CARDS;
                        gtk_label_set_text((GtkLabel *)vp->instructionLabel, "Select cards to change.");
                }
        }
        break;
        case SELECT_CARDS:
        break;
        case DEAL_PRESSED_2: {
                std::stringstream sstr;
                if (vp->cardFront[vp->currentFlipCard] == false) {
                        vp->cardFront[vp->currentFlipCard] = true;
                        
                        vp->cardNumber[vp->currentFlipCard] = vp->cardDeck[vp->cardLocation];
                        
                        sstr << vp->deckLocation << vp->cardDeck[vp->cardLocation++] + 1 << ".gif";
                        gtk_image_set_from_file((GtkImage *)vp->cardImages[vp->currentFlipCard], sstr.str().c_str());
                        gtk_widget_show(vp->cardImages[vp->currentFlipCard]);
                }

                
                if (++vp->currentFlipCard == 5) {
                    std::string str;
                        switch (vp->getHand()) {
                                case ROYAL_FLUSH:
                                        vp->currentScore += vp->currentBet * 250;
                                        
                                        sstr.str("");
                                        sstr << "Royal Flush!! Pays " 
                                                        << vp->currentBet * 250;
                                        gtk_label_set_text((GtkLabel *)vp->instructionLabel, 
                                                        sstr.str().c_str());
                                        break;
                                case FOUR_OF_A_KIND:
                                        vp->currentScore += vp->currentBet * 50;
                                        sstr.str("");
                                        sstr << "Four Of a Kind!! Pays " 
                                                        << vp->currentBet * 50;
                                        gtk_label_set_text((GtkLabel *)vp->instructionLabel, 
                                                        sstr.str().c_str());
                                        break;
                                case STRAIGHT_FLUSH:
                                        vp->currentScore += vp->currentBet * 25;
                                        sstr.str("");
                                        sstr << "Straight Flush!! Pays " 
                                                        << vp->currentBet * 25;
                                        gtk_label_set_text((GtkLabel *)vp->instructionLabel, 
                                                        sstr.str().c_str());
                                        break;
                                case FULL_HOUSE:
                                        vp->currentScore += vp->currentBet * 9;
                                        sstr.str("");
                                        sstr << "Full house!! Pays " 
                                                        << vp->currentBet * 9;
                                        gtk_label_set_text((GtkLabel *)vp->instructionLabel, 
                                                        sstr.str().c_str());
                                        break;
                                case FLUSH:
                                        vp->currentScore += vp->currentBet * 6;
                                        sstr.str("");
                                        sstr << "Flush!! Pays " 
                                                        << vp->currentBet * 6;
                                        gtk_label_set_text((GtkLabel *)vp->instructionLabel, 
                                                        sstr.str().c_str());
                                        break;
                                case STRAIGHT:
                                        vp->currentScore += vp->currentBet * 4;
                                        sstr.str("");
                                        sstr << "Straight!! Pays " 
                                                        << vp->currentBet * 4;
                                        gtk_label_set_text((GtkLabel *)vp->instructionLabel, 
                                                        sstr.str().c_str());
                                        break;
                                case TRIPS:
                                        vp->currentScore += vp->currentBet * 3;
                                        sstr.str("");
                                        sstr << "Trips!! Pays " 
                                                        << vp->currentBet * 3;
                                        gtk_label_set_text((GtkLabel *)vp->instructionLabel, 
                                                        sstr.str().c_str());
                                        break;
                                case TWO_PAIR:
                                        vp->currentScore += vp->currentBet * 2;
                                        sstr.str("");
                                        sstr << "Two Pair!! Pays " 
                                                        << vp->currentBet * 2;
                                        gtk_label_set_text((GtkLabel *)vp->instructionLabel, 
                                                        sstr.str().c_str());
                                        break;
                                case PAIR:
                                        vp->currentScore += vp->currentBet * 1;
                                        sstr.str("");
                                        sstr << "Pair!! Pays " 
                                                        << vp->currentBet * 1;
                                        gtk_label_set_text((GtkLabel *)vp->instructionLabel, 
                                                        sstr.str().c_str());
                                        break;
                                case HIGH_CARD:
                                        vp->currentScore += vp->currentBet * 0;
                                        sstr.str("");
                                        sstr << "Nothing.";
                                        gtk_label_set_text((GtkLabel *)vp->instructionLabel, 
                                                        sstr.str().c_str());
                                        break;
                        }
                        sstr.str("");
                        sstr << "$ " << vp->addCommas(vp->currentScore);
                        str = sstr.str();
                        sstr.str("");
                        sstr << std::setw(10) << std::setfill(' ') << str;
                        //sstr << std::setw(10) << "$  " << vp->currentScore;
                        gtk_label_set_text((GtkLabel *)vp->scoreLabel, sstr.str().c_str());
                        vp->gameState = HAND_OVER;
                        vp->handOverPause = 20; // 3 seconds
                }
        }
        break;
        case HAND_OVER:
                if (--vp->handOverPause != 0) {
                        return TRUE;
                }
                if (++vp->hand == 21) {
                        vp->exitGame(NULL, (void **)&vp);
                }
                else {
                        GdkColor lcolor;
                        std::string str;
                        std::stringstream sstr;
                        lcolor.red = 0xEEEE;
                        lcolor.green = 0xEEEE;
                        lcolor.blue = 0xEEEE;
                        
                        
                        sstr << vp->hand << " of 20";
                        str = sstr.str();
                        sstr.str("");
                        sstr << std::setw(10) << std::setfill(' ') << str;
                        
                        gtk_label_set_text((GtkLabel *)vp->handLabel, sstr.str().c_str());
                        vp->currentFlipCard = 0;
                        vp->cardLocation = 0;
                        vp->startHand();
                        for (int i = 0; i < 5; ++i) {
                            //vp->betButtons[i].changeState(TmgToggleButton::UP);
                        }
                        gtk_label_set_text((GtkLabel *)vp->instructionLabel,
                                        "Select a bet and press the deal button.");
                        vp->gameState = SELECT_BET;
                        
                        for (int i = 0; i < 5; ++i) {
                                gtk_image_set_from_file((GtkImage *)vp->cardImages[i], vp->deckBack.c_str());
                                vp->cardFront[i] = false;
                                
                                gtk_widget_show(vp->cardImages[i]);
                        }
                }
            break;
            case GAME_OVER:
            break;
    }
    
    return TRUE;
}

void VideoPoker::betPlaced(void *first, void *second) {
    VideoPoker *vp(reinterpret_cast<VideoPoker *>(first));
    if (!vp->toggleOccuring) {
        vp->toggleOccuring = true;
    }
    else {
        return;
    }
    
    if (vp->gameState == SELECT_BET) {
        for (int i = 0; i < 5; ++i) {
            if (second == &vp->betButtons[i] && vp->currentBet / 5 - 1 != i) {
                vp->betButtons[vp->currentBet / 5 - 1].changeState(TmgToggleButton::UP);
                vp->currentBet = (i + 1) * 5;
            }
            else if (second == &vp->betButtons[i]) {
                vp->betButtons[vp->currentBet / 5 - 1].changeState(TmgToggleButton::DOWN);
            }
        }
    }
    else {
        TmgToggleButton *toggle((TmgToggleButton *)second);

        toggle->changeState(toggle->getState() == TmgToggleButton::UP ? TmgToggleButton::DOWN: TmgToggleButton::UP);
    }
    vp->toggleOccuring = false;
}

void VideoPoker::cardSelected(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    VideoPoker *vp(reinterpret_cast<VideoPoker *>(ptr));
    int i = 0;
    if (vp->gameState != SELECT_CARDS) {
        return;
    }
    while (i < 5 && gtk_widget_get_parent(vp->cardImages[i]) != widget) {
        ++i;
    }
    if (i == 5) {
        return;
    }
    
    if (vp->cardFront[i]) {
        gtk_image_set_from_file((GtkImage *)vp->cardImages[i], vp->deckBack.c_str());
        vp->cardFront[i] = false;
    }
    else {
        std::stringstream sstr;
        vp->cardFront[i] = true;
        sstr << vp->deckLocation << vp->cardNumber[i] + 1 << ".gif";
        
        gtk_image_set_from_file((GtkImage *)vp->cardImages[i], sstr.str().c_str());
    }
    gtk_widget_show(vp->cardImages[i]);
}

void VideoPoker::dealPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    VideoPoker *vp(reinterpret_cast<VideoPoker *>(ptr));

    switch (vp->gameState) {
        case SELECT_BET: {
            std::string str;
            std::stringstream sstr;
            GdkColor dcolor, lcolor;
            dcolor.red = 0x8888;
            dcolor.green = 0x8888;
            dcolor.blue = 0x8888;
            lcolor.red = 0xAAAA;
            lcolor.green = 0xAAAA;
            lcolor.blue = 0xAAAA;
            
            

            for (int i = 0; i < 5; ++i) {
                if (vp->currentBet / 5 - 1 != i) {
                    vp->betButtons[i].changeState(TmgToggleButton::UP);
                }
                else {
                    vp->betButtons[i].changeState(TmgToggleButton::DOWN);
                }
            }
            
            vp->currentScore -= vp->currentBet;
            sstr << "$ " << vp->addCommas(vp->currentScore);
            str = sstr.str();
            sstr.str("");
            sstr << std::setw(10) << std::setfill(' ') << str;
            vp->currentFlipCard = 0;
            
            vp->gameState = DEAL_PRESSED_1;
        }
        break;
        case DEAL_PRESSED_1:
        break;
        case SELECT_CARDS:
                vp->gameState = DEAL_PRESSED_2;
                vp->currentFlipCard = 0;
        break;
        case DEAL_PRESSED_2:
        break;
        case HAND_OVER:
                vp->handOverPause = 1;
        break;
        case GAME_OVER:
        break;
    }
    
}
