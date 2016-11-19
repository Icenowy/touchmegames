
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

#include <wordhunt.h>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <fstream>
#include <sys/time.h>
#include <whxmlparser.h>

bool WordHunt::checkAvailable(char array[12][12], std::string word, int x, int y, bool direction) {
    int len(word.size());
    
    for (int i = 0; i < len; ++i) {
        if (array[x + (direction ? i : 0)][y + (direction ? 0 : i)] != ' '
                    && array[x + (direction ? i : 0)][y + (direction ? 0 : i)] != word[i]) {
            return false;
        }
    }
    return true;
}

bool WordHunt::greaterThan(std::pair<char, int> lh, std::pair<char, int> rh) {
    if (lh.second > rh.second) {
        return true;
    }
    return false;
}

void WordHunt::fillWithRandom(char array[12][12]) {
    std::map<char, int> occurences;
    std::vector<std::pair<char, int> > occurVec;
    int totalLetters(0);
    int currentFill;
    
    // count all the letters in the array
    for (int x = 0; x < 12; ++x) {
        for (int y = 0; y < 12; ++y) {
            if (array[x][y] != ' ') {
                ++occurences[array[x][y]];
                ++totalLetters;
            }
        }
    }
    
    // place the pairs into a vector
    for (std::map<char, int>::iterator it = occurences.begin(); it != occurences.end(); ++it) {
        occurVec.push_back(*it);
    }
    
    // now sort them from most occurences to fewest
    std::sort(occurVec.begin(), occurVec.end(), greaterThan);
    
    // next, add up each.  This'll help make sure that if there;s
    // a bunch of one letter in all of the words, there'll be a bunch
    // more of that letter in the array, randomly placed in thereS
    // sorry just reusing a value
    currentFill = 0;
    for (std::vector<std::pair<char, int> >::iterator it = occurVec.begin(); it != occurVec.end(); ++it) {
        currentFill += it->second;
        it->second = currentFill;
    }
    occurences.clear();
    for (int i = totalLetters; i < 144; ++i) {
        bool done(false);
        while (!done) {
            int x(random() % 12), y(random() % 12);
            // check to see if it's blank
            if (array[x][y] == ' ') {
                currentFill = random() % totalLetters;
                for (int i = 0; i < occurVec.size() && !done; ++i) {
                    if (occurVec[i].second >= currentFill) {
                        ++occurences[occurVec[i].first];
                        array[x][y] = tolower(occurVec[i].first);
                        done = true;
                    }
                }
            }
        }
    }
}

bool WordHunt::fillArray(char array[12][12], std::string *words, bool recurse) {
    bool done(false);
    int word(0);
    int wordTries(0);
    int forwords = random() % 3;
    std::string actualWord;
    if (!forwords) {
        actualWord = "";
        for (int i = words[word].size() - 1; i >= 0; --i) {
            actualWord += words[word][i];
        }
    }
    else {
        actualWord = words[word];
    }
    while (!done) {
        bool wordDirection = random() & 0x1;
        
        int xlocation = random() % 12;
        int ylocation = random() % 12;
        std::string wordstr;
        
        if (wordDirection) {
            if (xlocation + words[word].size() <= 12
                        && checkAvailable(array, actualWord, xlocation, ylocation, wordDirection)) {

                for (int i = 0; i < words[word].size(); ++i) {
                    if (forwords) {
                        array[xlocation + i][ylocation] = tolower(words[word][i]);
                    }
                    else {
                        array[xlocation + i][ylocation] = tolower(words[word][words[word].size() - (i + 1)]);
                    }
                }
                ++word;
                wordTries = 0;
                if (word == 10) {
                    done = true;
                }
                forwords = random() % 3;
                if (word < 10) {
                    if (!forwords) {
                        actualWord = "";
                        for (int i = words[word].size() - 1; i >= 0; --i) {
                            actualWord += words[word][i];
                        }
                    }
                    else {
                        actualWord = words[word];
                    }
                }
            }
                    
            }
            else {
                    if (ylocation + words[word].size() <= 12
                                && checkAvailable(array, actualWord, xlocation, ylocation, wordDirection)) {
                        for (int i = 0; i < words[word].size(); ++i) {
                            if (forwords) {
                                array[xlocation][ylocation + i] = tolower(words[word][i]);
                            }
                            else {
                                array[xlocation][ylocation + i] = tolower(words[word][words[word].size() - (i + 1)]);
                            }
                        }
                        ++word;
                        wordTries = 0;
                        if (word == 10) {
                            done = true;
                        }
                        forwords = random() % 3;
                        if (word < 10) {
                            if (!forwords) {
                                actualWord = "";
                                for (int i = words[word].size() - 1; i >= 0; --i) {
                                    actualWord += words[word][i];
                                }
                            }
                            else {
                                actualWord = words[word];
                            }
                        }
                }
        }
        if (!done && wordTries > 144) {
            bool good(false);
            for (wordTries = 0; wordTries < 10 && !good && recurse; ++wordTries) {
                for (int x = 0; x < 12; ++x) {
                    for (int y = 0; y < 12; ++y) {
                        array[x][y] = ' ';
                    }
                }
                good = fillArray(array, words, false);
            }
            if (!good) {
                std::cerr << "Error, could not fill words" << std::endl;
                exit(0);
            }
        }
    }
    fillWithRandom(array);
    return true;
}

