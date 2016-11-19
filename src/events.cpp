
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

// events

#include <touchmegames.h>
#include <stdint.h>

void TOUCHME_GAMES::showHighScores(GtkWidget *widget, GdkEvent *event, gpointer ptr) {
    TOUCHME_GAMES *sg(reinterpret_cast<TOUCHME_GAMES *>(ptr));
    GtkWidget *w;
    GdkColor *color;
    GtkStyle *style;
    intptr_t game;
    
    if (sg->lastGameSelected) {
            gtk_event_box_set_visible_window((GtkEventBox *)sg->lastGameSelected, FALSE);
            gtk_widget_destroy(sg->previewTable);
    }
    
    game = (intptr_t)g_object_get_data((GObject *)widget, KEY_STR);

    gtk_fixed_move((GtkFixed *)sg->gameSelectFixed, sg->gameBg, (game & 1 ? 160: 10), ((game / 2) * 140) + 60);
    gtk_widget_show(sg->gameBg);

    sg->lastGameSelected = widget;
    
    sg->createPreviewBox(sg->gameFromWidget(widget));
    gtk_widget_show(sg->startButton);
    gtk_widget_show(sg->aboutButton);
}

void TOUCHME_GAMES::showHelp(GtkWidget *widget, GdkEvent *event, gpointer ptr) {
    TOUCHME_GAMES *sg(reinterpret_cast<TOUCHME_GAMES *>(ptr));
    GtkWidget *w;
    GdkColor *color;
    GtkStyle *style;
    if (sg->lastGameSelected) {
            gtk_widget_destroy(sg->previewTable);
    }
    
    sg->createHelpBox(sg->gameFromWidget(sg->lastGameSelected));
}

void TOUCHME_GAMES::startGame(GtkWidget *widget, GdkEvent *event, gpointer ptr) {
    TOUCHME_GAMES *sg(reinterpret_cast<TOUCHME_GAMES *>(ptr));
    GtkWidget *w;
    
    if (sg->lastGameSelected) {
        widget = sg->lastGameSelected;
    }
    
    
    if (sg->gameFromWidget(widget)->backgroundColor() != NULL) {
        gtk_widget_modify_bg(sg->topWindow, GTK_STATE_NORMAL, sg->gameFromWidget(widget)->backgroundColor());
    }
    
    gtk_widget_hide(sg->gameSelectFixed);

    w = sg->gameFromWidget(widget)->init();
    
    gtk_box_pack_start((GtkBox *)sg->topVbox, w, FALSE, FALSE, 0);
    
    g_signal_connect(G_OBJECT(w), "hide",
                        G_CALLBACK(gameEnded), sg);
}

void TOUCHME_GAMES::gameEnded(GtkWidget *widget, gpointer ptr) {
    TOUCHME_GAMES *sg(reinterpret_cast<TOUCHME_GAMES *>(ptr));
    GdkColor color;

    gtk_widget_destroy(widget);
    
    gtk_widget_show_all(sg->gameSelectFixed);
    
    
    color.red = 0x9000;
    color.green = 0x9000;
    color.blue = 0x9000;
    
    gtk_widget_modify_bg(sg->topWindow, (GtkStateType)0, &color);
    
    if (sg->lastGameSelected) {
	    gtk_event_box_set_visible_window((GtkEventBox *)sg->lastGameSelected, FALSE);
	    gtk_widget_destroy(sg->previewTable);
    }
    
    sg->createPreviewBox(sg->gameFromWidget(sg->lastGameSelected));
}

