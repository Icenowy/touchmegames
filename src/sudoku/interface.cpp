
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

#include <sudoku.h>
#include <sstream>
#include <iomanip>


GtkWidget *Sudoku::createFixed() {
    GtkWidget *hbox;
    //GtkWidget *button;
    GtkWidget *eventBox;
    GtkWidget *image;
    fixed = gtk_fixed_new();
    gtk_widget_set_size_request(fixed, 800, 600);
    
    image = loadImage("bg_sudoku.png");
    gtk_fixed_put((GtkFixed *)fixed, image, 0, 0);
    
    image = loadImage("board.gif");
    gtk_fixed_put((GtkFixed *)fixed, image, 75, 50);
    gtk_fixed_put((GtkFixed *)fixed, createGrid(), 75, 50);
    
    
    hbox = gtk_hbox_new(FALSE, 0);
    
    eventBox = gtk_event_box_new();
    image = loadImage("button.png");
    gtk_container_add((GtkContainer *)eventBox, image);
    g_object_set_data((GObject *)eventBox, KEY_STR, (void *)0);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(numberClicked), (gpointer)this);
    gtk_box_pack_start((GtkBox *)hbox, eventBox, FALSE, FALSE, 4);
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    

    
     for (int i = 1; i < 7; ++i) {
         std::stringstream sstr;
         sstr << "button" << i << ".png";
         eventBox = gtk_event_box_new();
         image = loadImage(sstr.str());
         gtk_container_add((GtkContainer *)eventBox, image);
         g_object_set_data((GObject  *)eventBox, KEY_STR, (void *)i);
         g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                          G_CALLBACK(numberClicked), (gpointer)this);
         gtk_box_pack_start((GtkBox *)hbox, eventBox, FALSE, FALSE, 4);
         gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
     }
    
    gtk_fixed_put((GtkFixed *)fixed, hbox, 75, 500);
    
    gtk_fixed_put((GtkFixed *)fixed, createRight(), 590, 140);
    
    return fixed;
}


GtkWidget *Sudoku::createGrid() {
    GtkWidget *table;
    
    table = gtk_table_new(maxColumns, maxRows, TRUE);
    gtk_table_set_row_spacings((GtkTable *)table, 4);
    gtk_table_set_col_spacings((GtkTable *)table, 4);
    
    for (int x = 0; x < maxColumns; ++x) {
        for (int y = 0; y < maxRows; ++y) {
            GtkWidget *eventBox;
            std::stringstream sstr;
            eventBox = gtk_event_box_new();
            gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
            gtk_widget_set_size_request(eventBox, 64, 64);
            g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                              G_CALLBACK(tilePressed), (gpointer)this);
            g_object_set_data((GObject  *)eventBox, KEY_STR, (void *)0);

            
            tiles[x][y] = loadImage("null.gif");

            gtk_container_add((GtkContainer *)eventBox, tiles[x][y]);
            gtk_table_attach_defaults((GtkTable *)table,
                                       eventBox, x, x + 1, y, y + 1);
            gtk_widget_set_size_request(tiles[x][y], 39, 39);

        }
    }
    
    return table;
}

