
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

#include <threescompany.h>
#include <sstream>
#include <iomanip>


GtkWidget *ThreesCompany::createScoreBox() {
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *hbox;
    GtkRcStyle *rcstyle;
    GtkWidget *eventBox;
    GtkWidget *image;
    GdkColor color;
    std::stringstream sstr;
    
    hbox = gtk_hbox_new(false, 0);
    gtk_widget_set_size_request(hbox, 600, -1);
    
    rcstyle = gtk_rc_style_new();
    rcstyle->font_desc = pango_font_description_new();
    pango_font_description_set_size(rcstyle->font_desc, 12 * PANGO_SCALE);
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    rcstyle->fg[0] = color;
    rcstyle->color_flags[0] = GTK_RC_FG;
    
    label = gtk_label_new("Score:");
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
    gtk_widget_modify_style(label, rcstyle);
    
    sstr << std::setw(10) << "0";
    scoreLabel = gtk_label_new(sstr.str().c_str());
    gtk_widget_set_size_request(scoreLabel, 90, -1);
    gtk_box_pack_start((GtkBox *)hbox, scoreLabel, TRUE, TRUE, 0);
    gtk_widget_modify_style(scoreLabel, rcstyle);
    
    sstr.str("    ");
    label = gtk_label_new(sstr.str().c_str());
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
    gtk_widget_modify_style(label, rcstyle);
    
    label = gtk_label_new("High Score:");
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
    gtk_widget_modify_style(label, rcstyle);
    
    
//     sstr.str("");
//     sstr << std::setw(10) << scores[0].score;
    label = gtk_label_new(addCommas(scores[0].score).c_str());
    gtk_widget_set_size_request(label, 90, -1);
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, TRUE, 0);
    gtk_widget_modify_style(label, rcstyle);
    
    sstr.str("    ");
    label = gtk_label_new(sstr.str().c_str());
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
    gtk_widget_modify_style(label, rcstyle);
    
    label = gtk_label_new("Round:");
    gtk_widget_modify_style(label, rcstyle);
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
    
    sstr << std::setw(10) << "1";
    roundLabel = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_style(roundLabel, rcstyle);
    gtk_box_pack_start((GtkBox *)hbox, roundLabel, TRUE, FALSE, 0);
    pango_font_description_free(rcstyle->font_desc);
    return hbox;
}

GtkWidget *ThreesCompany::createCard(int num) {
    std::string str;
    std::stringstream sstr;
    GtkWidget *eventBox;
    GtkWidget *image;
    GtkRcStyle *style;
    GdkColor *color;
    
    sstr << deckLocation << num << ".gif";
    
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
    gtk_label_set_text((GtkLabel *)cardsLeftLabel, sstr.str().c_str());
    return eventBox;
}

void ThreesCompany::createMask() {
    GtkWidget *eventBox;
    GtkWidget *image;
    if (cardMask != NULL) {
        gtk_widget_destroy(cardMask);
    }
    cardMask = gtk_event_box_new();
    gtk_widget_set_size_request(cardMask, 50, 50);
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/threescompany/images/mask.xpm");
    image = loadImage("mask.png");
    gtk_container_add((GtkContainer *)cardMask, image);
    gtk_event_box_set_visible_window((GtkEventBox *)cardMask, FALSE);
    gtk_widget_add_events(cardMask, GDK_BUTTON_RELEASE_MASK);
    g_signal_connect(G_OBJECT(cardMask), "button-release-event",
                        G_CALLBACK(maskReleased), (gpointer)this);
    gtk_widget_show_all(cardMask);
}

