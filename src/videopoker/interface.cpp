
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

#include <videopoker.h>
#include <sstream>
#include <iomanip>


GtkWidget *VideoPoker::createScoreBox() {
    GtkWidget *hbox;
    GtkWidget *table;
    GtkWidget *label;
    GdkColor color;
    GtkWidget *hbox2;
    std::string str;
    std::stringstream sstr;
    PangoFontDescription *font;
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    font = pango_font_description_new();
//     pango_font_description_set_size(font, 11 * PANGO_SCALE);
    
    hbox = gtk_hbox_new(TRUE, 0);
    hbox2 = gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start((GtkBox *)hbox, hbox2, TRUE, FALSE, 0);
    
    
    table = gtk_table_new(1, 6, TRUE);

    
    label = gtk_label_new("Score:");
    
    gtk_widget_modify_font(label, font);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 0, 1);
    
    
    sstr << std::setw(10) << std::setfill(' ') << "$ 500";
    scoreLabel = gtk_label_new(sstr.str().c_str());
    gtk_misc_set_alignment(GTK_MISC(scoreLabel), 0.0, 0.5);
    gtk_widget_modify_font(scoreLabel, font);
    gtk_widget_modify_fg(scoreLabel, GTK_STATE_NORMAL, &color);
    gtk_table_attach_defaults(GTK_TABLE(table), scoreLabel, 1, 2, 0, 1);
    
    label = gtk_label_new("  Hi Score:");
    gtk_widget_modify_font(label, font);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_table_attach_defaults(GTK_TABLE(table), label, 2, 3, 0, 1);
    
    sstr.str("");
    sstr << "$ " << addCommas(scores[0].score);
    str = sstr.str();
    sstr.str("");
    sstr << std::setw(10) << std::setfill(' ') << str;
    
    hiScoreLabel = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_font(hiScoreLabel, font);
    gtk_widget_modify_fg(hiScoreLabel, GTK_STATE_NORMAL, &color);
    gtk_table_attach_defaults(GTK_TABLE(table), hiScoreLabel, 3, 4, 0, 1);
    
    
    label = gtk_label_new("  Hand:");
    gtk_widget_modify_font(label, font);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_table_attach_defaults(GTK_TABLE(table), label, 4, 5, 0, 1);
    
    sstr.str("");
    sstr << hand << " of 20";
    str = sstr.str();
    sstr.str("");
    sstr << std::setw(10) << std::setfill(' ') << str;
    
    handLabel = gtk_label_new(sstr.str().c_str());
    gtk_misc_set_alignment(GTK_MISC(handLabel), 0.0, 0.5);
    gtk_widget_modify_font(handLabel, font);
    gtk_widget_modify_fg(handLabel, GTK_STATE_NORMAL, &color);
    gtk_table_attach_defaults(GTK_TABLE(table), handLabel, 5, 6, 0, 1);
    
    pango_font_description_free(font);
    gtk_box_pack_start((GtkBox *)hbox2, table, TRUE, TRUE, 0);
    return hbox;
}

GtkWidget *VideoPoker::createHandBox() {
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *hbox;
    GtkWidget *image;
    GtkWidget *hbox2;
    
    hbox = gtk_hbox_new(TRUE, 0);
    hbox2 = gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start((GtkBox *)hbox, hbox2, FALSE, FALSE, 0);
    
    
    for (int i = 0; i < 5; ++i) {
            
            
            cardBoxes[i] = gtk_event_box_new();
            gtk_event_box_set_visible_window((GtkEventBox *)cardBoxes[i], FALSE);
            gtk_widget_add_events(cardBoxes[i], GDK_BUTTON_RELEASE_MASK);
            gtk_widget_add_events(cardBoxes[i], GDK_BUTTON_PRESS_MASK);
            gtk_widget_add_events(cardBoxes[i], GDK_BUTTON1_MOTION_MASK);
            g_signal_connect(G_OBJECT(cardBoxes[i]), "button-press-event",
                            G_CALLBACK(cardSelected), (gpointer)this);

            cardImages[i] = gtk_image_new_from_file(deckBack.c_str());
            gtk_widget_set_size_request(cardImages[i], 105, 135);
                
            gtk_container_add((GtkContainer *)cardBoxes[i], cardImages[i]);
                
            gtk_box_pack_start((GtkBox *)hbox2, cardBoxes[i], FALSE, FALSE, 5);
    }
        
    return hbox;
}

