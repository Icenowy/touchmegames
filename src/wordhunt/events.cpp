
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

// events for WordHunt

#include <wordhunt.h>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

void WordHunt::exitGame(GtkWidget *widget, gpointer *ptr) {
    WordHunt *wordhunt(reinterpret_cast<WordHunt *>(*ptr));
    SCORE_STRUCT newScore;
    
    newScore.name = "";
    newScore.score = wordhunt->totalScoreInt;
    newScore.date = "";
    delete [] wordhunt->words;
    
    wordhunt->GAME_PTR::addScore(wordhunt->scores, newScore);
    
    wordhunt->GAME_PTR::saveScores(wordhunt->scores);
    
    g_source_remove(wordhunt->timer);
    gtk_widget_hide(wordhunt->topvbox);
}

void WordHunt::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    WordHunt *wordhunt(reinterpret_cast<WordHunt *>(ptr));
    if (wordhunt->quitDialog()) {
        delete [] wordhunt->words;
        g_source_remove(wordhunt->timer);
        gtk_widget_hide(wordhunt->topvbox);
    }

}

void WordHunt::findWord(std::string word) {
    std::string attempt;
    std::string revWord;
    bool done(false);
    int len(word.size());
    
    for (int i = word.size() - 1; i >= 0; --i) {
        revWord += word[i];
    }
    
    for (int x = 0; x < 12 && !done; ++x) {
        for (int y = 0; y < 12 && !done; ++y) {
            
            attempt.clear();
            // horizontal check
            for (int i = 0; i + x < 12 && i < len; ++i) {
                //attempt += gtk_label_get_text((GtkLabel *)letters[x + i][y]);
                attempt += toupper((int)g_object_get_data((GObject *)letters[x + i][y], KEY_STR));
            }
    
            if (word == attempt || attempt == revWord) {
    
                    for (int i = 0; i + x < 12 & i < len; ++i) {
                        std::string str;
                        
                        str = "letterhi_";
                        str += tolower(attempt[i]);
                        str += ".png";
                        reloadImage(letters[x + i][y], str.c_str());
                    }
                done = true;
                return;
            }
            
            attempt.clear();
            // vertical check
            for (int i = 0; i + y < 12 && i < len; ++i) {
                //attempt += gtk_label_get_text((GtkLabel *)letters[x][y + i]);
                attempt += toupper((int)g_object_get_data((GObject *)letters[x][y + i], KEY_STR));
            }
            
            
            if (word == attempt || attempt == revWord) {
                for (int i = 0; i + y < 12 & i < len; ++i) {
                    //gtk_widget_modify_style(letters[x][y + i]->parent, bgCircle);
                    std::string str;
                    str = "letterhi_";
                    str += tolower(attempt[i]);
                    str += ".png";
                    reloadImage(letters[x][y + i], str.c_str());
                }
                done = true;
                return;
            }
            
        }
    }
}

void WordHunt::findWords() {
    for (int i = 0; i < 10; ++i) {
        if (!wordFound[i]) {
            findWord(words[i]);
        }
    }
}

