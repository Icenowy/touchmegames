
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
 

// events for GORILLA_THRILLA

#include <gorillathrilla.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <iomanip>

void GORILLA_THRILLA::exitGame(GtkWidget *widget, gpointer ptr) {
    GORILLA_THRILLA *g(reinterpret_cast<GORILLA_THRILLA *>(ptr));
    SCORE_STRUCT newScore;
    
    newScore.name = "";
    newScore.score = g->totalScore;
    newScore.date = "";
    
    g->GAME_PTR::addScore(g->scores, newScore);
    
    
    g->GAME_PTR::saveScores(g->scores);
    
    gtk_widget_hide(g->topFixed);
    
    g_source_remove(g->timer);
}

void GORILLA_THRILLA::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    GORILLA_THRILLA *g(reinterpret_cast<GORILLA_THRILLA *>(ptr));
    
    // Don't let the user quit if the score is being taken
    if (g->takingScore) {
        return;
    } 
    if (g->quitDialog()) {
        g_source_remove(g->timer);
        gtk_widget_hide(g->topFixed);
    }

}

gboolean GORILLA_THRILLA::timeout(gpointer ptr) {
    GORILLA_THRILLA *gt(reinterpret_cast<GORILLA_THRILLA *>(ptr));
    int score;
    std::stringstream sstr;
    score = atoi(gtk_label_get_text((GtkLabel *)gt->timeLabel));
    
    if (score > 0 && !gt->waitingForFirstClick) {
        score -= 20;
        
        sstr << score;
        gtk_label_set_text((GtkLabel *)gt->timeLabel, sstr.str().c_str());
        
        gt->timey += 10;
        
        gtk_fixed_move((GtkFixed *)gt->topFixed, gt->timeLabel, gt->timex + 25, gt->timey - 15);
        gtk_fixed_move((GtkFixed *)gt->topFixed, gt->diskImage, gt->timex, gt->timey);
            
    }
    
    return TRUE;
}

gint GORILLA_THRILLA::gameOverTimeout(gpointer ptr) {
    GORILLA_THRILLA *gt(reinterpret_cast<GORILLA_THRILLA *>(ptr));
    std::stringstream sstr;
    int score;
    
    score = atoi(gtk_label_get_text((GtkLabel *)gt->timeLabel));
    
    if (score) {
        sstr.str("");
        gt->totalScore += score / 50;
        score -= 20;
        
        gtk_label_set_text((GtkLabel *)gt->scoreLabel, gt->addCommas(gt->totalScore).c_str());
        
        sstr.str("");
        sstr << std::setfill('0') << std::setprecision(2) << score;
        
        gtk_label_set_text((GtkLabel *)gt->timeLabel, sstr.str().c_str());
        
        gt->timey += 10;
        
        gtk_fixed_move((GtkFixed *)gt->topFixed, gt->timeLabel, gt->timex + 25, gt->timey - 15);
        gtk_fixed_move((GtkFixed *)gt->topFixed, gt->diskImage, gt->timex, gt->timey);
    }
    else if (gt->cardsLeft) {
        --gt->cardsLeft;
        gt->totalScore += 500;
        sstr.str("");
        //sstr << std::setw(12) << std::setfill(' ') << gt->addCommas(gt->totalScore);
        //gtk_label_set_text((GtkLabel *)gt->scoreLabel, sstr.str().c_str());
        gtk_label_set_text((GtkLabel *)gt->scoreLabel, gt->addCommas(gt->totalScore).c_str());
        sstr.str("");
        sstr << std::setw(2) << std::setfill('0') << gt->cardsLeft;
        gtk_label_set_text((GtkLabel *)gt->remainingLabel, sstr.str().c_str());
    }
    if (score == 0 && gt->cardsLeft == 0) {
        gtk_widget_destroy(gt->messageImage);
        if (++gt->round == 4) {
            gt->exitGame(NULL, gt);
        }
        else {
            for (std::map<GtkWidget *, int>::iterator it = gt->cardMap->begin();
                                    it != gt->cardMap->end(); ++it) {
                if (GTK_IS_WIDGET(it->first)) {
                    gtk_widget_destroy(it->first);
                }
            }
            gt->cardMap->clear();
            gt->waitingForFirstClick = true;
            gt->startGame();
            gt->timey = 63;
            gtk_fixed_move((GtkFixed *)gt->topFixed, gt->timeLabel, gt->timex + 25, gt->timey - 15);
            gtk_fixed_move((GtkFixed *)gt->topFixed, gt->diskImage, gt->timex, gt->timey);
        }
        gt->takingScore = false;
        return FALSE;
    }
    return TRUE;
}


