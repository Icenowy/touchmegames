
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

#ifndef GAME_PTR_H
#define GAME_PTR_H


#include <string>
#include <vector>
#include <algorithm>
#include <gtk/gtk.h>
#include <tmgwave.h>
#include <config.h>


struct SCORE_STRUCT {
    std::string name;
    int score;
    double fscore;
    std::string date;
};

enum GAME_TYPES {
    UNKNOWN_GAME = 0,
    CARD_GAME,
    STRATEGY_GAME,
    WORD_GAME,
    DART_GAME
};

enum SCORE_TYPES {
    INTEGER = 0,
    INTEGER_DOWN,
    FLOAT,
    FLOAT_DOWN,
    TIME,
    TIME_DOWN
};

class GAME_PTR {
private:
    SCORE_TYPES scoreType;
    const GtkWidget *window;
    TmgWave *tmgWave;
    GtkWidget *exitDialog;
    GtkWidget *selWidget; // This widget is the event box in the menus
public:
    GAME_PTR();
    virtual ~GAME_PTR();
    // returns the name of the game
    virtual std::string name() const { return ""; };
    
    virtual GdkColor *backgroundColor() { return NULL; };
    
    // returns the filename of the image, thus allowing 
    // .jpg, .tiff, .bmp, etc
    virtual std::string imageName() const { return ""; };
    
    // returns the type of game, one of the GAME_TYPES
    virtual int gameType() const { return UNKNOWN_GAME; };
    
    // initializes the game's variables and returns the
    // top level h/vbox
    virtual GtkWidget *init() { return NULL; };

    virtual unsigned int score() const { return 0; };
    
    virtual void setWindow(const GtkWidget *mainWindow) { window = mainWindow; };
    virtual GtkWidget *getWindow() const { return (GtkWidget *)window; };
    
    virtual GtkWidget *getHelp();
    
    virtual GtkWidget *getAbout() const { return NULL; };
    virtual void cleanUp() {};
    
    // returns true to quit and false to cancel
    bool quitDialog();
    static void quitOkButtonReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    static void quitCancelButtonReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
    
    void setScoreType(SCORE_TYPES type);
    SCORE_TYPES getScoreType();
    
    static void userdone(GtkWidget *widget, gpointer ptr);
    
    int loadScores(SCORE_STRUCT scores[10]);
    int saveScores(const SCORE_STRUCT scores[10]);
    int addScore(SCORE_STRUCT scores[10],
                    SCORE_STRUCT newScore);
    int viewScores(SCORE_STRUCT scores[10], int newScore);

    void setWavePlayer(TmgWave *wave);
    
    // use this one if the wav file is under /TMGDIR/GAME_NAME/sounds
    void playWave(char *waveFile);
    // use this one if it is under another location
    void playWaveFullPath(char *waveFile);
    
    GtkWidget *loadImage(std::string imageFile);
    GtkWidget *loadMainImage(std::string imageFile);
    void reloadImage(GtkWidget *widget, std::string imageFile);
    void reloadMainImage(GtkWidget *widget, std::string imageFile);
    
    // utilities, etc.
    void addCommas(std::string &str);
    std::string addCommas(int number);
    void remCommas(std::string &str);
    
    // seed rand will call the srandom function with 
    // the gettimeofday value
    void seedRand();
    
    // Same as above, but will output the seed value
    // good for debugging games
    void seedRand(bool showSeed);
    
    // This will actually just seed the rand
    // function based on seed.  Such as if
    // a seed was output from the above function
    // and it gave a good debug rand.
    // Use this function instead of srand or srandom
    // as it uses one for Linux and one for Windows
    void seedRand(unsigned int seed);
    
    // pass the maximum number you want, i.e.
    // if you want 0-9, put max in.  This uses
    // the little (int)(max * (random() / (RAND_MAX + 1.0)))
    // usage from the man page.  This should
    // get better random numbers.
    int getRand(int max);
    
    void selectWidget(GtkWidget *widget);
    GtkWidget *selectWidget();
};

#endif // GAME_PTR_H
