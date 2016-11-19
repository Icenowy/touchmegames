
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

#ifndef VIDEO_POKER_H
#define VIDEO_POKER_H

#include <iostream>
#include <string>
#include <gameptr.h>
#include <tmgtogglebutton.h>


class VideoPoker : public GAME_PTR {
private:
    enum HANDS {
        ROYAL_FLUSH,
        FOUR_OF_A_KIND,
        STRAIGHT_FLUSH,
        FULL_HOUSE,
        FLUSH,
        STRAIGHT,
        TRIPS,
        TWO_PAIR,
        PAIR,
        HIGH_CARD
    };
    const std::string deckLocation;
    const std::string deckBack;
    GtkWidget *topfixed;
    GtkWidget *topvbox;
    SCORE_STRUCT scores[10];
    
    int cardDeck[52];
    int cardLocation;
    int currentBet;
    int hand;
    
    
    enum GAME_STATES {
            SELECT_BET,
            DEAL_PRESSED_1,
            SELECT_CARDS,
            DEAL_PRESSED_2,
            HAND_OVER,
            GAME_OVER,
    };
    GAME_STATES gameState;
    
    // interface.cpp vars
    GtkWidget *cardBoxes[5];
    TmgToggleButton betButtons[5];
    GtkWidget *cardImages[5];
    GtkWidget *handLabel;
    GtkWidget *scoreLabel;
    GtkWidget *instructionLabel;
    int currentScore;
    int handOverPause;
    GtkWidget *hiScoreLabel;
    
    bool cardFront[5];
    int cardNumber[5];
    
    bool toggleOccuring;
    GtkWidget *createScoreBox();
    GtkWidget *createHandBox();
    GtkWidget *createInstructionBox();
    GtkWidget *createBetBox();
    GtkWidget *createPayoutBox();
    GdkColor color;
    // support.cpp
    
    // shuffle the deck and redraw the backs of the 
    // cards
    void startHand();
    void shuffle(int *deck);
    bool isFlush();
    bool isStraight();
    int pairs();
    int trips();
    int getHand(); 
    
    // events
    int currentFlipCard;
    int timer;
    static gboolean timeout(gpointer ptr);
    static void exitGame(GtkWidget *widget, gpointer *ptr);
    static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void betPlaced(void *first, void *second);
    static void cardSelected(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void dealPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
public:
    VideoPoker();
    GdkColor *backgroundColor();
    
    std::string name() const;
    std::string imageName() const;
    
    // initializes the game's variables and returns the
    // top level topfixed
    GtkWidget *init();
    
    unsigned int score() const { return 0; };
    
    int gameType() const;
    GtkWidget *getHelp();
 
 
};

#endif // VIDEO_POKER_H