void GORILLA_THRILLA::runGameOver() {
    std::stringstream sstr;
    g_source_remove(timer);
    if (gameOver) {
        return;
    }
    
    gameOver = true;
    g_timeout_add(100, gameOverTimeout, this);
}


void GORILLA_THRILLA::takeScore(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    
    GORILLA_THRILLA *gt(reinterpret_cast<GORILLA_THRILLA *>(ptr));
    
    if (gt->gameOver) {
        return;
    }

    gt->takingScore = true;
    gt->messageImage = gt->loadImage("takingscore.png");
    
    gtk_fixed_put((GtkFixed *)gt->topFixed, gt->messageImage, 282, 258);
    gtk_widget_show_all(gt->messageImage);
    gt->runGameOver();
}


int GORILLA_THRILLA::getTimeLeft() {
    return atoi(gtk_label_get_text((GtkLabel *)timeLabel));
}


void GORILLA_THRILLA::dealPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    GORILLA_THRILLA *gt(reinterpret_cast<GORILLA_THRILLA *>(ptr));
    std::stringstream sstr;
    
    if (gt->gameOver) {
        return;
    }

    
    gtk_widget_hide(gt->dealSpot);

    gtk_widget_hide(gt->dispSpot);

    gt->waitingForFirstClick = 0;
    if (gt->cardsLeft) {

        if (gt->dealCard != NULL) {
            int i;
            for (i = 0; i < 28; ++i) {
                if (gt->dispCards[i] == NULL) {
                    break;
                }
            }
            if (i < 28) {
                gt->insertIntoDisplay((*gt->cardMap)[gt->dealCard], i);
            }
            // This means we have to put this card in somewhere!
            gt->cardMap->erase(gt->dealCard);
            gtk_widget_destroy(gt->dealCard);
            
        }
        sstr.str("");
        sstr << gt->remainingDeck[--gt->cardsLeft] << ".gif";
        gt->dealCard = gt->loadImage(sstr.str().c_str());
        
        (*gt->cardMap)[gt->dealCard] = gt->remainingDeck[gt->cardsLeft];
        
        sstr.str("");
        sstr << std::setw(2) << std::setfill('0') << gt->cardsLeft;
        
        gtk_label_set_text((GtkLabel *)gt->remainingLabel, sstr.str().c_str());
        gtk_container_add((GtkContainer *)gt->dealCardBox, gt->dealCard);
        gtk_widget_show_all(gt->dealCardBox);
        gt->selectedCard = NULL;
    }
}

gboolean GORILLA_THRILLA::dealCardPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    GORILLA_THRILLA *gt(reinterpret_cast<GORILLA_THRILLA *>(ptr));
    gt->waitingForFirstClick = false;
    
    if (gt->gameOver) {
        return TRUE;
    }
    
    if (gt->selectedCard == NULL) {
        gt->selectedCard = widget;
        gtk_widget_show(gt->dealSpot);
    }
    else if (gt->selectedCard != widget) {
        
        gtk_widget_hide(gt->dealSpot);
        gt->checkCards(gt->selectedCard, widget);
    }
    else {
        gtk_widget_hide(gt->dealSpot);
        gt->selectedCard = NULL;
    }
    return TRUE;
}

int GORILLA_THRILLA::getArrayPosition(int row, int card) {
    switch(row) {
        case 0: return card;
        case 1: return 6 + card;
        case 2: return 11 + card;
        case 3: return 17 + card;
        case 4: return 22 + card;
    }
    return -1;
}

int GORILLA_THRILLA::cardIn(int row, int card) {
    int location(getArrayPosition(row, card));
    if (row > 4 || card > 5 || card < 0) {
        return 0;
    }
    if (row % 2 && card > 4) {
        return 0;
    }
    if (location == -1) {
        return 0;
    }
    if (dispCards[location] != NULL) {
        return 1;
    }
    return 0;
}

int GORILLA_THRILLA::checkValidSelect(GtkWidget *widget) {
    int row, card;
    GtkWidget **location;
    
    location = std::find(dispCards, &dispCards[27], ((GtkBin *)widget)->child);
    if (location == NULL) {
        return 0;
    }
    card = location - dispCards;

    if (card < 0) {
        return 0;
    }
    row = getRow(card);
    card = getCard(card);
    
    if (row == 4) {
        return 1;
    }
    if (row % 2) { // odd row, having 5 cards
        ++row;
        if (cardIn(row, card) || cardIn(row, card + 1)) {
            return 0;
        }
        return 1;
    }
    ++row;
    if (cardIn(row, card - 1) || cardIn(row, card)) {
        return 0;
    }
    return 1;
}

