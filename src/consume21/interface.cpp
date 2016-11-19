
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

#include <consume21.h>
#include <sstream>
#include <iomanip>


GtkWidget *Consume21::createFixed() {
    GtkWidget *eventBox;
    GtkWidget *image;
    GtkWidget *button;
    GdkColor color;
    fixed = gtk_fixed_new();
    
    for (int i = 0; i < 5; ++i) {
        int offset(20);
        
        
        color.red = 0;
        color.green = 0;
        color.blue = 0;
        
        eventBox = gtk_event_box_new();
        gtk_event_box_set_visible_window((GtkEventBox *)eventBox, TRUE);
        gtk_widget_modify_bg(eventBox, (GtkStateType)0, &color);
        g_object_set_data((GObject *)eventBox, KEY_STR, (void *)(i + 1));
        gtk_widget_set_size_request(eventBox, 100, 20);
        
        gtk_fixed_put((GtkFixed *)fixed, eventBox, offset + i * 120, 200);
        columnTotalLabels[i] = gtk_label_new("0");
        color.red = 0xFFFF;
        color.green = 0xFFFF;
        color.blue = 0xFFFF;
        gtk_widget_modify_fg(columnTotalLabels[i], (GtkStateType)0, &color);
        gtk_container_add((GtkContainer *)eventBox, columnTotalLabels[i]);
        
        eventBox = gtk_event_box_new();
        gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
        g_object_set_data((GObject *)eventBox, KEY_STR, (void *)(i + 1));
        gtk_widget_set_size_request(eventBox, 100, 240);
        
        g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                         G_CALLBACK(colPressed), (gpointer)this);
        
        gtk_fixed_put((GtkFixed *)fixed, eventBox, offset + i * 120, 220);
        
    }
    
    color.red = 0;
    color.green = 0;
    color.blue = 0;
        
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, TRUE);
    gtk_widget_modify_bg(eventBox, (GtkStateType)0, &color);
    gtk_widget_set_size_request(eventBox, 150, 40);
        
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 640, 190);
    columnsTotal = gtk_label_new("0");
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    gtk_widget_modify_fg(columnsTotal, (GtkStateType)0, &color);
    gtk_container_add((GtkContainer *)eventBox, columnsTotal);
            
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/consume21/images/toprightbox.png");
    image = loadImage("toprightbox.png");
    gtk_fixed_put((GtkFixed *)fixed, image, 640, 20);
    gtk_fixed_put((GtkFixed *)fixed, createRightTop(), 640, 20);
    
    
    // now time for the spill "button"
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 150, 130);
    
    
//     spill = gtk_image_new_from_file("/usr/local/share/touchmegames/consume21/images/spillup.png");
    spill = loadImage("spillup.png");
    gtk_container_add((GtkContainer *)eventBox, spill);
    
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(spillPressed), (gpointer)this);
    
    g_signal_connect(G_OBJECT(eventBox), "button-release-event",
                     G_CALLBACK(spillReleased), (gpointer)this);
    
    
    // now time for the collect "button"
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 640, 500);
    
    
//     collect = gtk_image_new_from_file("/usr/local/share/touchmegames/consume21/images/collectup.png");
    collect = loadImage("collectup.png");
    gtk_container_add((GtkContainer *)eventBox, collect);
    
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(collectPressed), (gpointer)this);
    
    g_signal_connect(G_OBJECT(eventBox), "button-release-event",
                     G_CALLBACK(collectReleased), (gpointer)this);
    
    
    // progress bar
    progress.init();
    progress.width(320);
    progress.height(35);
//     progress.image("/usr/local/share/touchmegames/consume21/images/progress.gif");
    progress.defaultImage("progress1.gif");
    gtk_fixed_put((GtkFixed *)fixed, progress.widget(), 270, 110);
    
    
    gtk_fixed_put((GtkFixed *)fixed, createRightBottom(), 646, 285);
    
    
    // exit button
    eventBox = gtk_event_box_new();
//     button = gtk_image_new_from_file("/usr/local/share/touchmegames/images/exit.gif");
    button = loadMainImage("exit.gif");
    gtk_container_add((GtkContainer *)eventBox, button);
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), 
                     "button-press-event",
                     G_CALLBACK(userExit), (gpointer)this);
    
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 10, 530);
    
    
    // round image
//     roundImage = gtk_image_new_from_file("/usr/local/share/touchmegames/consume21/images/round1.png");
    roundImage = loadImage("round1.png");
    gtk_fixed_put((GtkFixed *)fixed, roundImage, 520, 20);
    
    
    return fixed;
}

