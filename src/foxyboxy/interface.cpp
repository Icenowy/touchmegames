
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

#include <foxyboxy.h>
#include <sstream>
#include <iomanip>

void FoxyBoxy::removeLine(int line) {
    for (int i = 0; i < 10; ++i) {
        int type((intptr_t)g_object_get_data((GObject *)blocks[i][line - 1], KEY_STR));
        std::stringstream sstr;
        if (type != 0) {
            
            sstr << "type" << type << ".png";
            
            reloadImage(blocks[i][line], sstr.str().c_str());
            g_object_set_data((GObject *)blocks[i][line], KEY_STR, (void *)type);
        }
        else {
            reloadImage(blocks[i][line], "blank.gif");
            g_object_set_data((GObject *)blocks[i][line], KEY_STR, (void *)0);
        }
    }
}

void FoxyBoxy::removeFlashLines() {
    int line(0);
    while (linesToFlash[line] != -1 && line < 4) {
        for (int i = linesToFlash[line]; i > 0; --i) {
                removeLine(i);
        }
        ++line;
    }
    for (int i = 0; i < 10; ++i) {
        reloadImage(blocks[i][0], "blank.gif");
        g_object_set_data((GObject *)blocks[i][0], KEY_STR, (void *)0);
    }
}

GtkWidget *FoxyBoxy::createScoreBox() {
    GtkWidget *hbox;
    hbox = gtk_hbox_new(FALSE, 0);
    return hbox;
}