gboolean WordHunt::timeout(gpointer ptr) {
    WordHunt *wh(reinterpret_cast<WordHunt *>(ptr));
    gdouble percent;
    std::stringstream sstr;
    double tmp, minute, second;
    //std::cout << "Game State: " << wh->gameState << std::endl;
    if (wh->gameState == GAME_ROUND_CLEANUP) {
        if (wh->timeLeft != 0) {
            std::stringstream sstr;
            wh->timeLeft -= 0.0066;
            if (wh->timeLeft <= 0.0) {
                wh->timeLeft = 0;
                wh->betweenRoundCountDown = 20;
                if (wh->round < 4) {
                    wh->gameState = GAME_BETWEEN_ROUNDS;
                    g_source_remove(wh->timer);
                    wh->timer = g_timeout_add(200, wh->timeout, wh);
                }
                else {
                    wh->timer = g_timeout_add(200, wh->timeout, wh);
                    g_source_remove(wh->timer);
                    wh->gameState = GAME_END;
                }
            }
            else {
                std::string str;
                    
                    //score = atoi(gtk_label_get_text((GtkLabel *)wh->totalScore));
                    wh->totalScoreInt += (int)(wh->timeLeft * 600.0);
                    sstr << wh->totalScoreInt;
                    str = sstr.str();
                    wh->addCommas(str);
                    gtk_label_set_text((GtkLabel *)wh->totalScore, str.c_str());
            }
            sstr.str("");
            second = (wh->timeLeft * 100.0) / 5.0 * 3.0;
            minute = second / 60.0;
            
            sstr << (int)minute << ':' << std::setw(2) << std::setfill('0') << (int)second;
            
            wh->progress.percent((int)(wh->timeLeft * 100));
            wh->progress.text(sstr.str().c_str());
            
            //gtk_progress_bar_set_fraction((GtkProgressBar *)wh->timeBar, wh->timeLeft);
            //gtk_progress_bar_set_text((GtkProgressBar *)wh->timeBar, sstr.str().c_str());
        }
        return TRUE;
    }
    
    
    if (wh->gameState == GAME_SHOW_WORDS) {
        --wh->betweenRoundCountDown;
        
        if (!wh->betweenRoundCountDown) {
            wh->gameState = GAME_BETWEEN_ROUNDS;
            wh->showNextRound();
            g_source_remove(wh->timer);
            wh->timer = g_timeout_add(200, wh->timeout, wh);
            wh->betweenRoundCountDown = 20;
        }
        return TRUE;
    }
    
    if (wh->gameState != GAME_PLAY) {
        --wh->betweenRoundCountDown;
        if (!wh->betweenRoundCountDown) {
            if (wh->gameState == GAME_BETWEEN_ROUNDS) {
                wh->startRound();
                sstr << wh->round;
                gtk_label_set_text((GtkLabel *)wh->roundLabel, sstr.str().c_str());
                wh->gameState = GAME_PLAY;
            }
            else {
                wh->exitGame(NULL, (void **)&wh);
                return FALSE;
            }
        }
        return TRUE;
    }
    
    wh->timeLeft -= .00333;
    if (wh->timeLeft <= 0) {
        wh->timeLeft = 0;
        wh->findWords();
        wh->betweenRoundCountDown = 20;
        if (wh->round < 3) {
            wh->gameState = GAME_SHOW_WORDS;
        }
        else {
            wh->gameState = GAME_END;
        }
    }
    
    
    second = (wh->timeLeft * 100.0) / 5.0 * 3.0;
    minute = second / 60.0;
    
    sstr << (int)minute << ':' << std::setw(2) << std::setfill('0') << (int)second;
    wh->progress.percent((int)(wh->timeLeft * 100));
    wh->progress.text(sstr.str().c_str());
    
    //gtk_progress_bar_set_fraction((GtkProgressBar *)wh->timeBar, wh->timeLeft);
    //gtk_progress_bar_set_text((GtkProgressBar *)wh->timeBar, sstr.str().c_str());
    
    return TRUE;
}

