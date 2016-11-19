

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

#include <gorillathrilla.h>
#include <algorithm>
#include <sstream>
#include <iomanip>



GtkWidget *GORILLA_THRILLA::createTopBox() {
    GtkWidget *hbox;
    GtkWidget *table;
    GtkWidget *label;
    GdkColor color;

    std::stringstream sstr;
    PangoFontDescription *font;
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    font = pango_font_description_new();
    pango_font_description_set_size(font, 11 * PANGO_SCALE);
    
    hbox = gtk_hbox_new(TRUE, 0);
    
    table = gtk_table_new(1, 7, TRUE);
    
    label = gtk_label_new(" ");
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 0, 1);
    
    label = gtk_label_new("Score:");
    
    gtk_widget_modify_font(label, font);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 0, 1);
    
    sstr << std::setw(12) << std::setfill(' ') << 0;
    scoreLabel = gtk_label_new(sstr.str().c_str());
    gtk_misc_set_alignment(GTK_MISC(scoreLabel), 0.0, 0.5);
    gtk_widget_modify_font(scoreLabel, font);
    gtk_widget_modify_fg(scoreLabel, GTK_STATE_NORMAL, &color);
    gtk_table_attach_defaults(GTK_TABLE(table), scoreLabel, 2, 3, 0, 1);
    
    label = gtk_label_new(" Hi Score:");
    gtk_widget_modify_font(label, font);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_table_attach_defaults(GTK_TABLE(table), label, 3, 4, 0, 1);
    
    sstr.str("");
    sstr << std::setw(12) << std::setfill(' ') << addCommas(scores[0].score);
    
    hiScoreLabel = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_font(hiScoreLabel, font);
    gtk_widget_modify_fg(hiScoreLabel, GTK_STATE_NORMAL, &color);
    gtk_table_attach_defaults(GTK_TABLE(table), hiScoreLabel, 4, 5, 0, 1);
    
    
    label = gtk_label_new(" Round:");
    gtk_widget_modify_font(label, font);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_table_attach_defaults(GTK_TABLE(table), label, 5, 6, 0, 1);
    
    sstr.str("");
    sstr << std::setw(12) << std::setfill(' ') << round;
    
    roundLabel = gtk_label_new(sstr.str().c_str());
    gtk_misc_set_alignment(GTK_MISC(roundLabel), 0.0, 0.5);
    gtk_widget_modify_font(roundLabel, font);
    gtk_widget_modify_fg(roundLabel, GTK_STATE_NORMAL, &color);
    gtk_table_attach_defaults(GTK_TABLE(table), roundLabel, 6, 7, 0, 1);
    
    pango_font_description_free(font);
    gtk_box_pack_start((GtkBox *)hbox, table, TRUE, TRUE, 0);
    return hbox;
}

GtkWidget *GORILLA_THRILLA::createSecondTopBox() {
    GtkWidget *hbox;
    GtkWidget *image;
    GtkWidget *eventBox;
    GtkWidget *label;
    hbox = gtk_hbox_new(false, 0);
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)hbox, label, TRUE, FALSE, 0);
    
    // change this from placing the buttons in a box to fixed.
    
    // left center
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    
    image = loadMainImage("buttons/button_quit.png");
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(userExit), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, image);
    //gtk_box_pack_start((GtkBox *)hbox, eventBox, TRUE, FALSE, 0);
    gtk_fixed_put((GtkFixed *)topFixed, eventBox, 142, 50);
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    
    
    gtk_widget_add_events(eventBox, GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(eventBox, GDK_BUTTON1_MOTION_MASK);
    
    
    image = loadMainImage("buttons/button_take_score.png");
    gtk_container_add((GtkContainer *)eventBox, image);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(takeScore), (gpointer)this);
    //gtk_box_pack_start((GtkBox *)hbox, eventBox, TRUE, FALSE, 0);
    gtk_fixed_put((GtkFixed *)topFixed, eventBox, 542, 50);
    
    
    return hbox;
}

