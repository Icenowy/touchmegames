
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

// main for flogthedolphin

#include <flogthedolphin.h>

GtkWidget *window;

void gameEnded(GtkWidget *widget, gpointer ptr) {
    gtk_widget_destroy(window);
}

int main(int argc, char **argv) {
    FLOG_THE_DOLPHIN g;
    GtkWidget *widget;
    GtkStyle *style;
    GdkColor color;
    
    gtk_init(&argc, &argv);
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
            
    gtk_widget_modify_bg(window, (GtkStateType)0, g.backgroundColor());
    
    g_signal_connect (G_OBJECT (window), "destroy",
            G_CALLBACK (gtk_main_quit), NULL);

    g_signal_connect (G_OBJECT (window), "delete_event",
            G_CALLBACK (gtk_main_quit), NULL);
    
    gtk_widget_set_usize(GTK_WIDGET(window), 800, 600);
    
    
    
    widget = g.init();
    gtk_container_add((GtkContainer *)window, widget);
    g_signal_connect(G_OBJECT(widget), "hide",
            G_CALLBACK(gameEnded), &g);
    
    gtk_widget_show(window);
    gtk_main();
    return 0;
}
