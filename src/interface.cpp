

/*	Copyright (C) 2005, 2006, 2007, 2008 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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

#include <touchmegames.h>
#include <sstream>
#include <iomanip>

void TOUCHME_GAMES::createWindow() {
    GtkWidget *label;
    GtkRcStyle *style;
    GdkColor color;
    GtkWidget *image;
    GtkWidget *back;
    GtkWidget *vbox;
    GtkWidget *selectVbox;
    GtkWidget *eventBox;
    GError *err;
    std::string str;

    topWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    str = INSTALL_DIR;
    str += "images/icon1.gif";
    gtk_window_set_default_icon_from_file(str.c_str(), &err);
    


    gtk_widget_modify_bg(topWindow, (GtkStateType)0, &color);
    
    g_signal_connect (G_OBJECT (topWindow), "destroy",
            G_CALLBACK (gtk_main_quit), NULL);

    g_signal_connect (G_OBJECT (topWindow), "delete_event",
            G_CALLBACK (gtk_main_quit), NULL);
            
    gtk_widget_set_size_request(GTK_WIDGET(topWindow), 800, 600);
    gtk_window_set_title (GTK_WINDOW (topWindow), "TouchMe Games");
    
    // This is set in the config.h file
    if (FULL_SCREEN) {
        gtk_window_fullscreen(GTK_WINDOW(topWindow));
    }
    
    topVbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add((GtkContainer *)topWindow, topVbox);
    
    selectVbox = gtk_vbox_new(FALSE, 0);
	
    gtk_widget_set_size_request(selectVbox, 760, 600);
	

    gameSelectFixed = gtk_fixed_new();
    image = gamePtr.loadMainImage("submenu/bg_sub.png");
    gtk_fixed_put((GtkFixed *)gameSelectFixed, image, 0, 0);
    
    startButton = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)startButton, FALSE);
    gtk_widget_add_events(startButton, GDK_BUTTON_PRESS_MASK);
    
    image = gamePtr.loadMainImage("buttons/button_start.png");
    gtk_container_add((GtkContainer *)startButton, image);
    gtk_fixed_put((GtkFixed *)gameSelectFixed, startButton, 400, 330);
    
    g_signal_connect(G_OBJECT(startButton), 
                     "button-press-event",
                     G_CALLBACK(startGame), (gpointer)this);
    
    aboutButton = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)aboutButton, FALSE);
    gtk_widget_add_events(aboutButton, GDK_BUTTON_PRESS_MASK);
    
    image = gamePtr.loadMainImage("buttons/button_about.png");
    gtk_container_add((GtkContainer *)aboutButton, image);
    gtk_fixed_put((GtkFixed *)gameSelectFixed, aboutButton, 600, 330);
    
    g_signal_connect(G_OBJECT(aboutButton), 
                     "button-press-event",
                     G_CALLBACK(showHelp), (gpointer)this);
    
    gameBg = gamePtr.loadMainImage("gamebuttonbg.gif");
    gtk_fixed_put((GtkFixed *)gameSelectFixed, gameBg, 0, 0);
    
    gtk_fixed_put((GtkFixed *)gameSelectFixed, selectVbox, 0, 0);
    gtk_box_pack_start((GtkBox *)topVbox, gameSelectFixed, TRUE, TRUE, 0);
    
    // Create and add the hbox for the gameSelect box and the game
    // preview box.
    topHbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)selectVbox, topHbox, FALSE, FALSE, 0);
    
    
    selectBox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)topHbox, selectBox, FALSE, FALSE, 10);
    
    previewBox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)topHbox, previewBox, FALSE, FALSE, 10);
    
    back = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)back, FALSE);
    gtk_widget_add_events(back, GDK_BUTTON_PRESS_MASK);

    //image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/buttons/button_back.png");
    image = gamePtr.loadMainImage("buttons/button_back.png");
    gtk_container_add((GtkContainer *)back, image);
    gtk_box_pack_start((GtkBox *)selectBox, back, FALSE, FALSE, 10);
    g_signal_connect(G_OBJECT(back), 
            "button-press-event",
            G_CALLBACK(showMain), (gpointer)this);
    


    gtk_widget_show_all(topWindow);
	
}

void TOUCHME_GAMES::addGamesToWindow(int gameType, int startNum) {
	//GtkWidget *table;
    GtkWidget *image;
    GtkWidget *label;
    GtkStyle *style;
    int cntr(0);
    bool moreGamesRight(false);
    bool moreGamesLeft(false);
    std::vector<GAME_PTR *>::iterator tmp;
    std::vector<GAME_PTR *>::iterator leftStart;
    std::vector<GAME_PTR *>::iterator rightStart;
    int x(0), y(0);
    
    gameTable = gtk_table_new(4, 2, FALSE);
    gtk_table_set_row_spacings((GtkTable *)gameTable, 2);
    gtk_table_set_col_spacings((GtkTable *)gameTable, 2);
    gtk_box_pack_start((GtkBox *)selectBox, gameTable, FALSE, FALSE, 5);
    
    for (std::vector<GAME_PTR *>::iterator it = games->begin() + startNum;
         it != games->end() && cntr < 6; ++it) {
        
//        if ((*it)->gameType() == gameType && !(((*it)->gameType() == DART_GAME && !dart.dartBoardPresent()))) {
        if ((*it)->gameType() == gameType) {
            GtkWidget *spacer;
            GtkWidget *eventBox;
            GtkWidget *vbox;
            GtkRcStyle *rcstyle;
            GdkColor color;
            std::string name((*it)->name());
            std::string imgloc(INSTALL_DIR);
            
            ++cntr;
            
            imgloc += name;
            imgloc += "/";
            imgloc += (*it)->imageName();
            
            image = gtk_image_new_from_file(imgloc.c_str());

            eventBox = gtk_event_box_new();
            gtk_widget_set_size_request(eventBox, 148, 138);
            gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);

            
            rcstyle = gtk_rc_style_new();
            color.red = 0x9000;
            color.green = 0x9000;
            color.blue = 0x9000;
            
            rcstyle->bg[0] = color;
            rcstyle->color_flags[0] = GTK_RC_BG;
            //rcstyle->bg_pixmap_name[0] = "/usr/local/share/touchmegames/images/gamebuttonbg.xpm";
            g_object_set_data((GObject *)eventBox, KEY_STR, (void *)(cntr - 1));
            gtk_widget_modify_style(eventBox, rcstyle);
            
            gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
            
//             (*gameMap)[eventBox] = *it;
            (*it)->selectWidget(eventBox);
            
            // initialises the screens.  then hide stupidGames box and add the new
            // game's top widget to the topVbox.
            
            g_signal_connect(G_OBJECT(eventBox), 
                            "button-press-event",
                            G_CALLBACK(showHighScores), (gpointer)this);
            
            
            vbox = gtk_vbox_new(false, 2);
            spacer = gtk_label_new(" ");
            gtk_widget_set_size_request(spacer, -1, 3);
            gtk_box_pack_start((GtkBox *)vbox, spacer, FALSE, FALSE, 0);
            //gtk_container_add((GtkContainer *)eventBox, image);
            gtk_container_add((GtkContainer *)eventBox, vbox);
            gtk_box_pack_start((GtkBox *)vbox, image, FALSE, FALSE, 5);
            
            label = gtk_label_new(name.c_str());
            gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
            gtk_table_attach_defaults((GtkTable *)gameTable, eventBox, x, x + 1, y, y + 1);


            if (++x == 2) {
                    ++y;
                    x = 0;
            }
            else {
            //	--y;
            }
            
            // this is for popups
            (*it)->setWindow(topWindow);
        }
        else {
            (*it)->selectWidget(0x0);
        }
        tmp = it;
        rightStart = it;
            
    }
    while (cntr != 6) {
//         image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/submenu/blankgame.gif");
        image = gamePtr.loadMainImage("submenu/blankgame.gif");
        //gtk_widget_set_size_request(image, 148, );
        gtk_table_attach_defaults((GtkTable *)gameTable, image, cntr % 2, (cntr % 2) + 1, cntr / 2, (cntr / 2) + 1);
        ++cntr;
    }
 
    while (++tmp != games->end()) {
        if ((*tmp)->gameType() == gameType) {
            moreGamesRight = true;
            
            tmp = games->end() - 1;
        }
    }
    
    if (startNum != 0) {
        tmp = games->begin() + startNum - 1;
    }
    else {
        tmp = games->begin();
    }
    
    while (--tmp != games->begin() - 1) {
        if ((*tmp)->gameType() == gameType) {

            moreGamesLeft = true;
                
            tmp = games->begin();
        }
    }
    
    
    if (moreGamesLeft) {
        GtkWidget *arrow;
        // create the left arrow
        
        leftStart = games->begin() + startNum;
        cntr = 0;
        while (cntr != 6 && leftStart != games->begin()) {
            --leftStart;
            if ((*leftStart)->gameType() == gameType) {
                ++cntr;
            }
        }
        
        arrow = gtk_event_box_new();
        gtk_event_box_set_visible_window((GtkEventBox *)arrow, FALSE);
        gtk_widget_add_events(arrow, GDK_BUTTON_PRESS_MASK);
        g_object_set_data((GObject *)arrow, KEY_STR, (void *)((gameType << 16) | (leftStart - games->begin())));
//   image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/buttons/button_left.png");
        image = gamePtr.loadMainImage("buttons/button_left.png");
        gtk_container_add((GtkContainer *)arrow, image);
        gtk_table_attach_defaults((GtkTable *)gameTable, arrow, 0, 1, 3, 4);
        g_signal_connect(G_OBJECT(arrow), 
                    "button-press-event",
                    G_CALLBACK(leftArrowPressed), (gpointer)this);
    }
    if (moreGamesRight) {
        GtkWidget *arrow;
        // create the left arrow
        arrow = gtk_event_box_new();
        gtk_event_box_set_visible_window((GtkEventBox *)arrow, FALSE);
        gtk_widget_add_events(arrow, GDK_BUTTON_PRESS_MASK);
        g_object_set_data((GObject *)arrow, KEY_STR, (void *)((gameType << 16) 
                | (rightStart - games->begin() + 1)));
//   image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/buttons/button_right.png");
        image = gamePtr.loadMainImage("buttons/button_right.png");
        gtk_container_add((GtkContainer *)arrow, image);
        gtk_table_attach_defaults((GtkTable *)gameTable, arrow, 1, 2, 3, 4);
        g_signal_connect(G_OBJECT(arrow), 
                        "button-press-event",
                        G_CALLBACK(rightArrowPressed), (gpointer)this);
    }
    gtk_widget_hide(gameBg);
}

void TOUCHME_GAMES::createPreviewBox(GAME_PTR *ptr) {
    SCORE_STRUCT scores[10];
    GtkWidget *label;
    GtkWidget *image;
    GtkWidget *eventBox;
    GdkColor color;
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    ptr->loadScores(scores);
    
    
    previewTable = gtk_table_new(14, 4, FALSE);
//    std::cout << "Creating: " << previewTable << std::endl;
    gtk_fixed_put((GtkFixed *)gameSelectFixed, previewTable, 360, 50);
    gtk_widget_set_size_request(previewTable, 400, 260);
    gtk_table_set_row_spacings((GtkTable *)previewTable, 2);
    gtk_table_set_col_spacings((GtkTable *)previewTable, 2);
    //gtk_box_pack_start((GtkBox *)previewBox, previewTable, FALSE, FALSE, 5);
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, 40, 30);
    gtk_table_attach_defaults((GtkTable *)previewTable, label, 1, 2, 0, 1);
    
    label = gtk_label_new("Name");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_widget_set_size_request(label, 150, 30);
    gtk_table_attach_defaults((GtkTable *)previewTable, label, 1, 2, 0, 1);
    
    
    label = gtk_label_new("Score");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_widget_set_size_request(label, 90, 30);
    gtk_table_attach_defaults((GtkTable *)previewTable, label, 2, 3, 0, 1);
    
    
    label = gtk_label_new("Date");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_widget_set_size_request(label, 90, 30);
    gtk_table_attach_defaults((GtkTable *)previewTable, label, 3, 4, 0, 1);
    
    
    for (int i = 0; i < 10; ++i) {
        std::stringstream sstr;
        sstr << std::setw(2) << i + 1 << '.';
        label = gtk_label_new(sstr.str().c_str());
        gtk_widget_modify_fg(label, (GtkStateType)0, &color);
        gtk_table_attach_defaults((GtkTable *)previewTable, label, 0, 1, i + 1, i + 2);
        
        sstr.str("");
        if (ptr->getScoreType() == INTEGER || ptr->getScoreType() == INTEGER_DOWN) {
            std::string str;
            GAME_PTR game;
            sstr << scores[i].score;
            str = sstr.str();
            sstr.str("");
            game.addCommas(str);
            sstr << str << "       ";
        }
        else if (ptr->getScoreType() == FLOAT) {
            if (scores[i].fscore) {
                sstr << std::setprecision(4) << scores[i].fscore << "       ";
            }
            else {
                sstr << "0.0" << "       ";
            }
        }
        else if (ptr->getScoreType() == TIME_DOWN) {
            sstr << std::setw(2) << std::setfill(' ') << (int)scores[i].fscore << ':' 
                << std::setw(2) << std::setfill('0') << (int)(scores[i].fscore * 100.0) % 100;
        }
        
        label = gtk_label_new(scores[i].name.c_str());
        gtk_widget_modify_fg(label, (GtkStateType)0, &color);
        gtk_table_attach_defaults((GtkTable *)previewTable, label, 1, 2, i + 1, i + 2);
        
        label = gtk_label_new(sstr.str().c_str());
        gtk_widget_modify_fg(label, (GtkStateType)0, &color);
        gtk_table_attach_defaults((GtkTable *)previewTable, label, 2, 3, i + 1, i + 2);
        
        label = gtk_label_new(scores[i].date.c_str());
        gtk_widget_modify_fg(label, (GtkStateType)0, &color);
        gtk_table_attach_defaults((GtkTable *)previewTable, label, 3, 4, i + 1, i + 2);
    }
    
    
    
    gtk_widget_show_all(previewTable);
}

void TOUCHME_GAMES::createHelpBox(GAME_PTR *ptr) {
    GtkWidget *label;
    GtkWidget *image;
    GtkWidget *eventBox;
    GtkWidget *widget;
    GdkColor color;
    
    previewTable = gtk_table_new(3, 4, FALSE);
    gtk_fixed_put((GtkFixed *)gameSelectFixed, previewTable, 360, 50);
    gtk_widget_set_size_request(previewTable, 400, 260);
    gtk_table_set_row_spacings((GtkTable *)previewTable, 2);
    gtk_table_set_col_spacings((GtkTable *)previewTable, 2);

    
    widget = ptr->getHelp();
    gtk_widget_set_size_request(widget, -1, 260);
    gtk_table_attach_defaults((GtkTable *)previewTable, widget, 0, 4, 0, 3);
	
	
	
/*	eventBox = gtk_event_box_new();
	gtk_widget_set_size_request(eventBox, 140, 108);
	gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
	gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
	
 image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/buttons/button_start.png");
	gtk_container_add((GtkContainer *)eventBox, image);
	gtk_table_attach_defaults((GtkTable *)previewTable, eventBox, 0, 1, 3, 4);
	
	g_signal_connect(G_OBJECT(eventBox), 
			 "button-press-event",
			 G_CALLBACK(startGame), (gpointer)this);
	
	eventBox = gtk_event_box_new();
	gtk_widget_set_size_request(eventBox, 140, 108);
	gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
	gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
	
 image = gtk_image_new_from_file("/usr/local/share/touchmegames/images/buttons/button_about.png");
	gtk_container_add((GtkContainer *)eventBox, image);
	gtk_table_attach_defaults((GtkTable *)previewTable, eventBox, 3, 4, 3, 4);
	
	g_signal_connect(G_OBJECT(image), 
			 "button-press-event",
			 G_CALLBACK(showHelp), (gpointer)this);*/
	
	
	gtk_widget_show_all(previewTable);
}

