
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

// events for Consume21

#include <consume21.h>
#include <sstream>
#include <iomanip>

void Consume21::exitGame(GtkWidget *widget, gpointer *ptr) {
	Consume21 *consume(reinterpret_cast<Consume21 *>(*ptr));
	SCORE_STRUCT newScore;
	
	newScore.name = "";
	newScore.score = consume->currentScore;
	newScore.date = "";
	
	
	consume->GAME_PTR::addScore(consume->scores, newScore);
	
	consume->GAME_PTR::saveScores(consume->scores);
	
	g_source_remove(consume->timer);
	gtk_widget_hide(consume->tophbox);
}

void Consume21::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    Consume21 *consume(reinterpret_cast<Consume21 *>(ptr));
    
    if (consume->quitDialog()) {
        g_source_remove(consume->timer);
        gtk_widget_hide(consume->tophbox);
    }
}


gboolean Consume21::timeout(gpointer ptr) {
    Consume21 *consume(reinterpret_cast<Consume21 *>(ptr));

    switch (consume->gameState) {
        case WAITING:
            // don't do anything, just wait for a press
            break;
        case RUNNING:
            --consume->timeLeft;
            if (consume->timeLeft == 0) {
                consume->gameState = OUT_OF_TIME;
            }
            consume->progress.percent((int)consume->timeLeft / 3);
            
            break;
        case TAKE_SCORE: {
            std::stringstream sstr;
            std::string str;
            // taking score
            if (consume->timeLeft) {
                int total;
                consume->getMultiplier();
                if (consume->multiplier != -1) {
                    consume->roundScore += consume->multiplier;
                    sstr << consume->roundScore;
                    str = sstr.str();
                    consume->addCommas(str);
                    gtk_label_set_text((GtkLabel *)consume->columnsTotal, str.c_str());
                    --consume->timeLeft;
                    consume->progress.percent((int)consume->timeLeft / 3);
                }
                else {
                    consume->timeLeft = 0;
                }
            }
            else {
                consume->gameState = WAITING;
                if (consume->roundScore != -1) {
                    consume->currentScore += consume->roundScore;
                }
                
                sstr << consume->currentScore;
                str = sstr.str();
                consume->addCommas(str);
                gtk_label_set_text((GtkLabel *)consume->scoreLabel, str.c_str());
                consume->roundOver();
                consume->startRound();
                g_source_remove(consume->timer);
                consume->timer = g_timeout_add(100, consume->timeout, ptr);
            }
            
            }
            break;
        case OUT_OF_TIME:
            consume->roundOver();
            consume->startRound();
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

void Consume21::colPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    Consume21 *consume(reinterpret_cast<Consume21 *>(ptr));
    int column(((int)g_object_get_data((GObject *)widget, KEY_STR) & 0xF) - 1);
    int cardNum((((int)g_object_get_data((GObject *)consume->mugImages[consume->totalMugImages - 1], KEY_STR) >> 4) & 0xF));
    int soft(0);
    std::string str;
    if (consume->gameState == WAITING) {
        consume->gameState = RUNNING;
    }
    if (consume->gameState != RUNNING) {
        return;
    }
        
    if (column != -1) {
        std::stringstream sstr;
        int total(0);
        
        // make sure it can fit into the column
        if (consume->columnTotal[column] + cardNum > 21) {
            if (cardNum == 11) {
                cardNum -= 10;
                if (consume->columnTotal[column] + 1 > 21) {
                    return;
                }
                
            }
            else { // now check to see if there's at least one 11 in the pile
                int card = consume->columnMugs[column];
                int whichCard = -1;
                for (int i = 0; i < card; ++i) {
                    if (consume->columnMugValues[column][i] == 11) {
                        // we can use this and subtract 10 from it to include
                        // the new card
                        whichCard = i;
                        
                        total += 1;
                    }
                    else {
                        total += consume->columnMugValues[column][i];
                    }
                }
                if (whichCard == -1) {
                    return;
                }
                
                // there's an ace(11) in the pile, now we need to check to see
                // that if we remove 10 from it, will it fit.
                
                if (total + cardNum > 21) {
                    
                    return;
                }
                consume->columnMugValues[column][whichCard] = 1;
                consume->columnTotal[column] = total;
                total = 0;
            }
        }
        gtk_fixed_move((GtkFixed *)consume->fixed, 
                 consume->mugImages[consume->totalMugImages - 1], 
                 column * 120 + 20, 220 + consume->columnMugs[column] * 40);
        
        
        
        // make this part of the column now
        consume->columnMugValues[column][consume->columnMugs[column]] = cardNum;
        ++consume->columnMugs[column];
        consume->columnTotal[column] += cardNum;
        
        
        
        sstr << consume->columnTotal[column];
        // check for 11s
        for (int i = 0; i < consume->columnMugs[column]; ++i) {
            if (consume->columnMugValues[column][i] == 11) {
                sstr << " - S";
            }
        }
        
        
        gtk_label_set_text((GtkLabel *)consume->columnTotalLabels[column], sstr.str().c_str());
        
        
        
        column += 1;
        column |= cardNum << 4;
        g_object_set_data((GObject *)consume->mugImages[consume->totalMugImages - 1], KEY_STR, (void *)column);
        
        consume->createMug();
        
        
        for (int i = 0; i < 5; ++i) {
            total += consume->columnTotal[i];
        }
        sstr.str("");
        sstr << total;
        str = sstr.str();
        consume->addCommas(str);
        gtk_label_set_text((GtkLabel *)consume->columnsTotal, str.c_str());
        
        
        consume->setTotalWin(total);
        
    }
}

void Consume21::spillPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    Consume21 *consume(reinterpret_cast<Consume21 *>(ptr));
    if (consume->gameState == WAITING) {
        consume->gameState = RUNNING;
    }
    if (consume->gameState != RUNNING) {
        return;
    }
//     gtk_image_set_from_file((GtkImage *)consume->spill, "/usr/local/share/touchmegames/consume21/images/spilldown.png");
    consume->reloadImage(consume->spill, "spilldown.png");
}

void Consume21::spillReleased(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    Consume21 *consume(reinterpret_cast<Consume21 *>(ptr));
    if (consume->gameState == WAITING) {
        consume->gameState = RUNNING;
    }
    if (consume->gameState != RUNNING) {
        return;
    }
    gtk_widget_destroy(consume->mugImages[consume->totalMugImages - 1]);
    --consume->totalMugImages;
    consume->createMug();
    gtk_widget_hide(consume->spill);
}

void Consume21::collectPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    Consume21 *consume(reinterpret_cast<Consume21 *>(ptr));
    if (consume->gameState == WAITING) {
        consume->gameState = RUNNING;
    }
    if (consume->gameState != RUNNING) {
        return;
    }
//     gtk_image_set_from_file((GtkImage *)consume->collect, "/usr/local/share/touchmegames/consume21/images/collectdown.png");
    consume->reloadImage(consume->collect, "collectdown.png");
}

void Consume21::collectReleased(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    Consume21 *consume(reinterpret_cast<Consume21 *>(ptr));
    if (consume->gameState == WAITING) {
        consume->gameState = RUNNING;
    }
    if (consume->gameState != RUNNING) {
        return;
    }
    g_source_remove(consume->timer);
    consume->timer = g_timeout_add(30, consume->timeout, ptr);
//     gtk_image_set_from_file((GtkImage *)consume->collect, "/usr/local/share/touchmegames/consume21/images/collectup.png");
    consume->reloadImage(consume->collect, "collectup.png");
    
    consume->gameState = TAKE_SCORE;
}
