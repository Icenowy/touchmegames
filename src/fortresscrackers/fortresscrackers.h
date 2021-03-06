
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

#ifndef FORTRESS_CRACKERS_H
#define FORTRESS_CRACKERS_H

#include <iostream>
#include <string>
#include <gameptr.h>
#include <altprogressbar.h>

class FortressCrackers : public GAME_PTR {
private:
    int gameWidth, gameHeight;
    GtkWidget *topFixed;
    
    SCORE_STRUCT scores[10];
    
    
    
    enum GAME_STATES {
        INIT_1,
        WAITING,
        RUNNING,
        TAKE_WILD_SCORE,
        TAKE_CARD_SCORE,
        TAKE_TIME_SCORE,
        TAKE_ROUND_SCORE,
        TAKE_TOTAL_SCORE,
        BETWEEN_ROUNDS,
        OUT_OF_TIME,
        OUT_OF_TIME1,
        GAME_OVER,
    };
    GAME_STATES gameState;
    
    
    GtkWidget *roundLabel;
    GtkWidget *scoreLabel;
    int currentScore;
    int gameScore;
    int roundScore;
    GtkWidget *hiScoreLabel;
    GtkWidget *topScoreLabel;
    GtkWidget *totalScoreLabel;
    GtkWidget *cardScoreLabel;
    GtkWidget *roundScoreLabel;
    GtkWidget *timeScoreLabel;
    GtkWidget *cardsLeftLabel;
    GtkWidget *streakLabel;
    GtkWidget *cards[36];
    GtkWidget *dealCard;
    GtkWidget *userCard;
    GtkWidget *bonus[5];
    GtkWidget *takingScore;
    
    AltProgressBar progress;
    
    
    int cardWidth, cardHeight, cardMiddle;
    int xpadding, ypadding;
    int cardOffset; // This is the offset of the 1+ card from the last
    int levelOne, levelTwo, levelThree, levelFour;
    int bonusWidth, bonusHeight;
    int takingScoreWidth, takingScoreHeight;
    int deck[52];
    int cardPosition;
    int currentBonus;
    int streak;
    int timeLeft;
    int stageCountDown;
    int cardBonus;
    int timeBonus;
    GdkColor bgcolor;
    
    int round;
    
    // interface.cpp 
    GtkWidget *createPlay();
    GtkWidget *createRight();
    
    // support.cpp
    void shuffle(int *deck);
    void startRound();
    char roundOver();
    
    bool isOnTop(GtkWidget *widget);
    bool isValid(GtkWidget *widget);
    void showTop();
    
    
    // events
    int timer;
    static gboolean timeout(gpointer ptr);
    static void exitGame(GtkWidget *widget, gpointer *ptr);
    static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void centerPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    static void dealPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    static void userPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    static void takeScore(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
public:
    FortressCrackers();
    GdkColor *backgroundColor();
    
    std::string name() const;
    std::string imageName() const;
    
    // initializes the game's variables and returns the
    // top level fixed widget
    GtkWidget *init();
    GtkWidget *getHelp();
    int gameType() const;
};

#endif // FortressCrackers_H
