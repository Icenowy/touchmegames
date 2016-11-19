
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

#include <fortresscrackers.h>
#include <sstream>
#include <iomanip>

// Fill this area with layout functions
GtkWidget *FortressCrackers::createPlay() {
    GtkWidget *fixed;
    GtkWidget *eventBox;
    GtkWidget *image;
    int idx(0);
    
    fixed = gtk_fixed_new();
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    
    image = loadMainImage("buttons/button_take_score.png");
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(takeScore), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, image);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 2, 2);
    
    // Next just add the cards
    // Add the first layer
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 3 * cardOffset, cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, cardOffset, 3 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 3 * cardOffset, 5 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 5 * cardOffset, 3 * cardOffset);
    ++idx;
    
    // Add the second layer
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 2 * cardOffset, cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 3 * cardOffset, 2 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 4 * cardOffset, cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, cardOffset, 2 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 2 * cardOffset, 3 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, cardOffset, 4 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 2 * cardOffset, 5 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 3 * cardOffset, 4 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 4 * cardOffset, 5 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 5 * cardOffset, 2 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 4 * cardOffset, 3 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 5 * cardOffset, 4 * cardOffset);
    ++idx;
    
    // Next the third layer
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 2 * cardOffset, 0);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 4 * cardOffset, 0);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 1 * cardOffset, cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 5 * cardOffset, cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 2 * cardOffset, 2 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 4 * cardOffset, 2 * cardOffset);
    ++idx;
    
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 2 * cardOffset, 4 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 4 * cardOffset, 4 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 1 * cardOffset, 5 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 5 * cardOffset, 5 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 2 * cardOffset, 6 * cardOffset);
    ++idx;
    
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 4 * cardOffset, 6 * cardOffset);
    ++idx;
    
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 0, 2 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 0, 4 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 6 * cardOffset, 2 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 6 * cardOffset, 4 * cardOffset);
    ++idx;
    
    
    // And finally the top level 4th
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 3 * cardOffset, 0);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 0, 3 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 3 * cardOffset, 6 * cardOffset);
    ++idx;
    
    cards[idx] = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(centerPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, cards[idx]);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 6 * cardOffset, 3 * cardOffset);
    ++idx;
    
    return fixed;
}

GtkWidget *FortressCrackers::createRight() {
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
    
    label = gtk_label_new("Streak:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new(" ");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Round Score:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Card Bonus:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Time Bonus:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Total Round:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    
    streakLabel = gtk_label_new("0");
    gtk_widget_modify_fg(streakLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)streakLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, streakLabel, TRUE, TRUE, 2);
    
    label = gtk_label_new(" ");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)rhvbox, label, FALSE, FALSE, 2);
    
    roundScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(roundScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)roundScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, roundScoreLabel, TRUE, TRUE, 2);
    
    cardScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(cardScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)cardScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, cardScoreLabel, TRUE, TRUE, 2);
    
    
    timeScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(timeScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)timeScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, timeScoreLabel, TRUE, TRUE, 2);
    
    totalScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(totalScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)totalScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, totalScoreLabel, TRUE, TRUE, 2);
    
    
    
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
GtkWidget *FortressCrackers::init() {
    GtkWidget *bgImage;
    GtkWidget *eventBox;
    GdkColor color;
    
    // Seed the random before anything happens
    seedRand();
    
    // Grab the current scores
    GAME_PTR::loadScores(scores);
    
    color.red = 0xFF00;
    color.green = 0xFF00;
    color.blue = 0xFF00;
    
    currentScore = 0;
    gameScore = 0;
    
    topFixed = gtk_fixed_new();
    gtk_widget_set_size_request(topFixed, 800, 600);
    
    bgImage = loadMainImage("games/bg.png");
    gtk_fixed_put((GtkFixed *)topFixed, bgImage, 0, 0);
    
    bgImage = loadImage("scorebg.png");
    gtk_fixed_put((GtkFixed *)topFixed, bgImage, 560, 125);
    
    bgImage = loadImage("playbg.png");
    gtk_fixed_put((GtkFixed *)topFixed, bgImage, 15, 15);
    
    gtk_fixed_put((GtkFixed *)topFixed, createPlay(), xpadding, ypadding);
    
    dealCard = loadImage("back.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(dealPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, dealCard);
    gtk_fixed_put((GtkFixed *)topFixed, eventBox, xpadding, 540 - cardHeight);
    
    userCard = loadImage("user.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(userPressed), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, userCard);
    gtk_fixed_put((GtkFixed *)topFixed, eventBox, xpadding + cardOffset * 6, 540 - cardHeight);
    
    
    // just add the bonus's here
    bonus[0] = loadImage("5000.png");
    gtk_fixed_put((GtkFixed *)topFixed, bonus[0], -bonusWidth, -bonusHeight);
    bonus[1] = loadImage("10000.png");
    gtk_fixed_put((GtkFixed *)topFixed, bonus[1], -bonusWidth, -bonusHeight);
    bonus[2] = loadImage("15000.png");
    gtk_fixed_put((GtkFixed *)topFixed, bonus[2], -bonusWidth, -bonusHeight);
    bonus[3] = loadImage("20000.png");
    gtk_fixed_put((GtkFixed *)topFixed, bonus[3], -bonusWidth, -bonusHeight);
    
    // bonus for not using the wild card
    bonus[4] = loadImage("5000.png");
    gtk_fixed_put((GtkFixed *)topFixed, bonus[4], -bonusWidth, -bonusHeight);
    
    
    takingScore = loadImage("takingscore.png");
    gtk_fixed_put((GtkFixed *)topFixed, takingScore, -takingScoreWidth, -takingScoreHeight);
    
    bgImage = loadImage("fc.png");
    gtk_fixed_put((GtkFixed *)topFixed, bgImage, 550, 20);
    
    gtk_fixed_put((GtkFixed *)topFixed, createRight(), 590, 140);
    
    cardsLeftLabel = gtk_label_new("15");
    gtk_widget_modify_fg(cardsLeftLabel, (GtkStateType)0, &color);
    gtk_fixed_put((GtkFixed *)topFixed, cardsLeftLabel, xpadding, 540 - cardHeight);
    
    round = 0;
    startRound();
    gtk_widget_show_all(topFixed);
    // Change this timeout if needed.
    timer = g_timeout_add(200, timeout, this);
    
    
    return topFixed;
}
