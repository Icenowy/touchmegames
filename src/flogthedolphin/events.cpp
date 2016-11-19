
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
 

// events for FLOG_THE_DOLPHIN

#include <flogthedolphin.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>

void FLOG_THE_DOLPHIN::exitGame(GtkWidget *widget, gpointer ptr) {
    FLOG_THE_DOLPHIN *g(reinterpret_cast<FLOG_THE_DOLPHIN *>(ptr));
    SCORE_STRUCT newScore;
    
    newScore.name = "";
    newScore.score = g->totalLength;
    newScore.date = "";
    
    g_source_remove(g->timer);
    
    g->GAME_PTR::addScore(g->scores, newScore);
    
    g->GAME_PTR::saveScores(g->scores);
    
    gtk_widget_hide(g->eventBox);
    // clean up after the game
    gtk_widget_destroy(g->eventBox);
}

void FLOG_THE_DOLPHIN::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    FLOG_THE_DOLPHIN *flog(reinterpret_cast<FLOG_THE_DOLPHIN *>(ptr));
        
    if (flog->quitDialog()) {
        g_source_remove(flog->timer);
        gtk_widget_hide(flog->eventBox);
    }
        
}


void FLOG_THE_DOLPHIN::moveBgImage(GtkWidget *image, int &x) {
    x -= rate;
    if (x <= -1200) {
        x += 2400;
        gtk_fixed_move((GtkFixed *)fixed, image, x, 400);
    }
    if (x <= 800) {
        gtk_fixed_move((GtkFixed *)fixed, image, x, 400);
    }
}

void FLOG_THE_DOLPHIN::moveRaft() {
    raft1x -= rate;
    if (raft1x < -175) {
        raft1x = 800 + rand() % 300;
        
        if (abs(raft1x - raft2x) < 160) {
            raft1x += 320;
        }
        if (abs(raft1x - buoyx) < 160) {
            raft1x += 320;
        }
        if (abs(raft1x - chairx) < 160) {
            raft1x += 320;
        }
        raft1hit = false;
        gtk_fixed_move((GtkFixed *)fixed, raftImage1, raft1x, 567);
    }
    if (raft1x <= 800) {
        gtk_fixed_move((GtkFixed *)fixed, raftImage1, raft1x, 567);
    }
    
    raft2x -= rate;
    if (raft2x < -175) {
        raft2x = 1400 + rand() % 800;
        if (abs(raft2x - raft1x) < 160) {
            raft2x += 320;
        }
        if (abs(raft2x - buoyx) < 160) {
            raft2x += 320;
        }
        if (abs(raft2x - chairx) < 160) {
            raft2x += 320;
        }
        raft2hit = false;
        gtk_fixed_move((GtkFixed *)fixed, raftImage2, raft2x, 567);
    }
    if (raft2x <= 800) {
        gtk_fixed_move((GtkFixed *)fixed, raftImage2, raft2x, 567);
    }
}

void FLOG_THE_DOLPHIN::moveChair() {
    chairx -= rate;
    if (chairx < -175) {
        chairx = 800 + rand() % 200;
        if (abs(chairx - raft1x) < 160) {
            chairx += 320;
        }
        if (abs(chairx - raft2x) < 160) {
            chairx += 320;
        }
        if (abs(chairx - buoyx) < 160) {
            chairx += 320;
        }
        
        chairhit = false;
        gtk_fixed_move((GtkFixed *)fixed, chairImage, chairx, 500);
    }
    if (chairx <= 800) {
        gtk_fixed_move((GtkFixed *)fixed, chairImage, chairx, 500);
    }
}

void FLOG_THE_DOLPHIN::moveBuoy() {
    buoyx -= rate;
    if (buoyx < -260) {
        buoyx = 800 + rand() % 50;
        if (abs(buoyx - raft1x) < 160) {
            buoyx += 320;
        }
        if (abs(buoyx - raft2x) < 160) {
            buoyx += 320;
        }
        if (abs(buoyx - chairx) < 160) {
            buoyx += 320;
        }
    }
    gtk_fixed_move((GtkFixed *)fixed, buoyImage, buoyx, 467);
}

