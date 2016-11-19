
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


#include <iostream>
#include <tmgglist.h>


TmgGList::TmgGList() {
    currentPos = 0;
    rowsShown = 4;
    listSize = 0;
    upButton = NULL;
    downButton = NULL;
    currentWidget = NULL;
    listvbox = NULL;
    activeBg.red = 0xcb00;
    activeBg.green = 0xff00;
    activeBg.blue = 0xfd00;
    
    activeFg.red = 0xFFFF;
    activeFg.green = 0xFFFF;
    activeFg.blue = 0xFFFF;
    
    passiveBg.red = 0xFFFF;
    passiveBg.green = 0xFFFF;
    passiveBg.blue = 0xFFFF;
    
    passiveFg.red = 0x0;
    passiveFg.green = 0x0;
    passiveFg.blue = 0x0;
}

GtkWidget *TmgGList::createScroll() {
    GtkWidget *up, *down;
    GtkWidget *eventBox;
    GtkWidget *spacer;
    vbox = gtk_vbox_new(FALSE, 0);
    std::cout << "Create Scroll " << __LINE__ << std::endl;
    std::cout.flush();
    
    if (upButton == NULL) {
        up = gtk_button_new_with_label("/\\");
        g_signal_connect(G_OBJECT(up), "button-press-event",
                         G_CALLBACK(upPressed), (gpointer)this);
        
        g_signal_connect(G_OBJECT(up), "button-release-event",
                         G_CALLBACK(upReleased), (gpointer)this);
        
        gtk_box_pack_start((GtkBox *)vbox, up, FALSE, FALSE, 0);
        upButton = up;
    }
    else {
        eventBox = gtk_event_box_new();
        std::cout << "Going to add here" << std::endl;
        std::cout.flush();
        gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
        gtk_container_add((GtkContainer *)eventBox, upButton);
        std::cout << "Going to add here" << std::endl;
        std::cout.flush();
        gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
        gtk_widget_add_events(eventBox, GDK_BUTTON_RELEASE_MASK);
        g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                        G_CALLBACK(upPressed), (gpointer)this);
        
        g_signal_connect(G_OBJECT(eventBox), "button-release-event",
                        G_CALLBACK(upReleased), (gpointer)this);
        gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 0);
        gtk_widget_show_all(eventBox);
        
    }
    
    // scroll, nothing right now
    spacer = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)vbox, spacer, TRUE, TRUE, 0);
    
    if (downButton == NULL) {
        
        down = gtk_button_new_with_label("\\/");
        g_signal_connect(G_OBJECT(down), "button-press-event",
                         G_CALLBACK(downPressed), (gpointer)this);
        
        g_signal_connect(G_OBJECT(down), "button-release-event",
                         G_CALLBACK(downReleased), (gpointer)this);
        
        gtk_box_pack_start((GtkBox *)vbox, down, FALSE, FALSE, 0);
        downButton = down;
    }
    else {
        eventBox = gtk_event_box_new();
        gtk_container_add((GtkContainer *)eventBox, downButton);
        gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
        gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
        gtk_widget_add_events(eventBox, GDK_BUTTON_RELEASE_MASK);
        g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                         G_CALLBACK(downPressed), (gpointer)this);
        
        g_signal_connect(G_OBJECT(eventBox), "button-release-event",
                         G_CALLBACK(downReleased), (gpointer)this);
        gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 0);
        gtk_widget_show_all(eventBox);
    }
    
    
    gtk_widget_show_all(vbox);
    return vbox;
}

