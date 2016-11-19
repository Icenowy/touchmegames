
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

#include <secretphrase.h>
#include <sstream>
#include <iomanip>

GtkWidget *SecretPhrase::createScoreBox() {
    GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *eventBox;
    GtkWidget *image;
    GtkRcStyle *rcstyle;
    GdkColor c;
    std::stringstream sstr;
    c.red = 0xFFFF;
    c.green = 0xFFFF;
    c.blue = 0xFFFF;
    
    rcstyle = gtk_rc_style_new();
    rcstyle->font_desc = pango_font_description_new();
    pango_font_description_set_size(rcstyle->font_desc, 16 * PANGO_SCALE);
    pango_font_description_set_weight(rcstyle->font_desc, PANGO_WEIGHT_BOLD);
    
    
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_size_request(hbox, 800, 75);
    
    vbox = gtk_vbox_new(FALSE, 0);
    label = gtk_label_new("Score:");
    gtk_widget_modify_style(label, rcstyle);
    gtk_widget_modify_fg(label, (GtkStateType)0, &c);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    scoreLabel = gtk_label_new("0");
    gtk_widget_modify_style(scoreLabel, rcstyle);
    gtk_widget_modify_fg(scoreLabel, (GtkStateType)0, &c);
    gtk_box_pack_start((GtkBox *)vbox, scoreLabel, FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, vbox, TRUE, FALSE, 0);
    
    
    vbox = gtk_vbox_new(FALSE, 0);
    label = gtk_label_new("Round:");
    gtk_widget_modify_style(label, rcstyle);
    gtk_widget_modify_fg(label, (GtkStateType)0, &c);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    roundLabel = gtk_label_new("1");
    gtk_widget_modify_style(roundLabel, rcstyle);
    gtk_widget_modify_fg(roundLabel, (GtkStateType)0, &c);
    gtk_box_pack_start((GtkBox *)vbox, roundLabel, FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, vbox, TRUE, FALSE, 0);
    
    vbox = gtk_vbox_new(FALSE, 0);
    label = gtk_label_new("Hi Score:");
    gtk_widget_modify_style(label, rcstyle);
    gtk_widget_modify_fg(label, (GtkStateType)0, &c);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    sstr << scores[0].score;
    hiScoreLabel = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_style(hiScoreLabel, rcstyle);
    gtk_widget_modify_fg(hiScoreLabel, (GtkStateType)0, &c);
    gtk_box_pack_start((GtkBox *)vbox, hiScoreLabel, FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, vbox, TRUE, FALSE, 0);

    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/exit.gif");
    image = loadMainImage("exit.gif");
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(userExit), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, image);
    
    gtk_box_pack_start((GtkBox *)hbox, eventBox, FALSE, FALSE, 0);
            
    pango_font_description_free(rcstyle->font_desc);
    
    return hbox;
}

