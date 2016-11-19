
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

#include <dartlib.h>
#include <gtk/gtk.h>
#include <sstream>

GtkWidget *multLabel;
GtkWidget *pointLabel;
GtkWidget *buf1Label;
GtkWidget *buf2Label;
GtkWidget *buf3Label;
DartLib dart;

gboolean timeout(gpointer ptr) {
    if (dart.gotDart()) {
        int number, mult, buff0, buff1, buff2;
        std::stringstream sstr;
        dart.readDart(number, mult, buff0, buff1, buff2);
        sstr << number;
        gtk_label_set_text((GtkLabel *)pointLabel, sstr.str().c_str());
        
        sstr.str("");
        sstr << mult;
        gtk_label_set_text((GtkLabel *)multLabel, sstr.str().c_str());
        
        sstr.str("");
        sstr << buff0;
        gtk_label_set_text((GtkLabel *)buf1Label, sstr.str().c_str());
        
        sstr.str("");
        sstr << buff1;
        gtk_label_set_text((GtkLabel *)buf2Label, sstr.str().c_str());
        
        sstr.str("");
        sstr << buff2;
        gtk_label_set_text((GtkLabel *)buf3Label, sstr.str().c_str());
    }
    return TRUE;
}


int main(int argc, char **argv) {
    GtkWidget *window;
    GtkWidget *box, *hbox;
    GtkWidget *label;
    
    gtk_init(&argc, &argv);
            
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
            
    g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (gtk_main_quit), NULL);
    
    g_signal_connect (G_OBJECT (window), "delete_event",
                    G_CALLBACK (gtk_main_quit), NULL);
            
    gtk_widget_show_all(window);
//    dart.init();
    
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_container_add((GtkContainer *)window, hbox);
    
    box = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, box, FALSE, FALSE, 5);
    
    label = gtk_label_new("Multiple");
    gtk_box_pack_start((GtkBox *)box, label, FALSE, FALSE, 10);
    
    label = gtk_label_new("Dart");
    gtk_box_pack_start((GtkBox *)box, label, FALSE, FALSE, 10);
    
    label = gtk_label_new("buff[0]");
    gtk_box_pack_start((GtkBox *)box, label, FALSE, FALSE, 10);
    
    label = gtk_label_new("buff[1]");
    gtk_box_pack_start((GtkBox *)box, label, FALSE, FALSE, 10);
    
    label = gtk_label_new("buff[2]");
    gtk_box_pack_start((GtkBox *)box, label, FALSE, FALSE, 10);
    
    
    box = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, box, FALSE, FALSE, 5);
    
    multLabel = gtk_label_new("0");
    gtk_box_pack_start((GtkBox *)box, multLabel, FALSE, FALSE, 10);
    
    pointLabel = gtk_label_new("0");
    gtk_box_pack_start((GtkBox *)box, pointLabel, FALSE, FALSE, 10);
    
    buf1Label = gtk_label_new("0");
    gtk_box_pack_start((GtkBox *)box, buf1Label, FALSE, FALSE, 10);
    
    buf2Label = gtk_label_new("0");
    gtk_box_pack_start((GtkBox *)box, buf2Label, FALSE, FALSE, 10);
    
    buf3Label = gtk_label_new("0");
    gtk_box_pack_start((GtkBox *)box, buf3Label, FALSE, FALSE, 10);
    
    
    gtk_timeout_add(200, timeout, NULL);
    
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}