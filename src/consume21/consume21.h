
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

#ifndef CONSUME21_H
#define CONSUME21_H

#include <iostream>
#include <string>
#include <gameptr.h>
#include <altprogressbar.h>

class Consume21 : public GAME_PTR {
private:
    
    GtkWidget *tophbox;
    GtkWidget *rightvbox;
    GtkWidget *image; // circle widget
    GtkWidget *fixed;
    GtkWidget *columnTotalLabels[5];
    GtkWidget *columnsTotal;
    GtkWidget *spill, *collect;
    GtkWidget *roundImage;
    GtkWidget *scoreBoxen[9];
    SCORE_STRUCT scores[10];
    AltProgressBar progress;
    float timeLeft;
    // Widgets for between rounds
    GtkWidget *topScoreLabel;
    
    enum GAME_STATES {
        WAITING,
        RUNNING,
        TAKE_SCORE,
        OUT_OF_TIME,
        GAME_OVER,
    };
    GAME_STATES gameState;
    
    
    GtkWidget *roundLabel;
    GtkWidget *scoreLabel;
    int currentScore;
    int roundScore;
    GtkWidget *hiScoreLabel;
    
    GdkColor bgcolor;
    
    
    GtkWidget *mugImages[35];
    int totalMugImages;
    int columnMugs[5];
    int columnTotal[5];
    int columnMugValues[5][7];
    int round;
    int multiplier;
    
    // interface.cpp 
    GtkWidget *createFixed();
    GtkWidget *createRightTop();
    GtkWidget *createRightBottom();
    void createMug();
    void setTotalWin(int i);
    
    // support.cpp
    void startRound();
    char roundOver();
    void getCurrentScore();
    int getMultiplier();
    
    // events
    int timer;
    static gboolean timeout(gpointer ptr);
    static void exitGame(GtkWidget *widget, gpointer *ptr);
    static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void colPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void spillPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void spillReleased(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void collectPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void collectReleased(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
public:
    Consume21();
    GdkColor *backgroundColor();
    
    std::string name() const;
    std::string imageName() const;
    
    // initializes the game's variables and returns the
    // top level h/vbox
    GtkWidget *init();
    GtkWidget *getHelp();
    int gameType() const;
};

#endif // Consume21_H
