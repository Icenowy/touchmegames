
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

// support functions for SecretPhrase

#include <secretphrase.h>
#include <algorithm>
#include <sys/time.h>
#include <sstream>
#include <xmlparser.h>

SecretPhrase::SecretPhrase() {
    color.red = 0x0000;
    color.green = 0x0000;
    color.blue = 0xA000;
    currentScore = 0;
    round = 1;
    
}

std::string SecretPhrase::name() const {
    return "secretphrase";
}

std::string SecretPhrase::imageName() const {
    return "secretphrase.gif";
}

GdkColor *SecretPhrase::backgroundColor() {
    return &color;
}

int SecretPhrase::gameType() const {
    return WORD_GAME;
}

GtkWidget *SecretPhrase::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "Secretphrase is a game where you have a blank\n"
            "phrase on the screen and you have to guess what\n"
            "letters are in that phrase.  At any time you can\n"
            "select the \"Solve Now\" and solve the phrase.\n"
            "You are given 4 incorrect guesses and for each\n"
            "incorrect guess left at the end of the round,\n"
            "you receive 5,000 extra points.\n\n"
            "The categories are Misc., Sports, Music, and\n"
            "Video.";
    
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}



void SecretPhrase::initRound() {
    std::string str;
    std::stringstream sstr2;
    
    gameState = SPINNING;
    solve = false;
    for (int x = 0; x < 14; ++x) {
        for (int y = 0; y < 4; ++y) {
//             gtk_image_set_from_file((GtkImage *)centerLetters[y][x], 
//                             "/usr/local/share/touchmegames/secretphrase/images/back.gif");
            reloadImage(centerLetters[y][x], "back.gif");
            letters[y][x] = ' ';
        }
    }
    fitPhrase(getPhrase());
    fillPhraseBacks();
    
    vowelsAllowed = false;
    
//     gtk_image_set_from_file((GtkImage *)guessesLeftImg, "/usr/local/share/touchmegames/secretphrase/images/3.gif");
    reloadImage(guessesLeftImg, "3.gif");
    guessesLeft = 3;
    
    for (int i = 0; i < 26; ++i) {
        used[i] = false;
    }
    
    str = (char )(++round + '0');
    gtk_label_set_text((GtkLabel *)roundLabel, str.c_str());
    
    gtk_label_set_text((GtkLabel *)infoLabel, 
                        "Select a letter, no vowels.");
    
    for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < 13; ++x) {
            std::stringstream sstr;
//             sstr << "/usr/local/share/touchmegames/secretphrase/images/" << (char)(x + (y * 13) + 'a') << ".gif";
//             gtk_image_set_from_file((GtkImage *)lowerLetters[y][x], sstr.str().c_str());
            sstr << (char)(x + (y * 13) + 'a') << ".gif";
            reloadImage(lowerLetters[y][x], sstr.str().c_str());
        }
    }
    
    
    sstr2 << "Round " << round + 1;
    gtk_label_set_text((GtkLabel *)initRoundLabel, sstr2.str().c_str());
}


void SecretPhrase::startRound() {
    
    
}

void SecretPhrase::initSpin() {
    
    gameState = SPINNING;
    g_source_remove(timer);
    timer = g_timeout_add(20, timeout, this);
    wheelSpin = 26;
    wheelCntr = 16;
    totalSpin = 0;
    
}

void SecretPhrase::spinWheel() {
    if ((wheelBarX1) <= -2210) {
        wheelBarX1 = wheelBarX2 + 2210 - wheelSpin;
    }
    else {
        wheelBarX1 -= wheelSpin;
        if (wheelSpin <= 4 && ((wheelBarX1 + 65) % 130 < 5)) {
            wheelSpin = 0;
            wheelCntr = 1;
        }
    }
    if ((wheelBarX2) <= -2210) {
        wheelBarX2 = wheelBarX1 + 2210;
    }
    else {
        wheelBarX2 -= wheelSpin;
        if (wheelSpin <= 4 && ((wheelBarX2 + 65) % 130 < 5)) {
            wheelSpin = 0;
            wheelCntr = 1;
        }
    }
    
    totalSpin += wheelSpin;
    gtk_fixed_move((GtkFixed *)fixed, wheelImage1, wheelBarX1, 0);
    gtk_fixed_move((GtkFixed *)fixed, wheelImage2, wheelBarX2, 0);
    
    
    
    --wheelCntr;
    if (!wheelCntr) {
        
        if (wheelSpin >= 4) {
                wheelSpin -= 2;
        }
        else { // we're done.  what number do we have?
            unsigned short values[17] = {
                    5000, 6875,  4250, 7625, 2000, 6125, 3500, 5750, 
                    2750, 4625, 8000, 2375, 3875, 6500, 3125, 7250, 
                    5375 };
            int x(wheelBarX1 <= 0 ? wheelBarX1: wheelBarX2);
            x = -x;
            
            currentValue = values[((x / 130) + 1) % 17];
            if (wheelBarX1 <= 0) {
                gtk_fixed_move((GtkFixed *)fixed, wheelImage1, (-x / 130) * 130 - 65, 0);
                gtk_fixed_move((GtkFixed *)fixed, wheelImage2, (-x / 130) * 130 + 2210 - 65, 0);
            }
            else {
                gtk_fixed_move((GtkFixed *)fixed, wheelImage2, (-x / 130) * 130 - 65, 0);
                gtk_fixed_move((GtkFixed *)fixed, wheelImage1, (-x / 130) * 130 + 2210 - 65, 0);
            }
            
            progressValue = 400;
            progress.percent(100);
            progress.show();
            gameState = RUNNING;

            g_source_remove(timer);
            timer = g_timeout_add(50, timeout, this);
        }
        wheelCntr = 15 + (rand() % 6);
    }
}

