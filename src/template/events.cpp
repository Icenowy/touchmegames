
/*	Copyright (C) 2005 - 2008 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02011-1307  USA
 */

// events for Template

#include <template.h>
#include <sstream>
#include <iomanip>

void Template::exitGame(GtkWidget *widget, gpointer *ptr) {
    Template *t(reinterpret_cast<Template *>(*ptr));
    SCORE_STRUCT newScore;
    
    newScore.name = "";
    newScore.score = t->currentScore;
    newScore.date = "";
    
    
    t->GAME_PTR::addScore(t->scores, newScore);
    
    t->GAME_PTR::saveScores(t->scores);
    
    g_source_remove(t->timer);
    
    // This let's the main class know this game is over
    gtk_widget_hide(t->topFixed);
}

void Template::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    Template *t(reinterpret_cast<Template *>(ptr));
    
    if (t->quitDialog()) {
        g_source_remove(t->timer);
        // This let's the main class know this game is over
        gtk_widget_hide(t->topFixed);
    }
 
}


gboolean Template::timeout(gpointer ptr) {
    Template *t(reinterpret_cast<Template *>(ptr));

    switch (t->gameState) {
        case WAITING:
            // don't do anything, just wait for a press
            break;
        case RUNNING:
            break;
        case TAKE_ROUND_SCORE:
            break;
        case TAKE_TOTAL_SCORE:
            break;
        case BETWEEN_ROUNDS:
            break;
        case OUT_OF_TIME:
            break;
        case GAME_OVER:
            exitGame(NULL, &ptr);
            return FALSE;
            break;
        default:
            break;
    }
    
    return TRUE;
}