GtkWidget *FoxyBoxy::createPlayArea() {
    GtkWidget *table;
    GtkWidget *hbox, *hbox2;
    GtkWidget *eventBox;
    GtkWidget *label;
    GtkWidget *image;
    GtkWidget *lhtopvbox, *vbox, *lhvbox, *rhvbox;
    GtkWidget *smallHbox;
    std::stringstream sstr;
    GtkRcStyle *rcstyle;
    GdkColor c;
    GdkColor light;
    c.red = 0xe600;
    c.green = 0xa500;
    c.blue = 0x0800;
    light.red = 0xffff;
    light.green = 0xffff;
    light.blue = 0xffff;
    
    // setup a style for all text
    rcstyle = gtk_rc_style_new();
    rcstyle->font_desc = pango_font_description_new();
    pango_font_description_set_size(rcstyle->font_desc, 12 * PANGO_SCALE);
    //pango_font_description_set_weight(rcstyle->font_desc, PANGO_WEIGHT_BOLD);
    color.red = 0xe600;
    color.green = 0xa500;
    color.blue = 0x0800;
    rcstyle->fg[0] = color;
    rcstyle->color_flags[0] = GTK_RC_FG;
    
    // master widget
    hbox = gtk_hbox_new(FALSE, 30);
    
    // left-most spacer
    label = gtk_label_new(" ");
    //gtk_widget_set_size_request(label, 10, 10);
    gtk_box_pack_start((GtkBox *)hbox, label, FALSE, FALSE, 0);

    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, 10, 200);

    // left side scores, etc.
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);

    hbox2 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, hbox2, FALSE, FALSE, 0);
    lhvbox = gtk_vbox_new(FALSE, 0);
    rhvbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox2, lhvbox, FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox2, rhvbox, FALSE, FALSE, 0);

    // High score label
    label = gtk_label_new("High Score:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_style(label, rcstyle);
    //gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &light);
    gtk_widget_set_size_request(label, 96, -1);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    // current score label
    label = gtk_label_new("Score:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_style(label, rcstyle);
    //gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &light);
    gtk_widget_set_size_request(label, 96, -1);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);

    // removed lines label
    label = gtk_label_new("Lines:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_style(label, rcstyle);
    //gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &light);
    gtk_widget_set_size_request(label, 96, -1);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    // level label
    label = gtk_label_new("Level:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_style(label, rcstyle);
    //gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &light);
    gtk_widget_set_size_request(label, 96, -1);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    

    // High score value
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    gtk_widget_set_size_request(eventBox, 60, -1);
    //gtk_widget_modify_bg(eventBox, (GtkStateType)0, &c);
    sstr << addCommas(scores[0].score);
    hiScoreLabel = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_style(hiScoreLabel, rcstyle);
    gtk_misc_set_alignment((GtkMisc *)hiScoreLabel, 1.0, 0.5);
    //gtk_widget_modify_fg(hiScoreLabel, (GtkStateType)0, &light);
    gtk_container_add((GtkContainer *)eventBox, hiScoreLabel);
    gtk_box_pack_start((GtkBox *)rhvbox, eventBox, FALSE, FALSE, 2);
    
    // current score value
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    gtk_widget_set_size_request(eventBox, 60, -1);
    //gtk_widget_modify_bg(eventBox, (GtkStateType)0, &c);
    scoreLabel = gtk_label_new("0");
    gtk_widget_modify_style(scoreLabel, rcstyle);
    gtk_misc_set_alignment((GtkMisc *)scoreLabel, 1.0, 0.5);
    //gtk_widget_modify_fg(scoreLabel, (GtkStateType)0, &light);
    gtk_container_add((GtkContainer *)eventBox, scoreLabel);
    gtk_box_pack_start((GtkBox *)rhvbox, eventBox, FALSE, FALSE, 2);
    
    // removed lines value
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    gtk_widget_set_size_request(eventBox, 60, -1);
    //gtk_widget_modify_bg(eventBox, (GtkStateType)0, &c);
    linesLabel = gtk_label_new("0");
    gtk_widget_modify_style(linesLabel, rcstyle);
    gtk_misc_set_alignment((GtkMisc *)linesLabel, 1.0, 0.5);
    //gtk_widget_modify_fg(linesLabel, (GtkStateType)0, &light);
    gtk_container_add((GtkContainer *)eventBox, linesLabel);
    gtk_box_pack_start((GtkBox *)rhvbox, eventBox, FALSE, FALSE, 2);
    
    // level value
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    gtk_widget_set_size_request(eventBox, 60, -1);
    //gtk_widget_modify_bg(eventBox, (GtkStateType)0, &c);
    levelLabel = gtk_label_new("1");
    gtk_widget_modify_style(levelLabel, rcstyle);
    gtk_misc_set_alignment((GtkMisc *)levelLabel, 1.0, 0.5);
    //gtk_widget_modify_fg(levelLabel, (GtkStateType)0, &light);
    gtk_container_add((GtkContainer *)eventBox, levelLabel);
    gtk_box_pack_start((GtkBox *)rhvbox, eventBox, FALSE, FALSE, 2);
    
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, 10, 120);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 5);
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    pauseButton = loadMainImage("buttons/button_pause.png");
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(pauseGame), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, pauseButton);
    
    gtk_box_pack_start((GtkBox *)hbox, vbox, FALSE, FALSE, 0);
    
    gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 0);
    
    
    // center section
    vbox = gtk_vbox_new(FALSE, 5);
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    table = gtk_table_new(22, 10, TRUE);
    gtk_container_add((GtkContainer *)eventBox, table);
    //gtk_widget_modify_bg(eventBox, (GtkStateType)0, &c);
    gtk_widget_set_size_request(eventBox, 250, 550);
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 22; ++y) {
            gtk_table_attach_defaults((GtkTable *)table, blocks[x][y], x, x + 1, y, y + 1);
        }
    }
    gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 0);
    
    
    table = gtk_table_new(1, 10, TRUE);
    gtk_box_pack_start((GtkBox *)vbox, table, FALSE, FALSE, 0);
    for (int x = 0; x < 10; ++x) {
        bottomEvents[x] = gtk_event_box_new();
        gtk_widget_set_size_request(bottomEvents[x], 25, 25);
        gtk_widget_modify_bg(bottomEvents[x], (GtkStateType)0, &c);
        gtk_table_attach_defaults((GtkTable *)table, bottomEvents[x], x, x + 1, 0, 1);
    }
    
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)hbox, label, FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, vbox, FALSE, FALSE, 0);
    
    
    
    
    // next block area
    
    vbox = gtk_vbox_new(FALSE, 10);
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, -1, 20);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("Next Block");
    gtk_widget_modify_style(label, rcstyle);
    //gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &light);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    smallHbox = gtk_hbox_new(FALSE, 0); // keep the l/r sides down
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    gtk_widget_set_size_request(eventBox, 125, 150);
    gtk_widget_modify_bg(eventBox, (GtkStateType)0, &c);
    table = gtk_table_new(6, 5, TRUE);
    gtk_widget_set_size_request(table, 125, 150);
    gtk_container_add((GtkContainer *)eventBox, table);
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 6; ++y) {
            gtk_table_attach_defaults((GtkTable *)table, nextBlocks[x][y], x, x + 1, y, y + 1);
        }
    }
    gtk_box_pack_start((GtkBox *)smallHbox, eventBox, TRUE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, smallHbox, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, 10, 50);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    gtk_widget_add_events(eventBox, GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(eventBox, GDK_BUTTON1_MOTION_MASK);
    
    g_signal_connect(G_OBJECT(eventBox), "button-press-event", 
                        G_CALLBACK(paddlePressed), (gpointer)this);
    g_signal_connect(G_OBJECT(eventBox), "button-release-event", 
                        G_CALLBACK(paddleReleased), (gpointer)this);
    
    paddleImage = loadImage("paddle.png");
    gtk_container_add((GtkContainer *)eventBox, paddleImage);
    
    
    
    gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 0);
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    
    image = loadMainImage("buttons/button_quit.png");
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(eventBox, GDK_BUTTON_RELEASE_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(userExit), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, image);
    gtk_box_pack_start((GtkBox *)vbox, eventBox, TRUE, FALSE, 0);
    
    gtk_box_pack_start((GtkBox *)hbox, vbox, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)hbox, label, FALSE, FALSE, 0);
    
    pango_font_description_free(rcstyle->font_desc);

    return hbox;
}

