
/*	Copyright (C) 2005 - 2008 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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

#ifndef FOTOSEEK_H
#define FOTOSEEK_H

#include <iostream>
#include <string>
#include <vector>
#include <gameptr.h>
#include <altprogressbar.h>
#include <tmgimage.h>

struct FSDifference {
    int centerx;
    int centery;
    int radius;
    bool found;
};

class FotoSeek : public GAME_PTR {
private:
    int gameWidth, gameHeight;
    int imageWidth, imageHeight;
    int playXOffset, playYOffset;
    int minRadius;
    unsigned long foundColor, missedColor, matchColor;
    GtkWidget *topFixed;
    TmgImage leftImage, rightImage, imageMask;
    AltProgressBar progress, points;
    int timeLeft, totalTime, betweenRound, pointsLeft, totalPoints;
    int ticksPerSecond;
    std::vector<std::string> *usedDiffs;
    
    GtkWidget *hintImages[3];
    int currentHint;
    
    SCORE_STRUCT scores[10];
    FSDifference diffCoords[5];
    int totalDiffs;
    
    enum GAME_STATES {
        WAITING,
        RUNNING,
        TAKE_ROUND_SCORE,
        TAKE_TOTAL_SCORE,
        BETWEEN_ROUNDS,
        OUT_OF_TIME,
        GAME_OVER,
    };
    GAME_STATES gameState;
    
    
    GtkWidget *roundLabel;
    GtkWidget *scoreLabel;
    int currentScore;
    int roundScore;
    GtkWidget *hiScoreLabel;
    GtkWidget *topScoreLabel;
    GtkWidget *totalScoreLabel;
    
    GdkColor bgcolor;
    
    int round;
    
    // interface.cpp
    void createPlayArea();
    void createTop();
    void test();
    void paintOutside(int x, int y, bool found);
    void paintOutsideHorizontal(int x, int y, bool found);
    void paintCircle(int x, int y, bool found);
    void paintCircle(FSDifference *diff, bool found);
    void paintUnfound();
    void updateScore(int points);
    
    // support.cpp
    void initVars();
    void startRound();
    char roundOver();
    void adjustDiffs();
    void addDiffs(char *file);
    void checkCircle(int x, int y);
    int maxImageNum(std::vector<std::string> dir);
    int maxImageSubNum(int imageNum, std::vector<std::string> dir);
    std::string getNextImage(char *path);
    
    
    // events
    int timer;
    static gboolean timeout(gpointer ptr);
    static void exitGame(GtkWidget *widget, gpointer *ptr);
    static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void playAreaClicked(GtkWidget *widget, GdkEventButton *event, gpointer *ptr);
    static void hintClicked(GtkWidget *widget, GdkEventButton *event, gpointer *ptr);
    
public:
    FotoSeek();
    GdkColor *backgroundColor();
    
    std::string name() const;
    std::string imageName() const;
    
    // initializes the game's variables and returns the
    // top level fixed widget
    GtkWidget *init();
    GtkWidget *getHelp();
    int gameType() const;
};

#endif // FOTOSEEK_H
