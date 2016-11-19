
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

// support functions for Sudoku

#include <sudoku.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <sys/time.h>


Sudoku::Sudoku() {
    bgcolor.red = 0x000;
    bgcolor.green = 0x000;
    bgcolor.blue = 0x000;
}

std::string Sudoku::name() const {
    return "sudoku";
}

std::string Sudoku::imageName() const {
    return "sudoku.gif";
}

GdkColor *Sudoku::backgroundColor() {
    return &bgcolor;
}

bool Sudoku::checkHorizontal(int number, int x, int y) {
    for (int col = 0; col < maxColumns; ++col) {
        if (numbers[col][y] == number) {
            return false;
        }
    }
    return true;
}

bool Sudoku::checkVertical(int number, int x, int y) {
    for (int row = 0; row < maxRows; ++row) {
        if (numbers[x][row] == number) {
            return false;
        }
    }
    return true;
}

int Sudoku::addNum(int number, int xsector, int ysector) {
    int x(rand() % perXSector + xsector * perXSector);
    int y(rand() % perYSector + ysector * perYSector);
    int tries(0);
    while (numbers[x][y] != 0 || !checkVertical(number, x, y) || !checkHorizontal(number, x, y)) {
        x = rand() % perXSector + xsector * perXSector;
        y = rand() % perYSector + ysector * perYSector;
        if (++tries > 9) {
            return 10;
        }
    }
    numbers[x][y] = number;
    return 0;
}

void Sudoku::generateGrid() {
    
    for (int y = 0; y < maxRows; ++y) {
        for (int x = 0; x < maxColumns; ++x) {
            numbers[x][y] = 0;
        }
    }
    
    for (int num = 1; num < maxInt + 1; ++num) {
        for (int y = 0; y < maxRows / perYSector; ++y) {
            for (int x = 0; x < maxColumns / perXSector; ++x) {
                if (addNum(num, x, y) == 10) {
                    generateGrid();
                    num = maxInt + 1;
                    y = maxRows;
                    x = maxColumns;
                }
            }
        }
    }
    
    // "mask" the numbers
    for (int x = 0; x < maxColumns; ++x) {
        for (int y = 0; y < maxRows; ++y) {
            numberMask[x][y] = numbers[x][y];
        }
    }
}

void Sudoku::removeNumbers(int total) {
    for (int i = 0; i < total; ++i) {
        int x(rand() % 6);
        int y(rand() % 6);
        
        while (numberMask[x][y] == 0) {
            x = rand() % 6;
            y = rand() % 6;
        }
        numberMask[x][y] = 0;
    }
}

void Sudoku::startRound() {
    std::stringstream sstr;
    
    if (round + 1 == 4) {
        gameState = GAME_OVER;
        return;
    }
    
    if (++round < 3) {
        maxColumns = 6;
        maxRows = 6;
        perXSector = 3;
        perYSector = 2;
        maxInt = 6;
    }
    
    if (round == 1) {
        generatePuzzle(10, 20, 20);
        
    }
    else if (round == 2) {
        generatePuzzle(10, 20, 24);
    }
    else {
        return;
    }
    sstr << round;
    gtk_label_set_text((GtkLabel *)roundLabel, sstr.str().c_str());
    
    gameState = RUNNING;
    timeLeft = 600;
    
    roundScore = 0;
    errors = 0;
    currentInt = 0;
}


GtkWidget *Sudoku::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "sudoku is a game which you\n"
        << "have to fill a 6x6 grid so that each\n"
        << "column, each row, and each of the 2x3\n"
        << "boxes contain the digits 1 to 6.\n"
        << "Points are based off of finishing the puzzle,\n"
        << "time and number of errors.  The first round\n"
        << "is easy and the second round is a medium\n"
        << "level.";
        
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int Sudoku::gameType() const {
    return STRATEGY_GAME;
}



char Sudoku::roundOver() {
    if (round == 2) {
        Sudoku *s(this);
        exitGame(NULL, (void **)&s);
        return 0;
    }
    progress.percent(100);
    progress.text("2:00");
    gtk_label_set_text((GtkLabel *)puzzleScoreLabel, "0");
    gtk_label_set_text((GtkLabel *)timeLeftScoreLabel, "0");
    gtk_label_set_text((GtkLabel *)errorsLabel, "0");
    gtk_label_set_text((GtkLabel *)errorPointsLabel, "0");
    gtk_label_set_text((GtkLabel *)roundScoreLabel, "0");
    gtk_label_set_text((GtkLabel *)totalScoreLabel, "0");
    return 0;
}

int Sudoku::colHas(int col, int num) {
    for (int row = 0; row < maxRows; ++row) {
        if (numberMask[col][row] == num) {
            return row;
        }
    }
    return -1;
}

