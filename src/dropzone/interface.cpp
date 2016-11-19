
/*	Copyright (C) 2008 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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

#include <dropzone.h>
#include <sstream>
#include <iomanip>

// Fill this area with layout functions

GtkWidget *DropZone::createPlayAreaEvent() {
    GtkWidget *eventBox;
    
    eventBox = gtk_event_box_new();
    gtk_widget_set_size_request(eventBox, playAreaWidth, playAreaHeight);
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(playAreaPressed), (gpointer)this);
    
    
    for (int y = 0; y < numRows; ++y) {
        for (int x = 0; x < numColumns; ++x) {
            int offset = (numRows & 1) ? (y & 1) : ((y & 1) ? 0: 1);
            
            int xoff, yoff;
            xoff = x * discRadius + playAreaTopX;
            yoff = playAreaBottomY - ((numRows - y) * (int)((double)discDiameter * (7.0 / 8.0)));
            
            discs[x][y] = gtk_image_new();
            gtk_fixed_put((GtkFixed *)topFixed, discs[x][y], xoff, yoff);
        }
    }
    
    nextDisc = gtk_image_new();
    onDeck = gtk_image_new();
    g_object_set_data((GObject *)nextDisc, KEY_STR, (gpointer)0);
    g_object_set_data((GObject *)onDeck, KEY_STR, (gpointer)0);
    gtk_fixed_put((GtkFixed *)topFixed, nextDisc, dropperX + dropperOffset, playAreaTopY + discDiameter);
    gtk_fixed_put((GtkFixed *)topFixed, onDeck, dropperX + dropperOffset, playAreaTopY);
    dropper = loadImage("dropper.png");
    
    for (int x = 0; x < 2; ++x) {
        for (int y = 0; y < 8; ++y) {
            riserScrew[x][y] = loadImage("riserbase.png");
            gtk_fixed_put((GtkFixed *)topFixed, riserScrew[x][y], 0, 0);
        }
    }
    riserTop = loadImage("risertop.png");
    gtk_fixed_put((GtkFixed *)topFixed, riserTop, 0, 0);

    return eventBox;
}

// GtkWidget *DropZone::createScoreArea() {
//     GtkWidget *vbox;
//     
//     vbox = gtk_vbox_new(FALSE, 0);
//             
//     progress.init();
//     progress.width(145);
//     progress.height(35);
//     progress.image("/usr/local/share/touchmegames/consume21/images/progress.gif");
//     gtk_box_pack_start((GtkBox *)vbox, progress.widget(), FALSE, FALSE, 0);
//     
//     return vbox;
// }

GtkWidget *DropZone::createScoreArea() {
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *lhvbox, *rhvbox;
    GtkWidget *label;
    GtkWidget *eventBox;
    GtkWidget *image;
    GdkColor color;
    std::string str;
    std::stringstream sstr;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    color.red = 0xF600;
    color.green = 0xFF00;
    color.blue = 0x0000;
    
    image = loadImage("dz.png");
    gtk_box_pack_start((GtkBox *)vbox, image, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, hbox, FALSE, FALSE, 0);
    lhvbox = gtk_vbox_new(FALSE, 0);
    rhvbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, lhvbox, FALSE, FALSE, 2);
    gtk_box_pack_start((GtkBox *)hbox, rhvbox, TRUE, TRUE, 2);
    
    
    label = gtk_label_new("High Score:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Score:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Round:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    sstr << scores[0].score;
    str = sstr.str();
    addCommas(str);
    label = gtk_label_new(str.c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, label, TRUE, TRUE, 2);
    
    scoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(scoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)scoreLabel, 1.0, 0.5);
    
    gtk_box_pack_start((GtkBox *)rhvbox, scoreLabel, TRUE, TRUE, 2);
    
    roundLabel = gtk_label_new("1");
    gtk_widget_modify_fg(roundLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)roundLabel, 1.0, 0.5);
    
    gtk_box_pack_start((GtkBox *)rhvbox, roundLabel, TRUE, TRUE, 2);

    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 5);
    
    label = gtk_label_new("Time Left:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 1);
    
    progress.init();
    progress.width(151);
    progress.height(32);
    progress.defaultImage("progress1.gif");
    progress.percent(100);
    progress.text("1:30");
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    progress.textColor(color);
    
    gtk_box_pack_start((GtkBox *)vbox, progress.widget(), FALSE, FALSE, 10);
    
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
//     pauseButton = gtk_image_new_from_file("/usr/local/share/touchmegames/images/buttons/button_pause.png");
    pauseButton = loadMainImage("buttons/button_pause.png");
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(pauseGame), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, pauseButton);
    
    gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 5);
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 80);
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/buttons/button_quit.png");
    image = loadMainImage("buttons/button_quit.png");
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(userExit), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, image);
    gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 5);
    gtk_widget_show_all(eventBox);
    
    return vbox;
}

bool DropZone::onBoard(int color) {
    for (int x = 0; x < numColumns; ++x) {
        for (int y = 0; y < numRows; ++y) {
            if (discLocations[x][y]) {
                if (getData(x, y) == color) {
                    return true;
                }
            }
        }
    }
    return false;
}

void DropZone::moveCurrent(int x, int y) {
    GtkWidget *image;
    std::stringstream sstr;
    int offset = (numRows & 1) ? (y & 1) : ((y & 1) ? 0: 1);
    // check to see if it is a valid location
    bool valid(offset == (x & 1));
    int color(0);
    
    while (color == 0) {
        color = getRand(8) + 1;
        if (onBoard(color) == false) {
            color = 0;
        }
    }

    if (valid) {
        g_object_set_data((GObject *) discs[x][y], KEY_STR, g_object_get_data((GObject *)nextDisc, KEY_STR));
    }
    g_object_set_data((GObject *)nextDisc, KEY_STR, g_object_get_data((GObject *)onDeck, KEY_STR));
    g_object_set_data((GObject *)onDeck, KEY_STR, (gpointer)color);
    
    
    if (g_object_get_data((GObject *)nextDisc, KEY_STR) != 0) {
        reloadImage(nextDisc, imageStrs[(int)g_object_get_data((GObject *)nextDisc, KEY_STR)]);
    }
    if (g_object_get_data((GObject *)onDeck, KEY_STR) != 0) {
        reloadImage(onDeck, imageStrs[(int)g_object_get_data((GObject *)onDeck, KEY_STR)]);
    }
    if (valid && g_object_get_data((GObject *)discs[x][y], KEY_STR) != 0) {
        moveDropper(x, y);
        reloadImage(discs[x][y], imageStrs[(int)g_object_get_data((GObject *)discs[x][y], KEY_STR)]);
    }
}

void DropZone::moveOld(int x, int y, int color) {
    discLocations[x][y] = true;
    reloadImage(discs[x][y], imageStrs[color]);
    g_object_set_data((GObject *)discs[x][y], KEY_STR, (gpointer)color);
}

void DropZone::moveDropper(int x, int y) {
    dropperX = playAreaTopX + x * discRadius - dropperWidth / 2 + discRadius;
    gtk_fixed_move((GtkFixed *)topFixed, dropper, dropperX, playAreaTopY);
    gtk_fixed_move((GtkFixed *)topFixed, nextDisc, dropperX + dropperOffset, playAreaTopY + discDiameter);
    gtk_fixed_move((GtkFixed *)topFixed, onDeck, dropperX + dropperOffset, playAreaTopY);
}

int DropZone::moveRiser() {
    // check for discs in the top row
    for (int x = 0; x < numColumns; ++x) {
        if (getData(x, 0) > 0) {
            // we have one in the top row, x them, game over
            while (x < numColumns) {
                int c = getData(x, 0);
                if (c > 0) {
                    std::string str("disc");
                    str += (char)((int)g_object_get_data((GObject *)discs[x][0], KEY_STR) + '0');
                    str += "b.png";
                    reloadImage(discs[x][0], str);
                }
                ++x;
            }
            // give the user some time to see that the game is over
            timeLeft = 20;
            gameState = OUT_OF_TIME;
            return -1;
        }
    }
    for (int y = 0; y < numRows - 1; ++y) {
        for (int x = 0; x < numColumns; ++x) {
            // move the discs from the row below to this one
            if (getData(x, y + 1) > 0) {
                int color = getData(x, y + 1);
                discLocations[x][y + 1] = false;
                reloadImage(discs[x][y + 1], "discb.png");
                g_object_set_data((GObject *)discs[x][y + 1], KEY_STR, (gpointer)0);
                moveOld(x, y, color);
            }
        }
    }
    
    
    
    --numRows;
    gtk_widget_show(riserTop);
    gtk_fixed_move((GtkFixed *)topFixed, riserTop, playAreaTopX,
                        playAreaBottomY - (screwHeight * (totalRows - numRows)) + discDiameter / 8);
    for (int i = 0; i < totalRows - numRows; ++i) {
        // show the needed screw images
        gtk_widget_show(riserScrew[0][i]);
        gtk_widget_show(riserScrew[1][i]);
        // move them to the proper location

        gtk_fixed_move((GtkFixed *)topFixed, riserScrew[0][i], ((playAreaWidth / 3) - (screwWidth / 2)) + playAreaTopX,
                        playAreaBottomY - (screwHeight * (i + 1)) + discDiameter / 8 + riserTopHeight);
        gtk_fixed_move((GtkFixed *)topFixed, riserScrew[1][i], (((playAreaWidth / 3) * 2) - (screwWidth / 2)) + playAreaTopX,
                        playAreaBottomY - (screwHeight * (i + 1)) + discDiameter / 8 + riserTopHeight);
    }
    return 0;
}

int DropZone::getData(int x, int y) {
    if (GTK_IS_OBJECT((GtkObject *)discs[x][y]) == FALSE) {
        return -1;
    }
    return (int)g_object_get_data((GObject *)discs[x][y], KEY_STR);
}

void DropZone::updateScore(int addition) {
    currentScore += addition;
    gtk_label_set_text((GtkLabel *)scoreLabel, addCommas(currentScore).c_str());
}

// Create the layout and start the round
GtkWidget *DropZone::init() {
    GtkWidget *image;
    // Grab the current scores
    GAME_PTR::loadScores(scores);
    
    numRows = totalRows;
    
    topFixed = gtk_fixed_new();
    gtk_widget_set_size_request(topFixed, gameWidth, gameHeight);
    
    currentScore = 0;
    
    image = loadImage("bg.png");
    gtk_fixed_put((GtkFixed *)topFixed, image, 0, 0);
    
    // there is a problem HERE!!!!
//     look into doing a recursive widget destroy...it seems as though 
//             the widget destroy only destroys all children if it is a window!!!
    gtk_fixed_put((GtkFixed *)topFixed, createPlayAreaEvent(), playAreaTopX, playAreaTopY);
    
    gtk_fixed_put((GtkFixed *)topFixed, dropper, dropperX, playAreaTopY);
    
    gtk_fixed_put((GtkFixed *)topFixed, createScoreArea(), scoreAreaTopX, scoreAreaTopY);
    round = 0;
    
    gtk_widget_show_all(topFixed);
    
    startRound();
    
    // Change this timeout if needed.
    timer = g_timeout_add(200, timeout, this);
    
    return topFixed;
}