GtkWidget *SecretPhrase::createAboveBoard() {
    GtkWidget *hbox;
    GtkWidget *label;
    GtkRcStyle *rcstyle;
    GdkColor c;
    GtkWidget *vbox;
    
    c.red = 0xFFFF;
    c.green = 0xFFFF;
    c.blue = 0xFFFF;
    
    rcstyle = gtk_rc_style_new();
    rcstyle->font_desc = pango_font_description_new();
    pango_font_description_set_size(rcstyle->font_desc, 16 * PANGO_SCALE);
    pango_font_description_set_weight(rcstyle->font_desc, PANGO_WEIGHT_BOLD);
    
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_size_request(hbox, 800, 75);
    
    label = gtk_label_new("Incorrect\nGuesses Left");
    gtk_widget_modify_style(label, rcstyle);
    gtk_widget_modify_fg(label, (GtkStateType)0, &c);
    gtk_box_pack_start((GtkBox *)hbox, label, FALSE, FALSE, 5);
    
//     guessesLeftImg = gtk_image_new_from_file("/usr/local/share/touchmegames/secretphrase/images/3.gif");
    guessesLeftImg = loadImage("3.gif");
    gtk_box_pack_start((GtkBox *)hbox, guessesLeftImg, FALSE, FALSE, 10);
    
    fixed = gtk_fixed_new();
    gtk_fixed_set_has_window((GtkFixed *)fixed, TRUE);
    gtk_widget_set_size_request(fixed, 260, 75);
    
//     wheelImage1 = gtk_image_new_from_file("/usr/local/share/touchmegames/secretphrase/images/wheel.gif");
//     wheelImage2 = gtk_image_new_from_file("/usr/local/share/touchmegames/secretphrase/images/wheel.gif");
    wheelImage1 = loadImage("wheel.gif");
    wheelImage2 = loadImage("wheel.gif");
    gtk_fixed_put((GtkFixed *)fixed, wheelImage1, 0, 0);
    gtk_fixed_put((GtkFixed *)fixed, wheelImage2, 2210, 0);
    
    gtk_box_pack_start((GtkBox *)hbox, fixed, TRUE, FALSE, 0);
    
    
    vbox = gtk_vbox_new(FALSE, 0);
    label = gtk_label_new("Category:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.5, 0.5);
    gtk_widget_modify_style(label, rcstyle);
    gtk_widget_modify_fg(label, (GtkStateType)0, &c);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 2);
    
    categoryWidget = gtk_label_new(" ");
    gtk_misc_set_alignment((GtkMisc *)categoryWidget, 0.5, 0.5);
    gtk_widget_modify_style(categoryWidget, rcstyle);
    gtk_widget_modify_fg(categoryWidget, (GtkStateType)0, &c);
    gtk_box_pack_start((GtkBox *)vbox, categoryWidget, FALSE, FALSE, 2);
    gtk_box_pack_start((GtkBox *)hbox, vbox, TRUE, FALSE, 15);
    pango_font_description_free(rcstyle->font_desc);
    return hbox;
}

GtkWidget *SecretPhrase::createBoard() {
    GtkWidget *hbox;
    GtkWidget *table;
    hbox = gtk_hbox_new(FALSE, 0);
    
    table = gtk_table_new(4, 14, TRUE);
    
    for (int x = 0; x < 14; ++x) {
        for (int y = 0; y < 4; ++y) {
//             centerLetters[y][x] = gtk_image_new_from_file("/usr/local/share/touchmegames/secretphrase/images/back.gif");
            centerLetters[y][x] = loadImage("back.gif");
            gtk_table_attach_defaults((GtkTable *)table, centerLetters[y][x], x, x + 1, y, y + 1);
        }
    }
    gtk_box_pack_start((GtkBox *)hbox, table, TRUE, FALSE, 0);
    return hbox;
}

GtkWidget *SecretPhrase::createBottomLetters() {
    GtkWidget *table;
    GtkWidget *hbox;
    table = gtk_table_new(2, 14, FALSE);
    for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < 13; ++x) {
            GtkWidget *eventBox;
            GtkWidget *image;
            std::stringstream sstr;
            
            eventBox = gtk_event_box_new();
            gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
//             sstr << "/usr/local/share/touchmegames/secretphrase/images/" << (char)(x + (y * 13) + 'a') << ".gif";
//             lowerLetters[y][x] = gtk_image_new_from_file(sstr.str().c_str());
            sstr << (char)(x + (y * 13) + 'a') << ".gif";
            lowerLetters[y][x] = loadImage(sstr.str().c_str());
            gtk_container_add((GtkContainer *)eventBox, lowerLetters[y][x]);
            g_object_set_data((GObject *)eventBox, KEY_STR, (void *)(x + (y * 13) + 'a'));
            gtk_table_attach_defaults((GtkTable *)table, eventBox, x, x + 1, y, y + 1);
            
            gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
            g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                                G_CALLBACK(lowerLetterPressed), (gpointer)this);
        }
    }
    gtk_table_set_row_spacings((GtkTable *)table, 5);
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, table, TRUE, TRUE, 0);
    
    return hbox;
}