int Sudoku::rowHas(int row, int num) {
    for (int col = 0; col < maxColumns; ++col) {
        if (numberMask[col][row] == num) {
            return col;
        }
    }
    return -1;
}

bool Sudoku::isEmpty(int col, int row) {
    return (numberMask[col][row] == 0);
}

int Sudoku::getSector(int col, int row) {
    return (row / 2) * 2 + (col / 3);
}

bool Sudoku::checkRows(int checkFor) {
    bool found(false);
    for (int i = 0; i < maxRows; ++i) {
        int totalNumbers(0), lastNumber, missing(0), empty(0);
        std::vector<int> numbersFound;
        for (int x = 0; x < maxColumns; ++x) {
            if (numberMask[x][i] != 0) {
                numbersFound.push_back(numberMask[x][i]);
            }
            else {
                empty = x;
            }
        }
        
        
        if (numbersFound.size() == 5) {
            char str[2] = { '0', 0 };
            found = true;
            lastNumber = 0;
            std::sort(numbersFound.begin(), numbersFound.end());
            for (std::vector<int>::iterator it = numbersFound.begin(); it != numbersFound.end(); ++it) {
                
                if (*it != lastNumber + 1) {
                    missing = lastNumber + 1;
                    it = numbersFound.end() - 1;
                }
                lastNumber = *it;
            }
            if (missing == 0) {
                missing = 6;
            }

            // fill in the blank
            numberMask[empty][i] = missing;
            setTile(tiles[empty][i], missing);
        }
        
    }
    return found;
}

bool Sudoku::checkCols(int checkFor) {
    bool found(false);
    for (int i = 0; i < maxColumns; ++i) {
        int lastNumber, missing(0), empty(0);
        std::vector<int> numbersFound;
        for (int y = 0; y < maxRows; ++y) {
            if (numberMask[i][y] != 0) {
                numbersFound.push_back(numberMask[i][y]);
            }
            else {
                empty = y;
            }
        }
        
        
        if (numbersFound.size() == 5) {
            char str[2] = { '0', 0 };
            found = true;
            lastNumber = 0;
            std::sort(numbersFound.begin(), numbersFound.end());
            for (std::vector<int>::iterator it = numbersFound.begin(); it != numbersFound.end(); ++it) {
                
                if (*it != lastNumber + 1) {
                    missing = lastNumber + 1;
                    it = numbersFound.end() - 1;
                }
                lastNumber = *it;
            }
            if (missing == 0) {
                missing = 6;
            }
            
            // fill in the blank
            numberMask[i][empty] = missing;
            str[0] = missing + '0';
            setTile(tiles[i][empty], missing);
        }
        
    }
    
    return found;
}

bool Sudoku::checkSectors(int checkFor) {
    bool found(false);
    
    for (int i = 0; i < maxRows; ++i) { // this gives us the 6 sectors
        int xstart((i & 1) * 3), xend(xstart + 3);
        int ystart((i >> 1) * 2), yend(ystart + 2);
        int lastNumber, missing(0), emptyx(0), emptyy(0);
        std::vector<int> numbersFound;
        
        for (int x = xstart; x < xend; ++x) {
            for (int y = ystart; y < yend; ++y) {
                if (numberMask[x][y] != 0) {
                    numbersFound.push_back(numberMask[x][y]);
                }
                else {
                    emptyx = x;
                    emptyy = y;
                }
            }
        }
        if (numbersFound.size() == 5) {
            char str[2] = { '0', 0 };
            found = true;
            lastNumber = 0;
            std::sort(numbersFound.begin(), numbersFound.end());
            for (std::vector<int>::iterator it = numbersFound.begin(); it != numbersFound.end(); ++it) {
                
                if (*it != lastNumber + 1) {
                    
                    missing = lastNumber + 1;
                    it = numbersFound.end() - 1;
                }
                lastNumber = *it;
            }
            if (missing == 0) {
                missing = 6;
            }
            
            // fill in the blank
            numberMask[emptyx][emptyy] = missing;
            setTile(tiles[emptyx][emptyy], missing);
        }
    }
    
    return found;
}

