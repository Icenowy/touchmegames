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

#ifndef TMG_TOGGLE_BUTTON_H
#define TMG_TOGGLE_BUTTON_H

#include <string>
#include <gtk/gtk.h>

class TmgToggleButton {
    public:
        enum ToggleStates {
            UP,
            DOWN,
        };
    private:
        GtkWidget *eventBox;
        GtkWidget *upImage;
        GtkWidget *downImage;
        GtkWidget *vbox;
        ToggleStates toggleState;
        
        void *toggleData;
        void (*toggleFunc)(void *, void *);
        
        static void pressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
        static void released(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
        static void show(GtkWidget *widget, gpointer ptr);
    public:
        TmgToggleButton();
        GtkWidget *init();
        // this grabs the images from the game's image directory
        void setImages(char *up, char *down);
        // this grabs the images from TMG's image directory
        void setMainImages(char *up, char *down);
        void setToggledEvent(void (*f)(void *, void *), void *ptr);
        
        void changeState(ToggleStates state);
        ToggleStates getState();
};


#endif //TMG_TOGGLE_BUTTON_H