gboolean FLOG_THE_DOLPHIN::timeout(gpointer ptr) {
    FLOG_THE_DOLPHIN *ftd(reinterpret_cast<FLOG_THE_DOLPHIN *>(ptr));
    float dolphiny;
    bool scaled(false), rateChanged(false);
    std::stringstream sstr;
    if (ftd->gameState == FLOG_THE_DOLPHIN::endOfGame) {
            if (--ftd->endOfFlogCountDown == 0) {
                    g_source_remove(ftd->timer);
                    ftd->exitGame(NULL, ftd);
                    return FALSE;
            }
            return TRUE;
    }
    else if (ftd->gameState == FLOG_THE_DOLPHIN::endOfRound) {
            if (--ftd->endOfFlogCountDown == 0) {
                    ftd->startRound();
                    ftd->gameState = FLOG_THE_DOLPHIN::startOfRound;
                    return TRUE;
            }
            else {
                    return TRUE;
            }
    }
    
    if (ftd->timeLeft > 0) {
            ftd->timeLeft -= 1;
            gtk_progress_bar_set_fraction((GtkProgressBar *)ftd->timeProgress, ((float)ftd->timeLeft / 4500.0));
    }
    
    if (ftd->gameState == FLOG_THE_DOLPHIN::startOfRound) {
            
            if (ftd->radians >= 0.785 && ftd->radians <= 2.355) {
                    gtk_widget_hide(ftd->dolphinImage);
            
                    if (++ftd->currentDolphin >= 22) {
                            ftd->currentDolphin = 21;
                    }
                    
                    ftd->dolphinImage = ftd->dolphinImages[ftd->currentDolphin];
                    gtk_widget_show(ftd->dolphinImage);
                    ftd->dolphinTilt = ftd->dolphinTiltReload;
            }
            ftd->radians += 0.0628;
            dolphiny = 400.0 - sin(ftd->radians) * 300.0;
            
            if (ftd->radians >= 3.14) {
                    ftd->radians = 0.0;
                    
                    gtk_widget_hide(ftd->dolphinImage);
            
                    
                    ftd->currentDolphin = 0;
                    
                    ftd->dolphinImage = ftd->dolphinImages[ftd->currentDolphin];
                    gtk_widget_show(ftd->dolphinImage);
                    ftd->dolphinTilt = ftd->dolphinTiltReload;
            }
            gtk_fixed_move((GtkFixed *)ftd->fixed, ftd->dolphinImage, 90, (int)dolphiny);
            return TRUE;
    }
    
    ftd->moveBgImage(ftd->bgImage1, ftd->bgImage1x);
    ftd->moveBgImage(ftd->bgImage2, ftd->bgImage2x);
    
    ftd->moveRaft();
    ftd->moveChair();
    ftd->moveBuoy();
    
    if (ftd->initialy) {
            dolphiny = ftd->initialy - ((((float)ftd->hvelocity * sin(ftd->angle) * ftd->t - 16 * ftd->t * ftd->t)) * 10);
    }
    else {
            dolphiny = 400 - ((((float)ftd->hvelocity * sin(ftd->angle) * ftd->t - 16 * ftd->t * ftd->t)) * 10);
    }
    if (dolphiny >= 400) {
            ftd->highDolphin = false;
            ftd->t = 0;
            dolphiny = 400;
            ftd->currentDolphin = 0;
            ftd->initialy = 0;
    }
    ftd->t += 0.05;
    ftd->dolphinTilt -= 0.05;
    if (ftd->dolphinTilt <= 0.0) {
            gtk_widget_hide(ftd->dolphinImage);
            
            if (++ftd->currentDolphin >= 22) {
                    ftd->currentDolphin = 21;
            }
            ftd->dolphinImage = ftd->dolphinImages[ftd->currentDolphin];
            gtk_widget_show(ftd->dolphinImage);
            ftd->dolphinTilt = ftd->dolphinTiltReload;
    }
    gtk_fixed_move((GtkFixed *)ftd->fixed, ftd->dolphinImage, 300, (int)dolphiny);
    ftd->radians += .1;
    if (ftd->t <= 0.06) {
            float totalTime((((ftd->hvelocity * ftd->hvelocity * 
                            sin(ftd->angle) * cos(ftd->angle)) / 16))
                            / (ftd->hvelocity * cos(ftd->angle)));
            ftd->dolphinTiltReload = totalTime / 22.0;
            ftd->dolphinTilt = ftd->dolphinTiltReload;
            gtk_widget_hide(ftd->dolphinImage);
            
            ftd->currentDolphin = 0;
            
            ftd->dolphinImage = ftd->dolphinImages[ftd->currentDolphin];
            gtk_widget_show(ftd->dolphinImage);
            
            
            ftd->hvelocity /= 3;
            ftd->hvelocity *= 2;
            if (ftd->hvelocity < 5) {
                    ftd->hvelocity = 50;
            }
            gtk_fixed_move((GtkFixed *)ftd->fixed, ftd->dolphinImage, 300, (int)dolphiny);
    }
    if (dolphiny + 200 >= 550) {
            
        if (!ftd->raft1hit && 350 - ftd->raft1x < 152 && 350 - ftd->raft1x > 0) {
            ftd->scale *= 3.0;
            scaled = true;
            ftd->raft1hit = true;
            
            if (ftd->hvelocity < 50) {
                ftd->hvelocity *= 3;
                ftd->hvelocity /= 2;
            }
            else if (ftd->hvelocity < 70) {
                ftd->hvelocity *= 5;
                ftd->hvelocity /= 4;
            }
            else if (ftd->hvelocity < 90) {
                ftd->hvelocity *= 7;
                ftd->hvelocity /= 6;
            }
            if (ftd->soundOn) {
                ftd->playWave("boing.wav");
            }
            //ftd->radians = 0.2;
            ftd->t = 0;
        }
        else if (!ftd->raft2hit && 350 - ftd->raft2x < 152 && 350 - ftd->raft2x > 0) {
            ftd->scale *= 3.0;
            scaled = true;
            ftd->raft2hit = true;
            
            if (ftd->hvelocity < 50) {
                ftd->hvelocity *= 3;
                ftd->hvelocity /= 2;
            }
            else if (ftd->hvelocity < 70) {
                ftd->hvelocity *= 5;
                ftd->hvelocity /= 4;
            }
            else if (ftd->hvelocity < 90) {
                ftd->hvelocity *= 7;
                ftd->hvelocity /= 6;
            }
            if (ftd->soundOn) {
                ftd->playWave("boing.wav");
            }
            //ftd->radians = 0.2;
            ftd->t = 0;
        }
        else if (!ftd->chairhit && 350 - ftd->chairx < 166 && 350 - ftd->chairx > 0) {
            ftd->scale *= 2.0;
            scaled = true;
            ftd->chairhit = true;
            
            if (ftd->hvelocity < 50) {
                ftd->hvelocity *= 3;
                ftd->hvelocity /= 2;
            }
            else if (ftd->hvelocity < 70) {
                ftd->hvelocity *= 5;
                ftd->hvelocity /= 4;
            }
            else if (ftd->hvelocity < 90) {
                ftd->hvelocity *= 7;
                ftd->hvelocity /= 6;
            }
            ftd->rate += 5;
            ftd->t = 0;
            rateChanged = true;
        }

    }
    
    
    if (ftd->t <= 0.06) {
        if (!rateChanged) {
            if (ftd->rate > 15) {
                ftd->rate -= 1;
            }
        }
    }
    
    if (scaled) {
        while (400 - ((((float)ftd->hvelocity * sin(ftd->angle) 
                            * ftd->t - 16 * ftd->t * ftd->t)) * 10) > dolphiny) {
            ftd->t += 0.05;
            ftd->dolphinTilt -= 0.05;
        }
        float totalTime((((ftd->hvelocity * ftd->hvelocity * 
                    sin(ftd->angle) * cos(ftd->angle)) / 16))
                    / (ftd->hvelocity * cos(ftd->angle)));
        ftd->dolphinTiltReload = totalTime / 22.0;
        ftd->dolphinTilt = ftd->dolphinTiltReload;
        
        gtk_widget_hide(ftd->dolphinImage);
        
        ftd->currentDolphin = 0;
        
        ftd->dolphinImage = ftd->dolphinImages[ftd->currentDolphin];
        gtk_widget_show(ftd->dolphinImage);
    }
    ftd->setLength();
    ftd->checkBuoy(dolphiny);
    
    if (dolphiny < -200 && !ftd->highDolphin) {
        ftd->highDolphin = true;
        if (ftd->soundOn) {
        ftd->playWave("highDolphin.wav");
        }
    }
    return TRUE;
}

