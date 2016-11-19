
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

#include <tritowers.h>
#include <sstream>
#include <iomanip>


GtkWidget *TriTowers::createTopBox() {
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *hbox;
    GtkRcStyle *rcstyle;
    GtkWidget *eventBox;
    GtkWidget *image;
    GdkColor color;
    std::stringstream sstr;
    
    hbox = gtk_hbox_new(false, 0);
    
    rcstyle = gtk_rc_style_new();
    rcstyle->font_desc = pango_font_description_new();
    pango_font_description_set_size(rcstyle->font_desc, 14 * PANGO_SCALE);
    pango_font_description_set_weight(rcstyle->font_desc, PANGO_WEIGHT_BOLD);
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    rcstyle->fg[0] = color;
    rcstyle->color_flags[0] = GTK_RC_FG;
    
    label = gtk_label_new("Score:");
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_style(label, rcstyle);
    
    sstr << std::setw(10) << "0";
    scoreLabel = gtk_label_new(sstr.str().c_str());
    gtk_widget_set_size_request(label, 110, -1);
    gtk_box_pack_start((GtkBox *)hbox, scoreLabel, TRUE, FALSE, 0);
    gtk_widget_modify_fg(scoreLabel, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_style(scoreLabel, rcstyle);
    
    sstr.str("    ");
    label = gtk_label_new(sstr.str().c_str());
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_style(label, rcstyle);
    
    label = gtk_label_new("High Score:");
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_style(label, rcstyle);
    
    
    sstr.str("");
    sstr << std::setw(10) << addCommas(scores[0].score);
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_set_size_request(label, 110, -1);
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_style(label, rcstyle);
    
    sstr.str("    ");
    label = gtk_label_new(sstr.str().c_str());
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_style(label, rcstyle);
    
    label = gtk_label_new("Round:");
    gtk_widget_modify_style(label, rcstyle);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
    
    sstr << std::setw(10) << "1";
    roundLabel = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(roundLabel, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_style(roundLabel, rcstyle);
    gtk_box_pack_start((GtkBox *)hbox, roundLabel, TRUE, FALSE, 0);
    
    pango_font_description_free(rcstyle->font_desc);
    return hbox;
}


GtkWidget *TriTowers::createMiddleBox() {
    GtkWidget *hbox;
    GtkWidget *label;
    GtkWidget *vbox;
    GtkWidget *image;
    GtkWidget *button;
    GdkColor color;
    GtkWidget *eventBox;
    PangoFontDescription *font;
    
    hbox = gtk_hbox_new(FALSE, 0);
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    // center center
    centerFixed = gtk_fixed_new();
            
    gtk_widget_set_size_request(GTK_WIDGET(centerFixed), 700, 350);
    
    gtk_box_pack_start((GtkBox *)hbox, centerFixed, TRUE, FALSE, 0);
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    
    image = loadMainImage("buttons/button_quit.png");
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(userExit), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, image);
    gtk_fixed_put((GtkFixed *)centerFixed, eventBox, 0, 20);
    gtk_widget_show_all(eventBox);
    
    eventBox = gtk_event_box_new();
    gtk_widget_add_events(eventBox, GDK_BUTTON_RELEASE_MASK);
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
		     G_CALLBACK(takeScorePressed), (gpointer)this);
    g_signal_connect(G_OBJECT(eventBox), "button-release-event",
		     G_CALLBACK(takeScoreReleased), (gpointer)this);
    
    
    image = loadMainImage("buttons/button_take_score.png");
    gtk_container_add((GtkContainer *)eventBox, image);
    gtk_fixed_put((GtkFixed *)centerFixed, eventBox, 584, 20);
    //gtk_box_pack_start((GtkBox *)hbox, eventBox, FALSE, FALSE, 20);
    
    color.red = 0xFFFF;
    color.green = 0;
    color.blue = 0;
    cardScore = gtk_label_new("");
    font = pango_font_description_new();
    pango_font_description_set_size(font, 16 * PANGO_SCALE);
    pango_font_description_set_weight(font, PANGO_WEIGHT_BOLD);
    gtk_widget_modify_font(cardScore, font);
    
    gtk_widget_modify_fg(cardScore, (GtkStateType)0, &color);
    gtk_fixed_put((GtkFixed *)centerFixed, cardScore, 0, 0);
    gtk_widget_show_all(centerFixed);
    
    pango_font_description_free(font);
    
    return hbox;
}

