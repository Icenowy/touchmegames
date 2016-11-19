
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

#ifndef ALT_PROGRESS_BAR_H
#define ALT_PROGRESS_BAR_H

#include <string>
#include <gtk/gtk.h>
#include <config.h>

// alternate progressbar for c++ gtk users
// allows user to place an image in the bar


class AltProgressBar {
    public:
        enum APBDirection {
            APBUp = 0,
            APBDown,
            APBLeft,
            APBRight,
        };
    private:
        bool doneInit;
        int frameHeight;
        int frameWidth;
        int imgx, imgy;
        int progressPercent;
        APBDirection progressDirection;
        
        
        GtkWidget *frameWidget;
        GtkWidget *fixedWidget;
        GtkWidget *imageWidget;
        GtkWidget *textWidget;
        GtkWidget *vbox;
        GtkWidget *hbox;
            
    public:
            
        AltProgressBar();
        void init();
        int width();
        void width(int w);
        int height();
        void height(int h);
        void image(std::string img);
        // gets the image from INSTALL_DIR + images/progressImages
        void defaultImage(std::string img);
        
        
        void percent(int p);
        int percent();
        
        void direction(APBDirection d);
        int direction();
        
        void text(std::string txt);
        std::string text();
        
        void textColor(GdkColor color);
        
        
        void hide();
        void show();
        GtkWidget *widget();
};

#endif // ALT_PROGRESS_BAR_H
