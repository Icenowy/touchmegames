
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

#include <fotoseek.h>
#include <sstream>
#include <iomanip>
#include <cmath>


// Fill this area with layout functions
void FotoSeek::createPlayArea() {
    GtkWidget *leftEventBox, *rightEventBox, *eventBox;
    
    leftEventBox = gtk_event_box_new();
    g_signal_connect(G_OBJECT(leftEventBox), "button-press-event",
                     G_CALLBACK(playAreaClicked), (gpointer)this);
    
    leftImage.loadImage("fotoseek/images/blank.png");
    leftImage.scale(imageWidth, imageHeight, GDK_INTERP_NEAREST);
    gtk_container_add((GtkContainer *)leftEventBox, leftImage.widget());
    
    gtk_fixed_put((GtkFixed *)topFixed, leftEventBox, playXOffset, playYOffset);
    
    rightEventBox = gtk_event_box_new();
    g_signal_connect(G_OBJECT(rightEventBox), "button-press-event",
                     G_CALLBACK(playAreaClicked), (gpointer)this);
    
    rightImage.loadImage("fotoseek/images/blank.png");
    rightImage.scale(imageWidth, imageHeight, GDK_INTERP_NEAREST);
    gtk_container_add((GtkContainer *)rightEventBox, rightImage.widget());
    
    gtk_fixed_put((GtkFixed *)topFixed, rightEventBox, playXOffset + imageWidth + 10, playYOffset);
    
    imageMask.loadImage("fotoseek/images/blank.png");
    imageMask.scale(imageWidth, imageHeight, GDK_INTERP_NEAREST);
    
    for (int i = 0; i < 3; ++i) {
        eventBox = gtk_event_box_new();
        gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
        hintImages[i] = loadImage("hint.png");
        g_object_set_data((GObject *)hintImages[i], KEY_STR, (void *)0);
        gtk_container_add((GtkContainer *)eventBox, hintImages[i]);
        gtk_fixed_put((GtkFixed *)topFixed, eventBox, 20, playYOffset + 70 * (i + 1));
        g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                         G_CALLBACK(hintClicked), (gpointer)this);
    }
    
}

void FotoSeek::createTop() {
    GtkWidget *vbox, *hbox, *label, *lhvbox, *rhvbox;
    GtkWidget *eventBox;
    GtkWidget *image;
    GdkColor color;
    std::stringstream sstr;
    std::string str;
            
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_size_request(vbox, 150, -1);
    
    color.red = 0xF600;
    color.green = 0xFF00;
    color.blue = 0x0000;
    
    image = loadImage("fotoseek.png");
    gtk_fixed_put((GtkFixed *)topFixed, image, 250, 20);
    
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
    
    gtk_fixed_put((GtkFixed *)topFixed, vbox, 25, 40);
    
    
    // Center Image
    
    // right side
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_size_request(vbox, 150, -1);
    
//     eventBox = gtk_event_box_new();
//     gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
//     image = loadMainImage("buttons/button_quit.png");
//     gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
//     g_signal_connect(G_OBJECT(eventBox), "button-press-event",
//                      G_CALLBACK(userExit), (gpointer)this);
//     gtk_container_add((GtkContainer *)eventBox, image);
//     gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 5);
//     gtk_widget_show_all(eventBox);
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    image = loadMainImage("buttons/button_quit.png");
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(userExit), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, image);
    gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 5);
    
    
    label = gtk_label_new("Points:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 1);
    
    points.init();
    points.width(151);
    points.height(25);
    points.defaultImage("progress1.gif");
    points.percent(100);
    points.text("1000");
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    points.textColor(color);
    
    gtk_box_pack_start((GtkBox *)vbox, points.widget(), FALSE, FALSE, 5);
    gtk_fixed_put((GtkFixed *)topFixed, vbox, gameWidth - 175, 10);
    
    
    // right side
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_widget_set_size_request(vbox, 150, -1);
    
    
    progress.init();
    progress.width(600);
    progress.height(25);
    progress.defaultImage("progress600.gif");
    progress.percent(100);
//     progress.text("1:30");
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    progress.textColor(color);
    
    gtk_box_pack_start((GtkBox *)vbox, progress.widget(), FALSE, FALSE, 10);
    gtk_fixed_put((GtkFixed *)topFixed, vbox, gameWidth / 8, 110);
}


