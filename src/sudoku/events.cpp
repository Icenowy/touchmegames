
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
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02011-1307  USA
 */

// events for Sudoku

#include <sudoku.h>
#include <sstream>
#include <iomanip>

void Sudoku::exitGame(GtkWidget *widget, gpointer *ptr) {
    Sudoku *sudoku(reinterpret_cast<Sudoku *>(*ptr));
    SCORE_STRUCT newScore;
    
    newScore.name = "";
    newScore.score = sudoku->currentScore;
    newScore.date = "";
    
    
    sudoku->GAME_PTR::addScore(sudoku->scores, newScore);
    
    sudoku->GAME_PTR::saveScores(sudoku->scores);
    
    g_source_remove(sudoku->timer);
    gtk_widget_hide(sudoku->tophbox);
}

void Sudoku::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    Sudoku *sudoku(reinterpret_cast<Sudoku *>(ptr));
    
    if (sudoku->quitDialog()) {
        g_source_remove(sudoku->timer);
        gtk_widget_hide(sudoku->tophbox);
    }
 
}


gboolean Sudoku::timeout(gpointer ptr) {
    Sudoku *sudoku(reinterpret_cast<Sudoku *>(ptr));
    int second, minute;
    std::stringstream sstr;
    std::string str;

    switch (sudoku->gameState) {
        case WAITING:
            // don't do anything, just wait for a press
            break;
        case RUNNING:
            --sudoku->timeLeft;
            if (sudoku->timeLeft <= 0) {
                sudoku->gameState = OUT_OF_TIME1;
            }
            sudoku->progress.percent((int)sudoku->timeLeft / 6);
            
            second = ((int)(sudoku->timeLeft / 5)) % 60;
            minute = (int)(sudoku->timeLeft / 5) / 60;
            
            sstr << (int)minute << ':' << std::setw(2) << std::setfill('0') << (int)second;
            
            sudoku->progress.text(sstr.str().c_str());
            
            break;
        case TAKE_PUZZLE_SCORE:
            if (sudoku->round == 1) {
                gtk_label_set_text((GtkLabel *)sudoku->puzzleScoreLabel, "50,000");
            }
            else {
                gtk_label_set_text((GtkLabel *)sudoku->puzzleScoreLabel, "75,000");
            }
            sudoku->gameState = TAKE_TIME_LEFT_SCORE;
            break;
        case TAKE_TIME_LEFT_SCORE:
            sstr << sudoku->timeLeft * 150;
            str = sstr.str();
            sudoku->addCommas(str);
            gtk_label_set_text((GtkLabel *)sudoku->timeLeftScoreLabel, str.c_str());
            sudoku->gameState = TAKE_ERRORS_SCORE;
            break;
        case TAKE_ERRORS_SCORE:
            if (sudoku->errors) {
                sstr << sudoku->errors * 5000;
                str = sstr.str();
                sudoku->addCommas(str);
                str = "-" + str;
                gtk_label_set_text((GtkLabel *)sudoku->errorPointsLabel, str.c_str());
            }
            sudoku->gameState = TAKE_ROUND_SCORE;
            break;
        case TAKE_ROUND_SCORE:
            if (sudoku->round == 1) {
                int score;
                score = 50000 - sudoku->errors * 5000 + sudoku->timeLeft * 150;
                sstr << score;
                str = sstr.str();
                sudoku->addCommas(str);
                gtk_label_set_text((GtkLabel *)sudoku->roundScoreLabel, str.c_str());
            }
            else {
                int score;
                score = 75000 - sudoku->errors * 5000 + sudoku->timeLeft * 150;
                sstr << score;
                str = sstr.str();
                sudoku->addCommas(str);
                gtk_label_set_text((GtkLabel *)sudoku->roundScoreLabel, str.c_str());
            }
            sudoku->gameState = TAKE_TOTAL_SCORE;
            break;
        case TAKE_TOTAL_SCORE:
            if (sudoku->round == 1) {
                sudoku->currentScore += 50000 - sudoku->errors * 5000 + sudoku->timeLeft * 150;
                sstr << sudoku->currentScore;
                str = sstr.str();
                sudoku->addCommas(str);
                gtk_label_set_text((GtkLabel *)sudoku->totalScoreLabel, str.c_str());
            }
            else {
                sudoku->currentScore += 75000 - sudoku->errors * 5000 + sudoku->timeLeft * 150;
                sstr << sudoku->currentScore;
                str = sstr.str();
                sudoku->addCommas(str);
                gtk_label_set_text((GtkLabel *)sudoku->totalScoreLabel, str.c_str());
            }
            gtk_label_set_text((GtkLabel *)sudoku->scoreLabel, str.c_str());
            sudoku->gameState = BETWEEN_ROUNDS;
            sudoku->stageCountDown = 15;
            break;
        case BETWEEN_ROUNDS:
            if (--sudoku->stageCountDown == 0) {
                sudoku->roundOver();
                sudoku->startRound();
            }
            break;
        case OUT_OF_TIME1:
            sudoku->playWave("buzzer.wav");
            sudoku->solve();
            sudoku->gameState = OUT_OF_TIME2;
            sudoku->stageCountDown = 10;
            break;
        case OUT_OF_TIME2:
            if (--sudoku->stageCountDown == 0) {
                sudoku->roundOver();
                sudoku->startRound();
            }
            break;
        case GAME_OVER:
            exitGame(NULL, &ptr);
            return FALSE;
            break;
        default:
            break;
        
    }
    
    return TRUE;
}