void TOUCHME_GAMES::createMainMenu() {
    GtkWidget *widget;
    GtkWidget *vbox;
    GdkColor color;
    GtkWidget *eventBox;
    GtkWidget *image;
    
    color.red = 0x9000;
    color.green = 0x9000;
    color.blue = 0x9000;

    mainMenu = gtk_fixed_new();
    gtk_widget_set_size_request(mainMenu, 800, 600);
    
    bgImage = gamePtr.loadMainImage("mainmenu/bg_main.png");
    gtk_fixed_put((GtkFixed *)mainMenu, bgImage, 0, 0);
    
    
    menuWidgets[0] = gtk_event_box_new();
    g_object_set_data((GObject *)menuWidgets[0], KEY_STR, (void *)CARD_GAME);
    gtk_event_box_set_visible_window((GtkEventBox *)menuWidgets[0], FALSE);
    gtk_widget_add_events(menuWidgets[0], GDK_BUTTON_PRESS_MASK);
    
    // caption below
    image = gamePtr.loadMainImage("mainmenu/cap_cardgames.png");
    menuCaptions[0] = image;
    gtk_fixed_put((GtkFixed *)mainMenu, image, 130, 279);
    
    image = gamePtr.loadMainImage("mainmenu/bg_icons.png");
    widget = gtk_fixed_new();
    gtk_fixed_put((GtkFixed *)widget, image, 0, 0);
    image = gamePtr.loadMainImage("mainmenu/icon_cards.png");
    gtk_fixed_put((GtkFixed *)widget, image, 0, 0);
    gtk_container_add((GtkContainer *)menuWidgets[0], widget);
    gtk_fixed_put((GtkFixed *)mainMenu, menuWidgets[0], 130, 150);
    g_signal_connect(G_OBJECT(menuWidgets[0]), 
                        "button-press-event",
                        G_CALLBACK(showSubMenu), (gpointer)this);
    
    
    
    menuWidgets[1] = gtk_event_box_new();
    g_object_set_data((GObject *)menuWidgets[1], KEY_STR, (void *)WORD_GAME);
    gtk_event_box_set_visible_window((GtkEventBox *)menuWidgets[1], FALSE);
    gtk_widget_add_events(menuWidgets[1], GDK_BUTTON_PRESS_MASK);
    
    // caption below
    image = gamePtr.loadMainImage("mainmenu/cap_wordhunt.png");
    menuCaptions[1] = image;
    gtk_fixed_put((GtkFixed *)mainMenu, image, 330, 279);
    
    // words
    image = gamePtr.loadMainImage("mainmenu/bg_icons.png");
    widget = gtk_fixed_new();
    gtk_fixed_put((GtkFixed *)widget, image, 0, 0);
    image = gamePtr.loadMainImage("mainmenu/icon_words.png");
    gtk_fixed_put((GtkFixed *)widget, image, 0, 0);
    gtk_container_add((GtkContainer *)menuWidgets[1], widget);
    gtk_fixed_put((GtkFixed *)mainMenu, menuWidgets[1], 330, 150);
    g_signal_connect(G_OBJECT(menuWidgets[1]), 
                        "button-press-event",
                        G_CALLBACK(showSubMenu), (gpointer)this);
    
    
    menuWidgets[2] = gtk_event_box_new();
    g_object_set_data((GObject *)menuWidgets[2], KEY_STR, (void *)STRATEGY_GAME);
    gtk_event_box_set_visible_window((GtkEventBox *)menuWidgets[2], FALSE);
    gtk_widget_add_events(menuWidgets[2], GDK_BUTTON_PRESS_MASK);
    
    // caption below
    image = gamePtr.loadMainImage("mainmenu/cap_strategy.png");
    menuCaptions[2] = image;
    gtk_fixed_put((GtkFixed *)mainMenu, image, 535, 280);
    
    // strategy
    image = gamePtr.loadMainImage("mainmenu/bg_icons.png");
    widget = gtk_fixed_new();
    gtk_fixed_put((GtkFixed *)widget, image, 0, 0);
    image = gamePtr.loadMainImage("mainmenu/icon_strategy.png");
    gtk_fixed_put((GtkFixed *)widget, image, 0, 0);
    gtk_container_add((GtkContainer *)menuWidgets[2], widget);
    gtk_fixed_put((GtkFixed *)mainMenu, menuWidgets[2], 530, 150);
    g_signal_connect(G_OBJECT(menuWidgets[2]), 
                        "button-press-event",
                        G_CALLBACK(showSubMenu), (gpointer)this);
    
    menuWidgets[3] = gtk_event_box_new();
    g_object_set_data((GObject *)menuWidgets[3], KEY_STR, (void *)DART_GAME);
    gtk_event_box_set_visible_window((GtkEventBox *)menuWidgets[3], FALSE);
    gtk_widget_add_events(menuWidgets[3], GDK_BUTTON_PRESS_MASK);
    
    // caption below
    image = gamePtr.loadMainImage("mainmenu/cap_darts.png");
    menuCaptions[3] = image;
    gtk_fixed_put((GtkFixed *)mainMenu, image, 230, 435);
    
    // darts
    image = gamePtr.loadMainImage("mainmenu/bg_icons.png");
    widget = gtk_fixed_new();
    gtk_fixed_put((GtkFixed *)widget, image, 0, 0);
    image = gamePtr.loadMainImage("mainmenu/icon_darts.png");
    gtk_fixed_put((GtkFixed *)widget, image, 0, 0);
    gtk_container_add((GtkContainer *)menuWidgets[3], widget);
    gtk_fixed_put((GtkFixed *)mainMenu, menuWidgets[3], 230, 305);
    g_signal_connect(G_OBJECT(menuWidgets[3]), 
                        "button-press-event",
                        G_CALLBACK(showSubMenu), (gpointer)this);
                        
    menuWidgets[4] = gtk_event_box_new();
    g_object_set_data((GObject *)menuWidgets[4], KEY_STR, (void *)UNKNOWN_GAME);
    gtk_event_box_set_visible_window((GtkEventBox *)menuWidgets[4], FALSE);
    gtk_widget_add_events(menuWidgets[4], GDK_BUTTON_PRESS_MASK);
    
    // caption below
    image = gamePtr.loadMainImage("mainmenu/cap_misc.png");
    menuCaptions[4] = image;
    gtk_fixed_put((GtkFixed *)mainMenu, image, 430, 430);
    
    // misc
    image = gamePtr.loadMainImage("mainmenu/bg_icons.png");
    widget = gtk_fixed_new();
    gtk_fixed_put((GtkFixed *)widget, image, 0, 0);
    image = gamePtr.loadMainImage("mainmenu/icon_misc.png");
    gtk_fixed_put((GtkFixed *)widget, image, 0, 0);
    gtk_container_add((GtkContainer *)menuWidgets[4], widget);
    gtk_fixed_put((GtkFixed *)mainMenu, menuWidgets[4], 430, 305);
    g_signal_connect(G_OBJECT(menuWidgets[4]), 
                        "button-press-event",
                        G_CALLBACK(showSubMenu), (gpointer)this);
    
    
    
    // Place holder for login/system stuff
    
    
    mainBack = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)mainBack, FALSE);
    gtk_widget_add_events(mainBack, GDK_BUTTON_PRESS_MASK);
    
    widget = gamePtr.loadMainImage("buttons/button_back.png");
    gtk_container_add((GtkContainer *)mainBack, widget);
    g_signal_connect(G_OBJECT(mainBack), 
                        "button-press-event",
                        G_CALLBACK(showMain), (gpointer)this);
    gtk_fixed_put((GtkFixed *)mainMenu, mainBack, 341, 517);
    
    mainAbout = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)mainAbout, FALSE);
    gtk_widget_add_events(mainAbout, GDK_BUTTON_PRESS_MASK);
    
    widget = gamePtr.loadMainImage("buttons/button_about.png");
    gtk_container_add((GtkContainer *)mainAbout, widget);
    g_signal_connect(G_OBJECT(mainAbout), 
                        "button-press-event",
                        G_CALLBACK(showAbout), (gpointer)this);
    gtk_fixed_put((GtkFixed *)mainMenu, mainAbout, 241, 517);
    
    
    mainQuit = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)mainQuit, FALSE);
    gtk_widget_add_events(mainQuit, GDK_BUTTON_PRESS_MASK);
    
    widget = gamePtr.loadMainImage("buttons/button_quit.png");
    gtk_container_add((GtkContainer *)mainQuit, widget);
    g_signal_connect(G_OBJECT(mainQuit), 
                     "button-press-event",
                     G_CALLBACK(gtk_main_quit), (gpointer)this);
    gtk_fixed_put((GtkFixed *)mainMenu, mainQuit, 441, 517);
    
    
    gtk_widget_show_all(mainMenu);
    
    
    createAbout();
}