GtkWidget *VideoPoker::createInstructionBox() {
    GtkStyle *style;
    GtkWidget *hbox;
    GdkColor color;
    PangoFontDescription *font;
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    hbox = gtk_hbox_new(TRUE, 0);
    
    font = pango_font_description_new();
    pango_font_description_set_size(font, 18 * PANGO_SCALE);
    
    instructionLabel = gtk_label_new("Select a bet and press the deal button.");
//     gtk_widget_set_style(instructionLabel, style);
    gtk_widget_modify_font(instructionLabel, font);
    gtk_widget_modify_fg(instructionLabel, GTK_STATE_NORMAL, &color);
    
    
    
    gtk_box_pack_start((GtkBox *)hbox, instructionLabel, TRUE, TRUE, 0);
    pango_font_description_free(font);
    return hbox;
}

GtkWidget *VideoPoker::createBetBox() {
    GtkWidget *label;
    GtkWidget *image;
    GtkWidget *button;
    GtkWidget *hbox, *hbox2;
    GtkWidget *vbox;
    GtkStyle *style;
    GtkWidget *eventBox;
    vbox = gtk_vbox_new(FALSE, 0);
    
    hbox = gtk_hbox_new(TRUE, 0);
    hbox2 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, hbox2, FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, hbox, FALSE, FALSE, 0);
    
    
    gtk_box_pack_start((GtkBox *)hbox2, betButtons[0].init(), FALSE, FALSE, 1);
    betButtons[0].setMainImages("buttons/button_s5.png", "buttons/button_s5down.png");
    betButtons[0].setToggledEvent(betPlaced, this);
    
    gtk_box_pack_start((GtkBox *)hbox2, betButtons[1].init(), FALSE, FALSE, 1);
    betButtons[1].setMainImages("buttons/button_s10.png", "buttons/button_s10down.png");
    betButtons[1].setToggledEvent(betPlaced, this);
    
    gtk_box_pack_start((GtkBox *)hbox2, betButtons[2].init(), FALSE, FALSE, 1);
    betButtons[2].setMainImages("buttons/button_s15.png", "buttons/button_s15down.png");
    betButtons[2].setToggledEvent(betPlaced, this);
    
    gtk_box_pack_start((GtkBox *)hbox2, betButtons[3].init(), FALSE, FALSE, 1);
    betButtons[3].setMainImages("buttons/button_s20.png", "buttons/button_s20down.png");
    betButtons[3].setToggledEvent(betPlaced, this);
    
    gtk_box_pack_start((GtkBox *)hbox2, betButtons[4].init(), FALSE, FALSE, 1);
    betButtons[4].setMainImages("buttons/button_s25.png", "buttons/button_s25down.png");
    betButtons[4].setToggledEvent(betPlaced, this);
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    image = loadMainImage("buttons/button_deal.png");
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    gtk_container_add((GtkContainer *)eventBox, image);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(dealPressed), (gpointer)this);
    
    gtk_box_pack_start((GtkBox *)vbox, eventBox, TRUE, TRUE, 5);
    
//     label = gtk_label_new("Deal");
//     style = gtk_widget_get_style(label);
//     pango_font_description_set_size(style->font_desc, 18 * PANGO_SCALE);
//     gtk_widget_set_style(label, style);
//     
//     button = gtk_button_new();
//     gtk_container_add((GtkContainer *)button, label);
//     gtk_widget_set_size_request(button, 100, 60);
//     gtk_box_pack_start((GtkBox *)hbox, button, TRUE, TRUE, 10);
//     g_signal_connect(G_OBJECT(button), "released",
//             G_CALLBACK(dealPressed), (gpointer)this);
//     
    hbox = gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start((GtkBox *)hbox, vbox, TRUE, TRUE, 0);
    return hbox;
}