GtkWidget *TmgGList::init() {
    GtkWidget *frame;
    
    cleanup(NULL, NULL, this);
    
    hbox = gtk_hbox_new(FALSE, 0);
    
    frame = gtk_frame_new(NULL);
    gtk_box_pack_start((GtkBox *)hbox, frame, FALSE, FALSE, 0);
    
    listvbox = gtk_vbox_new(FALSE, 0);
    if (listvboxWidth > 0) {
        std::cout << "Size Request blarg" << std::endl;
        std::cout.flush();
        gtk_widget_set_size_request(listvbox, listvboxWidth, -1);
        std::cout << "Size Request blarg" << std::endl;
        std::cout.flush();
    }
    
    gtk_container_add((GtkContainer *)frame, listvbox);
    //gtk_box_pack_start((GtkBox *)hbox, listvbox, FALSE, FALSE, 0);
    
    gtk_box_pack_start((GtkBox *)hbox, createScroll(), FALSE, FALSE, 0);
    
    
    list = g_slist_alloc();
    
    
    // now connect the destroy and size-request
    g_signal_connect(G_OBJECT(hbox), "destroy-event",
                     G_CALLBACK(cleanup), (gpointer)this);
    g_signal_connect(G_OBJECT(hbox), "delete-event",
                     G_CALLBACK(cleanup), (gpointer)this);
    
    g_signal_connect(G_OBJECT(hbox), "size-request",
                     G_CALLBACK(sizeAllocate), (gpointer)this);
    
    return hbox;
}

void TmgGList::show() {
    gtk_widget_show(hbox);
    gtk_widget_show(listvbox);
}

void TmgGList::hide() {
    gtk_widget_hide(hbox);
}

void TmgGList::reshowList() {
    
    // show the ones we can
    for (int i = 0; i < currentPos; ++i) {
        gtk_widget_hide((GtkWidget *)g_slist_nth_data(list, i + 1));
    }
    for (int i = currentPos; i < currentPos + rowsShown && i < listSize; ++i) {
        gtk_widget_show_all((GtkWidget *)g_slist_nth_data(list, i + 1));
    }
    for (int i = currentPos + rowsShown; i < listSize; ++i) {
        gtk_widget_hide((GtkWidget *)g_slist_nth_data(list, i + 1));
    }
    
}

void TmgGList::add(std::string str) {
    GtkWidget *label;
    GtkWidget *eventBox;
    
    // since it's just a string, create the label
    label = gtk_label_new(str.c_str());
    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
    gtk_widget_set_size_request(label, -1, 50); // big enough for fat fingers
    
    // create a eventBox for it
    eventBox = gtk_event_box_new();
    gtk_widget_modify_bg(eventBox, (GtkStateType)0, &passiveBg);
    gtk_widget_modify_fg(eventBox, (GtkStateType)0, &passiveFg);
    //gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    gtk_container_add((GtkContainer *)eventBox, label);
    
    // Create the signals
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(eventBox, GDK_BUTTON_RELEASE_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(listItemPressed), (gpointer)this);
        
    g_signal_connect(G_OBJECT(eventBox), "button-release-event",
                     G_CALLBACK(listItemReleased), (gpointer)this);
    
    // pack it in the vbox
    gtk_box_pack_start((GtkBox *)listvbox, eventBox, FALSE, FALSE, 0);
    
    // place it in the list
    list = g_slist_append(list, eventBox);
    
    ++listSize;
    
    // now that it's added, the list should be reshown, from the beginning
    currentPos = 0;
    reshowList();
}

void TmgGList::add(GtkWidget *widget) {
    std::cout << "TmgGList::add(GtkWidget *widget) Not yet implimented." << std::endl;
    return;
}

void TmgGList::add(std::vector<std::string> list) {
    std::cout << "TmgGList::add(std::vector<std::string> list) Not yet implimented." << std::endl;
    return;
}

void TmgGList::setSize(int width, int height) {
    if (width > 0) {
        listvboxWidth = width;
        if (listvbox != NULL) {
            gtk_widget_set_size_request(listvbox, listvboxWidth, -1);
        }
    }
    else if (height > 0) {
        std::cerr << "TmgGList::setHeight warning, setSize height not yet implimented." << std::endl;
    }
}


void TmgGList::setUpButton(std::string str) {
    if (upButton != NULL) {
        std::cerr << "TmgGList::setUpButton error: upButton != NULL" << std::endl;
        return;
    }
    upButton = gtk_image_new_from_file(str.c_str());
    std::cout << "Up Button " << upButton << std::endl;
}

void TmgGList::setDownButton(std::string str) {
    if (downButton != NULL) {
        std::cerr << "TmgGList::setDownButton error: downButton != NULL" << std::endl;
        return;
    }
    downButton = gtk_image_new_from_file(str.c_str());
}

void TmgGList::moveUp() {
    
}

void TmgGList::moveDown() {
    
}

