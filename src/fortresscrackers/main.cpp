
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

// main for Videopoker

#include <fortresscrackers.h>

GtkWidget *window;

void gameEnded(GtkWidget *widget, gpointer ptr) {
	gtk_widget_destroy(window);
}

int main(int argc, char **argv) {
    FortressCrackers fortresscrackers;
    GtkWidget *widget;
    TmgWave wave;
    
    gtk_init(&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(window, 800, 600);
    
    g_signal_connect (G_OBJECT (window), "destroy",
            G_CALLBACK (gtk_main_quit), NULL);

    g_signal_connect (G_OBJECT (window), "delete_event",
            G_CALLBACK (gtk_main_quit), NULL);
    
    gtk_widget_modify_bg(window, (GtkStateType)0, fortresscrackers.backgroundColor());
    
    widget = fortresscrackers.init();
    wave.init();
    fortresscrackers.setWavePlayer(&wave);
    gtk_container_add((GtkContainer *)window, widget);
    g_signal_connect(G_OBJECT(widget), "hide",
            G_CALLBACK(gameEnded), &fortresscrackers);
    
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
