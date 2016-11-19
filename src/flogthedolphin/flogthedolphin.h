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

#ifndef FLOG_THE_DOLPHIN_H
#define FLOG_THE_DOLPHIN_H

#include <iostream>
#include <string>
#include <map>
#include <gameptr.h>

#define ftdDeckLocation (INSTALL_DIR "/flogthedolphin/images/")

class FLOG_THE_DOLPHIN : public GAME_PTR {
private:
	enum GAME_STATES {
		startOfGame,
		startOfRound,
		middleOfRound,
		endOfRound,
		endOfGame
	};
	GtkWidget *eventBox;
	GdkColor bgcolor;
	GAME_STATES gameState;
	int round;
	GtkWidget *roundLabel;
	bool soundOn;
	
	std::string gameName;
	GtkWidget *topvbox;
	GtkWidget *bgImage1, *bgImage2;
	int bgImage1x, bgImage2x;
	GtkWidget *dolphinImage;
	GtkWidget *dolphinImages[25];
	int currentDolphin;
	double dolphinTilt, dolphinTiltReload;
	double dolphinx, dolphiny;
	double radians;
	double scale;
	GtkWidget *raftImage1, *raftImage2;
	int raft1x, raft2x;
	bool raft1hit, raft2hit;
	GtkWidget *chairImage;
	int chairx;
	bool chairhit;
	
	GtkWidget *buoyImage;
	int buoyx;
	gint timer;
	int rate;
	float hvelocity;
	float t;
	float newhvelocity;
	int angle;
	float initialy;
	
	int top8[8];
	int timeLeft;
	
	
	// events
	// variables
	float length;
	int totalLength;
	bool highDolphin;
	int endOfFlogCountDown;
	// functions
	static void exitGame(GtkWidget *widget, gpointer ptr);
	static void userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
	static gboolean timeout(gpointer ptr);
	static void flogTheDolphin(GtkWidget *widget, GdkEventKey *event, gpointer ptr);
	static void toggleSound(GtkWidget *widget, gpointer ptr);
	void checkBuoy(float dolphiny);
	void setLength();
	
	GtkWidget *fixed;
	void moveBgImage(GtkWidget *image, int &x);
	void moveRaft();
	void moveChair();
	void moveBuoy();
	SCORE_STRUCT scores[10];
	
	// interface.cpp
	// variables
	GtkWidget *scoreTable;
	GtkWidget *topScoreLabel;
	GtkWidget *scoreLabel;
	GtkWidget *currentLabel;
	
	GtkWidget *timeProgress;
	// functions
	GtkWidget *createScoreBox();
	GtkWidget *createTimeBox();
	
	// support.cpp
	void startRound();
	
public:
	FLOG_THE_DOLPHIN();
	virtual ~FLOG_THE_DOLPHIN();
	
	std::string name() const;
	std::string imageName() const;
	GdkColor *backgroundColor();
	GtkWidget *getHelp();
	// initializes the game's variables and returns the
	// top level h/vbox
	GtkWidget *init();
	int gameType() const;
	unsigned int score() const { return 0; };
};

#endif // FLOG_THE_DOLPHIN_H
