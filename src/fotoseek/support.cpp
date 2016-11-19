
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

// support functions for FotoSeek
// Functions which basically don't fit in
// events.cpp or interface.cpp

#include <fotoseek.h>
#include <fstream>
#include <sstream>
#include <cmath>
#include <tmgdir.h>

FotoSeek::FotoSeek() {
    imageWidth = 300;
    imageHeight = 400;
    gameHeight = 600;
    gameWidth = 800;
    
    minRadius = imageWidth / 15;
    
    playXOffset = 95;
    playYOffset = 150;
    
    foundColor = 0x00FFFF00;
    missedColor = 0x00FF0000;
    matchColor = 0x00FF0000;
    
    ticksPerSecond = 20;
    totalTime = ticksPerSecond * 61;
    totalPoints = 1000;
}

std::string FotoSeek::name() const {
    return "fotoseek";
}

std::string FotoSeek::imageName() const {
    return "fotoseek.png";
}

// This function doesn't even do anything anymore...really
// The background should be a modified bg.png
GdkColor *FotoSeek::backgroundColor() {
    return &bgcolor;
}

void FotoSeek::initVars() {
    // these get set everytime the game is started
    round = 0;
    currentHint = 0;
    totalTime = ticksPerSecond * 61;
    usedDiffs = new std::vector<std::string>;
    
}

void FotoSeek::startRound() {
    std::string str;
    std::string nextImage;
    int lh;
    std::stringstream sstr;
    
    nextImage = getNextImage("misc");
    // debug lines
//     std::cout << "Next Image: " << nextImage << std::endl;
//     nextImage = "2.3";
    if (nextImage == "") {
        return;
    }
    
    totalDiffs = 0;
    round += 1;
    sstr << round;
    gtk_label_set_text((GtkLabel *)roundLabel, sstr.str().c_str());
    
    currentHint = 0;
    for (int i = 0; i < 3; ++i) {
        if (g_object_get_data((GObject *)hintImages[i], KEY_STR) == 0) {
            reloadImage(hintImages[i], "hint.png");
        }
        else {
            reloadImage(hintImages[i], "hintdone.png");
            currentHint = i + 1;
        }
    }

    gameState = RUNNING;
    
    for (int i = 0; i < 5; ++i) {
        diffCoords[i].found = false;
    }
    
    lh = nextImage.find('.');
    
    str = "fotoseek/images/misc/";
    str += nextImage.substr(0, lh);
    str += ".png";
    leftImage.reloadImage(str.c_str());
    
    str = INSTALL_DIR;
    str += "fotoseek/images/misc/";
    str += nextImage;
    str += "a.diff";
    leftImage.applyDifference(str.c_str());
    leftImage.scale(imageWidth, imageHeight, GDK_INTERP_NEAREST);
    
    addDiffs((char *)str.c_str());
    str = "fotoseek/images/misc/";
    str += nextImage.substr(0, lh);
    str += ".png";
    rightImage.reloadImage(str.c_str());
    
    str = INSTALL_DIR;
    str += "fotoseek/images/misc/";
    str += nextImage;
    str += "b.diff";
    rightImage.applyDifference(str.c_str());
    rightImage.scale(imageWidth, imageHeight, GDK_INTERP_NEAREST);
    
    addDiffs((char *)str.c_str());
    
    
    adjustDiffs();
    
    totalTime -= ticksPerSecond;
    timeLeft = totalTime;
    
    pointsLeft = totalPoints;
}

void FotoSeek::adjustDiffs() {
    for (int i = 0; i < 5; ++i) {
        diffCoords[i].centerx /= 2;
        diffCoords[i].centery /= 2;
        diffCoords[i].radius /= 2;
        if (diffCoords[i].radius < minRadius) {
            diffCoords[i].radius = minRadius;
        }
    }
}

void FotoSeek::addDiffs(char *file) {
    TmgImageDiff diff;
    std::ifstream is;
    std::string str;
    int lastx, lasty;
    bool first(true);
    struct Coords {
        int top;
        int bottom;
        int left;
        int right;
    };
    Coords coords;
    
    is.open(file);
    if (!is) {
        std::cout << "Could not open " << str << std::endl;
    }
    coords.left = 10000;
    coords.right = 0;
    coords.top = 10000;
    coords.bottom = 0;
    while (is.read((char *)&diff, sizeof(TmgImageDiff))) {
//         std::cout << "X: " << diff.x << " Y: " << diff.y << std::endl;
        if (!first) {
            if ((diff.x > lastx + 30) || (diff.y - 20 > coords.bottom) || diff.y + 20 < coords.top) {
                if (coords.right - coords.left > 4 && coords.bottom - coords.top > 4) {
                    // new image, output/save the old
                    int x1, y1, radius;
//                     std::cout << "Top: " << coords.top << " Bottom: " << coords.bottom
//                             << " Left: " << coords.left << " Right: " << coords.right << std::endl;
                    
                    // center
                    x1 = (coords.right - coords.left) / 2 + coords.left;
                    y1 = (coords.bottom - coords.top) / 2 + coords.top;
                    radius = coords.right - coords.left > coords.bottom - coords.top ? coords.right - coords.left: coords.bottom - coords.top;
                    radius /= 2;
//                     std::cout << "Center: " << x1 << ", " << y1 << " Radius: " << radius << std::endl;
                    
                    diffCoords[totalDiffs].centerx = x1;
                    diffCoords[totalDiffs].centery = y1;
                    diffCoords[totalDiffs].radius = radius + 10;
                    ++totalDiffs;
                    
                }
                // now "re-init" the coords
                coords.left = 10000;
                coords.right = 0;
                coords.top = 10000;
                coords.bottom = 0;
            }
        }
        else {
            first = false;
        }
        if (diff.x < coords.left) {
            coords.left = diff.x;
        }
        if (diff.x > coords.right) {
            coords.right = diff.x;
        }
        if (diff.y < coords.top) {
            coords.top = diff.y;
        }
        if (diff.y > coords.bottom) {
            coords.bottom = diff.y;
        }
        lastx = diff.x;
        lasty = diff.y;
    }
    is.close();
    if (coords.right - coords.left > 4 && coords.bottom - coords.top > 4) {
        int x1, y1, radius;
//         std::cout << "Top: " << coords.top << " Bottom: " << coords.bottom
//                 << " Left: " << coords.left << " Right: " << coords.right << std::endl;
                    
                    // center
        x1 = (coords.right - coords.left) / 2 + coords.left;
        y1 = (coords.bottom - coords.top) / 2 + coords.top;
        radius = coords.right - coords.left > coords.bottom - coords.top ? coords.right - coords.left: coords.bottom - coords.top;
        radius /= 2;
//         std::cout << "Center: " << x1 << ", " << y1 << " Radius: " << radius << std::endl;
        diffCoords[totalDiffs].centerx = x1;
        diffCoords[totalDiffs].centery = y1;
        diffCoords[totalDiffs].radius = radius + 10;
        ++totalDiffs;
    }
    
}

