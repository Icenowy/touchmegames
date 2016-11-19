
/*	Copyright (C) 2005 - 2008 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02011-1307  USA
 */

// events for FortressCrackers

#include <fortresscrackers.h>
#include <sstream>
#include <iomanip>

void FortressCrackers::exitGame(GtkWidget *widget, gpointer *ptr) {
    FortressCrackers *t(reinterpret_cast<FortressCrackers *>(*ptr));
    SCORE_STRUCT newScore;
    
    newScore.name = "";
    newScore.score = t->gameScore;
    newScore.date = "";
    
    
    t->GAME_PTR::addScore(t->scores, newScore);
    
    t->GAME_PTR::saveScores(t->scores);
    
    g_source_remove(t->timer);
    
    // This let's the main class know this game is over
    gtk_widget_hide(t->topFixed);
}

void FortressCrackers::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    FortressCrackers *t(reinterpret_cast<FortressCrackers *>(ptr));
    
    if (t->quitDialog()) {
        g_source_remove(t->timer);
        // This let's the main class know this game is over
        gtk_widget_hide(t->topFixed);
    }
 
}


gboolean FortressCrackers::timeout(gpointer ptr) {
    FortressCrackers *f(reinterpret_cast<FortressCrackers *>(ptr));
    int second, minute;
    std::stringstream sstr;

    switch (f->gameState) {
        case INIT_1:
            f->showTop();
            f->gameState = WAITING;
            break;
        case WAITING:
            // don't do anything, just wait for a press
            break;
        case RUNNING:
            --f->timeLeft;
            if (f->timeLeft <= 0) {
                f->gameState = OUT_OF_TIME;
                f->stageCountDown = 20;
                f->playWave("buzzer.wav");
            }
            f->progress.percent((int)f->timeLeft / 6);
            
            second = ((int)(f->timeLeft / 5)) % 60;
            minute = (int)(f->timeLeft / 5) / 60;
            
            sstr << (int)minute << ':' << std::setw(2) << std::setfill('0') << (int)second;
            
            f->progress.text(sstr.str().c_str());
            break;
        case TAKE_WILD_SCORE:
            if (GTK_WIDGET_VISIBLE(f->userCard->parent) == TRUE) {
                // Hide the card and show the bonus[4]
                gtk_widget_hide(f->userCard->parent);
                
                gtk_widget_show(f->bonus[4]);
                gtk_fixed_move((GtkFixed *)f->topFixed, f->bonus[4], 
                               f->xpadding + f->cardOffset * 6 - (f->bonusWidth / 2), 540 - f->bonusHeight);
                
                f->cardBonus += 5000;
                gtk_label_set_text((GtkLabel *)f->cardScoreLabel, f->addCommas(f->cardBonus).c_str());
            }
            f->gameState = TAKE_CARD_SCORE;
            break;
        case TAKE_CARD_SCORE:
            
            if (--f->stageCountDown <= 0) {
                if (f->cardPosition < 52) {
                    f->cardBonus += 1000;
                    gtk_label_set_text((GtkLabel *)f->cardScoreLabel, f->addCommas(f->cardBonus).c_str());
                    
                    sstr << f->deck[f->cardPosition++] + 1 << ".png";
                    f->reloadImage(f->dealCard, sstr.str().c_str());
                    
                    sstr.str("");
                    sstr << 52 - f->cardPosition;
                    gtk_label_set_text((GtkLabel *)f->cardsLeftLabel, sstr.str().c_str());
                    
                }
                else {
                    // Speed up the timer
                    g_source_remove(f->timer);
                    f->timer = g_timeout_add(50, f->timeout, f);
                    f->gameState = TAKE_TIME_SCORE;
                }
                f->stageCountDown = 2;
            }
            break;
        case TAKE_TIME_SCORE:
            f->timeLeft -= 5;
            if (f->timeLeft >= 0) {
                f->timeBonus += 50;
                gtk_label_set_text((GtkLabel *)f->timeScoreLabel, f->addCommas(f->timeBonus).c_str());
                
                f->progress.percent((int)f->timeLeft / 6);
            
                second = ((int)(f->timeLeft / 5)) % 60;
                minute = (int)(f->timeLeft / 5) / 60;
            
                sstr << (int)minute << ':' << std::setw(2) << std::setfill('0') << (int)second;
            
                f->progress.text(sstr.str().c_str());
            }
            else {
                // Slow down the timer
                g_source_remove(f->timer);
                f->timer = g_timeout_add(200, f->timeout, f);
                f->gameState = TAKE_TOTAL_SCORE;
            }
            break;
        case TAKE_TOTAL_SCORE:
            f->currentScore = f->roundScore + f->cardBonus + f->timeBonus;
            gtk_label_set_text((GtkLabel *)f->totalScoreLabel, f->addCommas(f->currentScore).c_str());
            f->stageCountDown = 10;
            f->gameState = BETWEEN_ROUNDS;
            break;
        case BETWEEN_ROUNDS:
            if (f->stageCountDown == 10) {
                f->gameScore += f->currentScore;
                gtk_label_set_text((GtkLabel *)f->scoreLabel, f->addCommas(f->gameScore).c_str());
            }
            if (--f->stageCountDown == 0) {
                f->startRound();
            }
            break;
        case OUT_OF_TIME:
            if (--f->stageCountDown <= 0) {
                f->startRound();
            }
            break;
        case GAME_OVER:
            exitGame(NULL, &ptr);
            return FALSE;
            break;
        default:
            break;
    }
    
    return TRUE;
}


