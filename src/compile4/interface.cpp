
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

#include <compile4.h>
#include <sstream>
#include <iomanip>


GtkWidget *Compile4::createTopBox() {
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
    pango_font_description_set_size(rcstyle->font_desc, 18 * PANGO_SCALE);
    pango_font_description_set_weight(rcstyle->font_desc, PANGO_WEIGHT_BOLD);
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
    gtk_widget_set_size_request(scoreLabel, 140, -1);
    gtk_box_pack_start((GtkBox *)hbox, scoreLabel, TRUE, FALSE, 0);
    gtk_widget_modify_style(scoreLabel, rcstyle);
    
    sstr.str("    ");
    label = gtk_label_new(sstr.str().c_str());
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
    gtk_widget_modify_style(label, rcstyle);
    
    label = gtk_label_new("High Score:");
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
    gtk_widget_modify_style(label, rcstyle);
    
    
    sstr.str("");
    sstr << addCommas(scores[0].score);
    label = gtk_label_new(sstr.str().c_str());
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
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



GtkWidget *Compile4::createBottomBox() {
    GtkWidget *hbox;
    GtkWidget *img;
    GtkWidget *eventBox;
    GtkWidget *label;
    GtkRcStyle *rcstyleSm, *rcstyleBg;
    GtkWidget *vbox;
    GdkColor color;
    GtkWidget *table;
    
    rcstyleSm = gtk_rc_style_new();
    rcstyleSm->font_desc = pango_font_description_new();
    pango_font_description_set_size(rcstyleSm->font_desc, 12 * PANGO_SCALE);
    pango_font_description_set_weight(rcstyleSm->font_desc, PANGO_WEIGHT_BOLD);
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    rcstyleSm->fg[0] = color;
    rcstyleSm->color_flags[0] = GTK_RC_FG;
    
    rcstyleBg = gtk_rc_style_new();
    rcstyleBg->font_desc = pango_font_description_new();
    pango_font_description_set_size(rcstyleBg->font_desc, 18 * PANGO_SCALE);
    pango_font_description_set_weight(rcstyleBg->font_desc, PANGO_WEIGHT_BOLD);
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    rcstyleBg->fg[0] = color;
    rcstyleBg->color_flags[0] = GTK_RC_FG;
    
    
    hbox = gtk_hbox_new(FALSE, 0);
    
    fixed = gtk_fixed_new();
    
    gtk_widget_set_size_request(fixed, 560, 480);
    
    
    for (int i = 0; i < 21; ++i) {
            p1Circs[i] = loadImage("blue.gif");
            gtk_fixed_put((GtkFixed *)fixed, p1Circs[i], -80, -80);
            p2Circs[i] = loadImage("green.gif");
            gtk_fixed_put((GtkFixed *)fixed, p2Circs[i], -80, -80);
    }
    
    for (int i = 0; i < 7; ++i) {
        
        eventBox = gtk_event_box_new();
        gtk_widget_set_size_request(eventBox, 80, 480);
        gtk_fixed_put((GtkFixed *)fixed, eventBox, i * 80, 0);
        gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
        gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
        g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(colPressed), (gpointer)this);
        g_object_set_data((GObject *)eventBox, KEY_STR, (void *)i);
        img = loadImage("bg.png");
        gtk_container_add((GtkContainer *)eventBox, img);
            
    }
    
    scorebg = loadImage("scorebg.gif");
    
    gtk_fixed_put((GtkFixed *)fixed, scorebg, -340, -290);
    
    scoreWindow = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)scoreWindow, FALSE);
    color.red = 0xFFFF;
    color.green = 0x0;
    color.blue = 0x0;
    
    
    table = gtk_table_new(10, 3, FALSE);
    
    gtk_widget_set_size_request(scoreWindow, 300, 250);
    gtk_container_add((GtkContainer *)scoreWindow, table);
    
    gtk_fixed_put((GtkFixed *)fixed, scoreWindow, 160, 100);
    
    topScoreLabel = gtk_label_new("Win");
    gtk_widget_modify_style(topScoreLabel, rcstyleBg);
    gtk_table_attach_defaults((GtkTable *)table, topScoreLabel, 0, 3, 0, 1);
    
    label = gtk_label_new("Value");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 2, 3, 1, 2);
    
    label = gtk_label_new("Moves");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 2, 3);
    
    label = gtk_label_new(" x ");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 3, 4);
    
    label = gtk_label_new("Outcome");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 4, 5);
    
    label = gtk_label_new("Total");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 5, 6);
    
    label = gtk_label_new(" x ");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 6, 7);
    
    label = gtk_label_new("Time Per Move");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 7, 8);
    
    label = gtk_label_new("Total");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 8, 9);
    
    
    moveLabel = gtk_label_new("0");
    gtk_widget_modify_style(moveLabel, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, moveLabel, 1, 2, 2, 3);
    
    winLabel = gtk_label_new("Win");
    gtk_widget_modify_style(winLabel, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, winLabel, 1, 2, 4, 5);
    
    
    timePerMoveLabel = gtk_label_new("3");
    gtk_widget_modify_style(timePerMoveLabel, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, timePerMoveLabel, 1, 2, 7, 8);
    
    
    moveValueLabel = gtk_label_new("0");
    gtk_misc_set_alignment((GtkMisc *)moveValueLabel, 1.0, 0.5);
    gtk_widget_modify_style(moveValueLabel, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, moveValueLabel, 2, 3, 2, 3);
    
    winValueLabel = gtk_label_new("0");
    gtk_misc_set_alignment((GtkMisc *)winValueLabel, 1.0, 0.5);
    gtk_widget_modify_style(winValueLabel, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, winValueLabel, 2, 3, 4, 5);
    
    totalValue1Label = gtk_label_new("0");
    gtk_misc_set_alignment((GtkMisc *)totalValue1Label, 1.0, 0.5);
    gtk_widget_modify_style(totalValue1Label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, totalValue1Label, 2, 3, 5, 6);
    
    timePerMoveValueLabel = gtk_label_new("0");
    gtk_widget_set_size_request(timePerMoveValueLabel, 100, -1);
    gtk_misc_set_alignment((GtkMisc *)timePerMoveValueLabel, 1.0, 0.5);
    gtk_widget_modify_style(timePerMoveValueLabel, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, timePerMoveValueLabel, 2, 3, 7, 8);
    
    
    totalValue2Label = gtk_label_new("0");
    gtk_misc_set_alignment((GtkMisc *)totalValue2Label, 1.0, 0.5);
    gtk_widget_modify_style(totalValue2Label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, totalValue2Label, 2, 3, 8, 9);
    
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, 20, -1);
    gtk_box_pack_start((GtkBox *)hbox, label, FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, fixed, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, 10, -1);
    gtk_box_pack_start((GtkBox *)hbox, label, FALSE, FALSE, 0);
    
    table = gtk_table_new(14, 3, FALSE);
    
    label = gtk_label_new("Scoring");
    gtk_widget_modify_style(label, rcstyleBg);
    gtk_label_set_pattern((GtkLabel *)label, "_______");
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 3, 0, 1);
    
    label = gtk_label_new("Moves");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_label_set_pattern((GtkLabel *)label, "_____");
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 3, 1, 2);
    
    label = gtk_label_new("4");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 2, 3);
    
    label = gtk_label_new(" | ");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 3, 4);
    
    label = gtk_label_new("21");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 4, 5);
    
    label = gtk_label_new("18");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 2, 3, 2, 3);
    
    
    label = gtk_label_new(" | ");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 2, 3, 3, 4);
    
    label = gtk_label_new("1");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 2, 3, 4, 5);
    
    label = gtk_label_new(" x ");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 3, 5, 6);
    
    label = gtk_label_new("Outcome");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_label_set_pattern((GtkLabel *)label, "_______");
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 3, 6, 7);
    
    label = gtk_label_new("Win");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 7, 8);
    
    label = gtk_label_new("Tie");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 8, 9);
    
    label = gtk_label_new("Loss");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 9, 10);
    
    label = gtk_label_new("40");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 2, 3, 7, 8);
    
    label = gtk_label_new("20");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 2, 3, 8, 9);
    
    label = gtk_label_new("1");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 2, 3, 9, 10);
    
    label = gtk_label_new(" x ");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 3, 10, 11);

    label = gtk_label_new("Time Per Move(sec)");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_label_set_pattern((GtkLabel *)label, "__________________");
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 3, 11, 12);
    
    label = gtk_label_new("1");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 12, 13);
    
    label = gtk_label_new(" | ");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 13, 14);
    
    label = gtk_label_new("60");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 0, 1, 14, 15);
    
    label = gtk_label_new("60");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 2, 3, 12, 13);
    
    label = gtk_label_new(" | ");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 2, 3, 13, 14);
    
    label = gtk_label_new("1");
    gtk_widget_modify_style(label, rcstyleSm);
    gtk_table_attach_defaults((GtkTable *)table, label, 2, 3, 14, 15);
    
    
    gtk_box_pack_start((GtkBox *)hbox, table, FALSE, FALSE, 0);
    gtk_widget_show_all(hbox);
    
    pango_font_description_free(rcstyleSm->font_desc);
    pango_font_description_free(rcstyleBg->font_desc);
    return hbox;
}