GtkWidget *FotoSeek::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    // Keep this color
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "In fotoseek, you need to find five differences\n"
            << "between two images.  There are three hints that can\n"
            << "be used throughout the game.  But, these hints are worth\n"
            << "1,000 points at the end of each round and you can only\n"
            << "use each hint once.  At the end of each round, the time\n"
            << "remaining is also given as a bonus.";
            
        
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int FotoSeek::gameType() const {
    return UNKNOWN_GAME;
}



char FotoSeek::roundOver() {
    if (round == 2) {
        FotoSeek *s(this);
        exitGame(NULL, (void **)&s);
        return 0;
    }
    return 0;
}


void FotoSeek::checkCircle(int x, int y) {
    for (int i = 0; i < 5; ++i) {
        int distance((int)sqrt(pow(x - diffCoords[i].centerx, 2) + pow(y - diffCoords[i].centery, 2)));
        if (distance < diffCoords[i].radius) {
            paintCircle(&diffCoords[i], true);
            diffCoords[i].found = true;
            
            updateScore(pointsLeft);
            pointsLeft = totalPoints;
            
            // need to update the screen here
            return;
        }
    }
    
    if (timeLeft - 25 >= 0) {
        timeLeft -= 25;
    }
    else {
        timeLeft = 0;
    }
    return;
}

int FotoSeek::maxImageNum(std::vector<std::string> dir) {
    int max(0);
    for (int i = 0; i < dir.size(); ++i) {
        int tmpi(atoi(dir[i].c_str()));
        if (tmpi > max) {
            max = tmpi;
        }
    }
    return max;
}

int FotoSeek::maxImageSubNum(int imageNum, std::vector<std::string> dir) {
    int max(0);
    for (int i = 0; i < dir.size(); ++i) {
        int lh(dir[i].find('.'));
        int tmpi;
        if (lh >= dir[i].size()) {
            std::cerr << "Error loading diffs, invalid file: " << dir[i] << std::endl;
            exit(-1);
        }
        if (atoi(dir[i].substr(0, lh).c_str()) == imageNum) {
            tmpi = atoi(dir[i].substr(lh + 1, dir[i].size()).c_str());
            if (tmpi > max) {
                max = tmpi;
            }
        }
    }
    return max;
}

std::string FotoSeek::getNextImage(char *path) {
    std::vector<std::string> images, diffs;
    std::string fullPath(INSTALL_DIR);
    std::string nextFile;
    TmgDir dir;
    int maxImageNumber;
    
    
    fullPath += "fotoseek";
    fullPath += "/images/";
    fullPath += path;
    
//     std::cout << "Path: " << fullPath << std::endl;
    
    if (dir.printDirectory(fullPath, images, TmgDir::FILES_ONLY, "png") != 0) {
        std::cout << "Error printing Directory" << std::endl;
    }

    
    maxImageNumber = maxImageNum(images);

    
    if (dir.printDirectory(fullPath, diffs, TmgDir::FILES_ONLY, "diff") != 0) {
        std::cout << "Error printing Directory" << std::endl;
    }

    
    if (usedDiffs->size() == diffs.size() / 2) {
        FotoSeek *fs(this);
        // We've no more images left!!
        gameState = GAME_OVER;
        exitGame(NULL, (void **)&fs);
        return "";
    }
    
    while (nextFile.size() == 0) {
        std::stringstream sstr;
        int imageNum, imageSubNum;
        seedRand();
        imageNum = getRand(maxImageNumber) + 1;
        
        imageSubNum = maxImageSubNum(imageNum, diffs);
        imageSubNum = getRand(imageSubNum) + 1;
        sstr << imageNum << '.' << imageSubNum;
        
        nextFile = sstr.str();
        if (std::find(usedDiffs->begin(), usedDiffs->end(), nextFile) != usedDiffs->end()) {
            // We've seen this image already
            nextFile = "";
        }
    }
    usedDiffs->push_back(nextFile);
    return nextFile;
}