int WordHunt::checkMatch() {
    int match;
    std::string str, revstr;

    switch (direction) {
        case HORIZONTAL:
            for (int i = beginx; i <= endx; ++i) {
                //str += gtk_label_get_text((GtkLabel *)letters[i][beginy]);
                str += toupper((int)g_object_get_data((GObject *)letters[i][beginy], KEY_STR));
            }
            break;
        case VERTICAL:
            for (int i = beginy; i <= endy; ++i) {
                //str += gtk_label_get_text((GtkLabel *)letters[beginx][i]);
                str += toupper((int)g_object_get_data((GObject *)letters[beginx][i], KEY_STR));
            }
            break;
        case NONE:
        break;
    }
    
    for (match = 0; match < 10 && words[match] != str; ++match);
    
    if (match < 10 && !wordFound[match]) {
        //int score;
        std::stringstream sstr;
        std::string tmpstr;
        bool roundOver(true);
        playWave("gotword.wav");
        
        //score = atoi(gtk_label_get_text((GtkLabel *)totalScore));
        totalScoreInt += (int)(timeLeft * 3000.0);
        sstr << totalScoreInt;
        str = sstr.str();
        addCommas(str);
        gtk_label_set_text((GtkLabel *)totalScore, str.c_str());
        switch (direction) {
            case HORIZONTAL:
                for (int i = beginx; i <= endx; ++i) {
                    // fix me
                    std::string str;
                    char ch((int)g_object_get_data((GObject *)letters[i][beginy], KEY_STR));
                    str = "letterlo_";
                    str += ch;
                    str += ".png";
                    reloadImage(letters[i][beginy], str.c_str());
                    highlighted[i][beginy] = true;
                }
                break;
                case VERTICAL:
                        for (int i = beginy; i <= endy; ++i) {
                            // fix me
                            // gtk_widget_modify_style(letters[beginx][i]->parent, bgDone);
                            std::string str;
                            char ch((int)g_object_get_data((GObject *)letters[beginx][i], KEY_STR));
                            str = "letterlo_";
                            str += ch;
                            str += ".png";
                            reloadImage(letters[beginx][i], str.c_str());
                            highlighted[beginx][i] = true;
                        }
                        break;
                case NONE:
                        break;
        }
        //gtk_widget_modify_style(availableWords[match]->parent, bgDone);
        gtk_widget_modify_fg(availableWords[match], (GtkStateType)0, &darkYellowColor);
        wordFound[match] = true;
        hunting = false;
        beginx = -1;
        beginy = -1;
        endx = -1;
        endy = -1;
        lastx = -1;
        lasty = -1;
        direction = NONE;
        movement = MOVEMENT_NONE;
        clearing = false;
        for (int i = 0; i < 10; ++i) {
            if (!wordFound[i]) {
                roundOver = false;
            }
        }
        if (roundOver) {
            clearedBoard = true;
            gameState = GAME_ROUND_CLEANUP;
            showNextRound();
            g_source_remove(timer);
            timer = g_timeout_add(50, timeout, this);
        }
    }
    else {
            revstr = "";
            for (int i = str.size() - 1; i >= 0; --i) {
                    revstr += str[i];
            }
            for (match = 0; match < 10 && words[match] != revstr; ++match);
            
            if (match < 10 && !wordFound[match]) {
                    
                    bool roundOver(true);
                    std::stringstream sstr;
                    std::string str;
                    playWave("gotword.wav");
                    
                    //score = atoi(gtk_label_get_text((GtkLabel *)totalScore));
                    totalScoreInt += (int)(timeLeft * 3000.0);
                    sstr << totalScoreInt;
                    str = sstr.str();
                    addCommas(str);
                    gtk_label_set_text((GtkLabel *)totalScore, str.c_str());
                    switch (direction) {
                        case HORIZONTAL:
                            for (int i = beginx; i <= endx; ++i) {
                                char ch((int)g_object_get_data((GObject *)letters[i][beginy], KEY_STR));
                                str = "letterlo_";
                                str += ch;
                                str += ".png";
                                reloadImage(letters[i][beginy], str.c_str());
                                highlighted[i][beginy] = true;
                            }
                            break;
                        case VERTICAL:
                            for (int i = beginy; i <= endy; ++i) {
                                char ch((int)g_object_get_data((GObject *)letters[beginx][i], KEY_STR));
                                str = "letterlo_";
                                str += ch;
                                str += ".png";
                                reloadImage(letters[beginx][i], str.c_str());
                                highlighted[beginx][i] = true;
                        }
                        break;
                    case NONE:
                        break;
                }
                gtk_widget_modify_fg(availableWords[match], (GtkStateType)0, &darkYellowColor);
                
                wordFound[match] = true;
                hunting = false;
                beginx = -1;
                beginy = -1;
                endx = -1;
                endy = -1;
                lastx = -1;
                lasty = -1;
                direction = NONE;
                movement = MOVEMENT_NONE;
                clearing = false;
                for (int i = 0; i < 10; ++i) {
                    if (!wordFound[i]) {
                        roundOver = false;
                    }
                }
                if (roundOver) {
                    clearedBoard = true;
                    gameState = GAME_ROUND_CLEANUP;
                    showNextRound();
                    g_source_remove(timer);
                    timer = g_timeout_add(50, timeout, this);
                }
        }
    }
}

