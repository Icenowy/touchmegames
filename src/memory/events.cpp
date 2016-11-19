
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

// events for Memory

#include <memory.h>
#include <sstream>
#include <iomanip>

void Memory::exitGame(GtkWidget *widget, gpointer *ptr) {
    Memory *t(reinterpret_cast<Memory *>(*ptr));
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

void Memory::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    Memory *t(reinterpret_cast<Memory *>(ptr));
    
    if (t->quitDialog()) {
        g_source_remove(t->timer);
        // This let's the main class know this game is over
        gtk_widget_hide(t->topFixed);
    }
 
}


gboolean Memory::timeout(gpointer ptr) {
    Memory *m(reinterpret_cast<Memory *>(ptr));
    std::stringstream sstr;
    int second, minute;
    switch (m->gameState) {
        case WAITING:
            // don't do anything, just wait for a press
            break;
        case RUNNING:
            if (m->started == true) {
                --m->timeLeft;
                if (m->timeLeft <= 0) {
                    m->gameState = OUT_OF_TIME;
                    m->stageCountDown = 20;
                    m->playWave("buzzer.wav");
                }
                m->progress.percent((int)m->timeLeft / 6);
                
                second = ((int)(m->timeLeft / 5)) % 60;
                minute = (int)(m->timeLeft / 5) / 60;
                
                sstr << (int)minute << ':' << std::setw(2) << std::setfill('0') << (int)second;
                
                m->progress.text(sstr.str().c_str());
            }
            break;
        case FLIP_CARDS:
            if (--m->flipTicks == 0) {
                m->reloadImage(gtk_bin_get_child((GtkBin *)m->cardsPressed[0]), "back.png");
                m->reloadImage(gtk_bin_get_child((GtkBin *)m->cardsPressed[1]), "back.png");
                m->cardsPressed[0] = 0;
                m->cardsPressed[1] = 0;
                m->gameState = RUNNING;
            }
            else {
                // Keep the timer running
                --m->timeLeft;
                if (m->timeLeft <= 0) {
                    m->gameState = OUT_OF_TIME;
                    m->stageCountDown = 20;
                    m->playWave("buzzer.wav");
                }
                m->progress.percent((int)m->timeLeft / 6);
                
                second = ((int)(m->timeLeft / 5)) % 60;
                minute = (int)(m->timeLeft / 5) / 60;
                
                sstr << (int)minute << ':' << std::setw(2) << std::setfill('0') << (int)second;
                
                m->progress.text(sstr.str().c_str());
            }
            break;
        case TAKE_ROUND_SCORE:
            if (m->stateCntr) {
                --m->stateCntr;
            }
            else {
                m->timeLeft -= 5;
                m->timeBonus += 100;
                
                m->progress.percent((int)m->timeLeft / 6);
            
                second = ((int)(m->timeLeft / 5)) % 60;
                minute = (int)(m->timeLeft / 5) / 60;
            
                sstr << (int)minute << ':' << std::setw(2) << std::setfill('0') << (int)second;
            
                m->progress.text(sstr.str().c_str());
                
                m->updateScores();
                if (m->timeLeft <= 0) {
                    m->gameState = BETWEEN_ROUNDS;
                    m->stateCntr = 8;
                    
                    m->currentScore += m->matchScore - (m->attempts * 250) + m->timeBonus;
                    gtk_label_set_text((GtkLabel *)m->scoreLabel, m->addCommas(m->currentScore).c_str());
                    
                    g_source_remove(m->timer);
                    m->timer = g_timeout_add(200, m->timeout, m);
                }
            }
            break;
        case TAKE_TOTAL_SCORE:
            break;
        case BETWEEN_ROUNDS:
            if (m->stateCntr) {
                --m->stateCntr;
            }
            else {
                m->startRound();
            }
            break;
        case OUT_OF_TIME:
            if (--m->stageCountDown <= 0) {
                m->startRound();
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

void Memory::cardPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    Memory *m(reinterpret_cast<Memory *>(ptr));
    std::stringstream sstr;
    
    m->started = true;
    
    if (m->gameState == RUNNING) {
        sstr << (intptr_t)g_object_get_data((GObject *)widget, KEY_STR) + 1 << ".png";
        if ((intptr_t)g_object_get_data((GObject *)widget, KEY_STR) == -1) {
            return;
        }
        if (m->cardsPressed[0] == 0) {
            m->cardsPressed[0] = widget;
            m->reloadImage(gtk_bin_get_child((GtkBin *)widget), sstr.str().c_str());
        }
        else if (m->cardsPressed[1] == 0 && m->cardsPressed[0] != widget) {
            
            
            m->cardsPressed[1] = widget; // (intptr_t)g_object_get_data((GObject *)widget, KEY_STR) + 1;
            m->reloadImage(gtk_bin_get_child((GtkBin *)widget), sstr.str().c_str());
            
            if ((intptr_t)g_object_get_data((GObject *)m->cardsPressed[0], KEY_STR)
                    == (intptr_t)g_object_get_data((GObject *)m->cardsPressed[1], KEY_STR)) {
                
                // Set the data to -1 so that they can't get chosen again.
                g_object_set_data((GObject *)m->cardsPressed[0], KEY_STR, (void *)-1);
                g_object_set_data((GObject *)m->cardsPressed[1], KEY_STR, (void *)-1);
                
                // Cards match, don't flip
                m->cardsPressed[0] = 0;
                m->cardsPressed[1] = 0;
                
                m->matchScore += 5000 + 1000 * m->round;
                
                m->updateScores();
                
                // Check to see if all the cards are done
                if (++m->matches == m->totalMatches) {
                    m->gameState = TAKE_ROUND_SCORE;
                    
                    // Speed it up
                    g_source_remove(m->timer);
                    m->timer = g_timeout_add(50, m->timeout, m);
                    m->stateCntr = 10;
                }
            }
            else {
                // Round 1 = 1 second
                // Round 2 = 0.8 seconds
                // Round 3 = 0.6 seconds
                m->flipTicks = 6 - m->round;
                m->gameState = FLIP_CARDS;
                
                sstr.str("");
                sstr << ++m->attempts;
                gtk_label_set_text((GtkLabel *)m->attemptLabel, sstr.str().c_str());
                
                m->updateScores();
            }
        }
    }
}
