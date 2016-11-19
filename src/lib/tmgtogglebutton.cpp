/*	Copyright (C) 2005, 2006, 2007 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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

#include <tmgtogglebutton.h>
#include <config.h>
#include <iostream>


TmgToggleButton::TmgToggleButton() : toggleState(UP), toggleFunc(NULL) {
    // nothing important goes here
}

GtkWidget * TmgToggleButton::init() {
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    vbox = gtk_vbox_new(FALSE, 0);
    
    gtk_container_add((GtkContainer *)eventBox, vbox);
    
    upImage = gtk_image_new();
    gtk_box_pack_start((GtkBox *)vbox, upImage, FALSE, FALSE, 0);
    
    downImage = gtk_image_new();
    gtk_box_pack_start((GtkBox *)vbox, downImage, FALSE, FALSE, 0);
    
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(pressed), (gpointer)this);
    g_signal_connect(G_OBJECT(eventBox), "button-release-event",
                     G_CALLBACK(released), (gpointer)this);
    g_signal_connect(G_OBJECT(eventBox), "show",
                     G_CALLBACK(show), (gpointer)this);
    
    toggleState = UP;
    
    return eventBox;
}

// this grabs the images from the game's image directory
void TmgToggleButton::setImages(char *up, char *down) {
    
    gtk_image_set_from_file((GtkImage *)upImage, up);
    
    
    gtk_image_set_from_file((GtkImage *)downImage, down);
}

// this grabs the images from TMG's image directory
void TmgToggleButton::setMainImages(char *up, char *down) {
    std::string str(INSTALL_DIR);
    str += "images/";
    str += up;
    gtk_image_set_from_file((GtkImage *)upImage, str.c_str());
    
    str = INSTALL_DIR;
    str += "images/";
    str += down;
    gtk_image_set_from_file((GtkImage *)downImage, str.c_str());
}

void TmgToggleButton::setToggledEvent(void (*f)(void *, void *), void *ptr) {
    toggleFunc = f;
    toggleData = ptr;
}

// events

void TmgToggleButton::pressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    TmgToggleButton *ttb((TmgToggleButton *)ptr);
    if (ttb->toggleState == ttb->UP) {
        gtk_widget_show(ttb->downImage);
        gtk_widget_hide(ttb->upImage);
        ttb->toggleState = ttb->DOWN;
    }
    else {
        gtk_widget_show(ttb->upImage);
        gtk_widget_hide(ttb->downImage);
        ttb->toggleState = ttb->UP;
    }
    if (ttb->toggleFunc != NULL) {
        ttb->toggleFunc(ttb->toggleData, ttb);
    }
}

void TmgToggleButton::released(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
//     TmgToggleButton *ttb((TmgToggleButton *)ptr);
}

void TmgToggleButton::show(GtkWidget *widget, gpointer ptr) {
    TmgToggleButton *ttb((TmgToggleButton *)ptr);
    gtk_widget_show_all(ttb->eventBox);
    gtk_widget_hide(ttb->downImage);
}

void TmgToggleButton::changeState(TmgToggleButton::ToggleStates state) {
    toggleState = state;
    if (toggleState == DOWN) {
        gtk_widget_show(downImage);
        gtk_widget_hide(upImage);
    }
    else {
        gtk_widget_show(upImage);
        gtk_widget_hide(downImage);
    }
}

TmgToggleButton::ToggleStates TmgToggleButton::getState() {
    return toggleState;
}



