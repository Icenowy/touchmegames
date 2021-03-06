
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

#ifndef TOUCHME_GAMES_H
#define TOUCHME_GAMES_H

#include <map>

#include <gtk/gtk.h>
#include <touchmegames.h>
#include <gameptr.h>

// Includes for each game
//#include <demo.h>
#include <gorillathrilla.h>
#include <videopoker.h>
#include <flogthedolphin.h>
#include <foxyboxy.h>
#include <threescompany.h>
#include <tritowers.h>
#include <compile4.h>
#include <consume21.h>
#include <sudoku.h>
#include <dropzone.h>
#include <nonogram.h>
#include <fortresscrackers.h>
#include <memory.h>

#ifndef WIN32
// I haven't gotten the xml2 lib on my 
// windows machine yet.
#include <secretphrase.h>
#include <wordhunt.h>
#include <fotoseek.h>
#endif

#define VERSION "0.2.5"

class TOUCHME_GAMES {
private:
    // main settings
    bool fullScreen;

    GAME_PTR gamePtr;
    
    GtkWidget *topWindow;
    GtkWidget *topHbox;
    GtkWidget *topVbox;
    GtkWidget *selectBox;
    GtkWidget *gameSelectFixed;
    GtkWidget *startButton, *aboutButton;
    GtkWidget *previewBox;
    GtkWidget *previewTable;
    GtkWidget *previewBg;
    GtkWidget *lastGameSelected;
    GtkWidget *mainMenu;
    GtkWidget *menuCaptions[5];
    GtkWidget *menuWidgets[5];
    GtkWidget *gameTable;
    GtkWidget *aboutBox;
    GtkWidget *aboutImage;
    GtkWidget *mainBack;
    GtkWidget *mainAbout;
    GtkWidget *mainQuit;
    GtkWidget *bgImage;
    GtkWidget *gameBg;
    
    TmgWave tmgWave;
    
    // Instance of each game.
    //DEMO demo;
    GORILLA_THRILLA gorillathrilla;
    VideoPoker videopoker;
    FLOG_THE_DOLPHIN flogthedolphin;
    ThreesCompany threescompany;
    TriTowers tritowers;
    FoxyBoxy foxyboxy;
    Compile4 compile4;

    Consume21 consume21;
    Sudoku sudoku;
    DropZone dropzone;
    NonoGram nonogram;
    FortressCrackers fortress;
    Memory memory;
#ifndef WIN32
// I haven't gotten the xml2 lib on my 
// windows machine yet.
    FotoSeek fotoseek;
    SecretPhrase secretphrase;
    WordHunt wordhunt;
#endif
    
    std::vector<GAME_PTR *> *games;
//     std::map<GtkWidget *, GAME_PTR *> *gameMap;
    
    GtkStyle *mainStyle;
    
    void createWindow();
    void addGamesToWindow(int gameType, int startNum);
    void createPreviewBox(GAME_PTR *);
    void createHelpBox(GAME_PTR *ptr);
    
    void createMainMenu();
    void createAbout();
    
    // events.cpp
    static void showHighScores(GtkWidget *widget, GdkEvent *event, gpointer ptr);
    static void showHelp(GtkWidget *widget, GdkEvent *event, gpointer ptr);
    static void startGame(GtkWidget *widget, GdkEvent *event, gpointer ptr);
    static void gameEnded(GtkWidget *widget, gpointer ptr);
    static void showSubMenu(GtkWidget *widget, GdkEvent *event, gpointer ptr);
    static void showMain(GtkWidget *widget, GdkEvent *event, gpointer ptr);
    static void showAbout(GtkWidget *widget, GdkEvent *event, gpointer ptr);
    static void rightArrowPressed(GtkWidget *widget, GdkEvent *event, gpointer ptr);
    static void leftArrowPressed(GtkWidget *widget, GdkEvent *event, gpointer ptr);
    
    // support.cpp
    void loadSettings();
    
    GAME_PTR *gameFromWidget(GtkWidget *widget);
public:
    TOUCHME_GAMES();
    ~TOUCHME_GAMES();
    void init();
};


#endif // TOUCHME_GAMES_H
