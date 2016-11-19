
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

// support functions for FoxyBoxy

#include <foxyboxy.h>
#include <algorithm>
#include <sys/time.h>
#include <sstream>
#include <iostream>
#include <iomanip>

FoxyBoxy::FoxyBoxy() {
    color.red = 0xA000;
    color.green = 0xA000;
    color.blue = 0xA000;
    maxx = 10;
    maxy = 22;
    timeoutValue = 120;
    spedUpTimer = false;
    currentScore = 0;
    level = 1;
    lines = 0;
    moveDownTurn = false;
}

std::string FoxyBoxy::name() const {
    return "foxyboxy";
}

std::string FoxyBoxy::imageName() const {
    return "foxyboxy.png";
}

GdkColor *FoxyBoxy::backgroundColor() {
    return &color;
}

int FoxyBoxy::gameType() const {
    return UNKNOWN_GAME;
}

GtkWidget *FoxyBoxy::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "foxyboxy is a tetris game.  This one gives\n"
            << "points based on the number of lines removed at a\n"
            << "time and the current level.  You get 10 points for\n"
            << "one line, 20 points for 2 lines, 40 for 3 and 80\n"
            << "for 4.  This gets multiplied by the current level.\n";
    
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int FoxyBoxy::coordsOk(Coord c[4]) {
    for (int i = 0; i < 4; ++i) {
        if (c[i].y == maxy) {
            return HitBottom;
        }
        if (c[i].x == maxx || c[i].x < 0) {
            
            return HitSide;
        }
        if ((intptr_t)g_object_get_data((GObject *)blocks[c[i].x][c[i].y], KEY_STR)) {
            
            return HitBlock;
        }
    }
    return 1;
}

bool FoxyBoxy::canFitBlock() {
    if ((intptr_t)g_object_get_data((GObject *)blocks[4][1], KEY_STR)) {
        return false;
    }
    if ((intptr_t)g_object_get_data((GObject *)blocks[5][1], KEY_STR)) {
        return false;
    }
    if ((intptr_t)g_object_get_data((GObject *)blocks[6][1], KEY_STR)) {
        return false;
    }
    return true;
}

void FoxyBoxy::moveBlockDown() {
    Coord newCoords[4];
    memcpy(newCoords, coords, sizeof(newCoords));
    
    for (int i = 0; i < 4; ++i) {
        ++newCoords[i].y;
    }
    if (coordsOk(newCoords) > 0) {
        std::stringstream imgStr;
        imgStr << "type" << currentBox << ".png";

        
        // first clear the old location
        showBlocks("blank.gif");
        
        // next, copy the new coords into coords
        memcpy(coords, newCoords, sizeof(newCoords));
        // finally, show the new location
        showBlocks(imgStr.str());
    }
    else {
        extraMove = MoveNone;
        g_source_remove(timer);
        timer = g_timeout_add(timeoutValue, timeout, this);
        // ok here the block has hit either the bottom or another
        // block, so mark these blocks and create a new one.
        for (int i = 0; i < 4; ++i) {
            g_object_set_data((GObject *)blocks[coords[i].x][coords[i].y], KEY_STR, (void *)currentBox);
        }
        if (canFitBlock()) {
            checkLines();
            if (gameState == RUNNING) {
                createAndShowBlocks();
            }
        }
        else {
            
            gameState = GAME_OVER;
            
        }
    }
    updateBottom();
}

bool FoxyBoxy::isLeft(int x, int y) {
    if (x < 0 || x > 90) {
        return false;
    }
    if (y < 45 || y > 120) {
        return false;
    }
    return true;
}
	
bool FoxyBoxy::isRight(int x, int y) {
    if (x < 158 || x > 250) {
        return false;
    }
    if (y < 45 || y > 120) {
        return false;
    }
    return true;
}

bool FoxyBoxy::isRotate(int x, int y) {
    if (x < 85 || x > 170) {
        return false;
    }
    if (y < 0 || y > 70) {
        return false;
    }
    return true;
}