GtkWidget *WordHunt::initLh() {
    GtkRcStyle *style, *eventStyle;
    GdkColor color;
    char array[12][12];
    
    
    for (int y = 0; y < 12; ++y) {
        for (int x = 0; x < 12; ++x) {
            array[x][y] = ' ';
        }
    }

            
    style = gtk_rc_style_new();
    style->font_desc = pango_font_description_new();
    pango_font_description_set_size(style->font_desc, 28 * PANGO_SCALE);
    pango_font_description_set_weight(style->font_desc, PANGO_WEIGHT_BOLD);
    
    color.red = 0x3800;
    color.green = 0xd900;
    color.blue = 0x3000;
    
    style->fg[0] = color;
    style->color_flags[0] = GTK_RC_FG;
    
    
    table = gtk_table_new(numberOfRows, numberOfRows, TRUE);
    gtk_table_set_row_spacings((GtkTable *)table, 4);
    gtk_table_set_col_spacings((GtkTable *)table, 4);
    for (int x = 0; x < numberOfRows; ++x) {
        for (int y = 0; y < numberOfRows; ++y) {
            GtkWidget *eventBox;
            std::stringstream sstr;
            eventBox = gtk_event_box_new();
            gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
            gtk_widget_set_size_request(eventBox, 39, 39);
            gtk_widget_modify_style(eventBox, bgBlack);
            g_signal_connect(G_OBJECT(eventBox), "button-press-event",
                                G_CALLBACK(toggleBg), (gpointer)this);

            g_signal_connect(G_OBJECT(eventBox), 
                            "motion-notify-event",
                            G_CALLBACK(mouseMove), (gpointer)this);
//             sstr << "/usr/local/share/touchmegames/wordhunt/images/letter.png";
//             letters[x][y] = gtk_image_new_from_file(sstr.str().c_str());
            letters[x][y] = loadImage("letter.png");

            gtk_container_add((GtkContainer *)eventBox, letters[x][y]);
            gtk_table_attach_defaults((GtkTable *)table,
                            eventBox, x, x + 1, y, y + 1);
            gtk_widget_set_size_request(letters[x][y], 39, 39);

        }
    }
    pango_font_description_free(style->font_desc);
    return table;
}

GtkWidget *WordHunt::createListGUI() {
    GtkWidget *eventBox;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *image;
    GtkRcStyle *whiteStyle, *darkStyle, *eventStyle;
    GdkColor color;
    
    whiteStyle = gtk_rc_style_new();
    /*whiteStyle->font_desc = pango_font_description_new();
    pango_font_description_set_size(whiteStyle->font_desc, 16 * PANGO_SCALE);
    pango_font_description_set_weight(whiteStyle->font_desc, PANGO_WEIGHT_BOLD);*/
    color.red = 0xf600;
    color.green = 0xff00;
    color.blue = 0x0000;
    whiteStyle->fg[0] = color;
    whiteStyle->color_flags[0] = GTK_RC_FG;
    
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    for (int i = 0; i < 10; ++i) {
//         eventBox = gtk_event_box_new();
//         gtk_widget_modify_style(eventBox, bgBlack);
       
        availableWords[i] = gtk_label_new(" ");
//         gtk_container_add((GtkContainer *)eventBox, availableWords[i]);
        
        gtk_widget_modify_style(availableWords[i], whiteStyle);
        
        gtk_box_pack_start((GtkBox *)vbox, availableWords[i], FALSE, FALSE, 0);
    }
    
    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 5);
    
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

    //pango_font_description_free(whiteStyle->font_desc);
    return vbox;
}