GtkWidget *Sudoku::createRight() {
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *lhvbox, *rhvbox;
    GtkWidget *label;
    GtkWidget *eventBox;
    GtkWidget *image;
    GdkColor color;
    std::string str;
    std::stringstream sstr;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    color.red = 0xF600;
    color.green = 0xFF00;
    color.blue = 0x0000;
    
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, hbox, FALSE, FALSE, 0);
    lhvbox = gtk_vbox_new(FALSE, 0);
    rhvbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, lhvbox, FALSE, FALSE, 2);
    gtk_box_pack_start((GtkBox *)hbox, rhvbox, TRUE, TRUE, 2);
    
    label = gtk_label_new("High Score:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Score:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Round:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    sstr << scores[0].score;
    str = sstr.str();
    addCommas(str);
    label = gtk_label_new(str.c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, label, TRUE, TRUE, 2);
    
    scoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(scoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)scoreLabel, 1.0, 0.5);
    
    gtk_box_pack_start((GtkBox *)rhvbox, scoreLabel, TRUE, TRUE, 2);
    
    roundLabel = gtk_label_new("1");
    gtk_widget_modify_fg(roundLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)roundLabel, 1.0, 0.5);
    
    gtk_box_pack_start((GtkBox *)rhvbox, roundLabel, TRUE, TRUE, 2);

    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 5);
    
    label = gtk_label_new("Time Left:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 1);
    
    progress.init();
    progress.width(151);
    progress.height(32);
    progress.defaultImage("timebar_full.png");
    progress.percent(100);
    progress.text("2:00");
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    progress.textColor(color);
    
    gtk_box_pack_start((GtkBox *)vbox, progress.widget(), FALSE, FALSE, 10);
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 5);
    
    // scorevbox
    scorevbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, scorevbox, FALSE, FALSE, 5);
    
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, hbox, FALSE, FALSE, 0);
    lhvbox = gtk_vbox_new(FALSE, 0);
    rhvbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, lhvbox, FALSE, FALSE, 2);
    gtk_box_pack_start((GtkBox *)hbox, rhvbox, TRUE, TRUE, 2);
    
    color.red = 0xF600;
    color.green = 0xFF00;
    color.blue = 0x0000;
    label = gtk_label_new("Puzzle Score:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Time Score:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Errors:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("     x5,000:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Round Score:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Total Score:");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    
    
    puzzleScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(puzzleScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)puzzleScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, puzzleScoreLabel, TRUE, TRUE, 2);
    
    timeLeftScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(timeLeftScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)timeLeftScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, timeLeftScoreLabel, TRUE, TRUE, 2);
    
    errorsLabel = gtk_label_new("0");
    gtk_widget_modify_fg(errorsLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)errorsLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, errorsLabel, TRUE, TRUE, 2);
    
    errorPointsLabel = gtk_label_new("0");
    gtk_widget_modify_fg(errorPointsLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)errorPointsLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, errorPointsLabel, TRUE, TRUE, 2);
    
    roundScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(roundScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)roundScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, roundScoreLabel, TRUE, TRUE, 2);
    
    totalScoreLabel = gtk_label_new("0");
    gtk_widget_modify_fg(totalScoreLabel, (GtkStateType)0, &color);
    gtk_misc_set_alignment((GtkMisc *)totalScoreLabel, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, totalScoreLabel, TRUE, TRUE, 2);
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)rhvbox, label, FALSE, FALSE, 15);
    
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, false);
    
//     image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/buttons/button_quit.png");
    image = loadMainImage("buttons/button_quit.png");
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                     G_CALLBACK(userExit), (gpointer)this);
    gtk_container_add((GtkContainer *)eventBox, image);
    gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 5);
    gtk_widget_show_all(eventBox);
    
    return vbox;
}

void Sudoku::setGrid() {
    GdkColor color, origColor;
    color.red = 0xE000;
    color.green = 0;
    color.blue = 0;
    
    origColor.red = 0x0;
    origColor.green = 0x0;
    origColor.blue = 0x0;
    
    for (int x = 0; x < maxColumns; ++x) {
        for (int y = 0; y < maxRows; ++y) {
            gtk_widget_modify_fg(tiles[x][y], (GtkStateType)0, &color);
            if (numberMask[x][y] != 0) {
                std::stringstream sstr;
                g_object_set_data((GObject  *)tiles[x][y]->parent, KEY_STR, (void *)(255 - numberMask[x][y]));
                sstr << numberMask[x][y] << "prefill.png";
                reloadImage(tiles[x][y], sstr.str());
                gtk_widget_modify_fg(tiles[x][y], (GtkStateType)0, &origColor);
            }
            else {
                g_object_set_data((GObject  *)tiles[x][y]->parent, KEY_STR, (void *)numberMask[x][y]);
                reloadImage(tiles[x][y], "null.gif");
            }
        }
    }
}

void Sudoku::setTile(GtkWidget *tile, int number) {
    std::string str;
    str += number + '0';
    str += "error.png";
    reloadImage(tile, str);
}

// top level h/vbox
GtkWidget *Sudoku::init() {
    
    time_t sectime;
    GAME_PTR::loadScores(scores);
    maxColumns = 6;
    maxRows = 6;
    perXSector = 3;
    perYSector = 2;
    maxInt = 6;
    
    errors = 0;
    currentScore = 0;
    time(&sectime);
    // in case we find a problem later
    //std::cout << "Seed: " << sectime << std::endl;
    //srand(1168278592);
    srand(sectime);
    round = 0;
    
    tophbox = gtk_hbox_new(FALSE, 0);
    gtk_widget_set_size_request(tophbox, 800, 600);
    gtk_box_pack_start((GtkBox *)tophbox, createFixed(), FALSE, FALSE, 0);
    
    
    startRound();
    timer = g_timeout_add(200, timeout, this);
    
    gtk_widget_show_all(tophbox);
    
    return tophbox;
}