bool FoxyBoxy::isDown(int x, int y) {
    if (x < 90 || x > 160) {
        return false;
    }
    if (y < 70 || y > 166) {
        return false;
    }
    return true;
}

void FoxyBoxy::doAction(int x, int y) {
    if (isLeft(x, y)) {
        moveLeft(false);
        reloadImage(paddleImage, "paddleleft.png");
        
    }
    else if (isRight(x, y)) {
        moveRight(false);
        reloadImage(paddleImage, "paddleright.png");
    }
    else if (isRotate(x, y)) {
        rotateRight();
        reloadImage(paddleImage, "paddlerotate.png");
    }
    else if (isDown(x, y)) {
        moveDown();
        reloadImage(paddleImage, "paddledown.png");
    }
}

void FoxyBoxy::moveLeft(bool fromEvents) {
    Coord newCoords[4];
    
    memcpy(newCoords, coords, sizeof(newCoords));
    for (int i = 0; i < 4; ++i) {
        --newCoords[i].x;
    }
    if (coordsOk(newCoords) > 0) {
        std::stringstream imgStr;
        imgStr << "type" << currentBox << ".png";

        
        // first clear the old location
        showBlocks("blank.gif");
        
        // next, copy the new coords into coords
        memcpy(coords, newCoords, sizeof(newCoords));
        // finally, show the new location
        showBlocks(imgStr.str());
    }
    if (!fromEvents) {
        if (!(moveDownTurn & 1)) {
            justDidExtraMove = true;
        }
        extra = 1;
        extraMove = MoveLeft;
    }
    updateBottom();
}

void FoxyBoxy::moveRight(bool fromEvents) {
    Coord newCoords[4];
    memcpy(newCoords, coords, sizeof(newCoords));
    for (int i = 0; i < 4; ++i) {
        ++newCoords[i].x;
    }
    if (coordsOk(newCoords) > 0) {
        std::stringstream imgStr;
        imgStr << "type" << currentBox << ".png";

        
        // first clear the old location
        showBlocks("blank.gif");
        
        // next, copy the new coords into coords
        memcpy(coords, newCoords, sizeof(newCoords));
        // finally, show the new location
        showBlocks(imgStr.str());
    }
    if (!fromEvents) {
        if (!(moveDownTurn & 1)) {
            justDidExtraMove = true;
        }
        extra = 1;
        extraMove = MoveRight;
    }
    updateBottom();
}

void FoxyBoxy::moveDown() {
    g_source_remove(timer);
    timer = g_timeout_add(30, timeout, this);
    spedUpTimer = true;
    moveBlockDown();
    extraMove = MoveDown;
}