bool Sudoku::checkRow4() {
    bool found(false);
    for (int i = 0; i < maxRows; ++i) {
        int totalNumbers(0), lastNumber, missing1(0), missing2(0), empty1(0), empty2(0);
        std::vector<int> numbersFound;
        for (int x = 0; x < maxColumns; ++x) {
            if (numberMask[x][i] != 0) {
                numbersFound.push_back(numberMask[x][i]);
            }
            else {
                if (empty1 == 0) { // hmm, vectors?
                    empty1 = x;
                }
                else {
                    empty2 = x;
                }
            }
        }
        
        
        if (numbersFound.size() == 4) {
            char str[2] = { '0', 0 };
            
            lastNumber = 0;
            std::sort(numbersFound.begin(), numbersFound.end());
            for (std::vector<int>::iterator it = numbersFound.begin(); it != numbersFound.end(); ++it) {
                
                if (*it != lastNumber + 1) {
                    if (*it != lastNumber + 2) { // we're missing both
                        missing1 = lastNumber + 1;
                        missing2 = lastNumber + 2;
                    }
                    else {
                        if (missing1 == 0) {
                            missing1 = lastNumber + 1;
                        }
                        else {
                            missing2 = lastNumber + 1;
                            it = numbersFound.end() - 1; 
                        }
                    }
                }
                lastNumber = *it;
            }
            if (missing1 == 0) {
                missing1 = 5;
            }
            if (missing2 == 0) {
                missing2 = 6;
            }
            
            for (int row = 0; row < maxRows; ++row) {
                if (numberMask[empty1][row] == missing1 || numberMask[empty2][row] == missing2) {
                    // fill in the blank
                    numberMask[empty1][i] = missing2;
                    setTile(tiles[empty1][i], missing2);
                    
                    numberMask[empty2][i] = missing1;
                    setTile(tiles[empty2][i], missing1);
                    row = maxRows;
                    found = true;
                }
                else if (numberMask[empty1][row] == missing2 || numberMask[empty2][row] == missing1) {
                    // fill in the blank
                    numberMask[empty1][i] = missing1;
                    setTile(tiles[empty1][i], missing1);
                    
                    numberMask[empty2][i] = missing2;
                    setTile(tiles[empty2][i], missing2);
                    row = maxRows;
                    found = true;
                }
                
            }
        }
        
    }
    return found;
}



bool Sudoku::checkCol4() {
    bool found(false);
    for (int i = 0; i < maxColumns; ++i) {
        int totalNumbers(0), lastNumber, missing1(0), missing2(0), empty1(0), empty2(0);
        std::vector<int> numbersFound;
        for (int y = 0; y < maxRows; ++y) {
            if (numberMask[i][y] != 0) {
                numbersFound.push_back(numberMask[i][y]);
            }
            else {
                if (empty1 == 0) {
                    empty1 = y;
                }
                else {
                    empty2 = y;
                }
            }
        }
        
        
        if (numbersFound.size() == 4) {
            char str[2] = { '0', 0 };
            
            lastNumber = 0;
            std::sort(numbersFound.begin(), numbersFound.end());
            for (std::vector<int>::iterator it = numbersFound.begin(); it != numbersFound.end(); ++it) {
                
                if (*it != lastNumber + 1) {
                    if (*it != lastNumber + 2) { // we're missing both
                        missing1 = lastNumber + 1;
                        missing2 = lastNumber + 2;
                    }
                    else {
                        if (missing1 == 0) {
                            missing1 = lastNumber + 1;
                        }
                        else {
                            missing2 = lastNumber + 1;
                            it = numbersFound.end() - 1; 
                        }
                    }
                }
                lastNumber = *it;
            }
            if (missing1 == 0) {
                missing1 = 5;
            }
            if (missing2 == 0) {
                missing2 = 6;
            }
            
            for (int col = 0; col < maxColumns; ++col) {
                if (numberMask[col][empty1] == missing1 || numberMask[col][empty2] == missing2) {
                    // fill in the blank
                    numberMask[i][empty1] = missing2;
                    setTile(tiles[i][empty1], missing2);
                    
                    numberMask[i][empty2] = missing1;
                    setTile(tiles[i][empty2], missing1);

                    col = maxColumns;
                    found = true;
                }
                else if (numberMask[col][empty1] == missing2 || numberMask[col][empty2] == missing1) {
                    // fill in the blank
                    numberMask[i][empty1] = missing1;
                    setTile(tiles[i][empty1], missing1);
                    
                    numberMask[i][empty2] = missing2;
                    setTile(tiles[i][empty2], missing2);
                    col = maxColumns;
                    found = true;
                }
            }
        }
        
    }
    return found;
}

