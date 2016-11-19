
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

// The AI of this game comes from kwin4 written by Martin Heni, slightly modified

#ifndef COMPILE4_H
#define COMPILE4_H

#include <iostream>
#include <string>
#include <gameptr.h>

class Compile4 : public GAME_PTR {
private:
    typedef enum e_farbe {Niemand=0,Gelb=1,Rot=2,Tip=3,Rand=4} Farbe;   /* Spielerfarbe */
    typedef char FARBE;
    enum {
            SIZE_Y = 5,
            SIZE_X = 6,
            SIZE_Y_ALL = 36
    };
    
    int blinky[4][2];
    bool showBlink;
    
    GtkWidget *widgetGrid[7][6];
    int betweenRoundCounter;
    int winner;
    
    GtkWidget *topFixed;
    GtkWidget *topvbox;
    GtkWidget *image; // circle widget
    GtkWidget *fixed;
    SCORE_STRUCT scores[10];
    GtkWidget *p1Circs[21], *p2Circs[21];
    int grid[7][6];
    
    // Widgets for between rounds
    GtkWidget *scoreWindow;
    GtkWidget *topScoreLabel;
    GtkWidget *moveLabel, *winLabel, *timePerMoveLabel;
    GtkWidget *moveValueLabel, *winValueLabel, *timePerMoveValueLabel;
    GtkWidget *totalValue1Label, *totalValue2Label;
    GtkWidget *scorebg;
    int timeForMoves;
    int total1, total2;
    
    int move;
    int round;
    int totalScore;
    int circx, circy;
    int circendy;
    
    
    // german vars
    int aktzug;
    char amZug;
    int beginner;
    int mymaxreklev;
    int second;
    FARBE feldmatrix[SIZE_Y_ALL+1][SIZE_X+1];
    char anzahlmatrix[SIZE_Y_ALL+1];
    long aktwert;
    char lenofrow[38];
    char startofrow[38];
    
    
    
    enum GAME_STATES {
            PLAYER_TURN,
            CPU_TURN,
            DROPPING_PLAYER,
            DROPPING_CPU,
            BETWEEN_ROUNDS,
            MULT_MOVES_OUTCOME,
            MULT_TOTAL_TTM,
            TAKE_SCORE,
            OUT_OF_TIME,
            GAME_OVER,
    };
    GAME_STATES gameState;
    
    
    GtkWidget *roundLabel;
    GtkWidget *scoreLabel;
    int currentScore;
    GtkWidget *hiScoreLabel;
    
    GtkWidget *createTopBox();
    
    GtkWidget *createBottomBox();
    
    GdkColor bgcolor;
    
    // support.cpp
    void startRound();
    void cleanAndShowScore();
    long think(bool hint);
    long Wertung(Farbe farbe,FARBE feld[][SIZE_X+1],char anzahl[],int reklev,int zug);
    long Bewertung(Farbe farbe,FARBE feld[][SIZE_X+1]);
    char GameOver(FARBE feld[][SIZE_X+1],char anzahl[]);
    void DoMove(char move,Farbe farbe,FARBE feld[][SIZE_X+1],char anzahl[]);
    int GetCompMove();
    char SwitchPlayer(Farbe m_amZug);
    char roundOver();
    
    // events
    int timer;
    static gboolean timeout(gpointer ptr);
    static void exitGame(GtkWidget *widget, gpointer *ptr);
    static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void colPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
public:
    Compile4();
    GdkColor *backgroundColor();
    
    std::string name() const;
    std::string imageName() const;
    
    // initializes the game's variables and returns the
    // top level h/vbox
    GtkWidget *init();
    GtkWidget *getHelp();
    int gameType() const;
};

#endif // Compile4_H