void FLOG_THE_DOLPHIN::setLength() {
    std::stringstream sstr;
    length += rate / 30.0;
    sstr << (int)length;
    gtk_label_set_text((GtkLabel *)currentLabel, sstr.str().c_str());
}

void FLOG_THE_DOLPHIN::checkBuoy(float dolphiny) {
    float dolphinNosey(90 * sin(currentDolphin * 5 + 40) + dolphiny + 100), 
            dolphinNosex(90 * cos(currentDolphin * 5 + 40) + 400);
    
    int buoyy(buoyx + (int)((-112.0 / 32.0) * dolphinNosex) + 132 + 600);
    if (dolphinNosey > 480 && buoyy > 0) {
        
        if (dolphinNosey > buoyy && buoyx >= 368 && buoyx <= 500) {
            std::stringstream sstr;
            
            // Calculate the score
            for (int i = 0; i < 8; ++i) {
                if ((int)length > top8[i]) {
                    for (int j = 7; j >= i + 1; --j) {
                        top8[j] = top8[j - 1];
                    }
                    top8[i] = (int)length;
                    i = 9;
                }
            }
            totalLength = 0;
            for (int i = 0; i < 8; ++i) {
                totalLength += top8[i];
            }
            sstr << totalLength;
            gtk_label_set_text((GtkLabel *)scoreLabel, sstr.str().c_str());
            
            if (timeLeft > 0) {
                gameState = endOfRound;
            }
            else {
                gameState = endOfGame;
            }
            endOfFlogCountDown = 100;
            if (soundOn) {
                playWave("foghorn.wav");
            }
        }
    }
}

