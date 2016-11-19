
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
#include <iostream>

void toggleEvent(void *button, void *ptr) {
    TmgToggleButton *ttb((TmgToggleButton *)button);
    if (ttb->getState() == ttb->UP) {
        std::cout << "State is UP" << std::endl;
    }
    else {
        std::cout << "State is DOWN" << std::endl;
    }
}

int main(int argc, char **argv) {
    GtkWidget *window;
    GtkWidget *vbox;
    TmgToggleButton button, button2;
    
    gtk_init(&argc, &argv);
            
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
            
    g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (gtk_main_quit), NULL);
    
    g_signal_connect (G_OBJECT (window), "delete_event",
                    G_CALLBACK (gtk_main_quit), NULL);
            
    
    vbox = gtk_hbox_new(FALSE, 5);
    gtk_container_add((GtkContainer *)window, vbox);
    gtk_box_pack_start((GtkBox *)vbox, button.init(), FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, button2.init(), FALSE, FALSE, 0);
    button.setMainImages("buttons/button_s15.png", "buttons/button_s15down.png");
    button2.setMainImages("buttons/button_s15.png", "buttons/button_s15down.png");
    button.setToggledEvent(toggleEvent, &button);
    
    gtk_widget_show_all(window);
    //std::cout << "Keypad entry: " << kp.run() << std::endl;
            
    gtk_main();
    return 0;
}