void ThreesCompany::createHolds() {
    cardHolds[0] = gtk_event_box_new();
    gtk_widget_set_size_request(cardHolds[0], 96, 191);
    gtk_fixed_put((GtkFixed *)fixed, cardHolds[0], 155, 195);
    gtk_widget_add_events(cardHolds[0], GDK_BUTTON_RELEASE_MASK);
    gtk_event_box_set_visible_window((GtkEventBox *)cardHolds[0], FALSE);
    g_signal_connect(G_OBJECT(cardHolds[0]), "button-release-event",
                        G_CALLBACK(holdReleased), (gpointer)this);
    
    
    cardHolds[1] = gtk_event_box_new();
    gtk_widget_set_size_request(cardHolds[1], 96, 191);
    gtk_fixed_put((GtkFixed *)fixed, cardHolds[1], 286, 195);
    gtk_widget_add_events(cardHolds[1], GDK_BUTTON_RELEASE_MASK);
    gtk_event_box_set_visible_window((GtkEventBox *)cardHolds[1], FALSE);
    g_signal_connect(G_OBJECT(cardHolds[1]), "button-release-event",
                        G_CALLBACK(holdReleased), (gpointer)this);
    
    
    cardHolds[2] = gtk_event_box_new();
    gtk_widget_set_size_request(cardHolds[2], 96, 191);
    gtk_fixed_put((GtkFixed *)fixed, cardHolds[2], 417, 195);
    gtk_widget_add_events(cardHolds[2], GDK_BUTTON_RELEASE_MASK);
    gtk_event_box_set_visible_window((GtkEventBox *)cardHolds[2], FALSE);
    g_signal_connect(G_OBJECT(cardHolds[2]), "button-release-event",
                        G_CALLBACK(holdReleased), (gpointer)this);
    
    cardHolds[3] = gtk_event_box_new();
    gtk_widget_set_size_request(cardHolds[3], 96, 191);
    gtk_fixed_put((GtkFixed *)fixed, cardHolds[3], 548, 195);
    gtk_widget_add_events(cardHolds[3], GDK_BUTTON_RELEASE_MASK);
    gtk_event_box_set_visible_window((GtkEventBox *)cardHolds[3], FALSE);
    g_signal_connect(G_OBJECT(cardHolds[3]), "button-release-event",
                        G_CALLBACK(holdReleased), (gpointer)this);
    
    topCardHolds[0] = gtk_event_box_new();
    gtk_widget_set_size_request(topCardHolds[0], 96, 131);
    gtk_fixed_put((GtkFixed *)fixed, topCardHolds[0], 180, 35);
    gtk_widget_add_events(topCardHolds[0], GDK_BUTTON_RELEASE_MASK);
    gtk_event_box_set_visible_window((GtkEventBox *)topCardHolds[0], FALSE);
    g_signal_connect(G_OBJECT(topCardHolds[0]), "button-release-event",
                        G_CALLBACK(topHoldReleased), (gpointer)this);
    
    
    topCardHolds[1] = gtk_event_box_new();
    gtk_widget_set_size_request(topCardHolds[1], 96, 131);
    gtk_fixed_put((GtkFixed *)fixed, topCardHolds[1], 353, 35);
    gtk_widget_add_events(topCardHolds[1], GDK_BUTTON_RELEASE_MASK);
    gtk_event_box_set_visible_window((GtkEventBox *)topCardHolds[1], FALSE);
    g_signal_connect(G_OBJECT(topCardHolds[1]), "button-release-event",
                        G_CALLBACK(topHoldReleased), (gpointer)this);
    
    
    topCardHolds[2] = gtk_event_box_new();
    gtk_widget_set_size_request(topCardHolds[2], 96, 131);
    gtk_fixed_put((GtkFixed *)fixed, topCardHolds[2], 526, 35);
    gtk_widget_add_events(topCardHolds[2], GDK_BUTTON_RELEASE_MASK);
    gtk_event_box_set_visible_window((GtkEventBox *)topCardHolds[2], FALSE);
    g_signal_connect(G_OBJECT(topCardHolds[2]), "button-release-event",
                        G_CALLBACK(topHoldReleased), (gpointer)this);
}