void FoxyBoxy::showBlocks(std::string imgstr) {
    for (int i = 0; i < 4; ++i) {
        reloadImage(blocks[coords[i].x][coords[i].y], imgstr.c_str());
    }
}

void FoxyBoxy::createAndShowBlocks() {
    int type(nextBlockType);
    std::string imgstr("type");
    
    currentBox = (BoxTypes)type;
    switch(type) {
        case Type1:
            coords[0].x = 4;
            coords[0].y = 0;
            coords[1].x = 5;
            coords[1].y = 0;
            coords[2].x = 5;
            coords[2].y = 1;
            coords[3].x = 5;
            coords[3].y = 2;
            imgstr += "1.png";
            break;
        case Type2:
            coords[1].x = 4;
            coords[1].y = 0;
            coords[0].x = 5;
            coords[0].y = 0;
            coords[2].x = 4;
            coords[2].y = 1;
            coords[3].x = 4;
            coords[3].y = 2;
            imgstr += "2.png";
            break;
        case Type3:
            coords[0].x = 4;
            coords[0].y = 0;
            coords[1].x = 5;
            coords[1].y = 0;
            coords[2].x = 5;
            coords[2].y = 1;
            coords[3].x = 6;
            coords[3].y = 1;
            imgstr += "3.png";
            break;
        case Type4:
            coords[0].x = 4;
            coords[0].y = 1;
            coords[1].x = 5;
            coords[1].y = 1;
            coords[2].x = 5;
            coords[2].y = 0;
            coords[3].x = 6;
            coords[3].y = 0;
            imgstr += "4.png";
            break;
        case Type5:
            coords[0].x = 4;
            coords[0].y = 0;
            coords[1].x = 5;
            coords[1].y = 0;
            coords[2].x = 4;
            coords[2].y = 1;
            coords[3].x = 5;
            coords[3].y = 1;
            imgstr += "5.png";
            break;
        case Type6:
            coords[0].x = 5;
            coords[0].y = 0;
            coords[1].x = 5;
            coords[1].y = 1;
            coords[2].x = 5;
            coords[2].y = 2;
            coords[3].x = 5;
            coords[3].y = 3;
            imgstr += "6.png";
            break;
        case Type7:
            coords[0].x = 4;
            coords[0].y = 1;
            coords[1].x = 5;
            coords[1].y = 1;
            coords[2].x = 6;
            coords[2].y = 1;
            coords[3].x = 5;
            coords[3].y = 0;
            imgstr += "7.png";
            break;
    }
    showBlocks(imgstr);
    nextBlockType = (BoxTypes)((rand() % 7) + 1);
    showNextBlock();
}


void FoxyBoxy::initBoxes() {
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 22; ++y) {
            blocks[x][y] = loadImage("blank.gif");
            g_object_set_data((GObject *)blocks[x][y], KEY_STR, NULL);
        }
    }
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 6; ++y) {
            nextBlocks[x][y] = loadImage("blank.gif");
        }
    }
}