std::string SecretPhrase::getPhrase() {
    std::string descriptor, phrase, str;
    XmlParser parser;
    
    str = INSTALL_DIR;
    str += "secretphrase/phrases/";
    str += category;
    str += ".xml";
    
    
    
    if (!parser.openXmlFile(str, rootname, str)) {
        std::cout << str << std::endl;
    }
    
    for (int i = 0; i < 3; ++i) {
        int n(-1);
        while (n == nums[0] || n == nums[1] || n == nums[2]) {
            n = random() % parser.getNumberOfEntries();
            
            if (!parser.getNthPhrase(n, descriptor, phrase, str)) {
                n = nums[0];
            }
                
        }
        nums[i] = n;
    }
    gtk_label_set_text((GtkLabel *)categoryWidget, descriptor.c_str());
    
    parser.closeXmlFile();
    return phrase;
}

void SecretPhrase::fitPhrase(std::string phrase) {
    int ymin(0), ymax(4), x(0), y(0);
    unsigned int lh(0), rh(0);
    bool done(false);
    std::transform(phrase.begin(), phrase.end(), phrase.begin(), tolower);
    
    if (phrase.size() <= 28) {
        // keep to two lines
        ymin = 1;
        ymax = 2;
    }
    y = ymin;
    
    while (!done) {
        rh = phrase.find(' ', lh);
        if (rh > phrase.size()) {
            rh = phrase.size();
            done = true;
        }
        // make sure it fits on the current line
        if (x + (rh - lh) + 1 > 14) {
            ++y;
            x = 0;
        }
        
        strcpy(&letters[y][x], phrase.substr(lh, rh - lh).c_str());
        x += rh - lh;
        letters[y][x++] = ' ';
        lh = rh + 1;
        if (lh >= phrase.size()) {
            done = true;
        }
    }
    
    
}

void SecretPhrase::fillPhraseBacks() {
    for (int x = 0; x < 14; ++x) {
        for (int y = 0; y < 4; ++y) {
                
            if (letters[y][x] == '\'') {
                
//                 gtk_image_set_from_file((GtkImage *)centerLetters[y][x],
//                                 "/usr/local/share/touchmegames/secretphrase/images/apost.gif");
                reloadImage(centerLetters[y][x], "apost.gif");
                letters[y][x] = ' ';
            }
            else if (letters[y][x] == '-') {
//                 gtk_image_set_from_file((GtkImage *)centerLetters[y][x],
//                                 "/usr/local/share/touchmegames/secretphrase/images/dash.gif");
                reloadImage(centerLetters[y][x], "dash.gif");
                letters[y][x] = ' ';
            }
            else if (letters[y][x] == ',') {
//                 gtk_image_set_from_file((GtkImage *)centerLetters[y][x],
//                                 "/usr/local/share/touchmegames/secretphrase/images/comma.gif");
                reloadImage(centerLetters[y][x], "comma.gif");
                letters[y][x] = ' ';
            }
            else if (letters[y][x] != ' ') {
//                 gtk_image_set_from_file((GtkImage *)centerLetters[y][x],
//                                 "/usr/local/share/touchmegames/secretphrase/images/back.gif");
                reloadImage(centerLetters[y][x], "back.gif");
            }
            else {
//                 gtk_image_set_from_file((GtkImage *)centerLetters[y][x],
//                                 "/usr/local/share/touchmegames/secretphrase/images/blank.gif");
                reloadImage(centerLetters[y][x], "blank.gif");
            }
        }
    }
}

int SecretPhrase::getNumberOfOccurences(char ch) {
    int cnt(0);
    for (int i = 0; i < 4; ++i) {
        for (int l = 0; l < 14; ++l) {
            if (letters[i][l] == ch) {
                ++cnt;
            }
        }
    }
    return cnt;	
}

// returns true if found a key and false if not
bool SecretPhrase::handleKey(char ch) {
    int lbeg(handlePosL);
    
    for (int i = handlePosI; i < 4; ++i) {
        for (int l = lbeg; l < 14; ++l) {
            
            if (letters[i][l] == ch) {
                std::string str;
//                 str = "/usr/local/share/touchmegames/secretphrase/images/";
                str = INSTALL_DIR;
                str += "secretphrase/images/";
                str += ch;
                str += ".gif";
                
                handlePosI = i;
                handlePosL = l;
                if (++handlePosL == 14) {
                    handlePosL = 0;
                    ++handlePosI;
                }
                gtk_image_set_from_file((GtkImage *)centerLetters[i][l],
                                str.c_str());
                letters[i][l] = ' ';
                return true;
            }
        }
        lbeg = 0;
        
    }
    
    return false;
}

bool SecretPhrase::checkFinished() {
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 14; ++x) {
            if (letters[y][x] != ' ') {
                return false;
            }
        }
    }
    return true;
}

bool SecretPhrase::showLetter() {
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 14; ++x) {
            if (letters[y][x] != ' ') {
                std::string str;
//                 str = "/usr/local/share/touchmegames/secretphrase/images/";
                str = INSTALL_DIR;
                str += "secretphrase/images/";
                str += letters[y][x];
                str += ".gif";
                
                
                gtk_image_set_from_file((GtkImage *)centerLetters[y][x],
                                str.c_str());
                letters[y][x] = ' ';
                return true;
            }
        }
    }
    return false;
}
