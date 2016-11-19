
/*	Copyright (C) 2005 - 2008 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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

#include <keypad.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <iostream>
// A B C D E F G H I
// J K L M N O P Q R
// S T U V W X Y Z
// SPACE <- ENT

KEYPAD::KEYPAD(std::string str) : dialog(NULL) {

}

void KEYPAD::changeEntry(int keyval) {
    std::string str;
    
    if (keyval == GDK_space) {
        str = gtk_entry_get_text((GtkEntry *)entry);
        if (str.size()) {
            str.erase(str.size() - 1, 1);
        }
        if (str.size() < 14) {
            str += ' ';
        }
    }
    else if (keyval == GDK_BackSpace) {
        str = gtk_entry_get_text((GtkEntry *)entry);
        if (str.size()) {
            str.erase(str.size() - 1, 1);
        }
        if (str.size()) {
            str.erase(str.size() - 1, 1);
        }
    }
    else if (keyval == GDK_Return) {
        gtk_dialog_response((GtkDialog *)dialog, GTK_RESPONSE_OK);
        return;
    }
    else if ((keyval <= GDK_Z && keyval >= GDK_A) || (keyval <= GDK_z && keyval >= GDK_a)) {
        char ch(keyval);
        if (ch >= 'a' && ch <= 'z') {
            ch = ch - 'a' + 'A';
        }
        str = gtk_entry_get_text((GtkEntry *)entry);
        if (str.size()) {
            str.erase(str.size() - 1, 1);
        }
        if (str.size() < 14) {
            str += ch;
        }
    }
    else {
        return;
    }
    str += '_';
    gtk_entry_set_text((GtkEntry *)entry, str.c_str());
    gtk_editable_set_position((GtkEditable *)entry, 0);
    gtk_widget_grab_focus(entry);
}

void KEYPAD::buttonPressed(GtkWidget *widget, gpointer ptr) {
    KEYPAD *kp(reinterpret_cast<KEYPAD *>(ptr));
    std::string str(gtk_label_get_text(((GtkLabel *)((GtkBin *)widget)->child)));
    
    if (str == "SPACE") {
        kp->changeEntry(GDK_space);
    }
    else if (str == "BKSP") {
        kp->changeEntry(GDK_BackSpace);
    }
    else if (str == "ENTER") {
        kp->changeEntry(GDK_Return);
    }
    else {
        kp->changeEntry(str[0]);
    }
    gtk_editable_set_position((GtkEditable *)kp->entry, 0);
}

gboolean KEYPAD::keyPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    KEYPAD *kp(reinterpret_cast<KEYPAD *>(ptr));
    
    return TRUE;
}

gboolean KEYPAD::keyReleased(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    KEYPAD *kp(reinterpret_cast<KEYPAD *>(ptr));
    kp->changeEntry(event->keyval);
    gtk_editable_set_position((GtkEditable *)widget, 0);
    return TRUE;
}

void KEYPAD::init() {
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *hbox[3];
    int cntr(0);
    
    dialog = gtk_dialog_new();
    
    hbox[0] = gtk_hbox_new(FALSE, 0);
    hbox[1] = gtk_hbox_new(FALSE, 0);
    hbox[2] = gtk_hbox_new(FALSE, 0);
    
    entry = gtk_entry_new();
    gtk_entry_set_max_length((GtkEntry *)entry, 15);
    g_signal_connect(G_OBJECT(entry), "key-press-event",
                        G_CALLBACK(keyPressed), this);
    g_signal_connect(G_OBJECT(entry), "key-release-event",
                     G_CALLBACK(keyReleased), this);
    
    
    gtk_entry_set_text((GtkEntry *)entry, "_");
    gtk_editable_set_editable((GtkEditable *)entry, TRUE);
    gtk_box_pack_start((GtkBox *)((GtkDialog *)dialog)->vbox, entry, FALSE, FALSE, 0);
    
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        GtkWidget *nbutton;
        char str[2] = { ch, 0 };
        
        label = gtk_label_new(str);
        gtk_widget_set_name(label, "keypad_label");
        
        nbutton = gtk_button_new();
        gtk_container_add((GtkContainer *)nbutton, label);
        gtk_widget_set_size_request(nbutton, 40, 40);
        
        gtk_box_pack_start((GtkBox *)hbox[cntr], nbutton, TRUE, FALSE, 5);
        
        g_signal_connect(G_OBJECT(nbutton), "clicked",
                            G_CALLBACK(buttonPressed), this);
        
        if (ch == 'I' || ch == 'R') {
            ++cntr;
        }
    }
    gtk_box_pack_start((GtkBox *)((GtkDialog *)dialog)->vbox, hbox[0], TRUE, FALSE, 5);
    gtk_box_pack_start((GtkBox *)((GtkDialog *)dialog)->vbox, hbox[1], TRUE, FALSE, 5);
    gtk_box_pack_start((GtkBox *)((GtkDialog *)dialog)->vbox, hbox[2], TRUE, FALSE, 5);
    
    hbox[0] = gtk_hbox_new(FALSE, 0);
    
    label = gtk_label_new("SPACE");
    gtk_widget_set_name(label, "keypad_label");
    
    button = gtk_button_new();
    gtk_container_add((GtkContainer *)button, label);
    gtk_widget_set_size_request(button, 80, 40);
            
    gtk_box_pack_start((GtkBox *)hbox[0], button, TRUE, FALSE, 5);
    g_signal_connect(G_OBJECT(button), "clicked",
                        G_CALLBACK(buttonPressed), this);
    
    label = gtk_label_new("BKSP");
    gtk_widget_set_name(label, "keypad_label");
    
    button = gtk_button_new();
    gtk_container_add((GtkContainer *)button, label);
    gtk_widget_set_size_request(button, 80, 40);
            
    gtk_box_pack_start((GtkBox *)hbox[0], button, TRUE, FALSE, 5);
    g_signal_connect(G_OBJECT(button), "clicked",
                        G_CALLBACK(buttonPressed), this);
    
    
    label = gtk_label_new("ENTER");
    gtk_widget_set_name(label, "keypad_label");
    
    button = gtk_button_new();
    gtk_container_add((GtkContainer *)button, label);
    gtk_widget_set_size_request(button, 80, 40);
            
    gtk_box_pack_start((GtkBox *)hbox[0], button, TRUE, FALSE, 5);
    
    gtk_box_pack_start((GtkBox *)((GtkDialog *)dialog)->vbox, hbox[0], FALSE, FALSE, 0);
    g_signal_connect(G_OBJECT(button), "clicked",
                        G_CALLBACK(buttonPressed), this);
}

std::string KEYPAD::run() {
    std::string retstr;
    if (dialog == NULL) {
        init();
    }
    gtk_widget_show_all(dialog);
    gtk_dialog_run((GtkDialog *)dialog);
    retstr = gtk_entry_get_text((GtkEntry *)entry);
    
    if (retstr.size()) {
        retstr.erase(retstr.size() - 1, 1);
    }
    gtk_widget_destroy(dialog);
    return retstr;
}

