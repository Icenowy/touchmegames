
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

#include <touchmegames.h>


TOUCHME_GAMES::TOUCHME_GAMES() : fullScreen(FULL_SCREEN), lastGameSelected(NULL) {
    previewTable = NULL;
    gameTable = NULL;

//     std::cout << "Init of TouchMe Games" << std::endl;
    games = new std::vector<GAME_PTR *>;
//     gameMap = new std::map<GtkWidget *, GAME_PTR *>;
    
//  add your game here
//  games.push_back(&demo);
    games->push_back(&gorillathrilla);
    games->push_back(&flogthedolphin);
    games->push_back(&videopoker);
    games->push_back(&threescompany);
    games->push_back(&tritowers);
    games->push_back(&foxyboxy);
    games->push_back(&compile4);
    games->push_back(&consume21);
    games->push_back(&sudoku);
    games->push_back(&dropzone);
    games->push_back(&nonogram);
    games->push_back(&fortress);
    games->push_back(&memory);
#ifndef WIN32
// I haven't gotten the xml2 lib on my 
// windows machine yet.
    games->push_back(&fotoseek);
    games->push_back(&secretphrase);
    games->push_back(&wordhunt);
#endif
}

TOUCHME_GAMES::~TOUCHME_GAMES() {
    delete games;
}

void TOUCHME_GAMES::loadSettings() {

}

void TOUCHME_GAMES::init() {
    loadSettings();
    tmgWave.init();
    for (std::vector<GAME_PTR *>::iterator it = games->begin(); it != games->end(); ++it) {
        (*it)->setWavePlayer(&tmgWave);
    }
    
    
    createWindow();
    
    createMainMenu();
    
    gtk_box_pack_start((GtkBox *)topVbox, mainMenu, FALSE, FALSE, 0);
    
    
    mainStyle = gtk_widget_get_style(topWindow);

    if (fullScreen) {
        gtk_window_fullscreen((GtkWindow *)topWindow);
    }
   
    gtk_window_set_resizable((GtkWindow *)topWindow, FALSE);
    gtk_widget_show_all(topWindow);
    gtk_widget_hide(gameSelectFixed);
    gtk_widget_hide(aboutBox);
    gtk_widget_hide(mainBack);
    gtk_widget_hide(gameBg);
}


GAME_PTR *TOUCHME_GAMES::gameFromWidget(GtkWidget *widget) {
    GAME_PTR *ptr;
    for (std::vector<GAME_PTR *>::iterator it = games->begin(); it != games->end(); ++it) {
        if (widget == (*it)->selectWidget()) {
            ptr = *it;
        }
    }
    return ptr;
}
