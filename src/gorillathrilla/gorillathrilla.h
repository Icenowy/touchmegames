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

#ifndef GORILLA_THRILLA_H
#define GORILLA_THRILLA_H

#include <iostream>
#include <string>
#include <map>
#include <gameptr.h>


class GORILLA_THRILLA : public GAME_PTR {
private:
    int round;
    GtkWidget *roundLabel;
    
    GtkWidget *topvbox;
    GtkWidget *topFixed;

    gint timer;
    
    
    GtkWidget *scoreLabel;
    GtkWidget *hiScoreLabel;
    
    int totalScore;
    
    int cardsLeft;
    bool gameOver, waitingForFirstClick, takingScore;
    
    // events
    static void exitGame(GtkWidget *widget, gpointer ptr);
    static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static gboolean timeout(gpointer ptr);
    
    static gint gameOverTimeout(gpointer data);
    
    
    static void dealPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static gboolean dealCardPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static gboolean middleCardPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void takeScore(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    
    void runGameOver();
    
    
    int getTimeLeft();
    void checkCards(GtkWidget *lh, GtkWidget *rh);
    void clearCard(GtkWidget *widget);
    int checkWinner();
    int getArrayPosition(int row, int card);
    int cardIn(int row, int card);
    int checkValidSelect(GtkWidget *widget);
    
    // interface variables
    GtkWidget *centerFixed, *rightFixed, *bottomFixed;
    GtkWidget *timeLabel;
    int timex, timey;
    GtkWidget *diskImage;
    GtkWidget *messageDialog;
    GtkWidget *messageImage;
    GtkWidget *messageLabel;
    GtkWidget *takeScoreImage;
    
    GtkWidget *dispCards[28];
    GtkWidget *dispCardBoxes[28];
    
    std::map<GtkWidget *, int> *cardMap;
    
    GtkWidget *dispSpot;
    GtkWidget *remainingLabel;
    int remainingDeck[24];
    GtkWidget *selectedCard;
    
    GtkWidget *dealCard;
    GtkWidget *dealCardBox;
    GtkWidget *dealSpot;
    
    int cardNumbers[52];
    
    // interface functions
    void insertIntoDisplay(int cardNumber, int displayLocation);
    int isGameOver();
    GtkWidget *createCenterBox();
    GtkWidget *createTopBox();
    GtkWidget *createSecondTopBox();
    GtkWidget *createBottomBox();
    void createMessageDialog();
    
    int getRow(int);
    int getCard(int);
    void startGame(); // initializes all the cards for play
    
    SCORE_STRUCT scores[10];
    GdkColor bgcolor;
public:
    GORILLA_THRILLA();
    virtual ~GORILLA_THRILLA();
    
    std::string name() const;
    std::string imageName() const;
    GdkColor *backgroundColor();
    // initializes the game's variables and returns the
    // top level h/vbox
    GtkWidget *init();
    
    GtkWidget *getHelp();
    int gameType() const;
    
    unsigned int score() const { return 0; };
};

#endif // GORILLA_THRILLA_H