void FotoSeek::paintCircle(FSDifference *diff, bool found) {
    unsigned long color;
    if (found) {
//         std::cout << "User found the diff" << std::endl;
        color = foundColor;
    }
    else {
//         std::cout << "User did not find the diff" << std::endl;
        color = missedColor;
    }
    // y = yc + sqrt((double)((r * r) - ((x - xc) * (x - xc))));
    for (int r = diff->radius; r < diff->radius + 1; ++r) {
        for (int x = diff->centerx - r; x <= diff->centerx + r; ++x) {
            int y = diff->centery + (int)sqrt((double)((r * r) - (x - diff->centerx) * (x - diff->centerx)));
            if (x > 0 && x < imageWidth && y > 0 && y < imageHeight) {
                
                leftImage.setColor(x, y, 0x0);
                rightImage.setColor(x, y, 0x0);
            }
            y = diff->centery - (int)sqrt((double)((r * r) - (x - diff->centerx) * (x - diff->centerx)));
            if (x > 0 && x < imageWidth && y > 0 && y < imageHeight) {
                
                leftImage.setColor(x, y, 0x0);
                rightImage.setColor(x, y, 0x0);
            }
        }
    }
    for (int r = diff->radius + 1; r < diff->radius + 4; ++r) {
        for (int x = diff->centerx - r; x <= diff->centerx + r; ++x) {
            int y = diff->centery + (int)sqrt((double)((r * r) - (x - diff->centerx) * (x - diff->centerx)));
            if (x > 0 && x < imageWidth && y > 0 && y < imageHeight) {
                
                leftImage.setColor(x, y, color);
                rightImage.setColor(x, y, color);
            }
            y = diff->centery - (int)sqrt((double)((r * r) - (x - diff->centerx) * (x - diff->centerx)));
            if (x > 0 && x < imageWidth && y > 0 && y < imageHeight) {
                
                leftImage.setColor(x, y, color);
                rightImage.setColor(x, y, color);
            }
        }
    }
    for (int r = diff->radius + 4; r < diff->radius + 5; ++r) {
        for (int x = diff->centerx - r; x <= diff->centerx + r; ++x) {
            int y = diff->centery + (int)sqrt((double)((r * r) - (x - diff->centerx) * (x - diff->centerx)));
            if (x > 0 && x < imageWidth && y > 0 && y < imageHeight) {
                
                leftImage.setColor(x, y, 0x0);
                rightImage.setColor(x, y, 0x0);
            }
            y = diff->centery - (int)sqrt((double)((r * r) - (x - diff->centerx) * (x - diff->centerx)));
            if (x > 0 && x < imageWidth && y > 0 && y < imageHeight) {
                
                leftImage.setColor(x, y, 0x0);
                rightImage.setColor(x, y, 0x0);
            }
        }
    }
    // fill in based on y
    for (int r = diff->radius; r < diff->radius + 1; ++r) {
        for (int y = diff->centery - r; y <= diff->centery + r; ++y) {
            int x = diff->centerx + (int)sqrt((double)((r * r) - (y - diff->centery) * (y - diff->centery)));
            if (x > 0 && x < imageWidth && y > 0 && y < imageHeight) {
                
                leftImage.setColor(x, y, 0x0);
                rightImage.setColor(x, y, 0x0);
            }
            x = diff->centerx - (int)sqrt((double)((r * r) - (y - diff->centery) * (y - diff->centery)));
            if (x > 0 && x < imageWidth && y > 0 && y < imageHeight) {
                
                leftImage.setColor(x, y, 0x0);
                rightImage.setColor(x, y, 0x0);
            }
        }
    }
    for (int r = diff->radius + 1; r < diff->radius + 4; ++r) {
        for (int y = diff->centery - r; y <= diff->centery + r; ++y) {
            int x = diff->centerx + (int)sqrt((double)((r * r) - (y - diff->centery) * (y - diff->centery)));
            if (x > 0 && x < imageWidth && y > 0 && y < imageHeight) {
                
                leftImage.setColor(x, y, color);
                rightImage.setColor(x, y, color);
            }
            x = diff->centerx - (int)sqrt((double)((r * r) - (y - diff->centery) * (y - diff->centery)));
            if (x > 0 && x < imageWidth && y > 0 && y < imageHeight) {
                
                leftImage.setColor(x, y, color);
                rightImage.setColor(x, y, color);
            }
        }
    }
    for (int r = diff->radius + 4; r < diff->radius + 5; ++r) {
        for (int y = diff->centery - r; y <= diff->centery + r; ++y) {
            int x = diff->centerx + (int)sqrt((double)((r * r) - (y - diff->centery) * (y - diff->centery)));
            if (x > 0 && x < imageWidth && y > 0 && y < imageHeight) {
                
                leftImage.setColor(x, y, 0x0);
                rightImage.setColor(x, y, 0x0);
            }
            x = diff->centerx - (int)sqrt((double)((r * r) - (y - diff->centery) * (y - diff->centery)));
            if (x > 0 && x < imageWidth && y > 0 && y < imageHeight) {
                
                leftImage.setColor(x, y, 0x0);
                rightImage.setColor(x, y, 0x0);
            }
        }
    }
    leftImage.repaint();
    rightImage.repaint();
}

void FotoSeek::paintUnfound() {
    for (int i = 0; i < 5; ++i) {
        if (diffCoords[i].found == false) {
            std::cout << "Painting unfound " << i << std::endl;
            paintCircle(&diffCoords[i], false);
        }
    }
}

void FotoSeek::updateScore(int points) {
    currentScore += points;
    gtk_label_set_text((GtkLabel *)scoreLabel, addCommas(currentScore).c_str());
}

// Create the layout and start the round
GtkWidget *FotoSeek::init() {
    GtkWidget *image;
    // Grab the current scores
    GAME_PTR::loadScores(scores);
    
    currentScore = 0;
    topFixed = gtk_fixed_new();
    gtk_widget_set_size_request(topFixed, gameWidth, gameHeight);
    
    image = loadImage("bg.png");
    gtk_fixed_put((GtkFixed *)topFixed, image, 0, 0);
    initVars();
    
    round = 0;
    createTop();
    createPlayArea();
    startRound();
    
    // Change this timeout if needed.
    timer = g_timeout_add(50, timeout, this);
    
    gtk_widget_show_all(topFixed);
    return topFixed;
}