GtkWidget *VideoPoker::createPayoutBox() {
    GtkWidget *label;
    GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *image;
    GtkStyle *style;
    GtkWidget *frame;
    GtkWidget *table;
    
    PangoFontDescription *font;
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
//    font = pango_font_description_new();
//    pango_font_description_set_size(font, 11 * PANGO_SCALE);
    
    hbox = gtk_hbox_new(TRUE, 0);
    image = loadImage("payout.png");
//     vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, image, TRUE, TRUE, 0);
//     gtk_box_pack_start((GtkBox *)hbox, vbox, FALSE, FALSE, 0);

    gtk_widget_show_all(hbox);
    return hbox;
    //vbox = gtk_vbox_new(FALSE, 0);
    
    
    //gtk_box_pack_start((GtkBox *)hbox, vbox, FALSE, FALSE, 0);
    
    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type((GtkFrame *)frame, GTK_SHADOW_OUT);
    gtk_box_pack_start((GtkBox *)hbox, frame, TRUE, FALSE, 0);
    
    table = gtk_table_new(6, 3, false);
    gtk_table_set_row_spacings((GtkTable *)table, 3);
    gtk_table_set_col_spacings((GtkTable *)table, 10);
    gtk_container_add((GtkContainer *)frame, table);
    
    label = gtk_label_new("Royal Flush");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 0, 1);
    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
    
    
    label = gtk_label_new("250 to 1");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 1, 2, 0, 1);
    gtk_misc_set_alignment((GtkMisc *)label, 1, 0.5);
    
    
    
    label = gtk_label_new("Four Of A Kind");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 1, 2);
    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
    
    label = gtk_label_new("50 to 1");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 1, 2, 1, 2);
    gtk_misc_set_alignment((GtkMisc *)label, 1, 0.5);
    
    
    
    label = gtk_label_new("Straight Flush");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 2, 3);
    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
    
    label = gtk_label_new("25 to 1");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 1, 2, 2, 3);
    gtk_misc_set_alignment((GtkMisc *)label, 1, 0.5);
    
    
    label = gtk_label_new("Full House");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 2, 3, 0, 1);
    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
    
    label = gtk_label_new("9 to 1");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 3, 4, 0, 1);
    gtk_misc_set_alignment((GtkMisc *)label, 1, 0.5);
    
    
    
    
    label = gtk_label_new("Flush");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 2, 3, 1, 2);
    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
    
    label = gtk_label_new("6 to 1");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 3, 4, 1, 2);
    gtk_misc_set_alignment((GtkMisc *)label, 1, 0.5);
    
    
    
    label = gtk_label_new("Straight");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 2, 3, 2, 3);
    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
    
    label = gtk_label_new("4 to 1");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 3, 4, 2, 3);
    gtk_misc_set_alignment((GtkMisc *)label, 1, 0.5);
    
    
    
    
    label = gtk_label_new("Three of a Kind");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 4, 5, 0, 1);
    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
    
    label = gtk_label_new("3 to 1");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 5, 6, 0, 1);
    gtk_misc_set_alignment((GtkMisc *)label, 1, 0.5);
    
    
    
    label = gtk_label_new("Two Pair");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 4, 5, 1, 2);
    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
    
    label = gtk_label_new("2 to 1");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 5, 6, 1, 2);
    gtk_misc_set_alignment((GtkMisc *)label, 1, 0.5);
    
    
    
    label = gtk_label_new("Pair");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 4, 5, 2, 3);
    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
    
    label = gtk_label_new("1 to 1");
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_font(label, font);
    gtk_table_attach_defaults((GtkTable *)table, label, 5, 6, 2, 3);
    gtk_misc_set_alignment((GtkMisc *)label, 1, 0.5);
    
    pango_font_description_free(font);
    
    gtk_widget_show_all(hbox);
    return hbox;
}

// initializes the game's variables and returns the
// top level h/vbox
GtkWidget *VideoPoker::init() {
    GtkWidget *eventBox;
    GtkWidget *image;
    GtkWidget *label;
    
    currentBet = 5;
    hand = 1;
    
    topfixed = gtk_fixed_new();

//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/games/bg.png");
    image = loadImage("fullbg.png");

    gtk_fixed_put((GtkFixed *)topfixed, image, 0, 0);

    topvbox = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_size_request(topvbox, 800, 600);
    gtk_fixed_put((GtkFixed *)topfixed, topvbox, 0, 0);
    
    GAME_PTR::loadScores(scores);
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)topvbox, label, FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)topvbox, createScoreBox(), FALSE, FALSE, 4);
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)topvbox, label, FALSE, FALSE, 5);
    gtk_box_pack_start((GtkBox *)topvbox, createHandBox(), FALSE, FALSE, 4);
    gtk_box_pack_start((GtkBox *)topvbox, createInstructionBox(), FALSE, FALSE, 0);
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)topvbox, label, FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)topvbox, createBetBox(), FALSE, FALSE, 7);
    gtk_box_pack_start((GtkBox *)topvbox, createPayoutBox(), FALSE, FALSE, 5);
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)topvbox, label, FALSE, FALSE, 0);
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/exit.gif");
    image = loadMainImage("buttons/button_quit.png");
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(userExit), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, image);
    gtk_box_pack_start((GtkBox *)topvbox, eventBox, FALSE, FALSE, 0);
    
    for (int i = 0; i < 52; ++i) {
            cardDeck[i] = i;
    }
    startHand();
    
    currentScore = 500;
    timer = g_timeout_add(100, timeout, this);
//     gtk_toggle_button_set_active((GtkToggleButton *)betButtons[currentBet / 5 - 1], true);
    gtk_widget_show_all(topfixed);
    
    seedRand();
    
    return topfixed;
};

