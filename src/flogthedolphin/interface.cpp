

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

#include <flogthedolphin.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <sys/time.h>

GtkWidget *FLOG_THE_DOLPHIN::createScoreBox() {
    GtkWidget *label;
    GtkStyle *style;
    
    scoreTable = gtk_table_new(3, 2, FALSE);
    
    label = gtk_label_new("Top Score:");
    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
    style = gtk_widget_get_style(label);
    pango_font_description_set_size(style->font_desc, 14 * PANGO_SCALE);
    gtk_widget_set_style(label, style);
    gtk_table_attach_defaults((GtkTable *)scoreTable, label, 0, 1, 0, 1);
    
    topScoreLabel = gtk_label_new("0");
    gtk_misc_set_alignment((GtkMisc *)topScoreLabel, 1, 0.5);
    style = gtk_widget_get_style(topScoreLabel);
    pango_font_description_set_size(style->font_desc, 14 * PANGO_SCALE);
    gtk_widget_set_style(topScoreLabel, style);
    gtk_widget_set_size_request(topScoreLabel, 70, -1);
    gtk_table_attach_defaults((GtkTable *)scoreTable, topScoreLabel, 1, 2, 0, 1);
    
    label = gtk_label_new("Score:");
    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
    style = gtk_widget_get_style(label);
    pango_font_description_set_size(style->font_desc, 14 * PANGO_SCALE);
    gtk_widget_set_style(label, style);
    gtk_table_attach_defaults((GtkTable *)scoreTable, label, 0, 1, 1, 2);
    
    scoreLabel = gtk_label_new("0");
    gtk_misc_set_alignment((GtkMisc *)scoreLabel, 1, 0.5);
    style = gtk_widget_get_style(scoreLabel);
    pango_font_description_set_size(style->font_desc, 14 * PANGO_SCALE);
    gtk_widget_set_style(scoreLabel, style);
    gtk_table_attach_defaults((GtkTable *)scoreTable, scoreLabel, 1, 2, 1, 2);
    
    label = gtk_label_new("Current Flog:");
    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
    style = gtk_widget_get_style(label);
    pango_font_description_set_size(style->font_desc, 14 * PANGO_SCALE);
    gtk_widget_set_style(label, style);
    gtk_table_attach_defaults((GtkTable *)scoreTable, label, 0, 1, 2, 3);
    
    currentLabel = gtk_label_new("0");
    gtk_misc_set_alignment((GtkMisc *)currentLabel, 1, 0.5);
    style = gtk_widget_get_style(currentLabel);
    pango_font_description_set_size(style->font_desc, 14 * PANGO_SCALE);
    gtk_widget_set_style(currentLabel, style);
    gtk_table_attach_defaults((GtkTable *)scoreTable, currentLabel, 1, 2, 2, 3);
    
    gtk_widget_show_all(scoreTable);
    return scoreTable;
}

GtkWidget *FLOG_THE_DOLPHIN::createTimeBox() {
    GtkWidget *label;
    GtkWidget *vbox;
    GtkStyle *style;
    GtkWidget *button;
    vbox = gtk_vbox_new(FALSE, 5);
    
    label = gtk_label_new("Remaining Time");
    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
    style = gtk_widget_get_style(label);
    pango_font_description_set_size(style->font_desc, 14 * PANGO_SCALE);
    gtk_widget_set_style(label, style);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    
    timeProgress = gtk_progress_bar_new();
    gtk_progress_bar_set_fraction((GtkProgressBar *)timeProgress, 1.0);
    gtk_box_pack_start((GtkBox *)vbox, timeProgress, FALSE, FALSE, 0);
    
    button = gtk_button_new_with_label("Sound On");
    gtk_widget_set_size_request(button, -1, 30);
    g_signal_connect(G_OBJECT(button), "clicked",
                        G_CALLBACK(toggleSound), (gpointer)this);
    gtk_box_pack_start((GtkBox *)vbox, button, FALSE, FALSE, 0);
    
    return vbox;
}

