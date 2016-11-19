
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
 *	Foundation, Inc., 59 Temple Place, Suite 328, Boston, MA  02111-1287  USA
 */

// support functions for NonoGram
// Functions which basically don't fit in
// events.cpp or interface.cpp

#include <nonogram.h>
#include <sstream>

NonoGram::NonoGram() {
}

std::string NonoGram::name() const {
    return "nonogram";
}

std::string NonoGram::imageName() const {
    return "nonogram.png";
}

// This function doesn't even do anything anymore...really
// The background should be a modified bg.png
GdkColor *NonoGram::backgroundColor() {
    return &bgcolor;
}

void NonoGram::startRound() {
    std::stringstream sstr;
    bool good(false);
    
//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    ++nonoSize;
    mouseState = BLANK;
    
//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    if (round + 1 == 4) {
        gameState = GAME_OVER;
        return;
    }
    ++round;
    sstr << round;
    gtk_label_set_text((GtkLabel *)roundLabel, sstr.str().c_str());
    
//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    clearNonoGram();
    
//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    while (!good) {
        
        while (createNonoGram() != 0) {
            clearNonoGram();
        }
        
        good = solve();
    }

//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    createGrid();
    
    roundScore = 0;
    
//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    gameState = RUNNING;
    timeLeft = 600;
    
//     std::cout << "NonoGram::init() " << __LINE__ << std::endl;
//     std::cout.flush();
}


GtkWidget *NonoGram::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    // Keep this color
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);

    sstr << "NonoGram is a puzzle which cells in a grid have to be\n"
            << "colored or left blank according to numbers on the top\n"
            << "and left.\n"
            << "For example,  a clue of 1 2 2 would mean there\n"
            << "are sets of one, two and two filled squares, in that\n"
            << "order, which must have at least one blank square\n"
            << "between each group.\n"
            << "A single click will fill the square and a second\n"
            << "click will X out the square.";
        
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int NonoGram::gameType() const {
    return STRATEGY_GAME;
}



char NonoGram::roundOver() {
    
    gtk_widget_destroy(nonoTable);
    
    if (round == 3) {
        NonoGram *s(this);
        exitGame(NULL, (void **)&s);
        return 0;
    }
    
    progress.percent(100);
    progress.text("2:00");
    gtk_label_set_text((GtkLabel *)puzzleScoreLabel, "0");
    gtk_label_set_text((GtkLabel *)timeLeftScoreLabel, "0");
    gtk_label_set_text((GtkLabel *)roundScoreLabel, "0");
    gtk_label_set_text((GtkLabel *)totalScoreLabel, "0");
    
    return 0;
}

void NonoGram::clearNonoGram() {
    for (int j = 0; j < 10; ++j) {
        for (int i = 0; i < 6; ++i) {
            leftCnt[j][i] = 0;
            topCnt[j][i] = 0;
        }
    }
    for (int x = 0; x < nonoSize; ++x) {
        for (int y = 0; y < nonoSize; ++y) {
            nonoGram[x][y] = 0;
        }
    }
}