void WordHunt::toggleBg(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    WordHunt *wh(reinterpret_cast<WordHunt *>(ptr));
    GtkRcStyle *style;
    char ch;
    GtkWidget *letter;
    std::string str;
    if (wh->gameState != GAME_PLAY) {
        return;
    }
    letter = gtk_bin_get_child((GtkBin *)widget);
    ch = (int)g_object_get_data((GObject *)letter, KEY_STR);

    for (int x = 0; x < numberOfRows; ++x) {
        for (int y = 0; y < numberOfRows; ++y) {
            if (wh->letters[x][y]->parent == widget) {
                if (!wh->hunting) {
                    wh->hunting = true;
                    wh->beginx = x;
                    wh->beginy = y;
                    wh->endx = -1;
                    wh->endy = -1;
                    wh->direction = NONE;
                    wh->movement = MOVEMENT_NONE;
                    wh->clearing = false;

                    
                    str = "letterhi_";
                    str += ch;
                    str += ".png";
                    wh->reloadImage(letter, str.c_str());
                    wh->highlighted[x][y];
                    return;
                }
                else {
                    wh->movement = MOVEMENT_NONE;
                    if (wh->direction == HORIZONTAL) {
                        if (wh->checkForHorizontalEnd(x, y)) {
                            return;
                        }
                    }
                    else if (wh->direction == VERTICAL) {
                        if (wh->checkForVerticalEnd(x, y)) {
                            return;
                        }
                    }
                    else if (wh->direction == NONE && wh->hunting) {
                        if (wh->highlighted[x][y]) {
                            str += "letterlo_";
                            str += ch;
                            str += ".png";
                            wh->reloadImage(letter, str.c_str());
                        }
                        else {
                            str = "letterhi_";
                            str += ch;
                            str += ".png";
                            wh->reloadImage(letter, str.c_str());
                        }
                    }
                    GdkEventMotion moveEvent;
                    moveEvent.x = event->x;
                    moveEvent.y = event->y; 
                    wh->mouseMove(widget, &moveEvent, ptr);
                }
                return;
            }
        }
    }
}

