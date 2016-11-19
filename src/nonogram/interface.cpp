
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

#include <nonogram.h>
#include <sstream>
#include <iomanip>

// Fill this area with layout functions

GtkWidget *NonoGram::createGrid() {
    GtkWidget *image;
    int offset(nonoSize / 2 + nonoSize % 2);
    int posOffset;
    char str[7];
    
//     std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    nonoTable = gtk_table_new(10, 10, FALSE);
    
//     std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    for (int j = 0; j < offset; ++j) {
        for (int i = 0; i < nonoSize; ++i) {
            str[0] = leftCnt[i][j] + '0';
            str[1] = 0;
            strcat(str, ".png");
            
//             std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//             std::cout.flush();
            
            image = loadImage(str);
            
//             std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//             std::cout.flush();
            
            gtk_table_attach_defaults((GtkTable *)nonoTable, image, j, j + 1, i + offset, i + offset + 1);
        }
    }
    
//     std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    for (int j = 0; j < offset; ++j) {
        for (int i = 0; i < nonoSize; ++i) {
            str[0] = topCnt[i][j] + '0';
            str[1] = 0;
            strcat(str, ".png");
            image = loadImage(str);
            gtk_table_attach_defaults((GtkTable *)nonoTable, image, i + offset, i + offset + 1, j, j + 1);
        }
    }
    
//     std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    for (int x = 0; x < nonoSize; ++x) {
        for (int y = 0; y < nonoSize; ++y) {
            GtkWidget *image;
            GtkWidget *eventBox;
            std::string str("blockbg.png");
//             std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//             std::cout << "x, y " << x << ", " << y << std::endl;
//             std::cout.flush();
            
            image = loadImage(str);
//             image = loadImage("blockbg.png");
            
//             std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//             std::cout.flush();
            
            cells[x][y] = image;
            eventBox = gtk_event_box_new();
            
//             std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//             std::cout.flush();
            
            gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
            
            g_object_set_data((GObject  *)eventBox, KEY_STR, (void *)BLANK);
            
//             std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//             std::cout.flush();
            
            gtk_container_add((GtkContainer *)eventBox, cells[x][y]);
            gtk_widget_set_size_request(eventBox, 50, 50);
            
//             std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//             std::cout.flush();
            
            g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                             G_CALLBACK(blockClicked), (gpointer)this);
            g_signal_connect(G_OBJECT(eventBox), "motion-notify-event",
                             G_CALLBACK(blockMotion), (gpointer)this);
            
//             std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//             std::cout.flush();
            
            gtk_table_attach_defaults((GtkTable *)nonoTable, eventBox, x + offset, x + offset + 1, 
                y + offset, y + offset + 1);
            
//             std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//             std::cout.flush();
        }
    }
    
    
//     std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    if (round == 1) {
        posOffset = 72;
    }
    else if (round == 2) {
        posOffset = 47;
    }
    else {
        posOffset = 5;
    }
    gtk_fixed_put((GtkFixed *)topFixed, nonoTable, 35 + posOffset, 55 + posOffset);
    
    gtk_widget_show_all(topFixed);
    
//     std::cout << "NonoGram::createGrid() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    return nonoTable;
}

GtkWidget *NonoGram::createRight() {
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *lhvbox, *rhvbox;
    GtkWidget *label;
    GtkWidget *eventBox;
    GtkWidget *image;
    GtkWidget *scorevbox;
    GdkColor color;
    std::string str;
    std::stringstream sstr;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    color.red = 0xF600;
    color.green = 0xFF00;
    color.blue = 0x0000;
    
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
    progress.defaultImage("timebar_full.png");
    progress.percent(100);
    progress.text("2:00");
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    progress.textColor(color);
    
    gtk_box_pack_start((GtkBox *)vbox, progress.widget(), FALSE, FALSE, 10);
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 5);
    
    // scorevbox
    scorevbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, scorevbox, FALSE, FALSE, 5);
    
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, hbox, FALSE, FALSE, 0);
    lhvbox = gtk_vbox_new(FALSE, 0);
    rhvbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, lhvbox, FALSE, FALSE, 2);
    gtk_box_pack_start((GtkBox *)hbox, rhvbox, TRUE, TRUE, 2);
    
    color.red = 0xF600;
    color.green = 0xFF00;
    color.blue = 0x0000;
    label = gtk_label_new("Puzzle Score:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Time Score:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Round Score:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Total Score:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    
    
    puzzleScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(puzzleScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)puzzleScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, puzzleScoreLabel, TRUE, TRUE, 2);
    
    timeLeftScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(timeLeftScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)timeLeftScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, timeLeftScoreLabel, TRUE, TRUE, 2);
    
    
    roundScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(roundScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)roundScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, roundScoreLabel, TRUE, TRUE, 2);
    
    totalScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(totalScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)totalScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, totalScoreLabel, TRUE, TRUE, 2);
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)rhvbox, label, FALSE, FALSE, 15);
    
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

// Create the layout and start the round
GtkWidget *NonoGram::init() {
    GtkWidget *bgImage;

//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    // Grab the current scores
    GAME_PTR::loadScores(scores);
    
//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    currentScore = 0;
    nonoSize = 4;
    round = 0;
    
//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    topFixed = gtk_fixed_new();
    gtk_widget_set_size_request(topFixed, 800, 600);

//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    bgImage = loadImage("bg.png");
    gtk_fixed_put((GtkFixed *)topFixed, bgImage, 0, 0);
    
//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    gtk_fixed_put((GtkFixed *)topFixed, createRight(), 590, 140);
    
    clearNonoGram();
    
//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    startRound();
    
//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    gtk_widget_show_all(topFixed);

    // Change this timeout if needed.
    timer = g_timeout_add(200, timeout, this);
    
//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    return topFixed;
}