GtkWidget *FLOG_THE_DOLPHIN::init() {
    GtkStyle *style;
    GtkWidget *topbar;
    GtkWidget *image;
    GdkColor color;
    GtkWidget *eb;
    std::stringstream sstr;
    timeval tv;
    struct timezone tz;
    GAME_PTR::loadScores(scores);
    tz.tz_minuteswest = 0;
    tz.tz_dsttime = 0;
    totalLength = 0;
    for (int i = 0; i < 8; ++i) {
        top8[i] = 0;
    }
    timeLeft = 4500;
    length = 0;
    gettimeofday(&tv, &tz);
    seedRand();
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    
    gtk_widget_add_events(eventBox, GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(eventBox, GDK_BUTTON1_MOTION_MASK);
    
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(flogTheDolphin), (gpointer)this);
    gtk_widget_show(eventBox);
    
    topvbox = gtk_vbox_new(FALSE, 0);
    
    
    fixed = gtk_fixed_new();
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    gtk_widget_modify_bg(fixed, (GtkStateType)0, &color);
    
    gtk_container_add((GtkContainer *)eventBox, topvbox);
    
    gtk_box_pack_start((GtkBox *)topvbox, fixed, TRUE, TRUE, 0);
    
//     sstr << ftdDeckLocation << "topbar.gif";
//     topbar = gtk_image_new_from_file(sstr.str().c_str());
    topbar = loadImage("topbar.gif");
    gtk_fixed_put(GTK_FIXED(fixed), topbar, 0, 0);
    
    
//     sstr.str("");
//     sstr << ftdDeckLocation << "background.gif";
//     bgImage1 = gtk_image_new_from_file(sstr.str().c_str());
    bgImage1 = loadImage("background.gif");
    gtk_fixed_put(GTK_FIXED(fixed), bgImage1, 0, 400);
    bgImage1x = 0;
    
    
//     bgImage2 = gtk_image_new_from_file(sstr.str().c_str());
    bgImage2 = loadImage("background.gif");
    gtk_fixed_put(GTK_FIXED(fixed), bgImage2, 1200, 400);
    bgImage2x = 1200;
    
    
    for (int i = 0; i < 22; ++i) {
        std::stringstream sstr;
//         sstr << ftdDeckLocation << "dolphin" << i * 5 + 40 << ".gif";
//         dolphinImages[i] = gtk_image_new_from_file(sstr.str().c_str());
        sstr << "dolphin" << i * 5 + 40 << ".gif";
        dolphinImages[i] = loadImage(sstr.str().c_str());
    }
    dolphinImage = dolphinImages[0];
    
    
    radians = 0;
//     sstr.str("");
//     sstr << ftdDeckLocation << "raft.gif";
//     raftImage1 = gtk_image_new_from_file(sstr.str().c_str());
    raftImage1 = loadImage("raft.gif");
    gtk_fixed_put(GTK_FIXED(fixed), raftImage1, 300, 567);
    raft1x = 1200 + rand() % 400;
    raft1hit = false;
    
    
//     raftImage2 = gtk_image_new_from_file(sstr.str().c_str());
    raftImage2 = loadImage("raft.gif");
    gtk_fixed_put(GTK_FIXED(fixed), raftImage2, 300, 550);
    raft2x = raft1x + 400 + rand() % 400;
    raft2hit = false;
    
//     sstr.str("");
//     sstr << ftdDeckLocation << "chair.gif";
//     chairImage = gtk_image_new_from_file(sstr.str().c_str());
    chairImage = loadImage("chair.gif");
    gtk_fixed_put(GTK_FIXED(fixed), chairImage, 300, 500);
    chairx = 1200 + rand() % 400;
    chairhit = false;

//     sstr.str("");
//     sstr << ftdDeckLocation << "buoy.gif";
//     buoyImage = gtk_image_new_from_file(sstr.str().c_str());
    buoyImage = loadImage("buoy.gif");
    gtk_fixed_put(GTK_FIXED(fixed), buoyImage, 300, 467);
    buoyx = 1200 + rand() % 400;
    //chairhit = false;
    
    newhvelocity = 0;
    rate = 15;
    hvelocity = 20;
    t = 0;
    
    gtk_fixed_put((GtkFixed *)fixed, createTimeBox(), 10, 10);
    
    eb = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eb, false);
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/exit.gif");
    image = loadMainImage("exit.gif");
    gtk_widget_add_events(eb, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eb), "button-press-event",
                        G_CALLBACK(userExit), (gpointer)this);
    gtk_container_add((GtkContainer *)eb, image);
    gtk_fixed_put((GtkFixed *)fixed, eb, 170, 10);
    gtk_widget_show_all(eb);
    
    scoreTable = createScoreBox();
    gtk_widget_show_all(scoreTable);	
    gtk_fixed_put((GtkFixed *)fixed, scoreTable, 600, 20);
    startRound();
    
    scale = 200.0;
    gtk_widget_show_all(topvbox);
    for (int i = 0; i < 22; ++i) {
        gtk_fixed_put(GTK_FIXED(fixed), dolphinImages[i], 300, 400);
        gtk_widget_hide(dolphinImages[i]);
    }
    gtk_widget_show_all(dolphinImage);
    sstr.str("");
    sstr << scores[0].score;
    gtk_label_set_text((GtkLabel *)topScoreLabel, sstr.str().c_str());
    
    timer = g_timeout_add(20, timeout, this);
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    style = gtk_widget_get_style(eventBox);
    style->bg[0] = color;
    style->bg[1] = color;
    style->bg[2] = color;
    style->bg[3] = color;
    style->bg[4] = color;
    gtk_widget_set_style(eventBox, style);
    
    return eventBox;
}