int NonoGram::createNonoGram() {
    int numAdd = (nonoSize * nonoSize) / 2 + ((nonoSize * nonoSize) % 2);
    int baseIdx = 0;
    int cnt(0);
    

//     seedRand((unsigned int)1498890829);
    seedRand(true);
    while (numAdd) {
        int x, y, z;
        z = getRand(nonoSize * nonoSize);
        x = z / nonoSize;
        y = z % nonoSize;
        if (nonoGram[x][y] == 0) {
            nonoGram[x][y] = 1;
            --numAdd;
            
        }
        if (++cnt == 100000) {
            return -1;
        }

    }

    
    
    for (int x = 0; x < nonoSize; ++x) {
        int idx = 0;
        int cnt = 0;
        int current = 0;
        for (int y = 0; y < nonoSize; ++y) {
            if (nonoGram[x][y] == 0 && current == 1) {
                if (cnt == 0 || cnt == nonoSize) {
                    return -1;
                }
                leftCnt[baseIdx][idx++] = cnt;
                
                cnt = 0;
            }
            else if (nonoGram[x][y] == 1) {
                ++cnt;
            }
            current = nonoGram[x][y];
            if (current == 1 && y == nonoSize - 1) {
                if (cnt == 0 || cnt == nonoSize) {
                    return -1;
                }
                leftCnt[baseIdx][idx++] = cnt;
                
            }
        }
        ++baseIdx;
        
    }
    
    
    baseIdx = 0;
    for (int y = 0; y < nonoSize; ++y) {
        int idx = 0;
        int cnt = 0;
        int current = 0;
        for (int x = 0; x < nonoSize; ++x) {
            if (nonoGram[x][y] == 0 && current == 1) {
                if (cnt == 0 || cnt == nonoSize) {
                    return -1;
                }
                topCnt[baseIdx][idx++] = cnt;
                
                cnt = 0;
            }
            else if (nonoGram[x][y] == 1) {
                ++cnt;
            }
            current = nonoGram[x][y];
            if (current == 1 && x == nonoSize - 1) {
                if (cnt == 0 || cnt == nonoSize) {
                    return -1;
                }
                topCnt[baseIdx][idx++] = cnt;
                
            }
        }
        
        ++baseIdx;
        
    }

    for (int i = 0; i < nonoSize; ++i) {
        if (topCnt[i][0] == 0 || leftCnt[i][0] == 0) {
            return -1;
        }
    }
    
//     for (int x = 0; x < nonoSize; ++x) {
//         for (int y = 0; y < nonoSize; ++y) {
//             std::cout << (int)nonoGram[x][y] << ' ';
//         }
//         std::cout << std::endl;
//     }
    
    
    return 0;
}

int NonoGram::checkEdges(char ng[10][10]) {
    int retVal(0);
    
//     for (int x = 0; x < nonoSize; ++x) {
//         for (int y = 0; y < nonoSize; ++y) {
//             std::cout << (int)ng[x][y] << ' ';
//         }
//         std::cout << std::endl;
//     }
    
    // first check the left edge
    for (int x = 0; x < nonoSize; ++x) {
        // check to see if there is a block at the edge
        if (ng[x][0] == 1) {
            bool yes(true);
            // are there x blocks according to the first hint?
            for (int y = 0; y < leftCnt[x][0]; ++y) {
                if (ng[x][y] == 0) {
                    yes = false;
                    ++retVal;
                    // then fill it in you dunce!
                    ng[x][y] = 1;
                }
            }
            if (leftCnt[x][1]) {
                ng[x][leftCnt[x][0]] = 2;
            }
            else {
                // fill in until the other side
                for (int y = leftCnt[x][0]; y < nonoSize; ++y) {
                    ng[x][y] = 2;
                }
            }
            
        }
    }
    // first check the right edge
    for (int x = 0; x < nonoSize; ++x) {
        // check to see if there is a block at the edge
        if (ng[x][nonoSize - 1] == 1) {
            bool yes(true);
            int idx(0);
            
            // get the last non-zero number
            while (leftCnt[x][idx] != 0) {
                ++idx;
            }
            --idx;
            
            // are there x blocks according to the first hint?
            for (int y = nonoSize - 1; y > (nonoSize - 1) - leftCnt[x][idx]; --y) {
                if ((idx == 0 && ng[x][0] != 1) || idx != 0) {
                    if (ng[x][y] == 0) {
                        yes = false;
                        ++retVal;
                        // then fill it in you dunce!
                        ng[x][y] = 1;
                    }
                }
            }
            if (idx == 0 && ng[x][0] != 1) {
                // fill in down to 0
                for (idx = nonoSize - 1 - leftCnt[x][idx]; idx >= 0; --idx) {
                    ng[x][idx] = 2;
                }
            }
            else if (idx != 0) {
                ng[x][nonoSize - 1 - leftCnt[x][idx]] = 2;
            }
        }
    }
    
    // first check the top edge
    for (int y = 0; y < nonoSize; ++y) {
        // check to see if there is a block at the edge
        if (ng[0][y] == 1) {
            bool yes(true);
            // are there x blocks according to the first hint?
            for (int x = 0; x < topCnt[y][0]; ++x) {
                if (ng[x][y] == 0) {
                    yes = false;
                    ++retVal;
                    // then fill it in you dunce!
                    ng[x][y] = 1;
                }
            }
            if (topCnt[y][1]) {
                ng[topCnt[y][0]][y] = 2;
            }
            else {
                // fill in until the other side
                for (int x = topCnt[y][0]; x < nonoSize; ++x) {
                    ng[x][y] = 2;
                }
            }
        }
    }
    
    // first check the bottom edge
    for (int y = 0; y < nonoSize; ++y) {
        // check to see if there is a block at the edge
        if (ng[nonoSize - 1][y] == 1) {
            bool yes(true);
            int idx(0);
            
            // get the last non-zero number
            while (topCnt[y][idx] != 0) {
                ++idx;
            }
            --idx;
            
            // are there x blocks according to the first hint?
            for (int x = nonoSize - 1; x > (nonoSize - 1) - topCnt[y][idx]; --x) {
                if ((idx == 0 && ng[0][y] != 1) || idx != 0) {
                    if (ng[x][y] == 0) {
                        yes = false;
                        ++retVal;
                        // then fill it in you dunce!
                        ng[x][y] = 1;
                    }
                }
            }
            if (idx == 0 && ng[0][y] != 1) {
                // fill in down to 0
                for (idx = nonoSize - 1 - topCnt[y][idx]; idx >= 0; --idx) {
                    ng[idx][y] = 2;
                }
            }
            else if (idx != 0) {
                ng[nonoSize - 1 - topCnt[y][idx]][y] = 2;
            }
        }
    }
    
    
    return retVal;
}