void Sudoku::tilePressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    Sudoku *sudoku(reinterpret_cast<Sudoku *>(ptr));
    GtkWidget *image;
    if (sudoku->gameState == WAITING) {
        sudoku->gameState = RUNNING;
    }
    image = gtk_bin_get_child((GtkBin *)widget);
    if ((intptr_t)g_object_get_data((GObject *)widget, KEY_STR) < 7) {
        
        if (sudoku->currentInt != 0) {
            std::stringstream sstr;
            sstr << sudoku->currentInt << "user.png";
            // change the label
            sudoku->reloadImage(image, sstr.str());
            g_object_set_data((GObject *)widget, KEY_STR, (void *)sudoku->currentInt);
            if (!sudoku->correctPlacement(image, sudoku->currentInt)) {
                sudoku->playWave("catscream.wav");
                ++sudoku->errors;
                sstr.str("");
                sstr << sudoku->currentInt << "error.png";
                sudoku->reloadImage(image, sstr.str());
                
                sstr.str("");
                sstr << sudoku->errors;
                gtk_label_set_text((GtkLabel *)sudoku->errorsLabel, sstr.str().c_str());
            }
            else if (sudoku->checkSolution()) {
                sudoku->playWave("gong.wav");
                sudoku->gameState = TAKE_PUZZLE_SCORE;
            }
        }
        else if ((intptr_t)g_object_get_data((GObject *)widget, KEY_STR) < 7) {
            sudoku->reloadImage(image, "null.gif");
            g_object_set_data((GObject *)widget, KEY_STR, 0);
        }
    }
    else if ((intptr_t)g_object_get_data((GObject *)widget, KEY_STR) > 6) {
        // it's a prefilled number, set the currentInt to this
        sudoku->currentInt = 255 - (intptr_t)g_object_get_data((GObject *)widget, KEY_STR);
    }
    else if (sudoku->currentInt == 0) {
        
        if ((intptr_t)g_object_get_data((GObject *)widget, KEY_STR) < 7) {
            sudoku->reloadImage(image, "null.gif");
            g_object_set_data((GObject *)widget, KEY_STR, 0);
        }
    }
}

void Sudoku::numberClicked(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    Sudoku *sudoku(reinterpret_cast<Sudoku *>(ptr));
    
    sudoku->currentInt = (intptr_t)g_object_get_data((GObject *)widget, KEY_STR);
}

void Sudoku::solveClicked(GtkWidget *widget, gpointer ptr) {
    Sudoku *sudoku(reinterpret_cast<Sudoku *>(ptr));
    sudoku->solve();

}

