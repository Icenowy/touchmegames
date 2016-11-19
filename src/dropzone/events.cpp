
/*	Copyright (C) 2008 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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

// events for DropZone

#include <dropzone.h>
#include <sstream>
#include <iomanip>

void DropZone::exitGame(GtkWidget *widget, gpointer *ptr) {
    DropZone *t(reinterpret_cast<DropZone *>(*ptr));
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

void DropZone::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    DropZone *t(reinterpret_cast<DropZone *>(ptr));
    
    if (t->quitDialog()) {
        g_source_remove(t->timer);
        // This let's the main class know this game is over
        gtk_widget_hide(t->topFixed);
    }
 
}


gboolean DropZone::timeout(gpointer ptr) {
    DropZone *t(reinterpret_cast<DropZone *>(ptr));
    std::stringstream sstr;

    switch (t->gameState) {
        case WAITING:
            // don't do anything, just wait for a press
            break;
        case RUNNING:
            
            if (--t->timeLeft == 0) {
                t->moveRiser();
            }
            else if (((t->totalTime - t->timeLeft) % (t->totalTime / 8)) == 0) {
                //std::cout << "Rise: " << (t->totalTime - t->timeLeft)  (t->totalTime / 8)) << std::endl;
                t->moveRiser();
            }
            if (t->gameState == RUNNING) {
                t->progress.percent((t->timeLeft * 100) / t->totalTime);
                sstr << (t->timeLeft / 300) << ':' << std::setw(2) << std::setfill('0') << (t->timeLeft / 5) % 60;
                t->progress.text(sstr.str());
            }
            else {
                t->progress.percent(0);
                t->progress.text("0:00");
            }
            break;
        case BLINKING:
            if (--t->blinkCntr == 0) {
                int i;
                t->gameState = RUNNING;
                for (i = 0; i < 100 && t->d[i].x != -1; ++i) {
                    int x(t->d[i].x), y(t->d[i].y);
                    gtk_widget_show(t->discs[x][y]);
                    g_object_set_data((GObject *)t->discs[x][y], KEY_STR, (gpointer)0);
                    t->reloadImage(t->discs[x][y], "discb.png");
                    t->discLocations[x][y] = false;
                }
                // this is in interface.cpp
                t->updateScore((i - 1) * t->secondDrop * t->round);
                if (t->checkDropped() == 0) {
                    t->checkForFallthrough();
                }
                // gets cleared back to 100 on button press event
                t->secondDrop = 150;
                if (t->gameState == RUNNING) { 
                    // if we dropped into another remove
                    // it will change the game state to BLINKING
                    // this function will change it to either a new level
                    // or end of game.
                    t->checkEndOfLevel();
                }
            }
            else {
                if ((t->blinkCntr % 2) == 0) {
                    for (int i = 0; i < 100 && t->d[i].x != -1; ++i) {
                        gtk_widget_hide(t->discs[t->d[i].x][t->d[i].y]);
                    }
                }
                else { //if ((t->blinkCntr % 2) == 0) {
                    for (int i = 0; i < 100 && t->d[i].x != -1; ++i) {

                        gtk_widget_show(t->discs[t->d[i].x][t->d[i].y]);
                    }
                }
            }
            break;
        case TAKE_ROUND_SCORE:
            break;
        case TAKE_TOTAL_SCORE:
            break;
        case BETWEEN_ROUNDS:
            t->timeLeft -= 5;
            if (t->timeLeft <= 0) {
                t->updateScore((50 + t->timeLeft) * t->round);
                t->timeLeft = 0;
                g_source_remove(t->timer);
                t->timer = g_timeout_add(200, t->timeout, t);
                t->startRound();
            }
            else {
                t->updateScore(50 * t->round);
                t->progress.percent((t->timeLeft * 100) / t->totalTime);
                sstr << (t->timeLeft / 300) << ':' << std::setw(2) << std::setfill('0') << (t->timeLeft / 5) % 60;
                t->progress.text(sstr.str());
            }
            break;
        case OUT_OF_TIME:
            if (--t->timeLeft == 0) {
                t->gameState = GAME_OVER;
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

void DropZone::pauseGame(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    DropZone *dz(reinterpret_cast<DropZone *>(ptr));
    
    if (dz->gameState == RUNNING) {
        dz->gameState = PAUSED;
        dz->reloadMainImage(dz->pauseButton, "buttons/button_play.png");
        
        for (int y = 0; y < dz->numRows; ++y) {
            for (int x = 0; x < dz->numColumns; ++x) {
                gtk_widget_hide(dz->discs[x][y]);
            }
        }
        gtk_widget_hide(dz->nextDisc);
        gtk_widget_hide(dz->onDeck);
        
    }
    else if (dz->gameState == PAUSED) {
        dz->gameState = RUNNING;
        dz->reloadMainImage(dz->pauseButton, "buttons/button_pause.png");
        for (int y = 0; y < dz->numRows; ++y) {
            for (int x = 0; x < dz->numColumns; ++x) {
                gtk_widget_show(dz->discs[x][y]);
            }
        }
        gtk_widget_show(dz->nextDisc);
        gtk_widget_show(dz->onDeck);
    }
    
}

void DropZone::playAreaPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    DropZone *u(reinterpret_cast<DropZone *>(ptr));
    
    if (u->gameState == BLINKING) {
        int i;
        u->gameState = RUNNING;
        for (i = 0; i < 100 && u->d[i].x != -1; ++i) {
            int x(u->d[i].x), y(u->d[i].y);
            gtk_widget_show(u->discs[x][y]);
            g_object_set_data((GObject *)u->discs[x][y], KEY_STR, (gpointer)0);
            u->reloadImage(u->discs[x][y], "discb.png");
            u->discLocations[x][y] = false;
        }
                // this is in interface.cpp
        u->updateScore((i - 1) * u->secondDrop * u->round);
        if (u->checkDropped() == 0) {
            u->checkForFallthrough();
        }
                // gets cleared back to 100 on button press event
        u->secondDrop = 150;
        if (u->gameState == RUNNING) { 
                    // if we dropped into another remove
                    // it will change the game state to BLINKING
                    // this function will change it to either a new level
                    // or end of game.
            u->checkEndOfLevel();
        }
    }
    
    if (u->gameState != RUNNING) {
        return;
    }
    if (event->type != GDK_BUTTON_PRESS) {
        return;
    }
    
    u->secondDrop = 100;
    // check for single/double click!!
    if (event->x < (u->discRadius / 2)) {
        int x(0), y(0);
        u->dropDisc(x, -1, y, 0);
    }
    else {
        int x, y;
        gdouble n((event->x / (gdouble)u->discRadius) - 0.5);
        
//         std::cout << "N " << n << std::endl;
//         std::cout << "Offset " << (int)((event->x / u->discRadius)) - n << std::endl;
        x = (int)n;
        y = 0;
        u->dropDisc(x, (int)((event->x / u->discRadius)) - n, y, 0);
    }
    
    
}