GtkWidget *WordHunt::initRh() {
    GtkWidget *rhtopvbox;
    GtkWidget *vbox, *lhvbox, *rhvbox;
    GtkWidget *hbox;
    GtkWidget *eventBox;
    GtkWidget *label;
    GtkWidget *image;
    std::stringstream sstr;
    std::string str;
    GtkRcStyle *whiteStyle, *darkStyle, *eventStyle;
    GdkColor color;
    GtkWidget *topVBox, *topHBox;
    
    whiteStyle = gtk_rc_style_new();

    color.red = 0xf600;
    color.green = 0xff00;
    color.blue = 0x0000;
    whiteStyle->fg[0] = color;
    whiteStyle->color_flags[0] = GTK_RC_FG;
    
    darkStyle = gtk_rc_style_new();

    color.red = 0x0;
    color.green = 0x0;
    color.blue = 0x0;
    darkStyle->fg[0] = color;
    darkStyle->color_flags[0] = GTK_RC_FG;
    
    eventStyle = gtk_rc_style_new();
    color.red = 0x0;
    color.green = 0x0;
    color.blue = 0x0;
    eventStyle->bg[0] = color;
    eventStyle->color_flags[0] = GTK_RC_BG;
    
    rhtopvbox = gtk_vbox_new(FALSE, 0);
    

    vbox = gtk_vbox_new(FALSE, 0);

    gtk_box_pack_start((GtkBox *)rhtopvbox, vbox, FALSE, FALSE, 0);
    
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, hbox, FALSE, FALSE, 0);
    lhvbox = gtk_vbox_new(FALSE, 0);
    rhvbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, lhvbox, FALSE, FALSE, 2);
    gtk_box_pack_start((GtkBox *)hbox, rhvbox, TRUE, TRUE, 2);
    
    label = gtk_label_new("High Score:");
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_widget_modify_style(label, whiteStyle);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Score:");
    gtk_widget_modify_style(label, whiteStyle);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    label = gtk_label_new("Round:");
    gtk_widget_modify_style(label, whiteStyle);
    gtk_misc_set_alignment((GtkMisc *)label, 0.0, 0.5);
    gtk_box_pack_start((GtkBox *)lhvbox, label, FALSE, FALSE, 2);
    
    sstr << scores[0].score;
    str = sstr.str();
    addCommas(str);
    label = gtk_label_new(str.c_str());
    //gtk_widget_set_size_request(label, 80, -1);
    gtk_widget_modify_style(label, whiteStyle);
    gtk_misc_set_alignment((GtkMisc *)label, 1.0, 0.5);
    gtk_box_pack_start((GtkBox *)rhvbox, label, TRUE, TRUE, 2);
    
    totalScore = gtk_label_new("0");
    gtk_widget_modify_style(totalScore, whiteStyle);
    gtk_misc_set_alignment((GtkMisc *)totalScore, 1.0, 0.5);
    
    gtk_box_pack_start((GtkBox *)rhvbox, totalScore, TRUE, TRUE, 2);
    
    roundLabel = gtk_label_new("1");
    gtk_widget_modify_style(roundLabel, whiteStyle);
    gtk_misc_set_alignment((GtkMisc *)roundLabel, 1.0, 0.5);
    
    gtk_box_pack_start((GtkBox *)rhvbox, roundLabel, TRUE, TRUE, 2);

    label = gtk_label_new(" ");
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 5);
    
    label = gtk_label_new("Time Left:");
    gtk_widget_modify_style(label, whiteStyle);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 1);



    
    sstr.str("");
    sstr << INSTALL_DIR << "wordhunt/images/timebar_full.png";

    progress.init();
    progress.width(151);
    progress.height(32);
    progress.image(sstr.str());
    progress.percent(100);
    progress.text("1:00");
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    progress.textColor(color);
    
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)hbox, progress.widget(), TRUE, FALSE, 0);
    
    gtk_box_pack_start((GtkBox *)vbox, hbox, TRUE, FALSE, 10);
    
    gtk_box_pack_start((GtkBox *)rhtopvbox, createListGUI(), FALSE, FALSE, 5);


    //pango_font_description_free(whiteStyle->font_desc);

    topVBox = gtk_vbox_new(FALSE, 0);
    topHBox = gtk_hbox_new(FALSE, 0);
    
    gtk_widget_set_size_request(topVBox, 210, 430);
    
    
    gtk_box_pack_start((GtkBox *)topVBox, topHBox, TRUE, FALSE, 0);
    gtk_box_pack_start((GtkBox *)topHBox, rhtopvbox, TRUE, FALSE, 0);
    