void FoxyBoxy::rotateRight() {
    Coord newCoords[4];
    memcpy(newCoords, coords, sizeof(newCoords));
    switch (currentBox) {
        case Type1:
            if (newCoords[0].y == newCoords[1].y && newCoords[1].y < newCoords[2].y) {
                // "7" Configuration
                newCoords[0].x += 2;
                newCoords[1].x += 1;
                newCoords[1].y += 1;
                newCoords[3].x -= 1;
                newCoords[3].y -= 1;
            }
            else if (newCoords[0].x == newCoords[1].x && newCoords[1].x > newCoords[2].x) {
                newCoords[0].y += 2;
                newCoords[1].x -= 1;
                newCoords[1].y += 1;
                newCoords[3].x += 1;
                newCoords[3].y -= 1;
            }
            else if (newCoords[0].y == newCoords[1].y && newCoords[1].y > newCoords[2].y) {
                // "L" Configuration
                newCoords[0].x -= 2;
                newCoords[1].x -= 1;
                newCoords[1].y -= 1;
                newCoords[3].x += 1;
                newCoords[3].y += 1;
            }
            else if (newCoords[0].x == newCoords[1].x && newCoords[1].x < newCoords[2].x) {
                newCoords[0].y -= 2;
                newCoords[1].x += 1;
                newCoords[1].y -= 1;
                newCoords[3].x -= 1;
                newCoords[3].y += 1;
            }
            break;
        case Type2:
            if (newCoords[0].y == newCoords[1].y && newCoords[1].y < newCoords[2].y) {
                // Backwards "7" Configuration
                newCoords[0].y += 2;
                newCoords[1].x += 1;
                newCoords[1].y += 1;
                newCoords[3].x -= 1;
                newCoords[3].y -= 1;
            }
            else if (newCoords[0].x == newCoords[1].x && newCoords[1].x > newCoords[2].x) {
                newCoords[0].x -= 2;
                newCoords[1].x -= 1;
                newCoords[1].y += 1;
                newCoords[3].x += 1;
                newCoords[3].y -= 1;
            }
            else if (newCoords[0].y == newCoords[1].y && newCoords[1].y > newCoords[2].y) {
                // Backwards "L" Configuration
                newCoords[0].y -= 2;
                newCoords[1].x -= 1;
                newCoords[1].y -= 1;
                newCoords[3].x += 1;
                newCoords[3].y += 1;
            }
            else if (newCoords[0].x == newCoords[1].x && newCoords[1].x < newCoords[2].x) {
                newCoords[0].x += 2;
                newCoords[1].x += 1;
                newCoords[1].y -= 1;
                newCoords[3].x -= 1;
                newCoords[3].y += 1;
            }
            
            break;
        case Type3:
            if (newCoords[0].y == newCoords[1].y && newCoords[1].y < newCoords[2].y) {
                // "z" Configuration
                newCoords[0].x += 2;
                newCoords[1].x += 1;
                newCoords[1].y += 1;
                newCoords[3].x -= 1;
                newCoords[3].y += 1;
            }
            else if (newCoords[0].x == newCoords[1].x && newCoords[1].x > newCoords[2].x) {
                newCoords[0].y += 2;
                newCoords[1].x -= 1;
                newCoords[1].y += 1;
                newCoords[3].x -= 1;
                newCoords[3].y -= 1;
                std::swap(newCoords[0], newCoords[3]);
                std::swap(newCoords[1], newCoords[2]);
            }
            break;
        case Type4:
            if (newCoords[0].y == newCoords[1].y && newCoords[1].y > newCoords[2].y) {
                // Backwards "z" Configuration
                newCoords[0].y -= 1;
                newCoords[1].x -= 1;
                newCoords[2].y += 1;
                newCoords[3].x -= 1;
                newCoords[3].y += 2;
            }
            else if (newCoords[0].x == newCoords[1].x && newCoords[1].x < newCoords[2].x) {
                newCoords[0].y += 1;
                newCoords[1].x += 1;
                newCoords[2].y -= 1;
                newCoords[3].x += 1;
                newCoords[3].y -= 2;
            }
            
            break;
        case Type5:
                
            break;
        case Type6:
            
            if (newCoords[0].y == newCoords[1].y) {
                newCoords[0].x += 1;
                newCoords[1].y += 1;
                newCoords[2].x -= 1;
                newCoords[2].y += 2;
                newCoords[3].x -= 2;
                newCoords[3].y += 3;
            }
            else {
                newCoords[0].x -= 1;
                newCoords[1].y -= 1;
                newCoords[2].x += 1;
                newCoords[2].y -= 2;
                newCoords[3].x += 2;
                newCoords[3].y -= 3;
            }
            break;
        case Type7:
            if (newCoords[0].y == newCoords[1].y && newCoords[1].y > newCoords[3].y) {
                // __|__
                newCoords[0].x += 1;
                newCoords[0].y -= 1;
                newCoords[2].x -= 1;
                newCoords[2].y += 1;
                newCoords[3].x += 1;
                newCoords[3].y += 1;
            }
            else if (newCoords[0].x == newCoords[1].x && newCoords[1].x < newCoords[3].x) {
                newCoords[0].x += 1;
                newCoords[0].y += 1;
                newCoords[2].x -= 1;
                newCoords[2].y -= 1;
                newCoords[3].x -= 1;
                newCoords[3].y += 1;
            }
            else if (newCoords[0].y == newCoords[1].y && newCoords[1].y < newCoords[3].y) {
                newCoords[0].x -= 1;
                newCoords[0].y += 1;
                newCoords[2].x += 1;
                newCoords[2].y -= 1;
                newCoords[3].x -= 1;
                newCoords[3].y -= 1;
            }
            else if (newCoords[0].x == newCoords[1].x && newCoords[1].x > newCoords[3].x) {
                newCoords[0].x -= 1;
                newCoords[0].y -= 1;
                newCoords[2].x += 1;
                newCoords[2].y += 1;
                newCoords[3].x += 1;
                newCoords[3].y -= 1;
            }
            
            break;
    }
    if (coordsOk(newCoords) > 0) {
        std::stringstream imgStr;
        imgStr << "type" << currentBox << ".png";

        
        // first clear the old location
        showBlocks("blank.gif");
        
        // next, copy the new coords into coords
        memcpy(coords, newCoords, sizeof(newCoords));
        // finally, show the new location
        showBlocks(imgStr.str());
        updateBottom();
    }
}

