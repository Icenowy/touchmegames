
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

#ifndef WordHunt_H
#define WordHunt_H

#include <iostream>
#include <string>
#include <gameptr.h>
#include <altprogressbar.h>

#define numberOfRows 12


class WordHunt : public GAME_PTR {
private:
    enum LINK_DIRECTION {
        NONE,
        HORIZONTAL,
        VERTICAL
    };
    enum MOVEMENT_DIRECTION {
        MOVEMENT_NONE,
        MOVEMENT_LEFT,
        MOVEMENT_RIGHT,
        MOVEMENT_UP,
        MOVEMENT_DOWN
    };
    enum GAME_STATES {
        GAME_PLAY,
        GAME_ROUND_CLEANUP,
        GAME_SHOW_WORDS,
        GAME_BETWEEN_ROUNDS,
        GAME_END
    };
    int gameState;
    int betweenRoundCountDown;
    std::string *words;
    bool wordFound[10];
    bool clearedBoard;
    bool highlighted[12][12];
    
    GtkWidget *topvbox;
    GtkWidget *fixed;
    GtkWidget *nextRoundWidget, *nextRoundImage;
    SCORE_STRUCT scores[10];
    GtkWidget *totalScore;
    GtkWidget *roundLabel;
    double timeLeft;
    GtkRcStyle *bgCircle;
    GtkRcStyle *bgBlack;
    GtkRcStyle *bgDone;
    GdkColor lightYellowColor, darkYellowColor;
    
    // interface.cpp vars
    int totalScoreInt;
    int usedLists[3];
    int round;
    GdkColor bgcolor;
    GtkWidget *table;
    GtkWidget *letters[numberOfRows][numberOfRows];
    //GtkWidget *timeBar;
    AltProgressBar progress;
    
    // interface.cpp functions
    GtkWidget *availableWords[10];
    bool loadList(int list, std::string &listName);
    void startRound();
    void startGame();
    bool checkAvailable(char array[12][12], std::string word, int x, int y, bool direction);
    static bool greaterThan(std::pair<char, int> lh, std::pair<char, int> rh);
    void fillWithRandom(char array[12][12]);
    bool fillArray(char array[12][12], std::string *words, bool recurse);
    GtkWidget *createListGUI();
    GtkWidget *initLh();
    GtkWidget *initRh();
    void showNextRound();
    
    // support.cpp
    
    // events
    int timer;
    int beginx, beginy;
    int endx, endy;
    int ignorex, ignorey;
    int lastx, lasty;
    bool checkNew;
    bool takeCareOfMove;
    int direction;
    int movement;
    bool clearing;
    bool hunting;
    int checkMatch();
    void findWord(std::string word);
    void findWords();
    static gboolean timeout(gpointer ptr);
    static void toggleBg(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    static void exitGame(GtkWidget *widget, gpointer *ptr);
    static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
    static void mouseMove(GtkWidget *widget, GdkEventMotion *event, gpointer ptr);
    bool checkForHorizontalEnd(int x, int y);
    bool checkHorizontalOk(int x, int y);
    void moveHorizontal(int x, int y);
    bool checkForVerticalEnd(int x, int y);
    bool checkVerticalOk(int x, int y);
    void moveVertical(int x, int y);
public:
    WordHunt();
    
    std::string name() const;
    std::string imageName() const;
    
    // initializes the game's variables and returns the
    // top level h/vbox
    GtkWidget *init();
    GdkColor *backgroundColor();
    GtkWidget *getHelp();
    unsigned int score() const { return 0; };
    int gameType() const;
};

#endif // WordHunt_H