// Check to see if we have all of the blocks in that are 
// needed, if so, go back and fill in the Xs
int NonoGram::checkRcDone(char ng[10][10]) {
    int retVal(0);
    
    for (int x = 0; x < nonoSize; ++x) {
        int cnt(0), total(0);
        for (int idx = 0; idx < 6 && leftCnt[x][idx] != 0; ++idx) {
            total += leftCnt[x][idx];
        }
        for (int y = 0; y < nonoSize; ++y) {
            if (ng[x][y] == 1) {
                ++cnt;
            }
        }
        if (total == cnt) {
            // we have all needed blocks, X the rest out
            for (int y = 0; y < nonoSize; ++y) {
                if (ng[x][y] == 0) {
                    ++retVal;
                    ng[x][y] = 2;
                }
            }
        }
    }
    
    for (int y = 0; y < nonoSize; ++y) {
        int cnt(0), total(0);
        for (int idx = 0; idx < 6 && topCnt[y][idx] != 0; ++idx) {
            total += topCnt[y][idx];
        }
        for (int x = 0; x < nonoSize; ++x) {
            if (ng[x][y] == 1) {
                ++cnt;
            }
        }
        if (total == cnt) {
            // we have all needed blocks, X the rest out
            for (int x = 0; x < nonoSize; ++x) {
                if (ng[x][y] == 0) {
                    ++retVal;
                    ng[x][y] = 2;
                }
            }
        }
    }
    
    
    return retVal;
}


bool canMoveRight(char *array, int num, int pos, int len) {
    
    // it's at the end of the array
    if (pos == num - 1) {
        return false;
    }
    if (pos + len == num) {
        return false;
    }
    if (array[pos + len + 1] != 0) {
        return false;
    }
    return true;
}

bool NonoGram::nextSetup(char *array, int num) {
    int start(num);
    int len(1);
    bool foundStart(false);
   
   
    if (start == -1) {
//         std::cout << "Returning false here: " << __LINE__ << std::endl;
        return false;
    }
    while (!foundStart) {
        len = 1;
        while (start >= 0 && array[--start] != 1);
       
        while (start >= 0 && array[--start] == 1) {
            ++len;
        }
        ++start;
        
        if (canMoveRight(array, num, start, len) == true) {
            if (start == 0 && array[0] != 1) {
                return false;
            }
//             std::cout << "Can start here: " << start << ' ' << len << std::endl;
            foundStart = true;
        }
        
        if (start == 0 && foundStart == false) {
            return false;
        }
    }
   
//     std::cout << "I have a start of " << start << " and a len of " << len << std::endl;
   
    // Check to make sure when we move it it won't be off the end
    // or run into another
    if (start + len < num) {
        // If the space after the move is 1, don't do it
        if (start + len + 1 < num && array[start + len + 1] == 1) {
//             std::cout << "Returning false here: " << __LINE__ << std::endl;
            return false;
        }
        for (int i = len; i > 0; --i) {
            array[start + i] = array[start + i - 1];
        }
        array[start] = 0;
    }
    else { // over the end
//         std::cout << "Returning false here: " << __LINE__ << std::endl;
        return false;
    }
   
    return true;
}