void TOUCHME_GAMES::showSubMenu(GtkWidget *widget, GdkEvent *event, gpointer ptr) {
    TOUCHME_GAMES *sg(reinterpret_cast<TOUCHME_GAMES *>(ptr));
    intptr_t gameType((intptr_t)g_object_get_data((GObject *)widget, KEY_STR));
    
    if (sg->lastGameSelected) {
        sg->lastGameSelected = NULL;
        gtk_widget_destroy(sg->previewTable);
    }
    sg->addGamesToWindow(gameType, 0);
    gtk_widget_hide(sg->mainMenu);
    gtk_widget_show_all(sg->gameSelectFixed);
    gtk_widget_hide(sg->gameBg);
    gtk_widget_hide(sg->startButton);
    gtk_widget_hide(sg->aboutButton);
    
}

void TOUCHME_GAMES::showMain(GtkWidget *widget, GdkEvent *event, gpointer ptr) {
    TOUCHME_GAMES *sg(reinterpret_cast<TOUCHME_GAMES *>(ptr));
    
    gtk_widget_show(sg->bgImage);

    sg->gamePtr.reloadMainImage(sg->bgImage, "mainmenu/bg_main.png");
    
    if (sg->gameTable != NULL) {
        gtk_widget_destroy(sg->gameTable);
        sg->gameTable = NULL;
    }

    gtk_widget_hide(sg->gameSelectFixed);
    gtk_widget_show_all(sg->mainMenu);
    
    gtk_widget_hide(sg->aboutBox);
    gtk_widget_show(sg->mainAbout);
    gtk_widget_show(sg->mainQuit);
    gtk_widget_hide(sg->mainBack);
    gtk_widget_hide(sg->gameBg);
}

void TOUCHME_GAMES::showAbout(GtkWidget *widget, GdkEvent *event, gpointer ptr) {
    TOUCHME_GAMES *sg(reinterpret_cast<TOUCHME_GAMES *>(ptr));
    
    for (int i = 0; i < 5; ++i) {
        gtk_widget_hide(sg->menuWidgets[i]);
        gtk_widget_hide(sg->menuCaptions[i]);
    }
    
    gtk_widget_show(sg->aboutBox);
    gtk_widget_hide(sg->mainAbout);
    gtk_widget_hide(sg->mainQuit);
    gtk_widget_show(sg->mainBack);
}

void TOUCHME_GAMES::leftArrowPressed(GtkWidget *widget, GdkEvent *event, gpointer ptr) {
    TOUCHME_GAMES *tg(reinterpret_cast<TOUCHME_GAMES *>(ptr));
    unsigned long val;
    
    val = (intptr_t)g_object_get_data((GObject *)widget, KEY_STR);
    
    if (tg->lastGameSelected) {
        tg->lastGameSelected = NULL;
        gtk_widget_destroy(tg->previewTable);
    }
    
    if (tg->gameTable != NULL) {
            gtk_widget_destroy(tg->gameTable);
    }
    tg->addGamesToWindow(val >> 16, val & 0xFFFF);
    gtk_widget_hide(tg->mainMenu);
    gtk_widget_show_all(tg->gameSelectFixed);
    gtk_widget_hide(tg->gameBg);
    gtk_widget_hide(tg->startButton);
    gtk_widget_hide(tg->aboutButton);

}

void TOUCHME_GAMES::rightArrowPressed(GtkWidget *widget, GdkEvent *event, gpointer ptr) {
    TOUCHME_GAMES *tg(reinterpret_cast<TOUCHME_GAMES *>(ptr));
    unsigned long val;
    
    val = (intptr_t)g_object_get_data((GObject *)widget, KEY_STR);
    
    if (tg->lastGameSelected) {
        tg->lastGameSelected = NULL;
        gtk_widget_destroy(tg->previewTable);
    }
    
    if (tg->gameTable != NULL) {
            gtk_widget_destroy(tg->gameTable);
    }
    tg->addGamesToWindow(val >> 16, val & 0xFFFF);
    gtk_widget_hide(tg->mainMenu);
    gtk_widget_show_all(tg->gameSelectFixed);
    gtk_widget_hide(tg->gameBg);
    gtk_widget_hide(tg->startButton);
    gtk_widget_hide(tg->aboutButton);
}