//     std::cout << "dumb " << std::endl;
//     std::cout.flush();
    
    return topVBox;
}

bool WordHunt::loadList(int list, std::string &listName) {
    std::ifstream is;
    int currentList(0);
    WhXmlParser parser;
    std::string errStr;
    bool retval;
    std::string str;
    


    if (usedLists[0] == list || usedLists[1] == list || usedLists[2] == list) {
        return false;
    }
    str = INSTALL_DIR;
    str += "wordhunt/lists";
//     if (parser.openXmlFile("/usr/local/share/touchmegames/wordhunt/lists", "lists", errStr) == 0) {
    if (parser.openXmlFile(str.c_str(), "lists", errStr) == 0) {
        std::cerr << "Could not open lists file: " << errStr << std::endl;
        exit(0);
    }
    usedLists[round - 1] = list;
    
    retval = parser.getNthList(list, listName, words, errStr);
    parser.closeXmlFile();
    return retval;
}

void WordHunt::showNextRound() {
    std::string str;
    GtkWidget *label;
    GtkWidget *vbox;
    GtkRcStyle *whiteStyle;
    GdkColor color;
    ++round;
    
    
    whiteStyle = gtk_rc_style_new();
    whiteStyle->font_desc = pango_font_description_new();
    pango_font_description_set_size(whiteStyle->font_desc, 16 * PANGO_SCALE);
    pango_font_description_set_weight(whiteStyle->font_desc, PANGO_WEIGHT_BOLD);
    color.red = 0xf600;
    color.green = 0xff00;
    color.blue = 0x0000;
    whiteStyle->fg[0] = color;
    whiteStyle->color_flags[0] = GTK_RC_FG;
    
    if (clearedBoard && round < 4) {
        nextRoundImage = loadImage("bignextround.gif");
        gtk_fixed_put(GTK_FIXED(fixed), nextRoundImage, 100, 50);
    }
    else if (round < 4) {
        nextRoundImage = loadImage("bignextround.gif");
        gtk_fixed_put(GTK_FIXED(fixed), nextRoundImage, 100, 100);
    }
    else {
        nextRoundImage = loadImage("smallnextround.gif");
        gtk_fixed_put(GTK_FIXED(fixed), nextRoundImage, 100, 50);
    }
    
    
    vbox = gtk_vbox_new(FALSE, 0);
    while (!loadList(random() % 100, str));
    
    if (clearedBoard) {
        gtk_widget_set_size_request(vbox, 300, 425);
        gtk_fixed_put(GTK_FIXED(fixed), vbox, 100, 50);
        label = gtk_label_new("Adding Bonus Score!");
        gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
        gtk_widget_modify_style(label, whiteStyle);
        clearedBoard = false;
    }
    else {
        gtk_widget_set_size_request(vbox, 300, 425);
        gtk_fixed_put(GTK_FIXED(fixed), vbox, 100, 100);
    }
    
    if (round < 4) {
        label = gtk_label_new(str.c_str());
        gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
        gtk_widget_modify_style(label, whiteStyle);
        for (int i = 0; i < 10; ++i) {
            label = gtk_label_new(words[i].c_str());
            gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 2);
            gtk_widget_modify_style(label, whiteStyle);
        }
    }
    else {
        gtk_widget_set_size_request(vbox, 300, 200);
    }
    nextRoundWidget = vbox;
    
    for (int x = 0; x < 12; ++x) {
        for (int y = 0; y < 12; ++y) {
            highlighted[x][y] = false;
        }
    }
    gtk_widget_show_all(nextRoundWidget);
    gtk_widget_show_all(nextRoundImage);

    pango_font_description_free(whiteStyle->font_desc);
}

