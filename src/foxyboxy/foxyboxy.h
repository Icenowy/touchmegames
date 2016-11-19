
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

#ifndef FoxyBoxy_H
#define FoxyBoxy_H

#include <iostream>
#include <string>
#include <gameptr.h>


class FoxyBoxy : public GAME_PTR {
private:
    enum BoxTypes {
        NONE = 0,
        Type1,
        Type2,
        Type3,
        Type4,
        Type5,
        Type6,
        Type7,
    };
    enum CoordError {
        HitBottom = -3,
        HitSide = -2,
        HitBlock = -1,
    };
    GtkWidget *blocks[10][22];
    BoxTypes nextBlockType;
    GtkWidget *nextBlocks[5][6];
    GtkWidget *bottomEvents[10];
    GtkWidget *paddleImage;
    GtkWidget *kbEntry;
    GtkWidget *pauseButton;
    struct Coord {
        int x;
        int y;
    };
    enum ExtraMoves {
        MoveNone = 0,
        MoveLeft,
        MoveRight,
        MoveDown,
    };
    bool keyDown, mouseClicked;
    int moveDownTurn;
    ExtraMoves extraMove;
    bool justDidExtraMove;
    int extra;
    bool alreadyPressed;
    int timeoutValue;
    bool spedUpTimer;
    int maxx, maxy;
    Coord coords[4]; // the coordinates for the current piece
    BoxTypes currentBox;
    
    GtkWidget *topvbox;
    SCORE_STRUCT scores[10];
    
    
    enum GAME_STATES {
        WAITING_TO_START,
        RUNNING,
        PAUSED,
        FLASH_LINES,
        GAME_OVER,
    };
    GAME_STATES gameState;
    int linesToFlash[4];
    int flashCntr;
    // interface.cpp vars
    
    GtkWidget *scoreLabel;
    GtkWidget *hiScoreLabel;
    GtkWidget *linesLabel;
    GtkWidget *levelLabel;
    int level;
    int currentScore;
    int lines;
    void showBlocks(std::string imgstr);
    void createAndShowBlocks();
    void removeLine(int line);
    void removeFlashLines();
    GtkWidget *createScoreBox();
    GtkWidget *createPlayArea();
    void initBoxes();
    void showNextBlock();
    void updateBottom();
    
    GdkColor color;
    // support.cpp
    int coordsOk(Coord c[4]);
    bool canFitBlock();
    void moveBlockDown();
    bool isLeft(int x, int y);
    bool isRight(int x, int y);
    bool isRotate(int x, int y);
    bool isDown(int x, int y);
    void moveLeft(bool fromEvents);
    void moveRight(bool fromEvents);
    void moveDown();
    void doAction(int x, int y);
    void rotateRight();
    void checkLines(); // checks for lines to remove
    void flashLines();
    void updateScore();
    // events
    
    int timer;
    static gboolean timeout(gpointer ptr);
    static void exitGame(GtkWidget *widget, gpointer *ptr);
    static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void pauseGame(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void startButtonPressed(GtkWidget *widget, gpointer ptr);
    static gboolean paddlePressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    static gboolean paddleReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    static gboolean keyPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static gboolean keyReleased(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
public:
    FoxyBoxy();
    GdkColor *backgroundColor();
    
    std::string name() const;
    std::string imageName() const;
    GtkWidget *topFixed;

    // initializes the game's variables and returns the
    // top level h/vbox
    GtkWidget *init();
    
    unsigned int score() const { return 0; };
    
    int gameType() const;
    GtkWidget *getHelp();
};

#endif // FoxyBoxy_H