bool Sudoku::checkThirds() {
    bool found(false);

    for (int num = 1; num <= 6; ++num) {
        for (int side = 0; side < 2; ++side) {
            int colFoundNum[3], colMissing(-1), numCols(0);
            
            for (int col = side * 3; col < side * 3 + 3; ++col) {
                colFoundNum[col - side * 3] = colHas(col, num);  //I'm ugly, fix me
                if (colFoundNum[col - side * 3] >= 0) {
                    // found one
                    ++numCols;
                }
                else {
                    colMissing = col;
                }
            }
            if (numCols == 2) {
                int sector(3);
                for (int i = 0; i < 3; ++i) {
                    if (colFoundNum[i] != -1) {
                        sector -= colFoundNum[i] / 2;
                        
                    }
                }
                
                if (rowHas(sector * 2, num) != -1) {
                    char str[2] = { 0, 0 };
                    
                    numberMask[colMissing][sector * 2 + 1] = num;
                    setTile(tiles[colMissing][sector * 2 + 1], num);
                    found = true;
                }
                else if (rowHas(sector * 2 + 1, num) != -1) {
                    char str[2] = { 0, 0 };
                    
                    numberMask[colMissing][sector * 2] = num;
                    setTile(tiles[colMissing][sector * 2], num);
                    found = true;
                }
                else {
                    if (numberMask[colMissing][sector * 2] != 0) {
                        char str[2] = { 0, 0 };
                        numberMask[colMissing][sector * 2 + 1] = num;
                        setTile(tiles[colMissing][sector * 2 + 1], num);
                        found = true;
                    }
                    else if (numberMask[colMissing][sector * 2 + 1] != 0) {
                        
                        char str[2] = { 0, 0 };
                        setTile(tiles[colMissing][sector * 2 ], num);
                        numberMask[colMissing][sector * 2] = num;
                        found = true;
                    }
                }
            }
        }
    }
    return found;
}

bool Sudoku::checkSeconds() {
    bool found(false);
    for (int num = 1; num <= 6; ++num) {
        for (int section = 0; section < 3; ++section) {
            int rowFoundNum[2], rowMissing(-1), numRows(0);
            for (int row = section * 2; row < section * 2 + 2; ++row) {
                rowFoundNum[row - section * 2] = rowHas(row, num);  //I'm ugly, fix me
                if (rowFoundNum[row - section * 2] >= 0) {
                    // found one
                    ++numRows;
                }
                else {
                    rowMissing = row;
                }
            }
            if (numRows == 1) {
                // get which side, which is wrong, thus the side = side ? 0: 1
                int side(rowFoundNum[0] >= 0 ? rowFoundNum[0] / 3: rowFoundNum[1] / 3);
                int found(0), colMissing(-1);
                side = side ? 0: 1;
                // check too see if one of the columns on this side is empty
                for (int col = side * 3; col < side * 3 + 3; ++col) {
                    if (isEmpty(col, rowMissing)) {
                        ++found;
                        colMissing = col;
                    }
                }
                if (found == 1) {
                    setTile(tiles[colMissing][rowMissing], num);
                    numberMask[colMissing][rowMissing] = num;
                    return true;
                }
                
            }
        }
    }
    return found;
}

void Sudoku::generatePuzzle(int min, int max, int numToRemove) {
    bool found(true), finished(false);
    int iterations(0);
    int tmpMask[6][6];
    generateGrid();
    removeNumbers(numToRemove);
    memcpy((void *)tmpMask, (void *)numberMask, sizeof(numbers));
    while (!finished) {
        found = true;
        // remove this later
        setGrid();
        while (found && iterations < 80) {

            found = false;
            found = checkRows(maxRows - 1);
            if (!found) {
                found = checkCols(maxColumns - 1);
            }
            if (!found) {
                
                
                
                found = checkSectors(maxRows - 1);
            }
            if (!found) {
                    
                    found = checkRow4();
            }
            if (!found) {
                    found |= checkCol4();
            }
            if (!found) {
                found = checkThirds();
            }
            if (!found) {
                found = checkSeconds();
                if (found) {
                    ++iterations;
                }
            }
            ++iterations;

        }
        if (iterations == 80) {
        }
        else {
            if (checkSolution() && iterations >= min && iterations <= max) {
                finished = true;
            }

        }
        
        
        if (!finished) {
            iterations = 0;
            generateGrid();
            removeNumbers(numToRemove);
            memcpy((void *)tmpMask, (void *)numberMask, sizeof(numbers));
        }
    }
    memcpy((void *)numberMask, (void *)tmpMask, sizeof(numbers));
    
    setGrid();
}

void Sudoku::solve() {
    for (int x = 0; x < 6; ++x) {
        for (int y = 0; y < 6; ++y) {
            if (numberMask[x][y] == 0) {
                setTile(tiles[x][y], numbers[x][y]);
            }
        }
    }
}

bool Sudoku::checkSolution() {
    for (int x = 0; x < 6; ++x) {
        for (int y = 0; y < 6; ++y) {
            if (numbers[x][y] != numberMask[x][y]) {
                return false;
            }
        }
    }
    return true;
}


bool Sudoku::correctPlacement(GtkWidget *widget, int attempt) {
    for (int x = 0; x < 6; ++x) {
        for (int y = 0; y < 6; ++y) {
            if (tiles[x][y] == widget) {
                if (attempt != numbers[x][y]) {
                    numberMask[x][y] = attempt;
                    return false;
                }
                numberMask[x][y] = attempt;
                return true;
            }
        }
    }
    return false;
}
