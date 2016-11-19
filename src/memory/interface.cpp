
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

#include <memory.h>
#include <sstream>
#include <iomanip>

GtkWidget *Memory::createPlay() {
    GtkWidget *fixed;
    GtkWidget *eventBox;
    GtkWidget *image;
    int x, y;
    
    fixed = gtk_fixed_new();
    
    for (int x = 0; x < columns; ++x) {
        for (int y = 0; y < rows; ++y) {
            eventBox = gtk_event_box_new();
            gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
            g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                             G_CALLBACK(cardPressed), (gpointer)this);
            
            
            images[x][y] = loadImage("back.png");
            gtk_container_add((GtkContainer *)eventBox, images[x][y]);
            
            gtk_fixed_put((GtkFixed *)fixed, eventBox, cardWidth * x + x * 10, cardHeight * y + y * 10);
            
        }
    }
    
    return fixed;
}

GtkWidget *Memory::createRight() {
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
    
//     label = gtk_label_new(" ");
//     gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 5);
    
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
    
    
    
    label = gtk_label_new("Match Score:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Attempts:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("  x250:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Time Bonus:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Total Round:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    
    matchScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(matchScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)matchScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, matchScoreLabel, TRUE, TRUE, 2);
    
    attemptLabel = gtk_label_new("0");
    gtk_widget_modify_fg(attemptLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)attemptLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, attemptLabel, TRUE, TRUE, 2);
    
    attemptScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(attemptScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)attemptScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, attemptScoreLabel, TRUE, TRUE, 2);
    
    timeScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(timeScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)timeScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, timeScoreLabel, TRUE, TRUE, 2);
    
    totalRoundLabel = gtk_label_new("0");
    gtk_widget_modify_fg(totalRoundLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)totalRoundLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, totalRoundLabel, TRUE, TRUE, 2);
    
    
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 15);
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    
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
GtkWidget *Memory::init() {
    GtkWidget *bgImage;
    
    // Grab the current scores
    GAME_PTR::loadScores(scores);
    
    // Seed the random before anything happens
    seedRand();
    
    topFixed = gtk_fixed_new();
    gtk_widget_set_size_request(topFixed, 800, 600);
    
    bgImage = loadMainImage("games/bg.png");
    gtk_fixed_put((GtkFixed *)topFixed, bgImage, 0, 0);
    
    bgImage = loadImage("title.png");
    gtk_fixed_put((GtkFixed *)topFixed, bgImage, 100, 0);
    
    bgImage = loadImage("scorebg.png");
    gtk_fixed_put((GtkFixed *)topFixed, bgImage, 560, 125);
    
    bgImage = loadImage("playbg.png");
    gtk_fixed_put((GtkFixed *)topFixed, bgImage, 15, 125);
    
    gtk_fixed_put((GtkFixed *)topFixed, createPlay(), xpadding, ypadding);
    
    gtk_fixed_put((GtkFixed *)topFixed, createRight(), 590, 140);
    
    currentScore = 0;
    
    round = 0;
    startRound();
    
    // Change this timeout if needed.
    timer = g_timeout_add(200, timeout, this);
    
    gtk_widget_show_all(topFixed);
    return topFixed;
}
