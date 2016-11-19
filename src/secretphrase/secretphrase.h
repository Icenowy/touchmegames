
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

#ifndef SecretPhrase_H
#define SecretPhrase_H

#include <iostream>
#include <string>
#include <gameptr.h>
#include <altprogressbar.h>


class SecretPhrase : public GAME_PTR {
private:
	int totalSpin;
	int currentValue;
	GtkWidget *kbEntry;
	GtkWidget *fixed;
	GtkWidget *wheelImage1, *wheelImage2;
	GtkWidget *initWidget;
	int wheelNum;
	GtkWidget *wheelBar1, *wheelBar2;
	int wheelBarX1, wheelBarX2;
	int wheelSpin;
	GtkWidget *guessesLeftImg;
	int guessesLeft;
	// center box
	GtkWidget *centerLetters[4][14];
	char letters[4][14];
	int nums[3];
	char currentKey;
	int handleCountDown;
	int handlePosI, handlePosL;
	bool vowelsAllowed;
	bool solve;
	std::string category, rootname;
	GtkWidget *initRoundLabel;
	int wheelCntr;
	
	
	
	// lower box
	GtkWidget *infoLabel;
	AltProgressBar progress;
	int progressValue;
	GtkWidget *lowerLetters[2][13];
	bool used[26];
	
	GtkWidget *categoryWidget;
	
	GtkWidget *topvbox;
	SCORE_STRUCT scores[10];
	
	
	enum GAME_STATES {
		WAITING_TO_START,
		SELECTING_CATEGORY,
		RUNNING,
		HANDLING_KEY,
		SPINNING,
		AUTO_FILL,
		BETWEEN_ROUNDS,
		GAME_OVER,
	};
	enum Categories {
		CatMisc,
		CatSports,
		CatMusic,
		CatVideo	
	};
	GAME_STATES gameState;
	
	// interface.cpp vars
	GtkWidget *normVbox;
	GtkWidget *scoreLabel;
	GtkWidget *hiScoreLabel;
	GtkWidget *roundLabel;
	int round;
	int currentScore;
	
	GtkWidget *createScoreBox();
	GtkWidget *createAboveBoard();
	GtkWidget *createBoard();
	GtkWidget *createInfo();
	GtkWidget *createBottomLetters();
	GtkWidget *createInitScreen();
	GtkWidget *createNormScreen();
	
	GdkColor color;
	// support.cpp
	
	void initRound();
	void startRound();
	void initSpin();
	void spinWheel();
	std::string getPhrase();
	void fitPhrase(std::string phrase);
	void fillPhraseBacks();
	int getNumberOfOccurences(char ch);
	bool handleKey(char ch);
	bool checkFinished();
	bool showLetter();
	
	// events
	int timer;
	static gboolean timeout(gpointer ptr);
	static void exitGame(GtkWidget *widget, gpointer *ptr);
	static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
	static void lowerLetterPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
	static gboolean keyPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
	static gboolean keyReleased(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
	static void solvePhrase(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
	static void categorySelected(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
public:
	SecretPhrase();
	GdkColor *backgroundColor();
	
	std::string name() const;
	std::string imageName() const;
	
	// initializes the game's variables and returns the
	// top level h/vbox
	GtkWidget *init();
	
	unsigned int score() const { return 0; };
	
	int gameType() const;
	GtkWidget *getHelp();
};

#endif // SecretPhrase_H