void FLOG_THE_DOLPHIN::flogTheDolphin(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    FLOG_THE_DOLPHIN *ftd(reinterpret_cast<FLOG_THE_DOLPHIN *>(ptr));
    float rad;
    float dolphiny;
    float tmpRate;
    
    if (ftd->gameState != FLOG_THE_DOLPHIN::startOfRound) {
        return;
    }
    
    dolphiny = 400.0 - sin(ftd->radians) * 300.0;
    
    tmpRate = cos(ftd->radians);
    if (tmpRate < 0.0) {
        tmpRate = -tmpRate;
    }
    if (tmpRate < 0.33) {
        tmpRate = 0.33;
    }
    ftd->rate = (int)(tmpRate * 30.0);
    ftd->initialy = dolphiny;
    rad = ftd->radians;
    if (rad > 1.57) {
        rad -= 1.57;
    }
    //rad -= 0.7535;
    //rad *= 10;
    ftd->angle = 45 + (int)rad;
    
    ftd->gameState = FLOG_THE_DOLPHIN::middleOfRound;
    ftd->t = 0.0;
    float totalTime((((ftd->hvelocity * ftd->hvelocity * 
                    sin(ftd->angle) * cos(ftd->angle)) / 16))
            / (ftd->hvelocity * cos(ftd->angle)));
    ftd->dolphinTiltReload = totalTime / 22.0;
    ftd->dolphinTilt = ftd->dolphinTiltReload;

    gtk_widget_hide(ftd->dolphinImage);

    ftd->currentDolphin = 0;

    ftd->dolphinImage = ftd->dolphinImages[ftd->currentDolphin];
    
    gtk_fixed_move((GtkFixed *)ftd->fixed, ftd->raftImage1, ftd->raft1x, 567);
    gtk_fixed_move((GtkFixed *)ftd->fixed, ftd->raftImage2, ftd->raft2x, 567);
    
    gtk_widget_show(ftd->dolphinImage);
    if (ftd->soundOn) {
        ftd->playWave("dolphin.wav");
    }
}


void FLOG_THE_DOLPHIN::toggleSound(GtkWidget *widget, gpointer ptr) {
	FLOG_THE_DOLPHIN *ftd(reinterpret_cast<FLOG_THE_DOLPHIN *>(ptr));
	if (ftd->soundOn) {
		ftd->soundOn = false;
		gtk_button_set_label((GtkButton *)widget, "Sound Off");
	}
	else {
		ftd->soundOn = true;
		gtk_button_set_label((GtkButton *)widget, "Sound On");
	}
}