GtkWidget *GORILLA_THRILLA::createCenterBox() {
    GtkWidget *hbox;
    GtkWidget *label;
    GtkWidget *vbox;
    GdkColor color;
    PangoFontDescription *font;
    font = pango_font_description_new();
    pango_font_description_set_size(font, 16 * PANGO_SCALE);
    pango_font_description_set_weight(font, PANGO_WEIGHT_BOLD);
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    hbox = gtk_hbox_new(FALSE, 0);
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(GTK_WIDGET(label), 100, 90);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    
    label = gtk_label_new("1-2 = 5x");
    gtk_widget_modify_font(label, font);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_set_size_request(GTK_WIDGET(label), 100, 50);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("9-9 = 3x");
    gtk_widget_modify_font(label, font);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_set_size_request(GTK_WIDGET(label), 100, 50);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    gtk_box_pack_start((GtkBox *)hbox, vbox, FALSE, FALSE, 0);
    
    
    // center center
    centerFixed = gtk_fixed_new();
    
    gtk_widget_set_size_request(GTK_WIDGET(centerFixed), 590, 320);
    
    
    
    gtk_box_pack_start((GtkBox *)hbox, centerFixed, FALSE, FALSE, 0);
    
    timeLabel = gtk_label_new("600");
    gtk_widget_modify_font(timeLabel, font);
    gtk_widget_modify_fg(timeLabel, GTK_STATE_NORMAL, &color);
    gtk_fixed_put(GTK_FIXED(topFixed), timeLabel, 700, 80);
    
    
    
    dispSpot = loadImage("up.png");
    gtk_fixed_put(GTK_FIXED(centerFixed), dispSpot, 243, 400);
    
    
    diskImage = loadImage("banana.png");
    
    gtk_fixed_put((GtkFixed *)topFixed, diskImage, 700, 80);
    
    timex = 700;
    timey = 63;
    
    pango_font_description_free(font);
    
    return hbox;
}