GtkWidget *TriTowers::createBottomBox() {
    GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *image;
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *eventBox;
    GdkColor color;
    std::stringstream sstr;
    PangoFontDescription *font;
    hbox = gtk_hbox_new(FALSE, 0);
    
    font = pango_font_description_new();
    pango_font_description_set_size(font, 16 * PANGO_SCALE);
    pango_font_description_set_weight(font, PANGO_WEIGHT_BOLD);
    
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(GTK_WIDGET(label), 50, -1);
    gtk_box_pack_start((GtkBox *)hbox, label, FALSE, FALSE, 0);
    
    
    remainingLabel = gtk_label_new("24 Cards Remaining");
    gtk_widget_modify_font(remainingLabel, font);
    gtk_box_pack_start((GtkBox *)hbox, remainingLabel, FALSE, FALSE, 0);
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
            
    label = gtk_label_new("     ");
    gtk_box_pack_start((GtkBox *)hbox, label, FALSE, FALSE, 0);
    
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/cards/backs/deck10.png");
    image = loadMainImage("cards/backs/deck10.png");
    
    gtk_container_add((GtkContainer *)eventBox, image);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(dealPressed), (gpointer)this);
    
    gtk_box_pack_start((GtkBox *)hbox, eventBox, FALSE, FALSE, 0);
    gtk_widget_show_all(image);
    label = gtk_label_new("     ");
    gtk_box_pack_start((GtkBox *)hbox, label, FALSE, FALSE, 0);
    
    // Where the deal card goes
    dealCardBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)dealCardBox, false);
    gtk_widget_add_events(dealCardBox, GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events(dealCardBox, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(dealCardBox, GDK_BUTTON1_MOTION_MASK);
    
    
    gtk_box_pack_start((GtkBox *)hbox, dealCardBox, FALSE, FALSE, 0);
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    label = gtk_label_new("High Run");
    gtk_widget_modify_font(label, font);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 1);
    
    highRunLabel = gtk_label_new("0");
    gtk_widget_modify_font(highRunLabel, font);
    gtk_widget_modify_fg(highRunLabel, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, highRunLabel, FALSE, FALSE, 1);
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Current Run");
    gtk_widget_modify_font(label, font);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 1);

    runLabel = gtk_label_new("0");
    gtk_widget_modify_font(runLabel, font);
    gtk_widget_modify_fg(runLabel, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, runLabel, FALSE, FALSE, 1);
    gtk_box_pack_start((GtkBox *)hbox, vbox, TRUE, FALSE, 10);
    
//     eventBox = gtk_event_box_new();
//     gtk_widget_add_events(eventBox, GDK_BUTTON_RELEASE_MASK);
//     gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
//     g_signal_connect(G_OBJECT(eventBox), "button-press-event",
//                         G_CALLBACK(takeScorePressed), (gpointer)this);
//     g_signal_connect(G_OBJECT(eventBox), "button-release-event",
//                         G_CALLBACK(takeScoreReleased), (gpointer)this);
    
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/buttons/button_take_score.png");
//     gtk_container_add((GtkContainer *)eventBox, image);
//     gtk_box_pack_start((GtkBox *)hbox, eventBox, FALSE, FALSE, 20);
    
    
    pango_font_description_free(font);
    return hbox;
}

GtkWidget *TriTowers::createCard(int num) {
    std::string str;
    std::stringstream sstr;
    GtkWidget *eventBox;
    GtkWidget *image;
    //GtkRcStyle *style;
    GdkColor *color;
    
    sstr << deckLocation << num << ".png";
    
//     image = gtk_image_new_from_file(sstr.str().c_str());
    image = loadMainImage(sstr.str().c_str());
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    gtk_container_add((GtkContainer *)eventBox, image);
    gtk_widget_add_events(eventBox, GDK_BUTTON_RELEASE_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-release-event",
                        G_CALLBACK(cardReleased), (gpointer)this);
    
    // just a little cheat
    g_object_set_data((GObject *)eventBox, KEY_STR, (void *)num);
    
    sstr.str("");
    sstr << std::setw(2) << (52 - cardLocation);
    gtk_label_set_text((GtkLabel *)remainingLabel, sstr.str().c_str());
    return eventBox;
}


// initializes the game's variables and returns the
// top level h/vbox
GtkWidget *TriTowers::init() {
    GdkColor color;
    GtkWidget *label;
    GtkWidget *hbox;
    GtkWidget *bgImage;
    
    seedRand();
    GAME_PTR::loadScores(scores);
    waitingForFirstClick = true;
    totalScore = 0;
    run = 0;
    highrun = 0;
    
    topFixed = gtk_fixed_new();

    bgImage = loadImage("fullbg.png");
    gtk_fixed_put((GtkFixed *)topFixed, bgImage, 0, 0);
    

    topvbox = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_size_request(topvbox, 800, 600);
    
    gtk_box_pack_start((GtkBox *)topvbox, createTopBox(), FALSE, FALSE, 0);
    
    gtk_box_pack_start((GtkBox *)topvbox, createMiddleBox(), FALSE, FALSE, 0);
    
    
    timeDealie = gtk_progress_bar_new();
    gtk_widget_set_size_request(timeDealie, 600, 30);
    gtk_widget_show(timeDealie);
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, timeDealie, TRUE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)topvbox, hbox, FALSE, FALSE, 10);
    
    gtk_box_pack_start((GtkBox *)topvbox, createBottomBox(), FALSE, FALSE, 0);
    
    for (int i = 0; i < 28; ++i) {
            dispCards[i] = NULL;
            dispCardBoxes[i] = NULL;
    }
    dealCard = NULL;
    
    bonusImages[0] = loadImage("500.gif");
    gtk_fixed_put((GtkFixed *)centerFixed, bonusImages[0], 0, -300);

    bonusImages[1] = loadImage("1000.gif");
    gtk_fixed_put((GtkFixed *)centerFixed, bonusImages[1], 0, -300);

    bonusImages[2] = loadImage("1500.gif");
    gtk_fixed_put((GtkFixed *)centerFixed, bonusImages[2], 0, -300);
    
    addingBonus = loadImage("addingbonus.gif");
    gtk_fixed_put((GtkFixed *)centerFixed, addingBonus, 0, -300);
    round = 1;
    startRound();
    timer = g_timeout_add(100, timeout, this);
    
    gtk_fixed_put((GtkFixed *)topFixed, topvbox, 0, 10);
    gtk_widget_show_all(topFixed);
    
    
    return topFixed;
}