// top level h/vbox
GtkWidget *Compile4::init() {
    GdkColor color;
    GtkWidget *label;
    GtkWidget *fixed;
    GtkWidget *img;
    GtkWidget *eventBox;
    time_t sectime;
    GAME_PTR::loadScores(scores);
    
    currentScore = 0;
    time(&sectime);
    srand(sectime);

    topFixed = gtk_fixed_new();
    img = loadMainImage("games/bg.png");
    gtk_fixed_put((GtkFixed *)topFixed, img, 0, 0);
    
    img = loadImage("rhbg.png");
    gtk_fixed_put((GtkFixed *)topFixed, img, 585, 54);
    
    topvbox = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_size_request(topvbox, 800, 600);
    
    gtk_box_pack_start((GtkBox *)topvbox, createTopBox(), FALSE, FALSE, 0);
    
    
    img = loadImage("topbar.gif");
    
    gtk_box_pack_start((GtkBox *)topvbox, img, FALSE, FALSE, 5);
    

    gtk_box_pack_start((GtkBox *)topvbox, createBottomBox(), FALSE, FALSE, 0);
    
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    image = loadMainImage("buttons/button_quit.png");
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(userExit), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, image);
    gtk_box_pack_start((GtkBox *)topvbox, eventBox, FALSE, FALSE, 10);
    
    gtk_fixed_put((GtkFixed *)topFixed, topvbox, 0, 0);
    
    
    
    round = 0;
    gtk_widget_show_all(topFixed);
    
    startRound();
    gtk_widget_hide_all(scoreWindow);
    gtk_widget_hide(scorebg);
    timer = g_timeout_add(50, timeout, this);
    
    return topFixed;
}
