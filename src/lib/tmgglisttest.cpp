

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

#include <tmgglist.h>

int main(int argc, char **argv) {
    GtkWidget *window;
    TmgGList list;
    
    gtk_init(&argc, &argv);
            
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
            
    g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (gtk_main_quit), NULL);
    
    g_signal_connect (G_OBJECT (window), "delete_event",
                    G_CALLBACK (gtk_main_quit), NULL);
            
    
    list.setUpButton("../touchmusic/images/up.png");
    list.setDownButton("../touchmusic/images/down.png");
    gtk_container_add((GtkContainer *)window, list.init());
    list.setSize(400, -1);
    list.add("The Prodigy");
    list.add("Propellerheads");
    list.add("FooFighters");
    list.add("The thing");
    list.add("The other");
    
    list.show();
    
    gtk_widget_show_all(window);
    //std::cout << "Keypad entry: " << kp.run() << std::endl;
            
    gtk_main();
    return 0;
}