GtkWidget *GORILLA_THRILLA::createBottomBox() {
    GtkWidget *hbox;
    GtkWidget *image;
    GtkWidget *label;
    GtkWidget *eventBox;
    std::stringstream sstr;
    GtkWidget *vbox;
    GdkColor color;
    PangoFontDescription *font;
    font = pango_font_description_new();
    pango_font_description_set_size(font, 16 * PANGO_SCALE);
    pango_font_description_set_weight(font, PANGO_WEIGHT_BOLD);
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    
    hbox = gtk_hbox_new(FALSE, 0);
    
    label = gtk_label_new(" ");
    
    gtk_box_pack_start((GtkBox *)hbox, label, FALSE, FALSE, 0);
    
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    remainingLabel = gtk_label_new("24");
    gtk_widget_modify_font(remainingLabel, font);
    gtk_widget_modify_fg(remainingLabel, GTK_STATE_NORMAL, &color);
    gtk_box_pack_start((GtkBox *)vbox, remainingLabel, FALSE, FALSE, 0);
    //gtk_widget_set_name(remainingLabel, "normal");
    
    
    label = gtk_label_new("Cards Remaining");
    gtk_widget_modify_font(label, font);
    gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &color);
    gtk_widget_set_size_request(GTK_WIDGET(label), 210, 50);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, vbox, FALSE, FALSE, 0);
    gtk_widget_show(label);
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    
    gtk_widget_add_events(eventBox, GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(eventBox, GDK_BUTTON1_MOTION_MASK);
    
    
    label = gtk_label_new("     ");
    gtk_box_pack_start((GtkBox *)hbox, label, FALSE, FALSE, 0);
    
    image = loadImage("barrel.gif");
    gtk_container_add((GtkContainer *)eventBox, image);
    
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(dealPressed), (gpointer)this);
    
    gtk_box_pack_start((GtkBox *)hbox, eventBox, FALSE, FALSE, 0);
    gtk_widget_show_all(image);
    //fixed = gtk_fixed_new();
    //gtk_widget_set_size_request(GTK_WIDGET(fixed), 200, 80);
    
    //gtk_widget_ref(fixed);
    
    // Where the deal card goes
    dealCardBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)dealCardBox, false);
    
    
    gtk_box_pack_start((GtkBox *)hbox, dealCardBox, FALSE, FALSE, 10);
    
    gtk_widget_add_events(dealCardBox, GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events(dealCardBox, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(dealCardBox, GDK_BUTTON1_MOTION_MASK);
    
    g_signal_connect(G_OBJECT(dealCardBox), "button-press-event",
                        G_CALLBACK(dealCardPressed), (gpointer)this); 
    
    
    dealSpot = loadImage("left.png");

    gtk_box_pack_start((GtkBox *)hbox, dealSpot, FALSE, FALSE, 10);
    
    pango_font_description_free(font);
    
    return hbox;
}

int GORILLA_THRILLA::getRow(int i) {
    if (i < 6) {
        return 0;
    }
    if (i < 11) {
        return 1;
    }
    if (i < 17) {
        return 2;
    }
    if (i < 22) {
        return 3;
    }
    if (i < 28) {
        return 4;
    }
    return 0;
}

int GORILLA_THRILLA::getCard(int i) {
    if (i < 6) {
        return i;
    }
    if (i < 11) {
        return i - 6;
    }
    if (i < 17) {
        return i - 11;
    }
    if (i < 22) {
        return i - 17;
    }
    if (i < 28) {
        return i - 22;
    }
    return 0;
}

int GORILLA_THRILLA::checkWinner() {
    for (int i = 0; i < 28; ++i) {
        if (dispCards[i] != NULL) {
            return 0;
        }
    }
    return 1;
}

int GORILLA_THRILLA::isGameOver() {
    for (int i = 0; i < 28; ++i) {
        if (dispCards[i] != NULL) {
            return 0;
        }
    }
    if (cardsLeft != 0) {
        return 0;
    }
    return 1;
}

void GORILLA_THRILLA::insertIntoDisplay(int cardNumber, int displayLocation) {
    std::stringstream sstr;
    GtkWidget *image;
    int row, card;
    int xpos, ypos;
    
    if (dispCards[displayLocation] != NULL) {
        return;
    }
    row = getRow(displayLocation);
    card = getCard(displayLocation);
    
    sstr << cardNumber << ".gif";
    
    image = loadImage(sstr.str().c_str());
    dispCards[displayLocation] = image;
    (*cardMap)[dispCards[displayLocation]] = cardNumber;
    
    ypos = row * 50;
    xpos = (row % 2 ? 72: 24) + card * 96;
    
    // create the event box
    dispCardBoxes[displayLocation] = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)dispCardBoxes[displayLocation], FALSE);
    gtk_widget_add_events(dispCardBoxes[displayLocation], GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events(dispCardBoxes[displayLocation], GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(dispCardBoxes[displayLocation], GDK_BUTTON1_MOTION_MASK);
    
    g_signal_connect(G_OBJECT(dispCardBoxes[displayLocation]), "button-press-event",
                        G_CALLBACK(middleCardPressed), (gpointer)this); 
    
    gtk_container_add((GtkContainer *)dispCardBoxes[displayLocation], image);
    
    gtk_fixed_put(GTK_FIXED(centerFixed), dispCardBoxes[displayLocation], xpos, ypos);
    gtk_widget_show_all(dispCardBoxes[displayLocation]);
}

void GORILLA_THRILLA::startGame() {
    int xpos(100), ypos(100);
    int i, j;
    std::stringstream sstr;
    time_t sectime;
    int cards[52];
    GdkPixmap pixmap;
            
    
    cardsLeft = 24;
    sstr.str("");
    sstr << std::setw(2) << std::setfill('0') << cardsLeft;
    gtk_label_set_text((GtkLabel *)remainingLabel, sstr.str().c_str());
    
    // startup row 1
    time(&sectime);
    srand(sectime);
            
    gameOver = false;
    
    
    waitingForFirstClick = 1;
    
    for (i = 0; i < 52; ++i) {
            cards[i] = (i % 13) + 1;
    }
    for (j = 0; j < 10; ++j) {
        for (i = 0; i < 52; ++i) {
            int randnum(rand() % 52);
            std::swap(cards[i], cards[randnum]);
        }
    }
    
    cardMap->clear();
    
    
    // output the display cards
    for (i = 0; i < 28; ++i) {
        int row(getRow(i));
        int card(getCard(i));
        GtkWidget *image;
        
        sstr.str("");
        sstr << cards[i] << ".gif";
        image = loadImage(sstr.str().c_str());
        
        dispCards[i] = image;
        (*cardMap)[dispCards[i]] = cards[i];
        
        ypos = row * 50;
        xpos = (row % 2 ? 72: 24) + card * 96;
        
        // create the event box
        dispCardBoxes[i] = gtk_event_box_new();
        gtk_event_box_set_visible_window((GtkEventBox *)dispCardBoxes[i], FALSE);
        gtk_widget_add_events(dispCardBoxes[i], GDK_BUTTON_RELEASE_MASK);
        gtk_widget_add_events(dispCardBoxes[i], GDK_BUTTON_PRESS_MASK);
        gtk_widget_add_events(dispCardBoxes[i], GDK_BUTTON1_MOTION_MASK);
        g_signal_connect(G_OBJECT(dispCardBoxes[i]), "button-press-event",
                            G_CALLBACK(middleCardPressed), (gpointer)this); 
        gtk_container_add((GtkContainer *)dispCardBoxes[i], dispCards[i]);
        
        gtk_fixed_put(GTK_FIXED(centerFixed), dispCardBoxes[i], xpos, ypos);
        gtk_widget_show_all(dispCardBoxes[i]);
        
        
    }
    for (i = 0; i < 24; ++i) {
        remainingDeck[i] = cards[51 - i];
            
    }
    
    sstr.str("");
    sstr << remainingDeck[--cardsLeft] << ".gif";
    dealCard = loadImage(sstr.str().c_str());
    
    g_object_get((GObject *)dealCard, "mask", &pixmap, NULL);
    
    sstr.str("");
    sstr << std::setw(2) << std::setfill('0') << cardsLeft;
    gtk_label_set_text((GtkLabel *)remainingLabel, sstr.str().c_str());
    
    (*cardMap)[dealCard] = remainingDeck[cardsLeft];
    
    
    gtk_container_add((GtkContainer *)dealCardBox, dealCard);
    gtk_widget_show_all(dealCardBox);
    g_signal_connect(G_OBJECT(dealCardBox), "button-press-event",
                        G_CALLBACK(dealCardPressed), (gpointer)this); 
    
    
    sstr.str("");
    sstr << std::setw(12) << std::setfill(' ') << round;
    
    gtk_label_set_text((GtkLabel *)roundLabel, sstr.str().c_str());
    
    selectedCard = NULL;
    gameOver = false;
    
    gtk_widget_hide(dispSpot);
    
    gtk_widget_hide(dealSpot);
    timer = g_timeout_add(200, timeout, this);
    
    timex = 700;
    timey = 63;
    gtk_label_set_text((GtkLabel *)timeLabel, "600");
    gtk_fixed_move((GtkFixed *)topFixed, timeLabel, timex + 25, timey - 15);
    
}

void GORILLA_THRILLA::createMessageDialog() {


}

GtkWidget *GORILLA_THRILLA::init() {
    GtkStyle *style;
    GtkWidget *bgImage;
    GdkColor color;
    GtkWidget *label;
    GAME_PTR::loadScores(scores);
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    round = 1;
    totalScore = 0;
    gameOver = false;
    takingScore = false;
    
    topFixed = gtk_fixed_new();
    bgImage = loadImage("fullbg.png");
    gtk_fixed_put((GtkFixed *)topFixed, bgImage, 0, 0);
    
    topvbox = gtk_vbox_new(FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)topvbox, label, 
                        FALSE, FALSE, 0);
    
    gtk_box_pack_start((GtkBox *)topvbox, createTopBox(), 
                        FALSE, FALSE, 0);
    
    gtk_box_pack_start((GtkBox *)topvbox, createSecondTopBox(), 
                        FALSE, FALSE, 10);
    
    gtk_box_pack_start((GtkBox *)topvbox, createCenterBox(), 
                        FALSE, FALSE, 40);
                        
    gtk_box_pack_start((GtkBox *)topvbox, createBottomBox(), 
                        FALSE, FALSE, 0);
    
    
    
    style = gtk_widget_get_style(topvbox);
    style->bg[0] = color;
    style->bg[1] = color;
    style->bg[2] = color;
    style->bg[3] = color;
    style->bg[4] = color;
    gtk_widget_set_style(topvbox, style);
    
    createMessageDialog();
    
    gtk_fixed_put((GtkFixed *)topFixed, topvbox, 0, 0);
    gtk_widget_show_all(topFixed);
    
    startGame();
    
    return topFixed;
}