GtkWidget *Consume21::createRightTop() {
    GtkWidget *eventBox, *topEventBox;
    GtkWidget *vbox, *hbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    std::string str;
    
    
    color.red = 0x1D00;
    color.green = 0x6A00;
    color.blue = 0xB900;
    
    vbox = gtk_vbox_new(FALSE, 0);
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_size_request(hbox, 150, 140);
    gtk_box_pack_start((GtkBox *)hbox, vbox, TRUE, TRUE, 10);
    
    
    label = gtk_label_new("Score");
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 3);
    
    
    color.red = 0;
    color.green = 0;
    color.blue = 0;
    
    
    eventBox = gtk_event_box_new();
    gtk_widget_modify_bg(eventBox, (GtkStateType)0, &color);
    gtk_widget_set_size_request(eventBox, 120, 20);
    
    scoreLabel = gtk_label_new("0");
    gtk_container_add((GtkContainer *)eventBox, scoreLabel);
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    gtk_widget_modify_fg(scoreLabel, (GtkStateType)0, &color);
    
    gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 5);
    
    
    // High score section
    label = gtk_label_new("High Score");
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 5);
    
    
    color.red = 0xD000;
    color.green = 0;
    color.blue = 0;
    
    
    eventBox = gtk_event_box_new();
    gtk_widget_modify_bg(eventBox, (GtkStateType)0, &color);
    gtk_widget_set_size_request(eventBox, 120, 40);
    
    sstr << scores[0].score;
    str = sstr.str();
    addCommas(str);
    hiScoreLabel = gtk_label_new(str.c_str());
    gtk_container_add((GtkContainer *)eventBox, hiScoreLabel);
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    gtk_widget_modify_fg(hiScoreLabel, (GtkStateType)0, &color);
    
    gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 5);
    
    
    return hbox;
}

GtkWidget *Consume21::createRightBottom() {
    GtkWidget *label;
    GtkWidget *image;
    GtkWidget *vbox;
    GdkColor bgcolor, leftcolor, rightcolor;
    int values[9] = { 1000, 500, 400, 300, 250, 200, 150, 100, 50 };
    
    bgcolor.red = 0xE300;
    bgcolor.green = 0x2E00;
    bgcolor.blue = 0xF000;
    
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/consume21/images/lowrightbg.png");
    image = loadImage("lowrightbg.png");
    gtk_fixed_put((GtkFixed *)fixed, image, 640, 250);
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    for (int i = 0; i < 9; ++i) {
        std::stringstream sstr;
        
        scoreBoxen[i] = gtk_event_box_new();
        gtk_widget_modify_bg(scoreBoxen[i], (GtkStateType)0, &bgcolor);
        gtk_widget_set_size_request(scoreBoxen[i], 140, 21);
        gtk_event_box_set_visible_window((GtkEventBox *)scoreBoxen[i], FALSE);
        
        sstr << 105 - i << "   -   " << values[i];
        label = gtk_label_new(sstr.str().c_str());
        gtk_container_add((GtkContainer *)scoreBoxen[i], label);
        gtk_box_pack_start((GtkBox *)vbox, scoreBoxen[i], FALSE, FALSE, 0);
    }
    return vbox;
}

void Consume21::createMug() {
    //GtkWidget *mugImages[35];
    //int totalMugImages;
    GtkWidget *eventBox;
    GtkWidget *image;
    int mug;
    std::stringstream sstr;
    
    mug = getRand(14);
    if (mug >= 10) {
        mug = 10;
    }
    else {
        mug = mug % 10 + 1;
    }
    
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    if (mug == 1) {
        g_object_set_data((GObject *)eventBox, KEY_STR, (void *)(11 << 4));
    }
    else {
        g_object_set_data((GObject *)eventBox, KEY_STR, (void *)(mug << 4));
    }
        
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(colPressed), (gpointer)this);
        
    gtk_fixed_put((GtkFixed *)fixed, eventBox, 25, 25);
    
//     sstr << "/usr/local/share/touchmegames/consume21/images/" << mug << ".png";
    sstr << mug << ".png";
//     image = gtk_image_new_from_file(sstr.str().c_str());
    image = loadImage(sstr.str().c_str());
    gtk_container_add((GtkContainer *)eventBox, image);
    
    mugImages[totalMugImages++] = eventBox;
    gtk_widget_show_all(eventBox);
}

void Consume21::setTotalWin(int i) {
    i = 8 - (i - 97);
    
    for (int x = 0; x < 9; ++x) {
        gtk_event_box_set_visible_window((GtkEventBox *)scoreBoxen[x], FALSE);
    }
    if (i < 9) {
        gtk_event_box_set_visible_window((GtkEventBox *)scoreBoxen[i], TRUE);
    }
}

// top level h/vbox
GtkWidget *Consume21::init() {
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
    round = 0;
    
    tophbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)tophbox, createFixed(), FALSE, FALSE, 0);
    
    

    gtk_widget_show_all(tophbox);
    
    startRound();
    timer = g_timeout_add(100, timeout, this);
    
    return tophbox;
}