GtkWidget *SecretPhrase::createInitScreen() {
    GtkWidget *table;
    GtkWidget *eventBox;
    GtkWidget *image;
    GtkWidget *vbox, *hbox;
    GtkRcStyle *rcstyle, *largeRcStyle;
    GdkColor c, initColor;
    GtkWidget *label;
    std::stringstream sstr;
    
    c.red = 0xFFFF;
    c.green = 0xFFFF;
    c.blue = 0xFFFF;
    
    initColor.red = 0x0;
    initColor.green = 0x0;
    initColor.blue = 0x0;
    
    
    rcstyle = gtk_rc_style_new();
    rcstyle->font_desc = pango_font_description_new();
    pango_font_description_set_size(rcstyle->font_desc, 16 * PANGO_SCALE);
    pango_font_description_set_weight(rcstyle->font_desc, PANGO_WEIGHT_BOLD);
    
    largeRcStyle = gtk_rc_style_new();
    largeRcStyle->font_desc = pango_font_description_new();
    pango_font_description_set_size(largeRcStyle->font_desc, 18 * PANGO_SCALE);
    pango_font_description_set_weight(largeRcStyle->font_desc, PANGO_WEIGHT_BOLD);
    
    
    vbox = gtk_vbox_new(FALSE, 5);
    eventBox = gtk_event_box_new();
    initWidget = eventBox;
    gtk_widget_modify_bg(eventBox, (GtkStateType)0, &initColor);
    gtk_container_add((GtkContainer *)eventBox, vbox);
    gtk_widget_set_size_request(vbox, 800, 600);
    
    
    
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, hbox, FALSE, FALSE, 0);
    for (int i = 0; i < 20; ++i) {
        if (i & 1) {
//             image = gtk_image_new_from_file("/usr/local/share/touchmegames/secretphrase/images/lhcurtain.gif");
            image = loadImage("lhcurtain.gif");
        }
        else {
//             image = gtk_image_new_from_file("/usr/local/share/touchmegames/secretphrase/images/rhcurtain.gif");
            image = loadImage("rhcurtain.gif");
        }
        gtk_box_pack_start((GtkBox *)hbox, image, FALSE, FALSE, 0);
    }
    
    
    label = gtk_label_new("Pick a Category");
    gtk_widget_modify_style(label, largeRcStyle);
    gtk_widget_modify_fg(label, (GtkStateType)0, &c);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    sstr << "Round " << round + 1;
    initRoundLabel = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_style(initRoundLabel, rcstyle);
    gtk_widget_modify_fg(initRoundLabel, (GtkStateType)0, &c);
    gtk_box_pack_start((GtkBox *)vbox, initRoundLabel, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    table = gtk_table_new(2, 3, FALSE);
    gtk_table_set_row_spacings((GtkTable *)table, 50);
    // Misc
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/secretphrase/images/misc.gif");
    image = loadImage("misc.gif");
    gtk_container_add((GtkContainer *)eventBox, image);
    g_object_set_data((GObject *)eventBox, KEY_STR, (void *)(CatMisc));
    gtk_table_attach_defaults((GtkTable *)table, eventBox, 0, 1, 0, 1);
                    
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(categorySelected), (gpointer)this);

    // Sports
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/secretphrase/images/sports.gif");
    image = loadImage("sports.gif");
    gtk_container_add((GtkContainer *)eventBox, image);
    g_object_set_data((GObject *)eventBox, KEY_STR, (void *)(CatSports));
    gtk_table_attach_defaults((GtkTable *)table, eventBox, 1, 2, 0, 1);
                    
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(categorySelected), (gpointer)this);
    
    
    // Music
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/secretphrase/images/music.gif");
    image = loadImage("music.gif");
    gtk_container_add((GtkContainer *)eventBox, image);
    g_object_set_data((GObject *)eventBox, KEY_STR, (void *)(CatMusic));
    gtk_table_attach_defaults((GtkTable *)table, eventBox, 2, 3, 0, 1);
                    
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(categorySelected), (gpointer)this);
    
    // Video
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/secretphrase/images/video.gif");
    image = loadImage("video.gif");
    gtk_container_add((GtkContainer *)eventBox, image);
    g_object_set_data((GObject *)eventBox, KEY_STR, (void *)(CatVideo));
    gtk_table_attach_defaults((GtkTable *)table, eventBox, 0, 1, 1, 2);
                    
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(categorySelected), (gpointer)this);
    
    gtk_box_pack_start((GtkBox *)vbox, table, FALSE, FALSE, 0);
    
    
    pango_font_description_free(rcstyle->font_desc);
    pango_font_description_free(largeRcStyle->font_desc);
    
    return initWidget;
}