void TOUCHME_GAMES::createAbout() {
    GtkWidget *label;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *centerHbox, *centerVbox;
    GdkColor color;
    std::string str;
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    
    aboutBox = gtk_vbox_new(FALSE, 0);
    centerHbox = gtk_hbox_new(FALSE, 0);
    centerVbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)aboutBox, centerHbox, FALSE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)aboutBox, centerVbox, FALSE, FALSE, 10);
    gtk_widget_set_size_request(aboutBox, 625, 345);
    gtk_fixed_put((GtkFixed *)mainMenu, aboutBox, 86, 128);
    
    str = "TouchMe Games - Version ";
    str += VERSION;
    label = gtk_label_new(str.c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)centerVbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("(c) 2005-2008 Andy Grebe");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)centerVbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)centerVbox, label, FALSE, FALSE, 0);
    
    
    hbox = gtk_hbox_new(FALSE, 10);
    gtk_box_pack_start((GtkBox *)centerVbox, hbox, FALSE, FALSE, 0);
    
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, vbox, TRUE, FALSE, 0);
    
    
    label = gtk_label_new("Authors");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("Andy Grebe - Maintainer/Core Developer");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("\tandy.grebe@gmail.com");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, -1, 5);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    
    label = gtk_label_new("Erno Szabados - Graphics");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, -1, 5);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("Mike Smiley - Developer/Upgrader");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("\tmhsmiley@gmail.com");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, -1, 5);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    
    
    label = gtk_label_new("Linda Clemens - Graphics");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, -1, 5);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("Heather Swanson - Graphics");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, -1, 5);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    
    label = gtk_label_new("Farhan Kalsekar - Testing");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, -1, 5);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    //  Thanks to--------------------------------------------------
    
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, vbox, TRUE, FALSE, 0);
            
    label = gtk_label_new("Thanks To");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("Maflipp(?) - Bug Finder");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("\tmaflipp@sourceforge.net");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("Mike Gilorma - Bug Finder/Fixer");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("\tphatbeatsboy@hotmail.com");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, -1, 5);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    
    label = gtk_label_new("Dwinell Zajatz - Layout Help/Testing");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("\tdwinell@gmail.com");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, -1, 5);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("Nick Mulan - Testing");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("\tnjm137@yahoo.com");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, -1, 5);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, -1, 5);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, -1, 5);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_widget_set_size_request(label, -1, 5);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new(" ");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)centerVbox, label, FALSE, FALSE, 10);
    
    label = gtk_label_new("License - GNU General Public License");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)centerVbox, label, FALSE, FALSE, 0);
    
    label = gtk_label_new("Version 2, June 1991");
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)centerVbox, label, FALSE, FALSE, 0);

}
