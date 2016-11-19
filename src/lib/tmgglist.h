
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
// simple gui list

#ifndef TMG_GLIST_H
#define TMG_GLIST_H

#include <string>
#include <vector>
#include <gtk/gtk.h>

class TmgGList {
    private:
        GSList *list;
        GtkWidget *hbox, *vbox;
        GtkWidget *scrollFixed;
        GtkWidget *scrollEventBox;
        GtkWidget *listvbox; // "list" vbox
        GtkWidget *upButton;
        GtkWidget *downButton;
        int currentPos;
        int rowsShown;
        int listSize;
        int listvboxWidth, listvboxHeight;
        GtkWidget *currentWidget;
        GdkColor activeBg, passiveBg;
        GdkColor activeFg, passiveFg;
        
        GtkWidget *createScroll();
        
        void reshowList();
        
        
        static void upPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
        static void downPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
        static void upReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
        static void downReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
        
        static void listItemPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
        static void listItemReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr);
        
        static void showMain(GtkWidget *widget, gpointer ptr);
        static void sizeAllocate(GtkWidget *widget, GtkAllocation *allocation, gpointer ptr);
        
        static void cleanup(GtkWidget *widget, GdkEvent *event, gpointer ptr);
    public:
        TmgGList();
        GtkWidget *init();
        void show();
        void hide();
        void add(std::string str);
        void add(GtkWidget *widget);
        void add(std::vector<std::string> list);
//        void setRowsShown(int rs);
//        int getRowsShown();
        
        void setSize(int width, int height);
        
        void setUpButton(std::string str);
        void setDownButton(std::string str);
        
        void moveUp();
        void moveDown();
        int getCurrent();
        void setCurrent(int n);
        std::string getCurrentString();
        void clear();
};


#endif // TMG_GLIST_H