void WordHunt::mouseMove(GtkWidget *widget, GdkEventMotion *event, gpointer ptr) {
    WordHunt *wh(reinterpret_cast<WordHunt *>(ptr));
    std::string str;
    char ch;
    int X = (int)(event->x + widget->allocation.x);
    int Y = (int)(event->y + widget->allocation.y);
    GdkRectangle rect;
	
    if (wh->gameState != GAME_PLAY) {
        return;
    }
    
    rect.x = X;
    rect.y = Y;
    rect.width = 1;
    rect.height = 1;
    
    X = -1;
    Y = -1;
    for (int x = 0; x < numberOfRows; ++x) {
        for (int y = 0; y < numberOfRows; ++y) {
            if (gtk_widget_intersect(wh->letters[x][y]->parent, &rect, NULL)) {
                X = x;
                Y = y;
                x = numberOfRows;
                y = numberOfRows;
            }
        }
    }
    if (X == -1 || Y == -1) {
            return;
    }

    if (X != wh->ignorex || Y != wh->ignorey) {
        wh->ignorex = -1;
        wh->ignorey = -1;
    }
    else {
        return;
    }
    
    if (wh->direction == HORIZONTAL && wh->movement == MOVEMENT_NONE) {
        
        if (X < wh->lastx) {
            wh->movement  = MOVEMENT_LEFT;
        }
        else if (X == wh->lastx) {
            return;
        }
        else {
            wh->movement  = MOVEMENT_RIGHT;
        }
        wh->lastx = X;
    }
    else if (wh->direction == VERTICAL && wh->movement  == MOVEMENT_NONE) {
        
        if (Y < wh->lasty) {
            wh->movement  = MOVEMENT_UP;
        }
        else if (Y == wh->lasty) {
            return;
        }
        else {
            wh->movement  = MOVEMENT_DOWN;
        }
        wh->lasty = Y;
    }
    
    // first movement
    if (wh->endx == -1) {
     
        // Horizontal
        if (X != wh->beginx && Y == wh->beginy) {
            
            wh->direction = HORIZONTAL;
            wh->endx = wh->beginx;
            wh->endy = wh->beginy;
            wh->lastx = wh->endx;
            wh->lasty = wh->endy;
            wh->takeCareOfMove = false;
            ch = (int)g_object_get_data((GObject *)wh->letters[X][Y], KEY_STR);
            str = "letterhi_";
            str += ch;
            str += ".png";
            wh->reloadImage(wh->letters[X][Y], str.c_str());
            //return;
        }
        // Vertical
        else if (X == wh->beginx && Y != wh->beginy) {
            
            wh->direction = VERTICAL;
            wh->endx = wh->beginx;
            wh->endy = wh->beginy;
            wh->lastx = wh->endx;
            wh->lasty = wh->endy;
            wh->takeCareOfMove = false;
            ch = (int)g_object_get_data((GObject *)wh->letters[X][Y], KEY_STR);
            str = "letterhi_";
            str += ch;
            str += ".png";
            wh->reloadImage(wh->letters[X][Y], str.c_str());
            
        }
        else {
            return;
        }
    }
    
    // check they X and Y against the direction we're going
    switch(wh->direction) {
    case NONE:
        return;
    break;
    case HORIZONTAL:
        if (wh->checkHorizontalOk(X, Y)) {
            wh->moveHorizontal(X, Y);
        }
    break;
    case VERTICAL:
        if (wh->checkVerticalOk(X, Y)) {
            wh->moveVertical(X, Y);
        }
    break;
    }
    if (wh->direction == HORIZONTAL && wh->beginx == wh->endx) {
        wh->movement = MOVEMENT_NONE;
    }
    else if (wh->direction == VERTICAL && wh->beginy == wh->endy) {
        wh->movement = MOVEMENT_NONE;
    }
}

bool WordHunt::checkForHorizontalEnd(int x, int y) {
    std::string str;
    char ch;
    if (y != beginy) {
        return true;
    }
    if (x == beginx && x == endx) {
        if (highlighted[endx][y]) {
            ch = (int)g_object_get_data((GObject *)letters[endx][y], KEY_STR);
            str = "letterlo_";
            str += ch;
            str += ".png";
            reloadImage(letters[endx][y], str.c_str());
        }
        else {
            ch = (int)g_object_get_data((GObject *)letters[endx][y], KEY_STR);
            str = "letter_";
            str += ch;
            str += ".png";
            reloadImage(letters[endx][y], str.c_str());
        }
        hunting = false;
        return true;
    }
    else if (x == endx) {
        if (highlighted[endx][y]) {
            ch = (int)g_object_get_data((GObject *)letters[endx][y], KEY_STR);
            str = "letterlo_";
            str += ch;
            str += ".png";
            reloadImage(letters[endx][y], str.c_str());
        }
        else {
            ch = (int)g_object_get_data((GObject *)letters[endx][y], KEY_STR);
            str = "letter_";
            str += ch;
            str += ".png";
            reloadImage(letters[endx][y], str.c_str());
        }
        
        ignorex = x;
        ignorey = y;
        --endx;
        checkMatch();
        return true;
    }
    else if (x == beginx) {
        if (highlighted[beginx][y]) {
            ch = (int)g_object_get_data((GObject *)letters[beginx][y], KEY_STR);
            str = "letterlo_";
            str += ch;
            str += ".png";
            reloadImage(letters[beginx][y], str.c_str());
        }
        else {
            ch = (int)g_object_get_data((GObject *)letters[beginx][y], KEY_STR);
            str = "letter_";
            str += ch;
            str += ".png";
            reloadImage(letters[beginx][y], str.c_str());
        }
        
        ignorex = x;
        ignorey = y;
        ++beginx;
        checkMatch();
        return true;
    }
    return false;
}

