
/*	Copyright (C) 2005 - 2008 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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

// events for SecretPhrase

#include <secretphrase.h>
#include <sstream>
#include <iomanip>
#include <gdk/gdkkeysyms.h>

void SecretPhrase::exitGame(GtkWidget *widget, gpointer *ptr) {
    SecretPhrase *secretphrase(reinterpret_cast<SecretPhrase *>(*ptr));
    SCORE_STRUCT newScore;
    
    newScore.name = "";
    newScore.score = secretphrase->currentScore;
    newScore.date = "";
    
    secretphrase->GAME_PTR::addScore(secretphrase->scores, newScore);
    
    secretphrase->GAME_PTR::saveScores(secretphrase->scores);
    
    g_source_remove(secretphrase->timer);
    gtk_widget_hide(secretphrase->topvbox);
}

void SecretPhrase::userExit(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    SecretPhrase *secretPhrase(reinterpret_cast<SecretPhrase *>(ptr));
    
    if (secretPhrase->quitDialog()) {
        g_source_remove(secretPhrase->timer);
        gtk_widget_hide(secretPhrase->topvbox);
    }
    
}

gboolean SecretPhrase::timeout(gpointer ptr) {
    SecretPhrase *sp(reinterpret_cast<SecretPhrase *>(ptr));
    std::stringstream sstr;
    switch (sp->gameState) {
        case SELECTING_CATEGORY:
            
            break;
        case RUNNING:
            if (sp->progressValue) {
                if (--sp->progressValue) {
                    sp->progress.percent((sp->progressValue * 100) / 400);
                }
            }
            else {
                gtk_label_set_text((GtkLabel *)sp->infoLabel,
                                "Ran out of time.");
                sp->progress.hide();
                --sp->guessesLeft;
                if (sp->guessesLeft < 0) {
                    
                    sp->gameState = AUTO_FILL;
                    
                    return TRUE;
                }
                
                sstr.str("");
//                 sstr << "/usr/local/share/touchmegames/secretphrase/images/" << sp->guessesLeft << ".gif";
//                 gtk_image_set_from_file((GtkImage *)sp->guessesLeftImg, sstr.str().c_str());
                sstr << sp->guessesLeft << ".gif";
                sp->reloadImage(sp->guessesLeftImg, sstr.str().c_str());
                sp->handleCountDown = 20;
                
                sp->gameState = HANDLING_KEY;
            }
            break;
        case HANDLING_KEY:
            if (--sp->handleCountDown == 0) {
                if (sp->handleKey(sp->currentKey)) {
                    sp->currentScore += sp->currentValue;
                    sstr << sp->currentScore;
                    gtk_label_set_text((GtkLabel *)sp->scoreLabel, sstr.str().c_str());
                }
                else if (sp->checkFinished()) {
                    sp->gameState = BETWEEN_ROUNDS;
                }
                else if (!sp->solve) {
                    // if this is normal, then we would spin the wheel,
                    // but if its solve, then we just continue until they
                    // mess up or finish it.
                    sp->gameState = SPINNING;
                    if (sp->vowelsAllowed) {
                            gtk_label_set_text((GtkLabel *)sp->infoLabel, 
                                            "Select a letter.");
                    }
                    else {
                            gtk_label_set_text((GtkLabel *)sp->infoLabel, 
                                            "Select a letter, no vowels.");
                    }
                    sp->initSpin();
                }
                else if (sp->solve) {
                    // if this is normal, then we would spin the wheel,
                    // but if its solve, then we just continue until they
                    // mess up or finish it.
                    sp->gameState = RUNNING;
                    
                    gtk_label_set_text((GtkLabel *)sp->infoLabel, 
                                            "Solve the phrase.");
                    
                    
                    sp->progressValue = 400;
                    sp->progress.percent(100);
                    sp->progress.show();
                }
                
                sp->handleCountDown = 10;
            }
            break;
        case SPINNING:
            
            sp->spinWheel();
            
            break;
        case AUTO_FILL:
            if (--sp->handleCountDown == 0) {
                if (sp->showLetter()) {
                    sp->handleCountDown = 10;
                }
                else {
                    sleep(2);
                    if (sp->round == 3) {
                        sp->gameState = GAME_OVER;
                    }
                    else {
                        sp->gameState = SELECTING_CATEGORY;
                        gtk_widget_show_all(sp->initWidget);
                        gtk_widget_hide_all(sp->normVbox);
                        sp->progressValue = 400;
                        sp->progress.percent(100);
                        sp->progress.show();
                    }
                }
            }
            break;
        case BETWEEN_ROUNDS:
            if (!(--sp->handleCountDown)) {
                if (sp->guessesLeft > 0) {
                    sp->currentScore += 5000;
                    --sp->guessesLeft;
                    sstr << sp->currentScore;
                    gtk_label_set_text((GtkLabel *)sp->scoreLabel, sstr.str().c_str());
                    sstr.str("");
//                     sstr << "/usr/local/share/touchmegames/secretphrase/images/" 
//                                     << sp->guessesLeft << ".gif";
//                     gtk_image_set_from_file((GtkImage *)sp->guessesLeftImg, sstr.str().c_str());
                    sstr << sp->guessesLeft << ".gif";
                    sp->reloadImage(sp->guessesLeftImg, sstr.str().c_str());
                    
                }
                else {
                    if (sp->round == 3) {
                        sp->gameState = GAME_OVER;
                    }
                    else {
                        sp->gameState = SELECTING_CATEGORY;
                        gtk_widget_show_all(sp->initWidget);
                        gtk_widget_hide_all(sp->normVbox);
                    }
                }
                sp->handleCountDown = 10;
            }
            break;
        case GAME_OVER:
                sp->exitGame(NULL, (void **)&sp);
                break;
    }
    
    return TRUE;
}

void SecretPhrase::lowerLetterPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    SecretPhrase *sp(reinterpret_cast<SecretPhrase *>(ptr));
    if (sp->gameState != HANDLING_KEY && sp->gameState != SPINNING) {
        int x, y, key;
        std::stringstream sstr;
        sp->currentKey = (int)g_object_get_data((GObject *)widget, KEY_STR);
        key = sp->currentKey;
        if (sp->used[key - 'a']) {
            return;
        }
        if (key == 'a' || key == 'e' || key == 'i' || key == 'o' || key == 'u') {
            if (!sp->vowelsAllowed) {
                    return;
            }
        }
        else if (sp->getNumberOfOccurences(key) > 0) {
            sp->vowelsAllowed = true;
        }
        
        sp->used[key - 'a'] = true;
        sp->gameState = HANDLING_KEY;
        sp->handleCountDown = 10;
        sp->handlePosI = 0;
        sp->handlePosL = 0;
        
        x = (sp->currentKey - 'a') % 13;
        y = (sp->currentKey - 'a') / 13;
//         gtk_image_set_from_file((GtkImage *)sp->lowerLetters[y][x], "/usr/local/share/touchmegames/secretphrase/images/back.gif");
        sp->reloadImage(sp->lowerLetters[y][x], "back.gif");
        sstr << sp->getNumberOfOccurences(sp->currentKey) << ' ' << (char)(key - 0x20) << "'s found.";
        gtk_label_set_text((GtkLabel *)sp->infoLabel, sstr.str().c_str());
        sp->progress.hide();
        if (sp->getNumberOfOccurences(sp->currentKey) == 0) {
            --sp->guessesLeft;
            if (sp->guessesLeft < 0 || sp->solve) {
                sp->gameState = AUTO_FILL;
                return;
            }
            else {
                sstr.str("");
//                 sstr << "/usr/local/share/touchmegames/secretphrase/images/" << sp->guessesLeft << ".gif";
//                 gtk_image_set_from_file((GtkImage *)sp->guessesLeftImg, sstr.str().c_str());
                
                sstr << sp->guessesLeft << ".gif";
                sp->reloadImage(sp->guessesLeftImg, sstr.str().c_str());
                sp->handleCountDown = 20;
            }
        }
            
    }
}

gboolean SecretPhrase::keyPressed(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    SecretPhrase *sp(reinterpret_cast<SecretPhrase *>(ptr));
    
    // don't let the user do both keydown and mouse clicked
    // and also don't worry about the key repeat
    if (sp->gameState != HANDLING_KEY && sp->gameState != SPINNING) {
        int key, x, y;
        std::stringstream sstr;
        if (event->keyval >= GDK_A && event->keyval <= GDK_Z) {
            key = event->keyval + 0x20;
        }
        else if (event->keyval >= GDK_A && event->keyval <= GDK_Z) {
            key = event->keyval;
        }
        else {
            return TRUE;
        }
        if (sp->used[key - 'a']) {
            return TRUE;
        }
        if (key == 'a' || key == 'e' || key == 'i' || key == 'o' || key == 'u') {
            if (!sp->vowelsAllowed) {
                return TRUE;
            }
        }
        else if (sp->getNumberOfOccurences(key) > 0) {
            sp->vowelsAllowed = true;
        }
        sp->used[key - 'a'] = true;
        sp->currentKey = key;
        sp->gameState = HANDLING_KEY;
        sp->handleCountDown = 10;
        sp->handlePosI = 0;
        sp->handlePosL = 0;
        
        x = (sp->currentKey - 'a') % 13;
        y = (sp->currentKey - 'a') / 13;
//         gtk_image_set_from_file((GtkImage *)sp->lowerLetters[y][x], "/usr/local/share/touchmegames/secretphrase/images/back.gif");
        sp->reloadImage(sp->lowerLetters[y][x], "back.gif");
        sstr << sp->getNumberOfOccurences(sp->currentKey) << ' ' << (char)(key - 0x20) << "'s found.";
        gtk_label_set_text((GtkLabel *)sp->infoLabel, sstr.str().c_str());
        sp->progress.hide();
        if (sp->getNumberOfOccurences(sp->currentKey) == 0) {
            --sp->guessesLeft;
            if (sp->guessesLeft < 0 || sp->solve) {
                sp->gameState = AUTO_FILL;
                return TRUE;
            }
            else {
                sstr.str("");
//                 sstr << "/usr/local/share/touchmegames/secretphrase/images/" << sp->guessesLeft << ".gif";
//                 gtk_image_set_from_file((GtkImage *)sp->guessesLeftImg, sstr.str().c_str());
                sstr << sp->guessesLeft << ".gif";
                sp->reloadImage(sp->guessesLeftImg, sstr.str().c_str());
                
                sp->handleCountDown = 20;
            }
        }
        
    }

    return TRUE;
}

gboolean SecretPhrase::keyReleased(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    SecretPhrase *sp(reinterpret_cast<SecretPhrase *>(ptr));
    
    // let the paddle release take care of the other cleanup
    
    return TRUE;
}

void SecretPhrase::solvePhrase(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    SecretPhrase *sp(reinterpret_cast<SecretPhrase *>(ptr));
    if (!sp->solve && sp->gameState == RUNNING) {
        gtk_label_set_text((GtkLabel *)sp->infoLabel, 
                "Solve the Phrase.");
        sp->solve = true;
        sp->progressValue = 400;
        sp->progress.percent(100);
        sp->progress.show();
    }
}

void SecretPhrase::categorySelected(GtkWidget *widget, GdkEventKey *event, gpointer ptr) {
    SecretPhrase *sp(reinterpret_cast<SecretPhrase *>(ptr));
    int cat((int)g_object_get_data((GObject *)widget, KEY_STR));
    
    switch (cat) {
        case CatMisc:
            sp->rootname = "Misc";
            sp->category = "misc";
            break;
        case CatSports:
            sp->rootname = "Sports";
            sp->category = "sports";
            break;
        case CatMusic:
            sp->rootname = "Music";
            sp->category = "music";
            break;
        case CatVideo:
            sp->rootname = "Video";
            sp->category = "video";
            break;
    }
    sp->initRound();
    sp->initSpin();
    gtk_widget_hide(sp->initWidget);
    gtk_widget_show_all(sp->normVbox);
}

