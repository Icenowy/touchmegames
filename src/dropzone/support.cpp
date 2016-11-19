
/*	Copyright (C) 2008 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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

// support functions for DropZone
// Functions which basically don't fit in
// events.cpp or interface.cpp

#include <dropzone.h>
#include <sstream>
#include <fstream>
#include <cmath>


DropZone::DropZone() : gameWidth(800), gameHeight(600), gameName("dropzone") {
    playAreaWidth = 610;
    playAreaHeight = 545;
    playAreaTopX = 12;
    playAreaTopY = 15;
    playAreaBottomX = playAreaTopX + playAreaWidth;
    playAreaBottomY = playAreaTopY + playAreaHeight;
    
    totalRows = 8;
    
    scoreAreaTopX = 630;
    scoreAreaTopY = 40;
    
    // both must be <= 19
    numColumns = 19;
    numRows = totalRows;
    
    discDiameter = 61;
    discRadius = discDiameter / 2; 
    
    dropperWidth = 63;
    dropperOffset = 2;
    dropperX = playAreaTopX + (playAreaBottomX - playAreaTopX) / 2 - dropperWidth / 2;
    
    screwWidth = 60;
    screwHeight = 53;
    riserTopHeight = 19;
    
    strcpy(imageStrs[0], "");
    strcpy(imageStrs[1], "disc1.png");
    strcpy(imageStrs[2], "disc2.png");
    strcpy(imageStrs[3], "disc3.png");
    strcpy(imageStrs[4], "disc4.png");
    strcpy(imageStrs[5], "disc5.png");
    strcpy(imageStrs[6], "disc6.png");
    strcpy(imageStrs[7], "disc7.png");
    strcpy(imageStrs[8], "disc8.png");
    
    totalTime = 90 * 5; // 90 seconds and 5 interrupts/second
}

std::string DropZone::name() const {
    return gameName;
}

std::string DropZone::imageName() const {
    return "dropzone.png";
}

// This function doesn't even do anything anymore...really
// The background should be a modified bg.png
GdkColor *DropZone::backgroundColor() {
    return &bgcolor;
}

void DropZone::startRound() {
    std::stringstream sstr;
    
    numRows = totalRows;
    timeLeft = totalTime;
    seedRand();
    for (int x = 0; x < 19; ++x) {
        for (int y = 0; y < 19; ++y) {
            discLocations[x][y] = false;
        }
    }
    for (int x = 0; x < numColumns; ++x) {
        for (int y = 0; y < numRows; ++y) {
            dropped[x][y] = false;
        }
    }
    for (int x = 0; x < 2; ++x) {
        for (int y = 0; y < 8; ++y) {
            gtk_widget_hide(riserScrew[x][y]);
        }
    }
    
    gtk_fixed_move((GtkFixed *)topFixed, riserTop, playAreaTopX,
                    playAreaBottomY + discDiameter / 8);
    
    g_object_set_data((GObject *)nextDisc, KEY_STR, (gpointer)0);
    g_object_set_data((GObject *)onDeck, KEY_STR, (gpointer)0);
    
    ++round;
    if (loadRound(round) < 0) {
        gameState = GAME_OVER;
        return;
    }
    outputGame();
    sstr << round;
    gtk_label_set_text((GtkLabel *)roundLabel, sstr.str().c_str());
    moveCurrent(0, 0);
    moveCurrent(0, 0);

    dropperX = playAreaTopX + (playAreaBottomX - playAreaTopX) / 2 - dropperWidth / 2;
    gtk_fixed_move((GtkFixed *)topFixed, dropper, dropperX, playAreaTopY);
    gtk_fixed_move((GtkFixed *)topFixed, nextDisc, dropperX + dropperOffset, playAreaTopY + discDiameter);
    gtk_fixed_move((GtkFixed *)topFixed, onDeck, dropperX + dropperOffset, playAreaTopY);
    secondDrop = 100;

    gameState = RUNNING;
}


GtkWidget *DropZone::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    // Keep this color
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "DropZone is a game which you have\n"
            << "to remove all the disks from the screen.\n"
            << "This is done by dropping disks from the\n"
            << "top of the screen onto like colored disks.\n"
            << "When three or more disks of the same color\n"
            << "connect, they are removed from the screen.\n"
            << std::endl
            << "However, over time, the bottom will rise\n"
            << "and if the disks break on the top of the\n"
            << "screen, that is the end of the game.\n";
        
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int DropZone::gameType() const {
    return UNKNOWN_GAME;
}



char DropZone::roundOver() {
    if (round == 2) {
        DropZone *s(this);
        exitGame(NULL, (void **)&s);
        return 0;
    }
    return 0;
}

void DropZone::outputGame() {
    // comment this line when debugging
    // how the disks fall
    return;
    for (int y = 0; y < numRows; ++y) {
        for (int x = 0; x < numColumns; ++x) {
        
            int i = getData(x, y);
            if (i == -1) {
                std::cout << ' ';
            }
            else {
                std::cout << i;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// column says which column it should fall into
// xoffset is the offset from the center of the column
// if xoffset == 0, we will call the random % 2, 
// 0 for left 1 for right
int DropZone::dropDisc(int &column, gdouble xoffset, int &beginy, int color, bool doCrawl) {
    int x(column), y(-1), cnt(0);
    bool oneBelow(false), middleColumn(column & 1);
    int offset(xoffset == 0 ? 0: xoffset < 0 ? -1: 1);

    outputGame();

    if (column >= numColumns) {
        return 0;
    }
    
    if (beginy == 0) {
        if (discLocations[column][0] == true) {
//             std::cout << "Cannot fit! 1" << std::endl;
            return -1;
        }
        else if (column > 0 && discLocations[column - 1][0] == true) {
//             std::cout << "Cannot fit! 2" << std::endl;
            return -1;
        }
        else if  (column < numColumns - 1 && discLocations[column + 1][0] == true) {
//             std::cout << "Cannot fit! 3" << std::endl;
            return -1;
        }
    }
    
    for (int i = beginy; i < numRows - 1; ++i) {
//         std::cout << "Below: " << (discLocations[column][i + 2] ? "yes": "no") << std::endl;
        if (column  > 0 && discLocations[column - 1][i + 1]) {
            // there's one on that side, so you stay here
            y = i;
            i = numRows;
            discLocations[column][y] = true;
//             std::cout << "Setting location1 " << column << ' ' << y << std::endl;
        }
        else if (column < numColumns - 1 && discLocations[column + 1][i + 1]) {
            // there's one on that side, so you stay here
            y = i;
//             std::cout << "i: " << i << std::endl;
            i = numRows;
            discLocations[column][y] = true;
//             std::cout << "Setting location2 " << column << ' ' << y << std::endl;
        }
        else if (discLocations[column][i + 2]) {
            oneBelow = true;
//             std::cout << "One below " << i << std::endl;
            y = i + 1;
            i = numRows;
//             std::cout << "One below " << y << std::endl;
//             std::cout << "Setting location3 " << column << ' ' << y << ' ' << __LINE__ << std::endl;
        }
        else if (discLocations[column][i + 1]) {
            oneBelow = true;
//             std::cout << "One below " << i << std::endl;
            y = i;
            i = numRows;
//             std::cout << "One below " << y << std::endl;
//             std::cout << "Setting location3 " << column << ' ' << y << ' ' << __LINE__ << std::endl;
        }
//         else if ((column - 2 >= 0 && discLocations[column - 2][i]) 
//                   && (column + 2 < numColumns && discLocations[column + 2][i])) {
//             // there's a disc on either side, so you are stuck here
//             y = i;
//             i = numRows;
//             discLocations[column][y] = true;
//             std::cout << "Setting location3 " << column << ' ' << y << ' ' << __LINE__ << std::endl;
//         }
        else if (column - 2 >= 0 && discLocations[column - 2][i]) {
//             std::cout << "Changing the offset +" << std::endl;
            // change the offset
            offset = 1;
//             std::cout << "Setting location3 " << column << ' ' << y << ' ' << __LINE__ << std::endl;
        }
        else if (column + 2 < numColumns && discLocations[column + 2][i]) {
//             std::cout << "Changing the offset -" << std::endl;
            // change the offset
            offset = -1;
//             std::cout << "Setting location3 " << column << ' ' << y << ' ' << __LINE__ << std::endl;
        }
    }
    // make sure the disc won't fall off the grid
    if (column == 0) {
        offset = 1;
    }
    if (column == numColumns - 1) {
        offset = -1;
    }
    
    if (y == -1) {
        y = numRows - 1;
        if (!middleColumn) {
            discLocations[column][y] = true;
        }
    }
    
    if (oneBelow) {
        if (offset < 0) {
                // drop on the left side
            column -= 1;
        }
        else if (offset > 0) {
            column += 1;
        }
        else { // leave it up to chance
            int roll(rand() % 2 ? -1: 1);
//             std::cout << "Chance: " << roll << std::endl;
            column += roll;
        }
        discLocations[column][y] = true;
    }
    if (!oneBelow && y == numRows - 1) {
//         std::cout << "In the last row!!" << std::endl;
        if (column & 0x1) { // odd, should be even
            discLocations[column][y] = false;
            if (offset < 0) {
                // drop on the left side
                column -= 1;
            }
            else if (offset > 0) {
                column += 1;
            }
            else { // leave it up to chance
                int roll(rand() % 2 ? -1: 1);
                column += roll;
            }
            discLocations[column][y] = true;
        }
    }
    
    
    
//     std::cout << "Setting " << column << ' ' << y << " true " << (discLocations[column][y] ? "true": "false") << std::endl;
    
    if (beginy == y && column == x && beginy != 0) {
        discLocations[column][y] = false;
//         std::cout << "Done Drop Disc return -1 " << x << ' ' << y << std::endl;
        return -1;
    }
    
    if (color == 0) {
        moveCurrent(column, y);
    }
    else {
        moveOld(column, y, color);
    }
    
    for (int i = 0; i < 100; ++i) {
        d[i].clear();
    }
    d[0].x = column;
    d[0].y = y;
//     std::cout << "Setting2 " << column << ' ' << y << " true " << (discLocations[column][y] ? "true": "false") << std::endl;
    if (doCrawl) {
//         std::cout << "------------------------------Crawl: " << std::endl;
        if (crawl(d, getData(column, y), cnt) >= 2) {
            
            gameState = BLINKING;
            blinkCntr = 6;

            for (int i = 0; i < 100 && d[i].x != -1; ++i) {
//                 std::cout << d[i].x << ' ' << d[i].y << std::endl;
                dropped[d[i].x][d[i].y] = false;
            }
        }
    }
    beginy = y;
    outputGame();
//     std::cout << "Done Drop Disc" << std::endl;
    return 0;
}

bool DropZone::isNew(DZPath *d, int x, int y, int dnum) {
    for (int i = 0; i < dnum; ++i) {
        if (d[i].x == x && d[i].y == y) {
            return false;
        }
    }
    return true;
}

int DropZone::crawl(DZPath *d, int color, int &dnum) {
    int crawlCount(dnum);
    
    if (dnum > 100) {
// 	    std::cout << "Done early" << std::endl;
        return 0;
    }
//     std::cout << std::endl;
//     std::cout << "Crawl " << __LINE__ << ' ' << dnum << ' ' << d[crawlCount].x  << ' ' << d[crawlCount].y << std::endl;
//     std::cout << "Checking 0, " << crawlCount << std::endl;
    if (d[crawlCount].x > 1 &&  getData(d[crawlCount].x - 2, d[crawlCount].y) == color) {
        if (d[crawlCount].d000 == -1 && isNew(d, d[crawlCount].x - 2, d[crawlCount].y, dnum)) {
//             std::cout << "Found one to the 0 degrees!!" << std::endl;
            d[crawlCount].d000 = dnum + 1;
            d[dnum + 1].d090 = crawlCount;
            d[dnum + 1].x = d[crawlCount].x - 2;
            d[dnum + 1].y = d[crawlCount].y;
//             std::cout << "Going to crawl " << __LINE__ << std::endl;
//             std::cout.flush();
            crawl(d, color, ++dnum);
        }
        
    }
    
//     std::cout << "Checking 30, " << crawlCount << ' ' << dnum << std::endl;
    if (d[crawlCount].x > 0 && d[crawlCount].y > 0 && getData(d[crawlCount].x - 1, d[crawlCount].y - 1) == color) {
        if (d[crawlCount].d030 == -1 && isNew(d, d[crawlCount].x - 1, d[crawlCount].y - 1, dnum)) {
//             std::cout << "Found one to the 30 degrees!!" << std::endl;
            d[crawlCount].d030 = dnum + 1;
            d[dnum + 1].d120 = crawlCount;
            d[dnum + 1].x = d[crawlCount].x - 1;
            d[dnum + 1].y = d[crawlCount].y - 1;
//             std::cout << "Going to crawl " << __LINE__ << std::endl;
//             std::cout.flush();
            crawl(d, color, ++dnum);
        }
    }
    
    
//     std::cout << "Checking 60, " << crawlCount << std::endl;
//     std::cout.flush();
    if (d[crawlCount].x < numColumns - 1 && d[crawlCount].y > 0 && getData(d[crawlCount].x + 1, d[crawlCount].y - 1) == color) {
        if (d[crawlCount].d060 == -1 && isNew(d, d[crawlCount].x + 1, d[crawlCount].y - 1, dnum)) {
//             std::cout << "Found one to the 60 degrees!!" << std::endl;
            d[crawlCount].d060 = dnum + 1;
            d[dnum + 1].d150 = crawlCount;
            d[dnum + 1].x = d[crawlCount].x + 1;
            d[dnum + 1].y = d[crawlCount].y - 1;
//             std::cout << "Going to crawl " << __LINE__ << std::endl;
//             std::cout.flush();
            crawl(d, color, ++dnum);
        }
        
    }
    
//     std::cout << "Checking 90, " << crawlCount << std::endl;
    if (d[crawlCount].x < numColumns - 2 &&  getData(d[crawlCount].x + 2, d[crawlCount].y) == color) {
        if (d[crawlCount].d090 == -1 && isNew(d, d[crawlCount].x + 2, d[crawlCount].y, dnum)) {
//             std::cout << "Found one to the 90 degrees!!" << std::endl;
            d[crawlCount].d090 = dnum + 1;
            d[dnum + 1].d000 = crawlCount;
            d[dnum + 1].x = d[crawlCount].x + 2;
            d[dnum + 1].y = d[crawlCount].y;
//             std::cout << "Going to crawl " << __LINE__ << std::endl;
//             std::cout.flush();
            crawl(d, color, ++dnum);
        }
    }
    
//     std::cout << "Checking 120, " << crawlCount << std::endl;
    if (d[crawlCount].x < numColumns - 1 && d[crawlCount].y < numRows - 1 && getData(d[crawlCount].x + 1, d[crawlCount].y + 1) == color) {
        if (d[crawlCount].d120 == -1 && isNew(d, d[crawlCount].x + 1, d[crawlCount].y + 1, dnum)) {
//             std::cout << "Found one to the 120 degrees!! " << std::endl;
            d[crawlCount].d120 = dnum + 1;
            d[dnum + 1].d030 = crawlCount;
            d[dnum + 1].x = d[crawlCount].x + 1;
            d[dnum + 1].y = d[crawlCount].y + 1;
//             std::cout << "Going to crawl " << __LINE__ << std::endl;
//             std::cout.flush();
            
            crawl(d, color, ++dnum);
        }
        
    }
    
//     std::cout << "Checking 150, " << crawlCount << ' ' << d[crawlCount].y << std::endl;
    
    if (d[crawlCount].x > 0 && d[crawlCount].y < numRows - 1 && getData(d[crawlCount].x - 1, d[crawlCount].y + 1) == color) {
//         std::cout << "Colors " << getData(d[crawlCount].x - 1, d[crawlCount].y + 1) << ' ' << color << std::endl;
        if (d[crawlCount].d150 == -1 && isNew(d, d[crawlCount].x - 1, d[crawlCount].y + 1, dnum)) {
//             std::cout << "Found one to the 150 degrees!!" << std::endl;
            d[crawlCount].d150 = dnum + 1;
            d[dnum + 1].d060 = crawlCount;
            d[dnum + 1].x = d[crawlCount].x - 1;
            d[dnum + 1].y = d[crawlCount].y + 1;
//             std::cout << "Going to crawl " << __LINE__ << std::endl;
//             std::cout.flush();
            crawl(d, color, ++dnum);
        }
    }
    
//     std::cout << "Returning: " << dnum << std::endl; 
//     std::cout << "Crawl count: " << crawlCount << std::endl;
    
    return dnum;
}

int DropZone::checkDiscs() {
    bool found(false);
    
    // go though the rows
    // we can't check every one since they don't all have 
    // images 
    for (int y = 0; y < numRows && !found; ++y) {
        int cnt(0);
        int lastNum(0);
        for (int x = 0; x < numColumns && !found; ++x) {
            int current;
            int offset = (numRows & 1) ? (y & 1) : ((y & 1) ? 0: 1);
            for (int i = 0; i < 100; ++i) {
                d[i].clear();
            }
            
            if ((x & 1) == offset) {
                if (getData(x, y) != 0) {
                    d[0].x = x;
                    d[0].y = y;
                    cnt = 0;
//                     std::cout << std::endl << "-----------------------------------------starting main crawl" << std::endl;
                    if (crawl(d, getData(x, y), cnt) >= 2) {
//                         std::cout << "Found!!" << std::endl;
                        found = true;
                        gameState = BLINKING;
                        blinkCntr = 6;
                        for (int i = 0; i < 100 && d[i].x != -1; ++i) {
//                             std::cout << d[i].x << ' ' << d[i].y << std::endl;
                        }
                    }
//                     std::cout << "---------------------------------------Done crawl" << std::endl;
                }
            }
            
            lastNum = current;
        }
    }
//     std::cout << "Done" << std::endl;
//     std::cout.flush();
    return 0;
}

int DropZone::checkDropped() {
//     std::cout << "--------+++++++++++++++Check Dropped" << std::endl;
    for (int x = 0; x < numColumns; ++x) {
        for (int y = 0; y < numRows; ++y) {
            if (dropped[x][y]) {
                int cnt(0);
                for (int i = 0; i < 100; ++i) {
                    d[i].clear();
                }
                d[0].x = x;
                d[0].y = y;
//                 std::cout << "+++++++++++Crawl starting at " << x << ' ' << y << ' ' << getData(x, y) << std::endl;
                if (crawl(d, getData(x, y), cnt) >= 2) {
//                     std::cout << "Found!!" << std::endl;
                    gameState = BLINKING;
                    blinkCntr = 6;
                    for (int i = 0; i < 100 && d[i].x != -1; ++i) {
//                         std::cout << d[i].x << ' ' << d[i].y << std::endl;
                        dropped[d[i].x][d[i].y] = false;
                    }
                    return 1;
                }
            }
        }
    }
//     std::cout << "--------+++++++++++++++Done Check Dropped" << std::endl;
    return 0;
}

void DropZone::checkForFallthrough() {
    bool fallen(true);
//     std::cout << "Check for fall through" << std::endl;
    for (int x = 0; x < numColumns; ++x) {
        for (int y = 0; y < numRows; ++y) {
            dropped[x][y] = false;
        }
    }
    while (fallen) {
        fallen = false;
        for (int y = numRows - 2; y >= 0; --y) {
            int rowOffset = y & 1;
            for (int x = 0; x < numColumns; ++x) {
                if (discLocations[x][y]) {
                    // check the left and right
                    if (x >= rowOffset && discLocations[x - 1][y + 1]) {
//                         std::cout << "Bad 1" << std::endl;
                    }
                    else if (x <= numColumns - (1 + rowOffset) && discLocations[x + 1][y + 1]) {
//                         std::cout << "Bad 2" << std::endl;
                    }
                    // check below
//                     else if (y < numRows - 2 && discLocations[x][y + 2]) {
//                         std::cout << "Bad 3" << std::endl;
//                     }
                    else {
                        int tmpx(x), tmpy(y);
                        int color((intptr_t)g_object_get_data((GObject *)discs[x][y], KEY_STR));
//                         std::cout << "Time To drop" << std::endl;
                        outputGame();
//                         std::cout << "Drop this guy: " << tmpx << ' ' << tmpy << std::endl;
                        discLocations[x][y] = false;
                        reloadImage(discs[x][y], "discb.png");
                        g_object_set_data((GObject *)discs[x][y], KEY_STR, (gpointer)0);
                        if (dropDisc(tmpx, 0, tmpy, color, false) == 0) {
//                         tmpy += 1;
//                             std::cout << "Ended up at: " << tmpx << ' ' << tmpy << ' ' << getData(tmpx, tmpy) << std::endl;
                            dropped[tmpx][tmpy] = true;
                            
                            fallen = true;
                        }
//                         std::cout << "Done Time To drop" << std::endl;
                        outputGame();
                    }
                }
            }
        }
    }
    checkDropped();
//     std::cout << "Done check Fall Through" << std::endl;
}

int DropZone::loadRound(int r) {
    std::ifstream is;
    std::string str;
    bool loaded(false);
    
    str = INSTALL_DIR;
    str += "dropzone/levels";
    is.open(str.c_str());
    
    if (!is) {
        std::cerr << "Could not open levels file for DropZone: " 
                << strerror(errno) << std::endl;
        return -1;
    }
    while (!loaded && !is.eof()) {
        is >> str;
//         std::cout << str << std::endl;
        if (str.find("level-") == 0) {
            // not the best of error checking
//             std::cout << "Found a level " << atoi(str.substr(6, str.size()).c_str()) << std::endl;
            if (str.size() > 6 && r == atoi(str.substr(6, str.size()).c_str())) {
                int x(0), y(0);
                char ch;
                
//                 std::cout << "Found my level!! " << str << std::endl;
                while (!is.eof() && y < numRows && !loaded) {
                    // now we read in one at a time
                    is.get(ch);
//                     std::cout << ch;
                    if (ch != '\n') {
                        if (ch != '-' && ch != ' ') {
                            moveOld(x, y, ch - '0');
//                             std::cout << x << ' ' << y << " gets " << (int)(ch - '0') << std::endl;
                        }
                        if (++x == numColumns) {
                            ++y;
                            x = 0;
                        }
                    }
                }
                loaded = true;
            }
        }
    }
    is.close();
    if (loaded == false) {
        return -1;
    }
    return 0;
}

void DropZone::checkEndOfLevel() {
    // search all of the discs
    for (int x = 0; x < numColumns; ++x) {
        for (int y = 0; y < numRows; ++y) {
            if (getData(x, y) > 0) {
                return;
            }
        }
    }
//     std::cout << "End of Level" << std::endl;
    gameState = BETWEEN_ROUNDS;
    g_source_remove(timer);
    timer = g_timeout_add(20, timeout, this);
}
