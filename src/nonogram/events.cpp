
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

// events for NonoGram

#include <nonogram.h>
#include <sstream>
#include <iomanip>

void NonoGram::exitGame(GtkWidget *widget, gpointer *ptr) {
    NonoGram *t(reinterpret_cast<NonoGram *>(*ptr));
    SCORE_STRUCT newScore;
    
    newScore.name = "";
    newScore.score = t->currentScore;
    newScore.date = "";
    
    
    t->GAME_PTR::addScore(t->scores, newScore);
    
    t->GAME_PTR::saveScores(t->scores);
    
    g_source_remove(t->timer);
    
    // This let's the main class know this game is over
    gtk_widget_hide(t->topFixed);
}

void NonoGram::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    NonoGram *t(reinterpret_cast<NonoGram *>(ptr));
    
    if (t->quitDialog()) {
        g_source_remove(t->timer);
        // This let's the main class know this game is over
        gtk_widget_hide(t->topFixed);
    }
 
}


gboolean NonoGram::timeout(gpointer ptr) {
    NonoGram *nonoGram(reinterpret_cast<NonoGram *>(ptr));
    int second, minute;
    std::stringstream sstr;

    switch (nonoGram->gameState) {
        case WAITING:
            // don't do anything, just wait for a press
            break;
        case RUNNING:
            --nonoGram->timeLeft;
            if (nonoGram->timeLeft <= 0) {
                nonoGram->gameState = OUT_OF_TIME1;
                nonoGram->stageCountDown = 30;
                nonoGram->playWave("buzzer.wav");
            }
            nonoGram->progress.percent((int)nonoGram->timeLeft / 6);
            
            second = ((int)(nonoGram->timeLeft / 5)) % 60;
            minute = (int)(nonoGram->timeLeft / 5) / 60;
            
            sstr << (int)minute << ':' << std::setw(2) << std::setfill('0') << (int)second;
            
            nonoGram->progress.text(sstr.str().c_str());
            
            break;
        case TAKE_PUZZLE_SCORE:
            if (nonoGram->round == 1) {
                gtk_label_set_text((GtkLabel *)nonoGram->puzzleScoreLabel, "50,000");
            }
            else if (nonoGram->round == 2) {
                gtk_label_set_text((GtkLabel *)nonoGram->puzzleScoreLabel, "75,000");
            }
            else if (nonoGram->round == 3) {
                gtk_label_set_text((GtkLabel *)nonoGram->puzzleScoreLabel, "100,000");
            }
            nonoGram->gameState = TAKE_TIME_LEFT_SCORE;
            break;
        case TAKE_TIME_LEFT_SCORE:
            gtk_label_set_text((GtkLabel *)nonoGram->timeLeftScoreLabel, nonoGram->addCommas(nonoGram->timeLeft * 150).c_str());
            nonoGram->gameState = TAKE_ROUND_SCORE;
            break;
        case TAKE_ROUND_SCORE: {
            int tmpScore = (nonoGram->round - 1) * 25000 + 50000;
            int score;
            score = tmpScore + nonoGram->timeLeft * 150;
            
            gtk_label_set_text((GtkLabel *)nonoGram->roundScoreLabel, nonoGram->addCommas(score).c_str());
            
            nonoGram->gameState = TAKE_TOTAL_SCORE;
            }
            break;
        case TAKE_TOTAL_SCORE: {
            int tmpScore = (nonoGram->round - 1) * 25000 + 50000;
            
            nonoGram->currentScore += tmpScore + nonoGram->timeLeft * 150;
            gtk_label_set_text((GtkLabel *)nonoGram->totalScoreLabel, nonoGram->addCommas(nonoGram->currentScore).c_str());
            
            gtk_label_set_text((GtkLabel *)nonoGram->scoreLabel, nonoGram->addCommas(nonoGram->currentScore).c_str());
            nonoGram->gameState = BETWEEN_ROUNDS;
            nonoGram->stageCountDown = 15;
            }
            break;
        case BETWEEN_ROUNDS:
            if (--nonoGram->stageCountDown == 0) {
                nonoGram->roundOver();
                nonoGram->startRound();
            }
            break;
        case OUT_OF_TIME1:
            if (nonoGram->stageCountDown == 30) {
                nonoGram->fillBoard();
            }
            if (--nonoGram->stageCountDown == 0) {
                nonoGram->roundOver();
                nonoGram->startRound();
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

gboolean NonoGram::blockClicked(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    NonoGram *nonoGram(reinterpret_cast<NonoGram *>(ptr));
    GtkWidget *image;
    
    // No double/triple clicks
    if (event->type != GDK_BUTTON_PRESS) {
        return FALSE;
    }
    
    // Don't let the user change it once they're done.
    if (nonoGram->gameState != RUNNING) {
        return FALSE;
    }
    
    image = gtk_bin_get_child((GtkBin *)widget);
    switch ((intptr_t)g_object_get_data((GObject  *)widget, KEY_STR)) {
        case BLANK:
            nonoGram->reloadImage(image, "block.png");
            nonoGram->mouseState = FILLED;
            g_object_set_data((GObject  *)widget, KEY_STR, (void *)FILLED);
            break;
        case FILLED:
            nonoGram->reloadImage(image, "X.png");
            nonoGram->mouseState = XED;
            g_object_set_data((GObject  *)widget, KEY_STR, (void *)XED);
            break;
        case XED:
            nonoGram->reloadImage(image, "blockbg.png");
            nonoGram->mouseState = BLANK;
            g_object_set_data((GObject  *)widget, KEY_STR, (void *)BLANK);
            break;
    }
    if (nonoGram->checkBoard(NULL) == true) {
        nonoGram->gameState = TAKE_PUZZLE_SCORE;
    }
    return FALSE;
}


gboolean NonoGram::blockMotion(GtkWidget *widget, GdkEventMotion *event, gpointer ptr) {
    NonoGram *nonoGram(reinterpret_cast<NonoGram *>(ptr));
    int X = (int)(event->x + widget->allocation.x);
    int Y = (int)(event->y + widget->allocation.y);
    GdkRectangle rect;
    
    rect.x = X;
    rect.y = Y;
    rect.width = 1;
    rect.height = 1;
    
    for (int x = 0; x < nonoGram->nonoSize; ++x) {
        for (int y = 0; y < nonoGram->nonoSize; ++y) {
            if (gtk_widget_intersect(nonoGram->cells[x][y], &rect, NULL)) {
                switch (nonoGram->mouseState) {
                    case BLANK:
                        nonoGram->reloadImage(nonoGram->cells[x][y], "blockbg.png");
                        g_object_set_data((GObject  *)gtk_widget_get_parent(nonoGram->cells[x][y]), KEY_STR, (void *)BLANK);
                        break;
                    case FILLED:
                        nonoGram->reloadImage(nonoGram->cells[x][y], "block.png");
                        
                        g_object_set_data((GObject  *)gtk_widget_get_parent(nonoGram->cells[x][y]), KEY_STR, (void *)FILLED);
                        break;
                    case XED:
                        nonoGram->reloadImage(nonoGram->cells[x][y], "X.png");
                        
                        g_object_set_data((GObject  *)gtk_widget_get_parent(nonoGram->cells[x][y]), KEY_STR, (void *)XED);
                        break;
                }
            }
        }
    }
    
    return FALSE;
}