void WordHunt::startRound() {
    char array[12][12];
    hunting = false;
    movement = MOVEMENT_NONE;
    direction = NONE;

    if (nextRoundWidget != NULL) {
        gtk_widget_destroy(nextRoundWidget);
        gtk_widget_destroy(nextRoundImage);
        nextRoundWidget = NULL;
    }
    
    for (int i = 0; i < 10; ++i) {
        //gtk_widget_modify_style(availableWords[i]->parent, bgBlack);
        gtk_widget_modify_fg(availableWords[i], (GtkStateType)0, &lightYellowColor);
        gtk_label_set_text((GtkLabel *)availableWords[i], words[i].c_str());
        wordFound[i] = false;
    }
    
    for (int y = 0; y < 12; ++y) {
        for (int x = 0; x < 12; ++x) {
            array[x][y] = ' ';
        }
    }
    fillArray(array, words, true);
    for (int y = 0; y < 12; ++y) {
        for (int x = 0; x < 12; ++x) {
            std::stringstream sstr;
//             sstr << "/usr/local/share/touchmegames/wordhunt/images/letter_" << array[x][y] << ".png";
            sstr << "letter_" << array[x][y] << ".png";
            g_object_set_data((GObject *)letters[x][y], KEY_STR, (void *)array[x][y]);
//             gtk_image_set_from_file((GtkImage *)letters[x][y], sstr.str().c_str());
            reloadImage(letters[x][y], sstr.str().c_str());
        }
    }
    
    
    timeLeft = 1.00;
}

void WordHunt::startGame() {
    timeval tv;
    struct timezone tz;
    std::string str;
    tz.tz_minuteswest = 0;
    tz.tz_dsttime = 0;
    nextRoundWidget = NULL;
    gettimeofday(&tv, &tz);
    srandom(tv.tv_usec);
    gameState = GAME_BETWEEN_ROUNDS;
    betweenRoundCountDown = 20;
}

// initializes the game's variables and returns the
// top level h/vbox
GtkWidget *WordHunt::init() {
    GtkWidget *tophbox;
    GtkWidget *vbox;
    GdkColor color;
    GtkWidget *image;
    std::string str;
    words = new std::string[10];
    
    totalScore = 0;
    clearedBoard = false;
    for (int i = 0; i < 10; ++i) {
            wordFound[i] = false;
    }
    
    totalScoreInt = 0;
    lightYellowColor.red = 0xf600;
    lightYellowColor.green = 0xff00;
    lightYellowColor.blue = 0x0000;
    
    darkYellowColor.red = 0x7600;
    darkYellowColor.green = 0x7f00;
    darkYellowColor.blue = 0x0;
            
    
    round = 0;
    usedLists[0] = -1;
    usedLists[1] = -1;
    usedLists[2] = -1;
    startGame();
    
    bgCircle = gtk_rc_style_new();
    
//     bgCircle->bg_pixmap_name[0] = "/usr/local/share/touchmegames/wordhunt/images/letterBg.xpm";
    str = INSTALL_DIR;
    str += "wordhunt/image/letterBg.xpm";
    bgCircle->bg_pixmap_name[0] = (char *)str.c_str();
    
    beginx = -1;
    beginy = -1;
    hunting = false;
    
    bgBlack = gtk_rc_style_new();
    color.red = 0x0;
    color.green = 0x0;
    color.blue = 0x0;
    bgBlack->bg[0] = color;
    bgBlack->color_flags[0] = GTK_RC_BG;
    
    bgDone = gtk_rc_style_new();
    color.red = 0x5000;
    color.green = 0x5000;
    color.blue = 0x5000;
    bgDone->bg[0] = color;
    bgDone->color_flags[0] = GTK_RC_BG;
    
    timeLeft = 1.00;
    
    topvbox = gtk_vbox_new(FALSE, 0);
    GAME_PTR::loadScores(scores);
    
    tophbox = gtk_hbox_new(FALSE, 5);
    
    fixed = gtk_fixed_new();
    
    gtk_widget_set_size_request(fixed, 800, 600);
    
    image = loadImage("bg_wordhunt.png");
    
    gtk_widget_set_size_request(image, 800, 600);
    gtk_fixed_put((GtkFixed *)fixed, image, 0, 0);


    gtk_box_pack_start((GtkBox *)topvbox, fixed, FALSE, FALSE, 2);

    
    gtk_fixed_put(GTK_FIXED(fixed), tophbox, 23, 43);
    
    // by putting the table into an hbox and vbox,
    // we'll get the 45x45 size
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, initLh(), FALSE, FALSE, 2);
    gtk_box_pack_start((GtkBox *)tophbox, vbox, FALSE, FALSE, 2);
    
    //gtk_box_pack_start((GtkBox *)tophbox, initRh(), TRUE, FALSE, 2);
    gtk_fixed_put((GtkFixed *)fixed, initRh(), 560, 140);

    
    showNextRound();
    timeLeft = 0;
    
    timer = g_timeout_add(200, timeout, this);
    gtk_widget_show_all(topvbox);
    gtk_widget_show(image);
    return topvbox;
};