// initializes the game's variables and returns the
// top level h/vbox
GtkWidget *ThreesCompany::init() {
    GtkWidget *eventBox2;
    GtkWidget *image;
    GtkRcStyle *rcstyle;
    GdkColor color;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkStyle *style;
    GtkWidget *label;
    std::string str;
    
    seedRand();
    GAME_PTR::loadScores(scores);
    totalScore = 0;
    round = 1;
    cards[0] = NULL;
    cards[1] = NULL;
    cards[2] = NULL;
    fixed = gtk_fixed_new();
    gtk_widget_set_size_request(fixed, 800, 600);
    
    for (int i = 0; i < 52; ++i) {
        cardDeck[i] = i;
    }
    
    currentScore = 500;
    timer = g_timeout_add(100, timeout, this);
    
    
    topEventBox = gtk_event_box_new();
    rcstyle = gtk_rc_style_new();
    
    rcstyle->color_flags[0] = GTK_RC_BG;
    str = INSTALL_DIR;
    str += "threescompany/images/bg.png";

    rcstyle->bg_pixmap_name[0] = (char *)str.c_str();
    gtk_widget_modify_style(topEventBox, rcstyle);
    
    
    gtk_fixed_put((GtkFixed *)fixed, createScoreBox(), 100, 10);
    gtk_container_add((GtkContainer *)topEventBox, fixed);
    gtk_widget_show_all(topEventBox);
    
//     image = gtk_image_new_from_file(deckBack.c_str());
    image = loadMainImage(deckBack.c_str());
    gtk_fixed_put((GtkFixed *)fixed, image, 100, 445);
    
    vbox = gtk_vbox_new(FALSE, 0);
    label = gtk_label_new("Cards");
    rcstyle = gtk_rc_style_new();
    rcstyle->font_desc = pango_font_description_new();
    pango_font_description_set_size(rcstyle->font_desc, 12 * PANGO_SCALE);
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    rcstyle->fg[0] = color;
    rcstyle->color_flags[0] = GTK_RC_FG;
    gtk_widget_modify_style(label, rcstyle);
    
    
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    gtk_fixed_put((GtkFixed *)fixed, vbox, 10, 460);
    
    label = gtk_label_new("Left:");
    gtk_widget_modify_style(label, rcstyle);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    cardsLeftLabel = gtk_label_new("49");
    gtk_widget_modify_style(cardsLeftLabel, rcstyle);
    gtk_box_pack_start((GtkBox *)vbox, cardsLeftLabel, FALSE, FALSE, 0);
    
    cardMask = NULL;
    
    vbox = gtk_vbox_new(FALSE, 0);
    label = gtk_label_new("Time Left");
    gtk_widget_modify_style(label, rcstyle);
    
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    gtk_fixed_put((GtkFixed *)fixed, vbox, 500, 450);
    
    progress.init();
    progress.width(151);
    progress.height(32);
    progress.defaultImage("timebar_full.png");
    progress.percent(100);
    
    gtk_box_pack_start((GtkBox *)vbox, progress.widget(), FALSE, FALSE, 0);
    
    createHolds();
    
    takeScore = gtk_event_box_new();
    gtk_widget_add_events(takeScore, GDK_BUTTON_RELEASE_MASK);
    gtk_event_box_set_visible_window((GtkEventBox *)takeScore, FALSE);
    g_signal_connect(G_OBJECT(takeScore), "button-release-event",
                        G_CALLBACK(takeScoreReleased), (gpointer)this);
    
    image = loadMainImage("buttons/button_take_score.png");
    gtk_container_add((GtkContainer *)takeScore, image);
    gtk_box_pack_start((GtkBox *)vbox, takeScore, FALSE, FALSE, 20);
    
    
    for (int i = 0; i < 3; ++i) {
        cardHoldImages[0][i] = NULL;
        cardHoldImages[1][i] = NULL;
        cardHoldImages[2][i] = NULL;
        cardHoldImages[3][i] = NULL;
        topCardHoldImages[i] = NULL;
    }
    startRound();
    
    createMask();
    gtk_fixed_put((GtkFixed *)fixed, cardMask, -100, 500);
    gtk_widget_show(cardMask);
    gtk_widget_show_all(topEventBox);
    
    eventBox2 = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox2, false);
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/exit.gif");
    image = loadMainImage("buttons/button_quit.png");
    gtk_widget_add_events(eventBox2, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox2), "button-press-event",
                        G_CALLBACK(userExit), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox2, image);
    gtk_fixed_put((GtkFixed *)fixed, eventBox2, 20, 50);
    gtk_widget_show_all(eventBox2);
    
    pango_font_description_free(rcstyle->font_desc);
    shown = false;
    
    
    
    return topEventBox;
};


void ThreesCompany::clearHold(int hold, int mult) {
    std::stringstream sstr;
    
    for (int i = 0; i < 3; ++i) {
        gtk_widget_destroy(cardHoldImages[hold][i]);
        cardHoldImages[hold][i] = NULL;
    }
//     totalScore = atoi(gtk_label_get_text((GtkLabel *)scoreLabel));
    totalScore += 2000 * mult;

    gtk_label_set_text((GtkLabel *)scoreLabel, addCommas(totalScore).c_str());
}
