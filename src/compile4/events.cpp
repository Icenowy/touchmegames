
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
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02011-1307  USA
 */

// events for Compile4

#include <compile4.h>
#include <sstream>
#include <iomanip>


void Compile4::exitGame(GtkWidget *widget, gpointer *ptr) {
    Compile4 *compile(reinterpret_cast<Compile4 *>(*ptr));
    SCORE_STRUCT newScore;
    
    newScore.name = "";
    newScore.score = compile->currentScore;
    newScore.date = "";
    
    
    compile->GAME_PTR::addScore(compile->scores, newScore);
    
    compile->GAME_PTR::saveScores(compile->scores);
    
    g_source_remove(compile->timer);
    gtk_widget_hide(compile->topFixed);
}

void Compile4::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    Compile4 *compile(reinterpret_cast<Compile4 *>(ptr));
    
    if (compile->quitDialog()) {
        g_source_remove(compile->timer);
        gtk_widget_hide(compile->topFixed);
    }
}


gboolean Compile4::timeout(gpointer ptr) {
    Compile4 *compile(reinterpret_cast<Compile4 *>(ptr));
    
    if (compile->gameState == PLAYER_TURN) {
        ++compile->timeForMoves;
    }
    
    if (compile->gameState == DROPPING_PLAYER) {
        
        compile->circy += 20;
        
        
        if (compile->circy > compile->circendy) {
            int cmove;
            compile->winner = compile->roundOver();
            
            if (compile->winner) {
                if (compile->winner > 0) {
                    compile->betweenRoundCounter = 100;
                }
                else {
                    // tie
                    compile->betweenRoundCounter = 1;
                }
                compile->gameState = BETWEEN_ROUNDS;
                compile->showBlink = false;
                return TRUE;
            }
            
            compile->gameState = DROPPING_CPU;
            ++compile->move;
            cmove = compile->think(false);
            for (int y = 5; y >= 0; --y) {
                if (compile->grid[cmove][y] == 0) {
                    compile->grid[cmove][y] = 2;
                    compile->widgetGrid[cmove][y] = compile->p2Circs[compile->move / 2];
                    
                    compile->circx = (int)cmove * 80;
                    compile->circy = -80;
                    compile->circendy = 80 * (y);
    
                    gtk_fixed_move((GtkFixed *)compile->fixed, 
                            compile->p2Circs[compile->move >> 1], 
                            compile->circx, compile->circy);
                    return TRUE;
                }
            }
        }
        else {
            gtk_fixed_move((GtkFixed *)compile->fixed, 
                            compile->p1Circs[compile->move >> 1], 
                            compile->circx, compile->circy);
        }
    }
    else if (compile->gameState == DROPPING_CPU) {
        compile->circy += 20;
        gtk_fixed_move((GtkFixed *)compile->fixed, 
                        compile->p2Circs[compile->move >> 1], 
                        compile->circx, compile->circy);
        if (compile->circy == compile->circendy) {
            compile->winner = compile->roundOver();
            if (compile->winner) {
                if (compile->winner > 0) {
                    compile->betweenRoundCounter = 100;
                }
                else {
                    // tie
                    compile->betweenRoundCounter = 1;
                }
                compile->showBlink = false;
                compile->gameState = BETWEEN_ROUNDS;
                return TRUE;
            }
            
            compile->gameState = PLAYER_TURN;
            ++compile->move;
        }
    }
    else if (compile->gameState == BETWEEN_ROUNDS) {
            
        if ((--compile->betweenRoundCounter % 8) == 0) {
            for (int i = 0; i < 4; ++i) {
                if (compile->showBlink) {
                    gtk_widget_show(compile->widgetGrid[compile->blinky[i][0]][compile->blinky[i][1]]);
                }
                else {
                    gtk_widget_hide(compile->widgetGrid[compile->blinky[i][0]][compile->blinky[i][1]]);
                }
            }
            if (compile->showBlink) {
                compile->showBlink = false;
            }
            else {
                compile->showBlink = true;
            }
        }
        if (compile->betweenRoundCounter == 0) {
            compile->gameState = MULT_MOVES_OUTCOME;
            compile->cleanAndShowScore();
        }
    }
    else if (compile->gameState == MULT_MOVES_OUTCOME) {
        if (compile->winner > 0) {
            std::stringstream sstr;
            compile->total1 += compile->move;
            --compile->winner;
            sstr << compile->addCommas(compile->total1);
            gtk_label_set_text((GtkLabel *)compile->totalValue1Label, sstr.str().c_str());
        }
        else if (compile->winner == -1) {
            compile->gameState = MULT_TOTAL_TTM;
        }
        else {
            --compile->winner;
        }
    }
    else if (compile->gameState == MULT_TOTAL_TTM) {
        if (compile->timeForMoves > 0) {
            std::stringstream sstr;
            compile->total2 += compile->total1;
            --compile->timeForMoves;
            sstr << compile->addCommas(compile->total2);
                gtk_label_set_text((GtkLabel *)compile->totalValue2Label, sstr.str().c_str());
                if (!compile->timeForMoves) {
                        compile->currentScore += compile->total2;
                        sstr.str("");
                        sstr << compile->addCommas(compile->currentScore);
                        gtk_label_set_text((GtkLabel *)compile->scoreLabel, sstr.str().c_str());
                        
                }	
        }
        else if (compile->timeForMoves == -1) {
            
            if (compile->round == 3) {
                compile->exitGame(NULL, (void **)&compile);
                return FALSE;
            }
            compile->startRound();
            if ((compile->round & 1) == 1) {
                compile->gameState = PLAYER_TURN;
            }
            else {
                int cmove;
                compile->winner = compile->roundOver();
                if (compile->winner) {
                    if (compile->winner > 0) {
                        compile->betweenRoundCounter = 100;
                    }
                    else {
                        // tie
                        compile->betweenRoundCounter = 1;
                    }
                    compile->gameState = BETWEEN_ROUNDS;
                    compile->showBlink = false;
                    return TRUE;
                }
                
                compile->gameState = DROPPING_CPU;
                ++compile->move;
                cmove = compile->think(false);
                for (int y = 5; y >= 0; --y) {
                    if (compile->grid[cmove][y] == 0) {
                        compile->grid[cmove][y] = 2;
                        compile->widgetGrid[cmove][y] = compile->p2Circs[compile->move / 2];
                        
                        compile->circx = (int)cmove * 80;
                        compile->circy = -80;
                        compile->circendy = 80 * (y);
        
                        gtk_fixed_move((GtkFixed *)compile->fixed, 
                                        compile->p2Circs[compile->move >> 1], 
                                        compile->circx, compile->circy);
                        return TRUE;
                    }
                }
            }
        }
        else {
            --compile->timeForMoves;
#ifdef __unix__
            sleep(5);
#else
            _sleep(5000);
#endif
            gtk_widget_hide(compile->scorebg);
            gtk_widget_hide(compile->scoreWindow);
        }
    }
    
    return TRUE;
}

void Compile4::colPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    Compile4 *compile(reinterpret_cast<Compile4 *>(ptr));
    if (compile->gameState == PLAYER_TURN) {
        int col((int)g_object_get_data((GObject *)widget, KEY_STR));
        for (int y = 5; y >= 0; --y) {
            if (compile->grid[col][y] == 0) {
                compile->grid[col][y] = 1;
                compile->widgetGrid[col][y] = compile->p1Circs[compile->move / 2];
                compile->circx = (int)g_object_get_data((GObject *)widget, KEY_STR) * 80;
                compile->circy = -80;
                compile->circendy = 80 * (y);

                gtk_fixed_move((GtkFixed *)compile->fixed, 
                                compile->p1Circs[compile->move / 2], 
                                compile->circx, compile->circy);
                compile->gameState = DROPPING_PLAYER;
                return;
            }
        }
    }
}
