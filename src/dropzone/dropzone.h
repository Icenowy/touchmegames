
/*	Copyright (C) 2008 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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

#ifndef DROPZONE_H
#define DROPZONE_H

#include <iostream>
#include <string>
#include <gameptr.h>
#include <altprogressbar.h>

class DropZone : public GAME_PTR {
private:
    int gameWidth, gameHeight;
    std::string gameName;
    
    GtkWidget *topFixed;
    
    SCORE_STRUCT scores[10];
    
    int playAreaWidth, playAreaHeight;
    int playAreaTopX, playAreaTopY;
    int playAreaBottomX, playAreaBottomY;
    int scoreAreaTopX, scoreAreaTopY;
    int scoreAreaBottomX, scoreAreaBottmY;
    int numColumns, numRows;
    int totalRows; // set this in init and don't change it
    int discDiameter, discRadius;
    int dropperWidth;
    int dropperOffset;
    int dropperX;
    int blinkCntr;
    
    enum GAME_STATES {
        WAITING,
        RUNNING,
        BLINKING,
        TAKE_ROUND_SCORE,
        TAKE_TOTAL_SCORE,
        BETWEEN_ROUNDS,
        OUT_OF_TIME,
        PAUSED,
        GAME_OVER,
    };
    GAME_STATES gameState;
    
    struct DZPath {
        int d000;
        int d030;
        int d060;
        int d090;
        int d120;
        int d150;
        int d180;
        int x, y;
        void clear() {
            d000 = -1;
            d030 = -1;
            d060 = -1;
            d090 = -1;
            d120 = -1;
            d150 = -1;
            d180 = -1;
            x = -1;
            y = -1;
        }
    };
    DZPath d[100];
    int secondDrop;
    
    GtkWidget *roundLabel;
    GtkWidget *scoreLabel;
    int currentScore;
    int roundScore;
    GtkWidget *hiScoreLabel;
    GtkWidget *topScoreLabel;
    GtkWidget *totalScoreLabel;
    AltProgressBar progress;
    GtkWidget *pauseButton;
    int timeLeft;
    int totalTime;
    
    GdkColor bgcolor;
    
    int round;
    
    bool discLocations[19][19];
    GtkWidget *discs[19][10];
    char imageStrs[10][10];
    bool dropped[19][10];
    GtkWidget *riserScrew[2][8];
    GtkWidget *riserTop;
    int screwWidth, screwHeight;
    int riserTopHeight;
    
    
    // interface.cpp 
    GtkWidget *dropper;
    GtkWidget *nextDisc, *onDeck;
    GtkWidget *createPlayAreaEvent();
    GtkWidget *createScoreArea();
    bool onBoard(int color);
    void moveCurrent(int x, int y);
    void moveOld(int x, int y, int color);
    void moveDropper(int x, int y);
    int moveRiser();
    int getData(int x, int y);
    void updateScore(int addition);
    
    // support.cpp
    void startRound();
    char roundOver();
    int dropDisc(int &column, gdouble xoffset, int &beginy, int color, bool doCrawl = true);
    bool isNew(DZPath *d, int x, int y, int dnum);
    int crawl(DZPath *d, int color, int &dnum);
    int checkDiscs();
    int checkDropped();
    void checkForFallthrough();
    int loadRound(int r);
    void checkEndOfLevel();
    void outputGame();
    
    // events
    int timer;
    static gboolean timeout(gpointer ptr);
    static void exitGame(GtkWidget *widget, gpointer *ptr);
    static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void pauseGame(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void playAreaPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    
public:
    DropZone();
    GdkColor *backgroundColor();
    
    std::string name() const;
    std::string imageName() const;
    
    // initializes the game's variables and returns the
    // top level fixed widget
    GtkWidget *init();
    GtkWidget *getHelp();
    int gameType() const;
};

#endif // DROPZONE_H