GtkWidget *SecretPhrase::createInfo() {
    GtkWidget *hbox;
    GtkRcStyle *rcstyle;
    GdkColor c;
    
    c.red = 0xFFFF;
    c.green = 0xFFFF;
    c.blue = 0xFFFF;
    
    rcstyle = gtk_rc_style_new();
    rcstyle->font_desc = pango_font_description_new();
    pango_font_description_set_size(rcstyle->font_desc, 18 * PANGO_SCALE);
    pango_font_description_set_weight(rcstyle->font_desc, PANGO_WEIGHT_BOLD);
    
    hbox = gtk_hbox_new(FALSE, 0);
    
    infoLabel = gtk_label_new("Select a letter, no vowels.");
    gtk_widget_modify_style(infoLabel, rcstyle);
    gtk_widget_modify_fg(infoLabel, (GtkStateType)0, &c);
    gtk_box_pack_start((GtkBox *)hbox, infoLabel, FALSE, FALSE, 0);
    
    progress.init();
    progress.width(101);
    progress.height(26);
//     progress.image("/usr/local/share/touchmegames/secretphrase/images/progress.gif");
    progress.defaultImage("progress1.gif");
    gtk_box_pack_start((GtkBox *)hbox, progress.widget(), FALSE, FALSE, 10);
    pango_font_description_free(rcstyle->font_desc);
    return hbox;
}

GtkWidget *SecretPhrase::createNormScreen() {
    GtkWidget *label;
    GtkWidget *image;
    GtkWidget *eventBox;
    
    normVbox = gtk_vbox_new(FALSE, 0);
    
    gtk_box_pack_start((GtkBox *)normVbox, createScoreBox(), FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)normVbox, createAboveBoard(), FALSE, FALSE, 5);
    gtk_box_pack_start((GtkBox *)normVbox, createBoard(), FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)normVbox, createInfo(), FALSE, FALSE, 5);
    gtk_box_pack_start((GtkBox *)normVbox, createBottomLetters(), FALSE, FALSE, 0);
    
    
    eventBox = gtk_event_box_new();
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
            G_CALLBACK(solvePhrase), (gpointer)this);
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/secretphrase/images/solvenow.gif");
    image = loadImage("solvenow.gif");
    gtk_container_add((GtkContainer *)eventBox, image);
    gtk_box_pack_start((GtkBox *)normVbox, eventBox, FALSE, FALSE, 5);
    
    return normVbox;
}

// initializes the game's variables and returns the
// top level h/vbox
GtkWidget *SecretPhrase::init() {
    GtkWidget *eventBox;
    GtkWidget *image;
    GtkWidget *entry;
    time_t sectime;
    
    GAME_PTR::loadScores(scores);
    // reset all values
    
    
    currentScore = 0;
    round = 0;

    // seed the random with the number of seconds since 1970
    time(&sectime);
    srand(sectime);
    
    
    topvbox = gtk_vbox_new(FALSE, 0);
    
    currentScore = 0;
    timer = g_timeout_add(50, timeout, this);
    gtk_box_pack_start((GtkBox *)topvbox, createNormScreen(), FALSE, FALSE, 0);
    
    
    gtk_box_pack_start((GtkBox *)topvbox, createInitScreen(), FALSE, FALSE, 0);
    
    gtk_widget_show_all(topvbox);
    gtk_widget_hide_all(normVbox);
    
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
    
    wheelNum = 1;
    for (int i = 0; i < 3; ++i) {
            nums[i] = -1;
    }
    
    
    wheelBarX1 = 0;
    wheelBarX2 = 2210;
    
    gtk_widget_hide_all(normVbox);
    gameState = SELECTING_CATEGORY;
    
    currentScore = 0;
    
    return topvbox;
};

