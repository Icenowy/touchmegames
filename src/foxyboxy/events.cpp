
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

// events for FoxyBoxy
#include <foxyboxy.h>
#include <sstream>
#include <iomanip>
#include <gdk/gdkkeysyms.h>

void FoxyBoxy::exitGame(GtkWidget *widget, gpointer *ptr) {
    FoxyBoxy *foxyboxy(reinterpret_cast<FoxyBoxy *>(*ptr));
    SCORE_STRUCT newScore;
    
    newScore.name = "";
    newScore.score = foxyboxy->currentScore;
    newScore.date = "";
    
    
    foxyboxy->GAME_PTR::addScore(foxyboxy->scores, newScore);
    
    foxyboxy->GAME_PTR::saveScores(foxyboxy->scores);
    
    g_source_remove(foxyboxy->timer);
    gtk_widget_hide(foxyboxy->topFixed);
}

void FoxyBoxy::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    FoxyBoxy *foxyBoxy(reinterpret_cast<FoxyBoxy *>(ptr));

    if (foxyBoxy->quitDialog()) {
        g_source_remove(foxyBoxy->timer); 
        gtk_widget_hide(foxyBoxy->topFixed);
    }
        
}

gboolean FoxyBoxy::timeout(gpointer ptr) {
    FoxyBoxy *fb(reinterpret_cast<FoxyBoxy *>(ptr));
    
    switch (fb->gameState) {
        case RUNNING:
            //gtk_widget_grab_focus(fb->kbEntry);
            if (!fb->moveDownTurn) {
                fb->moveBlockDown();
                fb->moveDownTurn = 3;
            }
            else {
                --fb->moveDownTurn;
            }
            if (!fb->justDidExtraMove) {
                
                if (fb->extraMove == MoveRight && ((fb->moveDownTurn & fb->extra) | fb->extra == 3)) {
                    fb->moveRight(true);
                    fb->extra = 3;
                }
                else if (fb->extraMove == MoveLeft && ((fb->moveDownTurn & fb->extra) | fb->extra == 3)) {
                    fb->moveLeft(true);
                    fb->extra = 3;
                }
                else if (fb->extraMove == MoveDown) {
                    fb->moveBlockDown();
                }
            }
            else {
                    
                fb->justDidExtraMove = false;
            }
            break;
        case FLASH_LINES:
            if (--fb->flashCntr) {
                fb->flashLines();
            }
            else {
                fb->updateScore();
                fb->removeFlashLines();
                fb->createAndShowBlocks();
                fb->gameState = RUNNING;
                g_source_remove(fb->timer);
                fb->timer = g_timeout_add(fb->timeoutValue, fb->timeout, fb);
            }
            break;
        case WAITING_TO_START:
        case PAUSED:
            // nothing to do...I'm bored....press the button!!
            break;
        case GAME_OVER:
            fb->exitGame(NULL, (void **)&fb);
            break;
    }
    
    return TRUE;
}

void FoxyBoxy::pauseGame(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    FoxyBoxy *fb(reinterpret_cast<FoxyBoxy *>(ptr));
    
    if (fb->gameState == RUNNING) {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 22; ++j) {
                gtk_widget_hide(fb->blocks[i][j]);
            }
        }
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 6; ++j) {
                gtk_widget_hide(fb->nextBlocks[i][j]);
            }
        }
        
        fb->gameState = PAUSED;
        fb->reloadMainImage(fb->pauseButton, "buttons/button_play.png");
    }
    else if (fb->gameState == PAUSED) {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 22; ++j) {
                gtk_widget_show(fb->blocks[i][j]);
            }
        }
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 6; ++j) {
                gtk_widget_show(fb->nextBlocks[i][j]);
            }
        }
        
        fb->gameState = RUNNING;
        fb->reloadMainImage(fb->pauseButton, "buttons/button_pause.png");
    }
}

void FoxyBoxy::startButtonPressed(GtkWidget *widget, gpointer ptr) {
    FoxyBoxy *fb(reinterpret_cast<FoxyBoxy *>(ptr));
    
    fb->createAndShowBlocks();
    fb->gameState = RUNNING;
}

gboolean FoxyBoxy::paddlePressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    FoxyBoxy *fb(reinterpret_cast<FoxyBoxy *>(ptr));

    if (event->type == GDK_BUTTON_PRESS && !fb->alreadyPressed) {
        fb->alreadyPressed = true;
        fb->doAction((int)event->x, (int)event->y);
    }
    return TRUE;
}

gboolean FoxyBoxy::paddleReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    FoxyBoxy *fb(reinterpret_cast<FoxyBoxy *>(ptr));
    fb->alreadyPressed = false;
    fb->reloadImage(fb->paddleImage, "paddle.png");
    if (fb->spedUpTimer) {
        fb->spedUpTimer = false;
        if (fb->gameState == RUNNING) {
            g_source_remove(fb->timer);
            fb->timer = g_timeout_add(fb->timeoutValue, fb->timeout, fb);
        }
    }
    fb->extraMove = MoveNone;
}

gboolean FoxyBoxy::keyPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    FoxyBoxy *fb(reinterpret_cast<FoxyBoxy *>(ptr));
    
    // don't let the user do both keydown and mouse clicked
    // and also don't worry about the key repeat
    if (fb->keyDown || fb->mouseClicked) {
        return TRUE;
    }
    fb->keyDown = true;
    if (event->keyval == GDK_Left) {
        fb->moveLeft(false);
        fb->reloadImage(fb->paddleImage, "paddleleft.png");
    }
    else if (event->keyval == GDK_Right) {
        fb->moveRight(false);
        fb->reloadImage(fb->paddleImage, "paddleright.png");
    }
    else if (event->keyval == GDK_Up) {
        fb->rotateRight();
        fb->reloadImage(fb->paddleImage, "paddlerotate.png");
    }
    else if (event->keyval == GDK_Down) {
        fb->moveDown();
        fb->reloadImage(fb->paddleImage, "paddledown.png");
            
    }
    return TRUE;
}

gboolean FoxyBoxy::keyReleased(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    FoxyBoxy *fb(reinterpret_cast<FoxyBoxy *>(ptr));
    fb->keyDown = false;
    
    // let the paddle release take care of the other cleanup
    fb->paddleReleased(NULL, NULL, fb);
    return TRUE;
}