bool WordHunt::checkHorizontalOk(int x, int y) {
    // make sure it's on the same line
    if (beginy != y) {
        return false;
    }
    // it's ok
    return true;
}

void WordHunt::moveHorizontal(int x, int y) {
    std::string str;
    char ch;
    // its further left than endx currently is
    
    if (x < beginx) {
        GtkRcStyle *style;
        // if one got skipped, make sure to get it
        
        while (beginx > x) {
            --beginx;

            
            ch = (int)g_object_get_data((GObject *)letters[beginx][y], KEY_STR);
            str = "letterhi_";
            str += ch;
            str += ".png";
            reloadImage(letters[beginx][y], str.c_str());
        }
    }
    // else its closer than the endx currently is
    else if (x > endx) {
        GtkRcStyle *style;
        // if one got skipped, make sure to get it
        while (endx < x) {
            ++endx;
            ch = (int)g_object_get_data((GObject *)letters[endx][y], KEY_STR);
            
            str = "letterhi_";
            str += ch;
            str += ".png";
            reloadImage(letters[endx][y], str.c_str());
        }
    }
    else if (x == endx - 1 && endx != beginx && movement == MOVEMENT_RIGHT) {
        if (highlighted[endx][y]) {
            ch = (int)g_object_get_data((GObject *)letters[endx][y], KEY_STR);
            str = "letterlo_";
            str += ch;
            str += ".png";
            reloadImage(letters[endx][y], str.c_str());
        }
        else {
            ch = (int)g_object_get_data((GObject *)letters[endx][y], KEY_STR);
            str = "letter_";
            str += ch;
            str += ".png";
            reloadImage(letters[endx][y], str.c_str());
        }
        // all are cleared
        if (endx == beginx) {
                hunting = false;
        }
        else {
                endx = x;
        }
    }
    else if (x == beginx + 1 && beginx != endx && movement == MOVEMENT_LEFT) {
        if (highlighted[beginx][y]) {
            ch = (int)g_object_get_data((GObject *)letters[beginx][y], KEY_STR);
            str = "letterlo_";
            str += ch;
            str += ".png";
            reloadImage(letters[beginx][y], str.c_str());
        }
        else {
            ch = (int)g_object_get_data((GObject *)letters[beginx][y], KEY_STR);
            str = "letter_";
            str += ch;
            str += ".png";
            reloadImage(letters[beginx][y], str.c_str());
        }
        beginx = x;
    }
    
    checkMatch();
}

