
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

#ifndef _MEMORY_H
#define _MEMORY_H

#include <iostream>
#include <string>
#include <gameptr.h>
#include <altprogressbar.h>

class Memory : public GAME_PTR {
private:
    int gameWidth, gameHeight;
    GtkWidget *topFixed;
    int cardHeight, cardWidth;
    int xpadding, ypadding;
    int columns, rows;
    GtkWidget *cardsPressed[2];
    int flipTicks;
    
    
    SCORE_STRUCT scores[10];
    
    
    
    enum GAME_STATES {
        WAITING,
        RUNNING,
        FLIP_CARDS,
        TAKE_ROUND_SCORE,
        TAKE_TOTAL_SCORE,
        BETWEEN_ROUNDS,
        OUT_OF_TIME,
        GAME_OVER,
    };
    GAME_STATES gameState;
    int stateCntr;
    
    GtkWidget *roundLabel;
    GtkWidget *scoreLabel;
    int currentScore;
    int roundScore;
    GtkWidget *hiScoreLabel;
    GtkWidget *topScoreLabel;
    GtkWidget *totalRoundLabel;
    GtkWidget *matchScoreLabel;
    GtkWidget *attemptScoreLabel;
    GtkWidget *timeScoreLabel;
    GtkWidget *attemptLabel;
    AltProgressBar progress;
    
    int currentBonus;
    int attempts;
    int timeLeft;
    int stageCountDown;
    int matchScore;
    int cardBonus;
    int timeBonus;
    bool started;
    int totalMatches, matches;
    
    // Must be the same as columns, rows
    GtkWidget *images[5][4];
    
    GdkColor bgcolor;
    
    int round;
    
    // interface.cpp 
    GtkWidget *createPlay();
    GtkWidget *createRight();
    
    // support.cpp
    void startRound();
    char roundOver();
    void updateScores();
    
    // events
    int timer;
    static gboolean timeout(gpointer ptr);
    static void exitGame(GtkWidget *widget, gpointer *ptr);
    static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void cardPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
public:
    Memory();
    GdkColor *backgroundColor();
    
    std::string name() const;
    std::string imageName() const;
    
    // initializes the game's variables and returns the
    // top level fixed widget
    GtkWidget *init();
    GtkWidget *getHelp();
    int gameType() const;
};

#endif // Memory_H