void FortressCrackers::centerPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    FortressCrackers *f(reinterpret_cast<FortressCrackers *>(ptr));
    
    // No double/triple clicks
    if (event->type != GDK_BUTTON_PRESS) {
        return;
    }
    
    if (f->gameState > RUNNING) {
        return;
    }
    else if (f->gameState == WAITING) {
        f->gameState = RUNNING;
    }
    
    // Check to see if it's on the top of the pile
    if (f->isOnTop(gtk_bin_get_child((GtkBin *)widget)) == false) {
        return;
    }
    
    if (f->isValid(gtk_bin_get_child((GtkBin *)widget))) {
        gtk_widget_hide(widget);
        f->showTop();
    }
}

void FortressCrackers::dealPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    FortressCrackers *f(reinterpret_cast<FortressCrackers *>(ptr));
    
    // No double/triple clicks
    if (event->type != GDK_BUTTON_PRESS) {
        return;
    }
    
    if (f->gameState > RUNNING) {
        return;
    }
    else if (f->gameState == WAITING) {
        f->gameState = RUNNING;
    }
    
    if (f->cardPosition < 52) {
        std::stringstream sstr;
        sstr << f->deck[f->cardPosition++] + 1 << ".png";
        f->reloadImage(f->dealCard, sstr.str().c_str());
        sstr.str("");
        sstr << 52 - f->cardPosition;
        gtk_label_set_text((GtkLabel *)f->cardsLeftLabel, sstr.str().c_str());
    }
    f->streak = 0;
}


void FortressCrackers::userPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    FortressCrackers *f(reinterpret_cast<FortressCrackers *>(ptr));
    gtk_widget_hide(widget);
    f->reloadImage(f->dealCard, "user.png");
    f->deck[f->cardPosition - 1] = -1;
}

void FortressCrackers::takeScore(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    FortressCrackers *f(reinterpret_cast<FortressCrackers *>(ptr));
    int x, y;
    if (f->gameState > RUNNING) {
        return;
    }
    
    y = f->ypadding;
    x = f->xpadding + f->cardOffset * 3 + f->cardWidth / 2 - f->takingScoreWidth / 2;
            
    // Show the taking score image
    gtk_widget_show(f->takingScore);
    gtk_fixed_move((GtkFixed *)f->topFixed, f->takingScore, x, y);
    
    g_source_remove(f->timer);
    f->timer = g_timeout_add(50, f->timeout, f);
    
    f->stageCountDown = 5;
    f->gameState = TAKE_TIME_SCORE;
}
