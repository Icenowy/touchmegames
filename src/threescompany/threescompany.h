
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

#ifndef THREES_COMPANY_H
#define THREES_COMPANY_H

#include <iostream>
#include <string>
#include <gameptr.h>
#include <altprogressbar.h>


class ThreesCompany : public GAME_PTR {
private:
    const std::string deckLocation;
    const std::string deckBack;
    GtkWidget *topvbox;
    GtkWidget *fixed;
    GtkWidget *topEventBox;
    GtkWidget *takeScore;
    SCORE_STRUCT scores[10];
    
    int cardDeck[52];
    int cardLocation;
    int round;
    int totalScore;
    
    
    enum GAME_STATES {
            PLAY_ROUND,
            BETWEEN_ROUNDS,
            TAKE_SCORE,
            OUT_OF_TIME,
            GAME_OVER,
    };
    GAME_STATES gameState;
    int betweenRoundCntr;
    GtkWidget *cards[3];
    
    // interface.cpp vars
    AltProgressBar progress;
    float timeLeft;
    GtkWidget *cardHoldImages[4][3];
    GtkWidget *cardHolds[4];
    GtkWidget *topCardHolds[3];
    GtkWidget *topCardHoldImages[3];
    GtkWidget *cardMask;
    GtkWidget *cardMasksCard;
    bool shown;
    
    GtkWidget *roundLabel;
    GtkWidget *scoreLabel;
    GtkWidget *cardsLeftLabel;
    GtkWidget *instructionLabel;
    int currentScore;
    int handOverPause;
    
    GtkWidget *hiScoreLabel;
    GtkRcStyle *downStyle, *upStyle;
    
    
    int cardNumber[5];
    
    bool toggleOccuring;
    GtkWidget *createScoreBox();
    GtkWidget *createCard(int num);
    void createHolds();
    void createMask();
    GdkColor bgcolor;
    // support.cpp
    
    // shuffle the deck and redraw the backs of the 
    // cards
    void startRound();
    void shuffle(int *);
    
    // events
    int currentFlipCard;
    int timer;
    static gboolean timeout(gpointer ptr);
    static void exitGame(GtkWidget *widget, gpointer *ptr);
    static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void cardSelected(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void dealPressed(GtkWidget *widget, gpointer *ptr);
    static void cardReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    
    
    static void holdReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    static void topHoldReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    static void maskReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    
    static void takeScorePressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    static void takeScoreReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    
    int checkTrip(int hold);
    void clearHold(int hold, int mult);
public:
    ThreesCompany();
    GdkColor *backgroundColor();
    
    std::string name() const;
    std::string imageName() const;
    
    // initializes the game's variables and returns the
    // top level h/vbox
    GtkWidget *init();
    GtkWidget *getHelp();
    int gameType() const;
    unsigned int score() const { return 0; };
};

#endif // ThreesCompany_H