bool WordHunt::checkForVerticalEnd(int x, int y) {
    std::string str;
    char ch;
    if (x != beginx) {
        return true;
    }
    if (y == beginy && y == endy) {
        if (highlighted[x][beginy]) {
            ch = (int)g_object_get_data((GObject *)letters[x][beginy], KEY_STR);
            str = "letterlo_";
            str += ch;
            str += ".png";
            reloadImage(letters[x][beginy], str.c_str());
        }
        else {
            ch = (int)g_object_get_data((GObject *)letters[x][beginy], KEY_STR);
            str = "letter_";
            str += ch;
            str += ".png";
            reloadImage(letters[x][beginy], str.c_str());
        }
        hunting = false;
        return true;
    }
    else if (y == endy) {
        if (highlighted[x][endy]) {
            ch = (int)g_object_get_data((GObject *)letters[x][endy], KEY_STR);
            str = "letter_lo";
            str += ch;
            str += ".png";
            reloadImage(letters[x][endy], str.c_str());
        }
        else {
            ch = (int)g_object_get_data((GObject *)letters[x][endy], KEY_STR);
            str = "letter_";
            str += ch;
            str += ".png";
            reloadImage(letters[x][endy], str.c_str());
        }
        
        ignorex = x;
        ignorey = y;
        --endy;
        checkMatch();
        return true;
    }
    else if (y == beginy) {
        if (highlighted[x][beginy]) {
            ch = (int)g_object_get_data((GObject *)letters[x][beginy], KEY_STR);
            str = "letter_lo";
            str += ch;
            str += ".png";
            reloadImage(letters[x][beginy], str.c_str());
        }
        else {
            ch = (int)g_object_get_data((GObject *)letters[x][beginy], KEY_STR);
            str = "letter_";
            str += ch;
            str += ".png";
            reloadImage(letters[x][beginy], str.c_str());
        }
        ignorex = x;
        ignorey = y;
        ++beginy;
        checkMatch();
        return true;
    }
    return false;
}

bool WordHunt::checkVerticalOk(int x, int y) {
	// make sure it's on the same line
	if (beginx != x) {
		return false;
	}
	// it's ok
	return true;
}

void WordHunt::moveVertical(int x, int y) {
    std::string str;
    char ch;
    // its further up than endy currently is
    if (y < beginy) {
        GtkRcStyle *style;
        // if one got skipped, make sure to get it
        
        while (beginy > y) {
            --beginy;
            ch = (int)g_object_get_data((GObject *)letters[x][beginy], KEY_STR);
            str = "letterhi_";
            str += ch;
            str += ".png";
            reloadImage(letters[x][beginy], str.c_str());
            
            }
    }
    // else its closer than the endy currently is
    else if (y > endy) {
        GtkRcStyle *style;
        // if one got skipped, make sure to get it
        while (endy < y) {
            ++endy;

            ch = (int)g_object_get_data((GObject *)letters[x][endy], KEY_STR);
            str = "letterhi_";
            str += ch;
            str += ".png";
            reloadImage(letters[x][endy], str.c_str());
            
            }
    }
    else if (y == endy - 1 && endy != beginy && movement == MOVEMENT_DOWN) {
        if (highlighted[x][endy]) {
      
            ch = (int)g_object_get_data((GObject *)letters[x][endy], KEY_STR);
            str = "letterlo_";
            str += ch;
            str += ".png";
            reloadImage(letters[x][endy], str.c_str());
        }
        else {
            ch = (int)g_object_get_data((GObject *)letters[x][endy], KEY_STR);
            str = "letter_";
            str += ch;
            str += ".png";
            reloadImage(letters[x][endy], str.c_str());
        }
            // all are cleared
        if (endy == beginy) {
            hunting = false;
        }
        else {
            endy = y;
        }
    }
    else if (y == beginy + 1 && beginy != endy && movement == MOVEMENT_UP) {
        if (highlighted[x][beginy]) {
            ch = (int)g_object_get_data((GObject *)letters[x][beginy], KEY_STR);
            str = "letterlo_";
            str += ch;
            str += ".png";
            reloadImage(letters[x][beginy], str.c_str());
        }
        else {
            ch = (int)g_object_get_data((GObject *)letters[x][beginy], KEY_STR);
            str = "letter_";
            str += ch;
            str += ".png";
            reloadImage(letters[x][beginy], str.c_str());
        }
        beginy = y;
    }
    checkMatch();
}