void FoxyBoxy::checkLines() {
    int lineCntr(0);
    for (int y = 0; y < 22; ++y) {
        int cntr(0);
        for (int x = 0; x < 10 && lineCntr < 4; ++x) {
            if ((intptr_t)g_object_get_data((GObject *)blocks[x][y], KEY_STR)) {
                if (++cntr == 10) {
                    
                    linesToFlash[lineCntr++] = y;
                    gameState = FLASH_LINES;
                }
            }
        }
    }
    if (gameState == FLASH_LINES) {
        
        while (lineCntr < 4) {
            linesToFlash[lineCntr++] = -1;
        }
        g_source_remove(timer);
        timer = g_timeout_add(100, timeout, this);
        flashCntr = 5;
    }
}

void FoxyBoxy::flashLines() {
    if ((flashCntr & 1) == 0) {
        int line(0);
        while (linesToFlash[line] != -1 && line < 4) {
            for (int i = 0; i < 10; ++i) {
                reloadImage(blocks[i][linesToFlash[line]], "blank.gif");
            }
            ++line;
        }
    }
    else {
        int line(0);
        while (linesToFlash[line] != -1 && line < 4) {
            
            for (int i = 0; i < 10; ++i) {
                    int type((intptr_t)g_object_get_data((GObject *)blocks[i][linesToFlash[line]], KEY_STR));
                    std::stringstream sstr;
                    sstr << "type" << type << ".png";
                    reloadImage(blocks[i][linesToFlash[line]], sstr.str().c_str());
            }
            ++line;
        }
    }
}

void FoxyBoxy::updateScore() {
    int removedLines(0);
    int tmpscore(0);
    std::stringstream sstr;
    for (; linesToFlash[removedLines] != -1 && removedLines < 4; ++removedLines);
    switch (removedLines) {
        case 1: tmpscore = 10; break;
        case 2: tmpscore = 20; break;
        case 3: tmpscore = 40; break;
        case 4: tmpscore = 80; break;
    }
    currentScore += tmpscore * level;
    lines += removedLines;
    if (lines >= level * 10) {
        ++level;
        if (timeoutValue > 30) {
            timeoutValue -= 10;
            g_source_remove(timer);
            
            timer = g_timeout_add(timeoutValue, timeout, this);
        }
        sstr << level;
        gtk_label_set_text((GtkLabel *)levelLabel, sstr.str().c_str());
        sstr.str("");
    }
    sstr << addCommas(currentScore);
    gtk_label_set_text((GtkLabel *)scoreLabel, sstr.str().c_str());
    sstr.str("");
    
    sstr << addCommas(lines);
    gtk_label_set_text((GtkLabel *)linesLabel, sstr.str().c_str());
    sstr.str("");
}