void FoxyBoxy::showNextBlock() {
    std::stringstream sstr;
    sstr << "type" << nextBlockType << ".png";
    // clear the boxen
    for (int x = 1; x < 4; ++x) {
            for (int y = 1; y < 5; ++y) {
                reloadImage(nextBlocks[x][y], "blank.gif");
            }
    }
    switch (nextBlockType) {
        case Type1:
            reloadImage(nextBlocks[1][2], sstr.str().c_str());
            reloadImage(nextBlocks[2][2], sstr.str().c_str());
            reloadImage(nextBlocks[2][3], sstr.str().c_str());
            reloadImage(nextBlocks[2][4], sstr.str().c_str());
        break;
        case Type2:
            reloadImage(nextBlocks[2][2], sstr.str().c_str());
            reloadImage(nextBlocks[3][2], sstr.str().c_str());
            reloadImage(nextBlocks[2][3], sstr.str().c_str());
            reloadImage(nextBlocks[2][4], sstr.str().c_str());
        break;
        case Type3:
            reloadImage(nextBlocks[1][2], sstr.str().c_str());
            reloadImage(nextBlocks[2][2], sstr.str().c_str());
            reloadImage(nextBlocks[2][3], sstr.str().c_str());
            reloadImage(nextBlocks[3][3], sstr.str().c_str());
        break;
        case Type4:
            reloadImage(nextBlocks[1][3], sstr.str().c_str());
            reloadImage(nextBlocks[2][3], sstr.str().c_str());
            reloadImage(nextBlocks[2][2], sstr.str().c_str());
            reloadImage(nextBlocks[3][2], sstr.str().c_str());
        break;
        case Type5:
            reloadImage(nextBlocks[1][2], sstr.str().c_str());
            reloadImage(nextBlocks[2][2], sstr.str().c_str());
            reloadImage(nextBlocks[1][3], sstr.str().c_str());
            reloadImage(nextBlocks[2][3], sstr.str().c_str());
        break;
        case Type6:
            reloadImage(nextBlocks[2][1], sstr.str().c_str());
            reloadImage(nextBlocks[2][2], sstr.str().c_str());
            reloadImage(nextBlocks[2][3], sstr.str().c_str());
            reloadImage(nextBlocks[2][4], sstr.str().c_str());
        break;
        case Type7:
            reloadImage(nextBlocks[1][3], sstr.str().c_str());
            reloadImage(nextBlocks[2][2], sstr.str().c_str());
            reloadImage(nextBlocks[2][3], sstr.str().c_str());
            reloadImage(nextBlocks[3][3], sstr.str().c_str());
        break;
}
}


void FoxyBoxy::updateBottom() {
    for (int x = 0; x < 10; ++x) {
        if (x != coords[0].x && x != coords[1].x && x != coords[2].x && x != coords[3].x) {
            gtk_event_box_set_visible_window((GtkEventBox *)bottomEvents[x], FALSE);
        }
        else {
            gtk_event_box_set_visible_window((GtkEventBox *)bottomEvents[x], TRUE);
        }
    }
}

// initializes the game's variables and returns the
// top level h/vbox
GtkWidget *FoxyBoxy::init() {
    GtkStyle *style;
    GtkWidget *eventBox;
    GtkWidget *bgImage;
    GdkColor color;
    GtkWidget *entry;
    time_t sectime;
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;

    GAME_PTR::loadScores(scores);
    // reset all values
    timeoutValue = 120;
    spedUpTimer = false;
    currentScore = 0;
    level = 1;
    lines = 0;
    moveDownTurn = false;
    justDidExtraMove = false;
    extraMove = MoveNone;
    alreadyPressed = false;
    keyDown = false;
    mouseClicked = false;
    
    // seed the random with the number of seconds since 1970
    time(&sectime);
    srand(sectime);

    topFixed = gtk_fixed_new();
    bgImage = loadImage("fullbg.png");
    gtk_fixed_put((GtkFixed *)topFixed, bgImage, 0, 0);

    
    topvbox = gtk_vbox_new(FALSE, 0);
    
    initBoxes();
    
    
    gtk_box_pack_start((GtkBox *)topvbox, createScoreBox(), FALSE, FALSE, 4);
    
    gtk_box_pack_start((GtkBox *)topvbox, createPlayArea(), FALSE, FALSE, 4);
    for (int x = 0; x < 10; ++x) {
            gtk_event_box_set_visible_window((GtkEventBox *)bottomEvents[x], FALSE);
    }
    
    currentScore = 0;
    timer = g_timeout_add(timeoutValue, timeout, this);
    nextBlockType = (BoxTypes)((rand() % 7) + 1);

    gtk_fixed_put((GtkFixed *)topFixed, topvbox, 0, 0);
    gtk_widget_show_all(topFixed);

    gameState = RUNNING;
    startButtonPressed(NULL, this);
    
    // I'm sure there's a better way to get the keyboard input
    // if there is no real keyboard widget, but I didn't read
    // enough
    kbEntry = gtk_entry_new();
    gtk_widget_set_size_request(kbEntry, 1, 1);
    gtk_widget_show(kbEntry);
    gtk_widget_grab_focus(kbEntry);
    gtk_box_pack_start((GtkBox *)topvbox, kbEntry, FALSE, FALSE, 0);
    
    g_signal_connect(G_OBJECT(kbEntry), "key-press-event", 
                        G_CALLBACK(keyPressed), (gpointer)this);
    g_signal_connect(G_OBJECT(kbEntry), "key-release-event", 
                        G_CALLBACK(keyReleased), (gpointer)this);
    
    
    return topFixed;
};
