
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
 *	Foundation, Inc., 59 Temple Place, Suite 328, Boston, MA  02111-1287  USA
 */

#ifndef TRI_TOWERS_H
#define TRI_TOWERS_H

#include <iostream>
#include <string>
#include <gameptr.h>


class TriTowers : public GAME_PTR {
private:
    const std::string deckLocation;
    const std::string deckBack;
    GtkWidget *topFixed;
    GtkWidget *topvbox;
    GtkWidget *centerFixed;
    GtkWidget *takeScore;
    SCORE_STRUCT scores[10];
    bool waitingForFirstClick;
    
    int cardDeck[52];
    int cardLocation;
    int round;
    int totalScore;
    int run, highrun;
    
    int swordx, swordy;
    GtkWidget *swordImage, *swordImages[3];
    int bonusLevel;
    GtkWidget *bonusImages[3];
    GtkWidget *addingBonus;
    enum GAME_STATES {
            PLAY_ROUND,
            BETWEEN_ROUNDS,
            COUNT_CARDS,
            TAKE_SCORE,
            OUT_OF_TIME,
            DROPPING_SWORD,
            GAME_OVER,
    };
    GAME_STATES gameState;
    
    GtkWidget *dispCards[28];
    GtkWidget *dispCardBoxes[28];
    
    // interface.cpp vars
    GtkWidget *timeBar;
    float timeLeft;
    
    GtkWidget *roundLabel;
    GtkWidget *scoreLabel;
    GtkWidget *remainingLabel;
    GtkWidget *instructionLabel;
    GtkWidget *dealCard;
    GtkWidget *dealCardBox;
    int currentScore;
    GtkWidget *hiScoreLabel;
    GtkRcStyle *downStyle, *upStyle;
    GtkWidget *runLabel;
    GtkWidget *highRunLabel;
    GtkWidget *timeDealie;
    GtkWidget *cardScore;
    
    GtkWidget *createTopBox();
    GtkWidget *createMiddleBox();
    GtkWidget *createBottomBox();
    GtkWidget *createCard(int num);
    GdkColor bgcolor;
    
    // support.cpp
    int getRow(int i);
    int getCard(int i);
    int checkWinner();
    void checkCards(GtkWidget *lh, GtkWidget *rh);
    
    // shuffle the deck and redraw the backs of the 
    // cards
    void startRound();
    void shuffle(int *);
    
    // events
    int timer;
    static gboolean timeout(gpointer ptr);
    static void exitGame(GtkWidget *widget, gpointer *ptr);
    static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    int getArrayPosition(int row, int card);
    int cardIn(int row, int card);
    int checkValidSelect(GtkWidget *widget);
    static void cardSelected(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void dealPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    static void cardReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    
    static void takeScorePressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    static void takeScoreReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    
    
public:
    TriTowers();
    GdkColor *backgroundColor();
    
    std::string name() const;
    std::string imageName() const;
    
    // initializes the game's variables and returns the
    // top level h/vbox
    GtkWidget *init();
    GtkWidget *getHelp();
    int gameType() const;
};

#endif // TriTowers_H