int NonoGram::fillOverlapsSub(int n[6], char ng[10]) {
    char tmp[2][10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int idx(0);
    int offset(0);
    int totalSpace(0);
    int retVal(0);
    int size(nonoSize);
    int ands(0);
    
    // These 6s are suspect
    for (int i = 0; i < 6 && n[i]; ++i) {
        totalSpace += n[i];
        
        if (i + 1 < 6 && n[i + 1]) {
            ++totalSpace;
        }
    }
    for (int i = 0; i < nonoSize; ++i) {
        tmp[0][i] = 1;
    }
    
    if (totalSpace == size) {
        int arrayIdx(0);
        for (int idx = 0; idx < 6 && n[idx]; ++idx) {
            for (int i = 0; i < n[idx]; ++i) {
                ng[offset + arrayIdx] = 1;
                if (ng[offset + arrayIdx++] == 0) {
                    ++retVal;
                }
            }
            arrayIdx++;
        }
        return retVal;
    }
    while (totalSpace + offset < size) {
        int arrayIdx = 0;
        bool valid(false);
        
        
        while (!valid && totalSpace + offset <= size) {
            valid = true;
            arrayIdx = 0;
            for (int i = 0; i < nonoSize; ++i) {
                tmp[1][i] = 0;
            }
            
            // Fill the tmp array
            for (int idx = 0; idx < 6 && n[idx]; ++idx) {
                for (int i = 0; i < n[idx]; ++i) {
                    if (ng[offset + arrayIdx] == 2) {
                        valid = false;
                    }
                    tmp[1][offset + arrayIdx++] = 1;
                }
                arrayIdx++;
            }
            if (valid == false) {
                
                
                
                while (valid == false && nextSetup(tmp[1], nonoSize)) {
                    valid = true;
                    
                    for (int i = 0; i < nonoSize; ++i) {
                        if (ng[i] == 2 && tmp[1][i] == 1) {
                            valid = false;
                        }
                    }
                }
                if (valid == false) {
                    ++offset;
                }
            }
        }
        // We don't have a valid array
        if (ands == 0 && valid == false) {
            
            return 0;
        }
        

        do {
            bool valid(true);
       
            
            for (int i = 0; i < nonoSize; ++i) {
                if (ng[i] == 2 && tmp[1][i] == 1) {
                    valid = false;
                }
                if (ng[i] == 1 && tmp[1][i] == 0) {
                    valid = false;
                }
                
            }
            
            
            // check to make sure if it is valid
            if (valid) {
                
                ++ands;
                for (int i = 0; i < nonoSize; ++i) {
                    tmp[0][i] = tmp[0][i] & tmp[1][i];
                    
                }
                
            }
            else {
                
            }
        } while (nextSetup(tmp[1], nonoSize));
       
        ++offset;
    }
   
   
    for (int i = 0; i < nonoSize; ++i) {
        if (tmp[0][i]) {
            if (ng[i] == 0) {
                ng[i] = 1;
                ++retVal;
            }
        }
        
    }
    
    
    return retVal;
}

int NonoGram::fillOverlaps(char ng[10][10]) {
    int retVal(0);
    char tmp[10];
    
    for (int x = 0; x < nonoSize; ++x) {
        memcpy(tmp, ng[x], 10);
        retVal += fillOverlapsSub(leftCnt[x], tmp);
        
        memcpy(ng[x], tmp, 10);
        
    }
    for (int y = 0; y < nonoSize; ++y) {
        for (int x = 0; x < nonoSize; ++x) {
            tmp[x] = ng[x][y];
            
            
        }
        
        retVal += fillOverlapsSub(topCnt[y], tmp);
        for (int x = 0; x < nonoSize; ++x) {
            ng[x][y] = tmp[x];
            
        }
        
    }
    
    return retVal;
}

bool NonoGram::solve() {
    char ng[10][10];
    int retVal(1);
    
    
    for (int x = 0; x < nonoSize; ++x) {
        for (int y = 0; y < nonoSize; ++y) {
            ng[x][y] = 0;
        }
    }
    while (retVal > 0 && checkBoard(ng) == false) {
        retVal = 0;
        retVal += checkEdges(ng);
        retVal += checkRcDone(ng);
        retVal += fillOverlaps(ng);
    }
    
    if (checkBoard(ng) == false) {
        
        return false;
    }
    return true;
}

// This function will fill up the board with the correct answers
void NonoGram::test() {
    char ng[10][10];
    int retval = 1;
    
    for (int x = 0; x < nonoSize; ++x) {
        for (int y = 0; y < nonoSize; ++y) {
            ng[y][x] = (intptr_t)g_object_get_data((GObject  *)gtk_widget_get_parent(cells[x][y]), KEY_STR);
        }
    }
    while (retval) {
        retval = 0;
        retval += fillOverlaps(ng);
        retval += checkRcDone(ng);
    }

    for (int x = 0; x < nonoSize; ++x) {
        for (int y = 0; y < nonoSize; ++y) {
            
            
            if (ng[y][x] == 1) {
                reloadImage(cells[x][y], "block.png");
                g_object_set_data((GObject  *)gtk_widget_get_parent(cells[x][y]), KEY_STR, (void *)FILLED);
            }
            else if (ng[y][x] == 2) {
                reloadImage(cells[x][y], "X.png");
                g_object_set_data((GObject  *)gtk_widget_get_parent(cells[x][y]), KEY_STR, (void *)XED);
            }
        }
        
    }
}

bool NonoGram::checkBoard(char ng[10][10]) {
    bool retVal(true);
    
    
    for (int x = 0; x < nonoSize; ++x) {
        for (int y = 0; y < nonoSize; ++y) {
            if (ng == NULL) {
                if ((intptr_t)g_object_get_data((GObject  *)gtk_widget_get_parent(cells[x][y]), KEY_STR) == FILLED && nonoGram[y][x] != 1) {
//                     std::cout << "Error at " << x << ' ' << y << std::endl;
                    retVal = false;
                }
            
                if (nonoGram[y][x] == 1 && (intptr_t)g_object_get_data((GObject  *)gtk_widget_get_parent(cells[x][y]), KEY_STR) != FILLED) {
//                     std::cout << "Missing at " << x << ' ' << y << std::endl;
                    retVal = false;
                }
            }
            else {
                if (ng[x][y] == 1 && nonoGram[x][y] != 1) {
                    return false;
                }
                else if (ng[x][y] == 0 && nonoGram[x][y] == 1) {
                    return false;
                }
            }
        }
    }
//     for (int x = 0; x < nonoSize; ++x) {
//         for (int y = 0; y < nonoSize; ++y) {
//             std::cout << (int)nonoGram[x][y] << ' ';
//         }
//         std::cout << std::endl;
//     }
    return retVal;
}

void NonoGram::fillBoard() {
    for (int x = 0; x < nonoSize; ++x) {
        for (int y = 0; y < nonoSize; ++y) {
            if ((intptr_t)g_object_get_data((GObject  *)gtk_widget_get_parent(cells[x][y]), KEY_STR) == BLANK) {
                if (nonoGram[y][x] == 1) {
                    reloadImage(cells[x][y], "blockerr.png");
                }
                else {
                    reloadImage(cells[x][y], "Xerr.png");
                }
            }
            else if ((intptr_t)g_object_get_data((GObject  *)gtk_widget_get_parent(cells[x][y]), KEY_STR) != nonoGram[y][x]) {
                if (nonoGram[y][x] == 1) {
                    reloadImage(cells[x][y], "blockerr.png");
                }
                else {
                    reloadImage(cells[x][y], "Xerr.png");
                }
            }
        }
    }
    
}