int TmgGList::getCurrent() {
    if (currentWidget == NULL) {
        return -1;
    }
    return g_slist_index(list, currentWidget);
}

std::string TmgGList::getCurrentString() {
    if (currentWidget == NULL) {
        return "";
    }
    return std::string(gtk_label_get_text((GtkLabel *)gtk_bin_get_child((GtkBin *)currentWidget)));
}


void TmgGList::upPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
//    TmgGList *glist((TmgGList *)ptr);
    std::cout << "Up Pressed" << std::endl;
}

void TmgGList::downPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
//    TmgGList *glist((TmgGList *)ptr);
    std::cout << "Down Pressed" << std::endl;
}

void TmgGList::upReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    TmgGList *glist((TmgGList *)ptr);
    std::cout << "Up Released" << std::endl;
    if (glist->currentPos > 0) {
        --glist->currentPos;
        glist->reshowList();
    }
}

void TmgGList::downReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    TmgGList *glist((TmgGList *)ptr);
    
    if (glist->currentPos < glist->listSize - glist->rowsShown) {
        ++glist->currentPos;
        glist->reshowList();
    }
}

void TmgGList::listItemPressed(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    TmgGList *glist((TmgGList *)ptr);
    
    if (glist->currentWidget != NULL) {
        gtk_widget_modify_bg(glist->currentWidget, (GtkStateType)0, &glist->passiveBg);
        gtk_widget_modify_fg(glist->currentWidget, (GtkStateType)0, &glist->passiveFg);
    }
    glist->currentWidget = widget;
    
    gtk_widget_modify_bg(glist->currentWidget, (GtkStateType)0, &glist->activeBg);
    gtk_widget_modify_fg(glist->currentWidget, (GtkStateType)0, &glist->activeFg);
}

void TmgGList::listItemReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
//    TmgGList *glist((TmgGList *)ptr);
}

void TmgGList::showMain(GtkWidget *widget, gpointer ptr) {
//    TmgGList *glist((TmgGList *)ptr);
//    int width(0), height;
//    GSList *listCopy(glist->list);
    std::cout << "Show Main" << std::endl;
    // get the widest one

//      if (g_slist_length(listCopy) > 0) {
//          
//          do {
//              if (listCopy->data != NULL) {
//                  int tmpwidth;
//                  gtk_widget_get_size_request((GtkWidget *)listCopy->data, &tmpwidth, &height);
//                 std::cout << "width: " << tmpwidth << std::endl;
//                 if (tmpwidth > width) {
//                     width = tmpwidth;
//                 }
//             }
//             listCopy = g_slist_next(listCopy);
//         }
//         while ((listCopy != NULL));
//     }
//     
//     
//     std::cout << "Show " << width << std::endl;
//     gtk_widget_set_size_request(glist->listvbox, width, -1);
}

void TmgGList(GtkWidget *widget, gpointer ptr) {
    
}

void TmgGList::clear() {
    GSList *listCopy(list);
	
    if (g_slist_length(listCopy) > 0) {
        
        do {
            if (listCopy->data != NULL) {
                gtk_widget_destroy((GtkWidget *)listCopy->data);
            }
            listCopy = g_slist_next(listCopy);
        }
        while ((listCopy != NULL));
    }
    gtk_widget_show_all(vbox);
    
    
    g_slist_free(list);
    list = g_slist_alloc();
    
    currentPos = 0;
    rowsShown = 4;
    listSize = 0;
    currentWidget = NULL;
}

void TmgGList::sizeAllocate(GtkWidget *widget, GtkAllocation *allocation, gpointer ptr) {
    TmgGList *glist((TmgGList *)ptr);
    std::cout << "Size allocate" << std::endl;
    glist->reshowList();
}


void TmgGList::cleanup(GtkWidget *widget, GdkEvent *event, gpointer ptr) {
    TmgGList *glist((TmgGList *)ptr);
    std::cout << "Cleanup" << std::endl;
    glist->currentPos = 0;
    glist->rowsShown = 4;
    glist->listSize = 0;
    glist->upButton = NULL;
    glist->downButton = NULL;
    glist->currentWidget = NULL;
    glist->listvbox = NULL;
}
