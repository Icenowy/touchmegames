
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

#ifndef NONOGRAM_H
#define NONOGRAM_H

#include <iostream>
#include <string>
#include <gameptr.h>
#include <altprogressbar.h>

class NonoGram : public GAME_PTR {
private:
    int gameWidth, gameHeight;
    GtkWidget *topFixed;
    
    SCORE_STRUCT scores[10];
    
    int nonoSize;
    char nonoGram[10][10];
    int leftCnt[10][6], topCnt[10][6];
    int mouseState;
    GtkWidget *nonoTable;
    GtkWidget *cells[7][7];
    
    enum GAME_STATES {
        WAITING,
        RUNNING,
        TAKE_PUZZLE_SCORE,
        TAKE_TIME_LEFT_SCORE,
        TAKE_ERRORS_SCORE,
        TAKE_ROUND_SCORE,
        TAKE_TOTAL_SCORE,
        BETWEEN_ROUNDS,
        OUT_OF_TIME1,
        OUT_OF_TIME2,
        GAME_OVER,
    };
    GAME_STATES gameState;
    
    enum BLOCK_STATES {
        BLANK,
        FILLED,
        XED,
    };
    
    
    GtkWidget *roundLabel;
    GtkWidget *scoreLabel;
    int currentScore;
    int roundScore;
    int stageCountDown;
    GtkWidget *hiScoreLabel;
    GtkWidget *topScoreLabel;
    GtkWidget *totalScoreLabel;
    GtkWidget *puzzleScoreLabel;
    GtkWidget *roundScoreLabel;
    GtkWidget *timeLeftScoreLabel;
    int timeLeft;
    AltProgressBar progress;
    
    
    GdkColor bgcolor;
    
    int round;
    
    // interface.cpp 
    GtkWidget *createGrid();
    GtkWidget *createRight();
    
    // support.cpp
    void startRound();
    char roundOver();
    void clearNonoGram();
    int createNonoGram();
    int checkEdges(char ng[10][10]);
    int checkRcDone(char ng[10][10]);
    bool nextSetup(char n[6], int num);
    int fillOverlapsSub(int n[6], char ng[10]);
    int fillOverlaps(char ng[10][10]);
    bool solve();
    void test();
    bool checkBoard(char ng[10][10]);
    void fillBoard();
    
    // events
    int timer;
    static gboolean timeout(gpointer ptr);
    static void exitGame(GtkWidget *widget, gpointer *ptr);
    static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static gboolean blockClicked(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static gboolean blockMotion(GtkWidget *widget, GdkEventMotion *event, gpointer ptr);
public:
    NonoGram();
    GdkColor *backgroundColor();
    
    std::string name() const;
    std::string imageName() const;
    
    // initializes the game's variables and returns the
    // top level fixed widget
    GtkWidget *init();
    GtkWidget *getHelp();
    int gameType() const;
};

#endif // NONOGRAM_H
