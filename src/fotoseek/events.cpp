
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

// events for FotoSeek

#include <fotoseek.h>
#include <sstream>
#include <iomanip>

void FotoSeek::exitGame(GtkWidget *widget, gpointer *ptr) {
    FotoSeek *fs(reinterpret_cast<FotoSeek *>(*ptr));
    SCORE_STRUCT newScore;
    
    newScore.name = "";
    newScore.score = fs->currentScore;
    newScore.date = "";
    
    
    fs->GAME_PTR::addScore(fs->scores, newScore);
    
    fs->GAME_PTR::saveScores(fs->scores);
    
    g_source_remove(fs->timer);
    
    delete fs->usedDiffs;
    
    // This let's the main class know this game is over
    gtk_widget_hide(fs->topFixed);
}

void FotoSeek::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    FotoSeek *fs(reinterpret_cast<FotoSeek *>(ptr));
    
    if (fs->quitDialog()) {
        g_source_remove(fs->timer);
        delete fs->usedDiffs;
        // This let's the main class know this game is over
        gtk_widget_hide(fs->topFixed);
    }
 
}


gboolean FotoSeek::timeout(gpointer ptr) {
    FotoSeek *fs(reinterpret_cast<FotoSeek *>(ptr));
    std::stringstream sstr;

    switch (fs->gameState) {
        case WAITING:
            // don't do anything, just wait for a press
            break;
        case RUNNING:
            if (--fs->timeLeft <= 0) {
                fs->paintUnfound();
                fs->betweenRound = 5 * fs->ticksPerSecond;
                fs->gameState = BETWEEN_ROUNDS;
                fs->progress.percent(0);
//                 sstr << 0 << ':' << std::setw(2) << std::setfill('0') << 0;
//                 fs->progress.text(sstr.str());
                break;
            }
            
            fs->progress.percent((fs->timeLeft * 100) / fs->totalTime);
//             sstr << (fs->timeLeft / 300) << ':' << std::setw(2) << std::setfill('0') << (fs->timeLeft / fs->ticksPerSecond) % 60;
//             fs->progress.text(sstr.str());
            
            if (fs->pointsLeft > 100) {
                --fs->pointsLeft;
                fs->points.percent((fs->pointsLeft * 100) / fs->totalPoints);
                sstr.str("");
                sstr << fs->pointsLeft;
                fs->points.text(sstr.str());
            }
            
            break;
        case TAKE_ROUND_SCORE:
            if (fs->timeLeft <= 0) {
                
                if (--fs->betweenRound <= 0) {
                    fs->betweenRound = fs->ticksPerSecond * 3;
                    if (fs->currentHint != 3) {
                        fs->reloadImage(fs->hintImages[fs->currentHint], "1000.png");
                        
                        fs->updateScore(1000);
                        ++fs->currentHint;
                        
                        if (fs->currentHint != 3) {
                            fs->betweenRound = fs->ticksPerSecond / 2;
                        }
                    }
                    else {
                        fs->startRound();
                    }
                }

                break;
            }
            else {
                // add a bonus for the time left
                fs->timeLeft -= fs->ticksPerSecond;
                fs->updateScore(50);
                if (fs->timeLeft < 0) {
                    fs->progress.percent(0);
                }
                else {
                    fs->progress.percent((fs->timeLeft * 100) / fs->totalTime);
                }
                fs->betweenRound = fs->ticksPerSecond / 2;
            }
            
            break;
        case TAKE_TOTAL_SCORE:
            break;
        case BETWEEN_ROUNDS:
            if (--fs->betweenRound <= 0) {
                exitGame(NULL, &ptr);
                return FALSE;
            }
            break;
        case OUT_OF_TIME:
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

void FotoSeek::playAreaClicked(GtkWidget *widget, GdkEventButton *event, gpointer *ptr) {
    FotoSeek *fs(reinterpret_cast<FotoSeek *>(ptr));
    if (fs->gameState == RUNNING) {
        fs->checkCircle((int)event->x, (int)event->y);
        // check for round over
        for (int i = 0; i < 5; ++i) {
            if (fs->diffCoords[i].found == false) {
                return;
            }
        }
        // round over
        fs->gameState = TAKE_ROUND_SCORE;
        fs->betweenRound = 5 * fs->ticksPerSecond;
//         std::cout << "Round over" << std::endl;
    }
}

void FotoSeek::hintClicked(GtkWidget *widget, GdkEventButton *event, gpointer *ptr) {
    FotoSeek *fs(reinterpret_cast<FotoSeek *>(ptr));
    if (fs->gameState == RUNNING) {
//         std::cout << "Hint Clicked" << std::endl;
        if (fs->currentHint < 3) {
            for (int i = 0; i < 5; ++i) {
                if (fs->diffCoords[i].found == false) {
                    fs->paintCircle(&fs->diffCoords[i], true);
                    fs->diffCoords[i].found = true;
                    fs->reloadImage(fs->hintImages[fs->currentHint], "hintdone.png");
                    g_object_set_data((GObject *)fs->hintImages[fs->currentHint], KEY_STR, (void *)1);
                    ++fs->currentHint;
                    // check for round over
                    for (int i = 0; i < 5; ++i) {
                        if (fs->diffCoords[i].found == false) {
                            return;
                        }
                    }
                    // round over
                    fs->gameState = TAKE_ROUND_SCORE;
                    fs->betweenRound = 5 * fs->ticksPerSecond;
//                     std::cout << "Round over" << std::endl;
                    return;
                }
            }
        }
    }
}