gboolean GORILLA_THRILLA::middleCardPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    GORILLA_THRILLA *gt(reinterpret_cast<GORILLA_THRILLA *>(ptr));
    GtkWidget **location;
    int row, card;
    
    gt->waitingForFirstClick = false;

    
    if (gt->gameOver) {
        return TRUE;
    }
    
    location = std::find(gt->dispCards, &gt->dispCards[28], ((GtkBin*)widget)->child);
    if (location != NULL) {
        row = gt->getRow(location - gt->dispCards);
        card = gt->getCard(location - gt->dispCards);
    }
    
    if (gt->checkValidSelect(widget) == 0) {
        return TRUE;
    }
    
    if (gt->selectedCard == NULL) {
        int xpos, ypos;
        // find the location to place the arrow...
        ypos = row * 50 + 95;
        xpos = (row % 2 ? 60: 24) + card * 96 + 28;
        gtk_fixed_move(GTK_FIXED(gt->centerFixed), gt->dispSpot, xpos, ypos);
        gtk_widget_show(gt->dispSpot);
        gt->selectedCard = widget;
    }
    else if (gt->selectedCard != widget) {
        gtk_widget_hide(gt->dispSpot);
        gt->checkCards(gt->selectedCard, widget);
    }
    else {
        gtk_widget_hide(gt->dispSpot);
        gt->selectedCard = NULL;
    }
    return FALSE;
}

void GORILLA_THRILLA::checkCards(GtkWidget *lh, GtkWidget *rh) {
    int absval(0);
    int lhcard((*cardMap)[((GtkBin *)lh)->child] - 1); 
    int rhcard((*cardMap)[((GtkBin *)rh)->child] - 1);
    absval = abs(lhcard - rhcard);
    if (absval <= 1 || absval == 12) {
        std::stringstream sstr;
        
        if (lhcard == 8 && rhcard == 8) {
            totalScore += getTimeLeft() * 3;
        }
        else if ((lhcard == 0 || rhcard == 0) && absval == 1) {
            totalScore += getTimeLeft() * 5;
        }
        else {
            totalScore += getTimeLeft();
        }

        gtk_label_set_text((GtkLabel *)scoreLabel, addCommas(totalScore).c_str());
                        
        if (lh != dealCardBox) {
            GtkWidget **location;
            location = std::find(dispCards, &dispCards[27], ((GtkBin*)lh)->child);
            if (location != NULL) {
                dispCards[location - dispCards] = NULL;
            }
            
            cardMap->erase(((GtkBin *)lh)->child);
            gtk_widget_destroy(((GtkBin *)lh)->child);
            gtk_widget_destroy(lh);
        }
        else {
            cardMap->erase(((GtkBin *)lh)->child);
            
            gtk_widget_destroy(((GtkBin *)lh)->child);
            dealCard = NULL;
            if (checkWinner() == 0) {
                dealPressed(NULL, NULL, this);
            }
        }
        if (rh != dealCardBox) {
            GtkWidget **location;
            location = std::find(dispCards, &dispCards[27], ((GtkBin*)rh)->child);
            if (location != NULL) {
                dispCards[location - dispCards] = NULL;
            }
            
            cardMap->erase(((GtkBin *)rh)->child);
            gtk_widget_destroy(rh);
        }
        else {
            cardMap->erase(((GtkBin *)rh)->child);
            
            gtk_widget_destroy(((GtkBin *)rh)->child);
            dealCard = NULL;
            if (checkWinner() == 0) {
                dealPressed(NULL, NULL, this);
            }
        }
        if (checkWinner()) {
            totalScore += 10000;
            //sstr << addCommas(totalScore);
            gtk_label_set_text((GtkLabel *)scoreLabel, addCommas(totalScore).c_str());
            
            messageImage = loadImage("bonus.png");
            gtk_fixed_put((GtkFixed *)topFixed, messageImage, 282, 258);
            gtk_widget_show_all(messageImage);
            
            timey = 63;
            gtk_fixed_move((GtkFixed *)topFixed, timeLabel, timex + 25, timey - 15);
            gtk_fixed_move((GtkFixed *)topFixed, diskImage, timex, timey);
            runGameOver();
        }
        else if (isGameOver()) {
            // I don't think this code ever gets reached
            // use the game over function here too
            //std::cout << "Game over!!" << std::endl;
        }
        else {
            timey = 63;
            gtk_fixed_move((GtkFixed *)topFixed, timeLabel, timex + 25, timey - 15);
            gtk_fixed_move((GtkFixed *)topFixed, diskImage, timex, timey);
            gtk_label_set_text((GtkLabel *)timeLabel, "600");
        }
    }
    gtk_widget_hide(dealSpot);
    gtk_widget_hide(dispSpot);
    selectedCard = NULL;
}

