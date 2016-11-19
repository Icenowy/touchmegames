
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

#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <string>
#include <gameptr.h>
#include <altprogressbar.h>

class Sudoku : public GAME_PTR {
private:
    
    GtkWidget *fixed;
    GtkWidget *tophbox;
    GtkWidget *scorevbox;
    
    SCORE_STRUCT scores[10];
    AltProgressBar progress;
    int timeLeft;
    // Widgets for between rounds
    GtkWidget *topScoreLabel;
    GtkWidget *tiles[6][6];
    int numbers[6][6];
    int numberMask[6][6];
    int maxColumns, maxRows;
    int perXSector, perYSector;
    int maxInt;
    int currentInt;
    int errors;
    int stageCountDown;
    
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
    
    
    GtkWidget *roundLabel;
    GtkWidget *scoreLabel;
    int currentScore;
    int roundScore;
    GtkWidget *hiScoreLabel;
    
    // score vbox
    GtkWidget *puzzleScoreLabel;
    GtkWidget *timeLeftScoreLabel;
    GtkWidget *errorsLabel;
    GtkWidget *errorPointsLabel;
    GtkWidget *roundScoreLabel;
    GtkWidget *totalScoreLabel;
    
    GdkColor bgcolor;
    
    int round;
    
    // interface.cpp 
    GtkWidget *createFixed();
    GtkWidget *createGrid();
    GtkWidget *createRight();
    void setTile(GtkWidget *tile, int number);
    void setGrid();
    
    // support.cpp
    bool checkHorizontal(int number, int x, int y);
    bool checkVertical(int number, int x, int y);
    int addNum(int number, int xsector, int ysector);
    void generateGrid();
    void removeNumbers(int total);
    void startRound();
    char roundOver();
    
    int colHas(int col, int num);
    int rowHas(int row, int num);
    bool isEmpty(int col, int row);
    int getSector(int col, int row);
    bool checkRows(int checkFor);
    bool checkCols(int checkFor);
    bool checkSectors(int checkFor);
    bool checkRow4();
    bool checkCol4();
    bool checkSeconds();
    bool checkThirds();
    void generatePuzzle(int min, int max, int numToRemove);
    void solve();
    bool checkSolution();
    bool correctPlacement(GtkWidget *widget, int attempt);
    
    // events
    int timer;
    static gboolean timeout(gpointer ptr);
    static void exitGame(GtkWidget *widget, gpointer *ptr);
    static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void tilePressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void numberClicked(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void solveClicked(GtkWidget *, gpointer ptr);
    
public:
    Sudoku();
    GdkColor *backgroundColor();
    
    std::string name() const;
    std::string imageName() const;
    
    // initializes the game's variables and returns the
    // top level h/vbox
    GtkWidget *init();
    GtkWidget *getHelp();
    int gameType() const;
};

#endif // Sudoku_H
