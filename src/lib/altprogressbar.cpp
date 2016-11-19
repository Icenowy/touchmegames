
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

#include <altprogressbar.h>
#include <iostream>

AltProgressBar::AltProgressBar() : doneInit(false), frameHeight(1), frameWidth(1), 
    imgx(0), imgy(0), progressPercent(100), progressDirection(APBLeft) {
	
}


void AltProgressBar::init() {
    GdkColor color;
    GtkWidget *textHbox, *textVbox;
    
    color.red = 0x0;
    color.green = 0x0;
    color.blue = 0x0;
    
    doneInit = true;
    frameWidget = gtk_frame_new(NULL);
    fixedWidget = gtk_fixed_new();
    gtk_widget_set_has_window((GtkWidget *)fixedWidget, TRUE);
    gtk_widget_modify_bg(fixedWidget, GTK_STATE_NORMAL, &color);

    imageWidget = gtk_image_new();
    
    hbox = gtk_hbox_new(FALSE, 0);
    vbox = gtk_vbox_new(FALSE, 0);
    
    gtk_container_add((GtkContainer *)frameWidget, fixedWidget);
    gtk_fixed_put((GtkFixed *)fixedWidget, imageWidget, 0, 0);
    
    gtk_box_pack_start((GtkBox *)hbox, frameWidget, FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, hbox, FALSE, FALSE, 0);
    
    
    gtk_frame_set_shadow_type((GtkFrame *)frameWidget, GTK_SHADOW_IN);
    gtk_widget_set_size_request(frameWidget, frameWidth, frameHeight);
    gtk_widget_set_size_request(fixedWidget, frameWidth - 1, frameHeight - 1);
    gtk_widget_set_size_request(imageWidget, frameWidth - 2, frameHeight - 2);
    
    
    textHbox = gtk_hbox_new(TRUE, 0);
    textVbox = gtk_vbox_new(TRUE, 0);
    gtk_fixed_put((GtkFixed *)fixedWidget, textHbox, 0, 0);
    gtk_box_pack_start((GtkBox *)textHbox, textVbox, TRUE, TRUE, 0);
    textWidget = gtk_label_new("");
    gtk_box_pack_start((GtkBox *)textVbox, textWidget, TRUE, TRUE, 0);

}

int AltProgressBar::width() {
	return frameWidth;
}

void AltProgressBar::width(int w) {
    frameWidth = w;
    if (doneInit) {
        gtk_widget_set_size_request(frameWidget, frameWidth, frameHeight);
        gtk_widget_set_size_request(textWidget, frameWidth, frameHeight);
    }
}

int AltProgressBar::height() {
	return frameHeight;
}

void AltProgressBar::height(int h) {
    frameHeight = h;
    if (doneInit) {
        gtk_widget_set_size_request(frameWidget, frameWidth, frameHeight);
	gtk_widget_set_size_request(textWidget, frameWidth, frameHeight);
    }
}

void AltProgressBar::image(std::string img) {
    if (img.size() && doneInit) {
        gtk_image_set_from_file((GtkImage *)imageWidget, img.c_str());
        gtk_widget_set_size_request(imageWidget, frameWidth - 2, frameHeight - 2);
    }
}

// gets the image from INSTALL_DIR + images/progressImages
void AltProgressBar::defaultImage(std::string img) {
    if (img.size() && doneInit) {
        std::string str(INSTALL_DIR);
        str += "images/progressImages/";
        str += img;
        gtk_image_set_from_file((GtkImage *)imageWidget, str.c_str());
        gtk_widget_set_size_request(imageWidget, frameWidth - 2, frameHeight - 2);
    }
}

GtkWidget *AltProgressBar::widget() {
    if (doneInit) {
        return vbox;
    }
    std::cerr << "AltProgressBar has not been initialized." << std::endl;
    return NULL;
}

void AltProgressBar::percent(int p) {
    if (p < 0 || p > 100) {
        std::cerr << "AltProgressBar percent must be between 0 and 100" << std::endl;
    }
    //gtk_widget_get_size_request(vbox, &x, &y);
    
    switch (progressDirection) {
        case APBLeft:
            
            imgx = 0 - (frameWidth - (p * frameWidth) / 100);
            gtk_fixed_move((GtkFixed *)fixedWidget, imageWidget, imgx, imgy);
            break;
        default:
                std::cerr << "AltProgressBar currently only supports APBLeft" << std::endl;
    }
    progressPercent = p;
}

int AltProgressBar::percent() {
    return progressPercent;
}

void AltProgressBar::direction(APBDirection d) {
    
}

int AltProgressBar::direction() {
    return progressDirection;
}

void AltProgressBar::text(std::string txt) {
    gtk_label_set_text((GtkLabel *)textWidget, txt.c_str());
}

std::string AltProgressBar::text() {
    return gtk_label_get_text((GtkLabel *)textWidget);
}

void AltProgressBar::textColor(GdkColor color) {
    if (!doneInit) {
        return;
    }
    gtk_widget_modify_fg(textWidget, (GtkStateType)0, &color);
}

void AltProgressBar::hide() {
    gtk_widget_hide(hbox);
}

void AltProgressBar::show() {
    gtk_widget_show(hbox);
}
