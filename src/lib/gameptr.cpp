
/*	Copyright (C) 2005 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
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

#include <gameptr.h>
#include <keypad.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#ifndef WIN32
#include <pwd.h>
#endif

GAME_PTR::GAME_PTR() : scoreType(INTEGER) {
    tmgWave = NULL;
}

GAME_PTR::~GAME_PTR() {
}

// returns true to quit and false to cancel
bool GAME_PTR::quitDialog() {
    GtkWidget *image;
    GtkWidget *fixed;
    GtkWidget *vbox;
    GtkWidget *ok, *cancel;
    GtkWidget *eventBox;
    GtkWidget *hbox;
    std::string tmpStr;
    exitDialog = gtk_dialog_new();
    
    fixed = gtk_fixed_new();
    tmpStr = INSTALL_DIR;
    tmpStr += "images/games/exitbg.png";
    
    image = gtk_image_new_from_file(tmpStr.c_str());
    gtk_fixed_put((GtkFixed *)fixed, image, 0, 0);
            
    gtk_box_pack_start((GtkBox *)GTK_DIALOG(exitDialog)->vbox, fixed, FALSE, FALSE, 0);
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_fixed_put((GtkFixed *)fixed, vbox, 0, 0);
    tmpStr = INSTALL_DIR;
    tmpStr += "images/confirmexit.png";
    
    image = gtk_image_new_from_file(tmpStr.c_str());
    gtk_box_pack_start((GtkBox *)vbox, image, FALSE, FALSE, 0);
//     label = gtk_label_new(" ");
//     gtk_widget_set_size_request(label, 20, -1);
//     gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start((GtkBox *)vbox, hbox, FALSE, FALSE, 0);
            
    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    tmpStr = INSTALL_DIR;
    tmpStr += "images/buttons/button_ok.png";
    ok = gtk_image_new_from_file(tmpStr.c_str());
    
    gtk_container_add((GtkContainer *)eventBox, ok);
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    
    g_signal_connect(G_OBJECT(eventBox), "button-release-event",
                     G_CALLBACK(quitOkButtonReleased), (gpointer)this);
    
    gtk_box_pack_start((GtkBox *)hbox, eventBox, TRUE, FALSE, 10);

    eventBox = gtk_event_box_new();
    gtk_event_box_set_visible_window((GtkEventBox *)eventBox, FALSE);
    gtk_widget_add_events(eventBox, GDK_ALL_EVENTS_MASK);
    tmpStr = INSTALL_DIR;
    tmpStr += "images/buttons/button_cancel.png";
    cancel = gtk_image_new_from_file(tmpStr.c_str());
    
//     cancel = gtk_image_new_from_file("/usr/local/share/touchmegames/images/buttons/button_cancel.png");
    gtk_container_add((GtkContainer *)eventBox, cancel);
    gtk_widget_add_events(eventBox, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(eventBox), "button-release-event",
                     G_CALLBACK(quitCancelButtonReleased), (gpointer)this);
    
    gtk_box_pack_start((GtkBox *)hbox, eventBox, TRUE, FALSE, 10);
            
    //gtk_window_set_position((GtkWindow *)exitDialog, GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_transient_for((GtkWindow *)exitDialog, (GtkWindow *)getWindow());
//     property = gtk_widget_class_find_style_property(GTK_DIALOG_GET_CLASS(exitDialog), "action-area-border");
//     if (property != NULL) {
    gtk_widget_set_size_request(GTK_DIALOG(exitDialog)->action_area, 1, 1);
        
    gtk_widget_show_all(exitDialog);
    
    if (gtk_dialog_run((GtkDialog *)exitDialog) == 1) {
        gtk_widget_destroy(exitDialog);
        return true;
    }
    gtk_widget_destroy(exitDialog);
    return false;
}

void GAME_PTR::quitOkButtonReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    GAME_PTR *gamePtr((GAME_PTR *)ptr);
    gtk_dialog_response((GtkDialog *)gamePtr->exitDialog, 1);
}

void GAME_PTR::quitCancelButtonReleased(GtkWidget *widget, GdkEventButton *event, gpointer ptr) {
    GAME_PTR *gamePtr((GAME_PTR *)ptr);
    gtk_dialog_response((GtkDialog *)gamePtr->exitDialog, 0);
}


GtkWidget *GAME_PTR::getHelp() {
	GtkWidget *vbox;
	GtkWidget *label;
		
	vbox = gtk_vbox_new(FALSE, 0);
	label = gtk_label_new("No help available for this game.");
	gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 0);
	return vbox;
}

void GAME_PTR::setScoreType(SCORE_TYPES type) {
	scoreType = type;
}

SCORE_TYPES GAME_PTR::getScoreType() {
	return scoreType;
}

int GAME_PTR::loadScores(SCORE_STRUCT scores[10]) {
	std::ifstream is;
	std::string scoreFile;
	std::string testStr;
	std::string home;
	int idx(-1);
	
#ifndef WIN32
    uid_t uid;
    struct passwd *pw;
 
    uid = getuid();
    pw = getpwuid(uid);
    home = pw->pw_dir;
 
 
	scoreFile = home + "/.touchmegames/" + name() + "/" + name() +  ".scores";
#else
     scoreFile = INSTALL_DIR;
     scoreFile += "/" + name() + "/" + name() + ".scores";
#endif // win32
	
	
	is.open(scoreFile.c_str());
	if (!is) {
		while (++idx != 10) {
			// We don't have 10 top scores in the file, so create blanks
			scores[idx].name = "";
			if (scoreType == INTEGER_DOWN) {
				scores[idx].score = 0;
			}
			else {
				scores[idx].score = 0;
			}
			scores[idx].fscore = 0.0;
			scores[idx].date = "";
		}
		return -1;
	}
	
	while (++idx != 10 && !is.eof()) {
		getline(is, scores[idx].name);
		
		if (is.eof()) {
			// Do something here, corrupted file - BLARG
			// set idx to 0, as if the file wasn't there
			idx = -1;
		}
		
		getline(is, testStr);
		if (scoreType == INTEGER) {
			if (testStr.find_first_not_of("0123456789") > testStr.size()) {
				scores[idx].score = atoi(testStr.c_str());
			}
			else {
				// Do something here, corrupted file - BLARG
			}
		}
		else if (scoreType == INTEGER_DOWN) {
			if (testStr.find_first_not_of("0123456789") > testStr.size()) {
				scores[idx].score = atoi(testStr.c_str());
			}
			else {
				// Do something here, corrupted file - BLARG
			}
		}
		else if (scoreType == FLOAT) {
			if (testStr.find_first_not_of("0123456789.") > testStr.size()) {
				scores[idx].fscore = atof(testStr.c_str());
			}
			else {
				// Do something here, corrupted file - BLARG
			}
		}
		else if(scoreType == TIME_DOWN) {
			if (testStr.find_first_not_of("0123456789.") > testStr.size()) {
				scores[idx].fscore = atof(testStr.c_str());
			}
			else {
				// Do something here, corrupted file - BLARG
			}
		}
		getline(is, scores[idx].date);
	}
	
	while (idx != 10) {
		// We don't have 10 top scores in the file, so create blanks
		scores[idx].name = "";
		if (scoreType == INTEGER_DOWN) {
			scores[idx].score = 0;
		}
		else {
			scores[idx].score = 0;
		}
		scores[idx++].date = "";
	}
	is.close();
	return 0;
}

int GAME_PTR::saveScores(const SCORE_STRUCT scores[10]) {
    std::ofstream os;
    std::string scoreFile, home;
    int idx(-1);

#ifndef WIN32
    uid_t uid;
    struct passwd *pw;

    uid = getuid();
    pw = getpwuid(uid);
    home = pw->pw_dir;


    scoreFile = home + "/.touchmegames/" + name() + "/" + name() +  ".scores";
#else
     scoreFile = INSTALL_DIR;
     scoreFile += "/" + name() + "/" + name() + ".scores";
#endif // win32
    
    os.open(scoreFile.c_str());
    if (!os) {
        GtkWidget *dialog;
        GtkWidget *label;
        GtkWidget *button;
        std::stringstream sstr;
        int res;
        dialog = gtk_dialog_new();
        gtk_window_set_position((GtkWindow *)dialog, GTK_WIN_POS_CENTER_ALWAYS);
        gtk_window_set_title(&((GtkDialog *)dialog)->window,
                "Create Scores File");
        
        sstr << "There is no file at location: " << scoreFile
                        << ".\nWould you like to create this file now "
                        << "to save your scores?";
                        
        label = gtk_label_new(sstr.str().c_str());
        gtk_box_pack_start((GtkBox *)((GtkDialog *)dialog)->vbox, label,
                                FALSE, FALSE, 0);

        label = gtk_label_new("OK");
        button = gtk_button_new();
        gtk_container_add(GTK_CONTAINER(button), label);
        gtk_dialog_add_action_widget(GTK_DIALOG(dialog), 
                                        button, GTK_RESPONSE_ACCEPT);
        gtk_widget_show_all(button);
        
        gtk_window_set_position((GtkWindow *)dialog, GTK_WIN_POS_CENTER_ALWAYS);
        gtk_widget_show_all(dialog);
        
        res = gtk_dialog_run(GTK_DIALOG(dialog));
        if (res == GTK_RESPONSE_ACCEPT) {
            std::string tmpstr;
            struct stat buf;
            tmpstr = home;
            tmpstr += "/.touchmegames"; // first check for the touchmegames folder
            if (stat(tmpstr.c_str(), &buf) == -1) {
                    // create the folder
                            
                            
#ifdef __unix__
                if (mkdir(tmpstr.c_str(), 0777) == -1) {
#else
                if (mkdir(tmpstr.c_str()) == -1) {
#endif
                    GtkWidget *ndialog;
        
                    gtk_dialog_run((GtkDialog *)ndialog);
                    gtk_widget_destroy(ndialog);
                    gtk_widget_destroy(dialog);
                    exit(1);
                }
            }
            tmpstr += "/" + name(); // next the game's folder
            if (stat(tmpstr.c_str(), &buf) == -1) {
                // create the folder
#ifdef __unix__
                if (mkdir(tmpstr.c_str(), 0777) == -1) {
#else
                if (mkdir(tmpstr.c_str()) == -1) {
#endif
                    GtkWidget *ndialog;
                    gtk_dialog_run((GtkDialog *)ndialog);
                    gtk_widget_destroy(ndialog);
                    gtk_widget_destroy(dialog);
                    exit(1);
                }
            }
            
            os.clear();
            os.open(scoreFile.c_str());
            if (!os) {
                std::cerr << "Still could not open file."
                                << std::endl;
                exit(0);
            }
        }
        else {
            gtk_widget_destroy(dialog);
            return 0;
        }
        gtk_widget_destroy(dialog);
    
    }
    
    while (++idx != 10) {
        if (scoreType == INTEGER || scoreType == INTEGER_DOWN) {
            os << scores[idx].name << std::endl
                    << scores[idx].score << std::endl
                    << scores[idx].date
                    << std::endl;
        }
        else if (scoreType == FLOAT || scoreType == TIME_DOWN) {
            os << scores[idx].name << std::endl
                    << std::setprecision(4)
                    << scores[idx].fscore << std::endl
                    << scores[idx].date
                    << std::endl;
        }
    }
    os.close();
    return 0;
}

int GAME_PTR::addScore(SCORE_STRUCT scores[10], SCORE_STRUCT newScore) {
	for (int i = 0; i < 10; ++i) {
		if (scoreType == INTEGER) {
			if (scores[i].score < newScore.score) {
				int idx(10);
				time_t tsec;
				std::stringstream sstr;
				tm *t;
				while (--idx != i) {
					scores[idx].name = scores[idx - 1].name;
					scores[idx].score = scores[idx - 1].score;
					scores[idx].date = scores[idx - 1].date;
				}
				scores[i].name = "";
				scores[i].score = newScore.score;
				tsec = time(NULL);
				t = localtime(&tsec);
				sstr << std::setw(2) << std::setfill('0') 
						<< t->tm_mon + 1 << '/'
						<< std::setw(2) << std::setfill('0') 
						<< t->tm_mday << '/'
						<< std::setw(2) << std::setfill('0') 
						<< t->tm_year - 100;
				scores[i].date = sstr.str();
				
				viewScores(scores, i);
				i = 10;
			}
		}
		if (scoreType == INTEGER_DOWN) {
			std::cout << "score " << scores[i].score << " newscore " << newScore.score << std::endl;
			if (scores[i].score > newScore.score || scores[i].score == 0) { //why? cause it's impossible, so far...
				int idx(10);
				time_t tsec;
				std::stringstream sstr;
				tm *t;
				while (--idx != i) {
					scores[idx].name = scores[idx - 1].name;
					scores[idx].score = scores[idx - 1].score;
					scores[idx].date = scores[idx - 1].date;
				}
				scores[i].name = "";
				scores[i].score = newScore.score;
				tsec = time(NULL);
				t = localtime(&tsec);
				sstr << std::setw(2) << std::setfill('0') 
						<< t->tm_mon + 1 << '/'
						<< std::setw(2) << std::setfill('0') 
						<< t->tm_mday << '/'
						<< std::setw(2) << std::setfill('0') 
						<< t->tm_year - 100;
				scores[i].date = sstr.str();
				
				viewScores(scores, i);
				i = 10;
			}
		}
		else if (scoreType == FLOAT || scoreType == TIME) {
			if (scores[i].fscore < newScore.fscore) {
				int idx(10);
				time_t tsec;
				std::stringstream sstr;
				tm *t;
				while (--idx != i) {
					scores[idx].name = scores[idx - 1].name;
					scores[idx].fscore = scores[idx - 1].fscore;
					scores[idx].date = scores[idx - 1].date;
				}
				scores[i].name = "";
				scores[i].fscore = newScore.fscore;
				tsec = time(NULL);
				t = localtime(&tsec);
				sstr << std::setw(2) << std::setfill('0') 
						<< t->tm_mon + 1 << '/'
						<< std::setw(2) << std::setfill('0') 
						<< t->tm_mday << '/'
						<< std::setw(2) << std::setfill('0') 
						<< t->tm_year - 100;
				scores[i].date = sstr.str();
				
				viewScores(scores, i);
				i = 10;
			}
		}
		else if (scoreType == TIME_DOWN) {
			if (scores[i].fscore > newScore.fscore || scores[i].fscore == 0.0) {
				int idx(10);
				time_t tsec;
				std::stringstream sstr;
				tm *t;
				while (--idx != i) {
					scores[idx].name = scores[idx - 1].name;
					scores[idx].fscore = scores[idx - 1].fscore;
					scores[idx].date = scores[idx - 1].date;
				}
				scores[i].name = "";
				scores[i].fscore = newScore.fscore;
				tsec = time(NULL);
				t = localtime(&tsec);
				sstr << std::setw(2) << std::setfill('0') 
						<< t->tm_mon + 1 << '/'
						<< std::setw(2) << std::setfill('0') 
						<< t->tm_mday << '/'
						<< std::setw(2) << std::setfill('0') 
						<< t->tm_year - 100;
				scores[i].date = sstr.str();
				
				viewScores(scores, i);
				i = 10;
			}
		}
	}
	// do we really need to return something here?
	return 0;
}

void GAME_PTR::userdone(GtkWidget *widget, gpointer ptr) {
	std::cout << "User Done" << std::endl;
	gtk_button_clicked((GtkButton *)ptr);
}

// set newScore < 0 if you didn't win and newScore >= 0 if adding a score
int GAME_PTR::viewScores(SCORE_STRUCT scores[10], int newScore) {
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *highScoresDialog;
    GtkWidget *nameEntry;
    std::string retString("");
    int i;
    GdkColor color;
    GtkWidget *eventBox;
    
    color.red = 0xFFFF;
    color.green = 0xEAEA;
    color.blue = 0x0;
    
    highScoresDialog = gtk_dialog_new();
    gtk_window_set_position((GtkWindow *)highScoresDialog, GTK_WIN_POS_CENTER_ALWAYS);
    
    
    gtk_window_set_title(&((GtkDialog *)highScoresDialog)->window,
                            "High Scores");
    
                            
    label = gtk_label_new("High Scores");
    
    
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (highScoresDialog)->vbox),
                    label, TRUE, TRUE, 0);
    
    gtk_widget_show(label);
    
    hbox = gtk_hbox_new(FALSE, 0);
    
    
    gtk_box_pack_start((GtkBox *)((GtkDialog *)highScoresDialog)->vbox,
                    hbox, FALSE, FALSE, 0);
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    gtk_box_pack_start((GtkBox *)hbox,
                    vbox, FALSE, FALSE, 0);
    
    for (i = 0; i < 10; ++i) {
            std::stringstream sstr;
            sstr << std::setw(3) << std::setfill(' ') 
                            << std::setiosflags(std::ios_base::left) << i + 1;
            label = gtk_label_new(sstr.str().c_str());
            if (i == newScore) {
                    eventBox = gtk_event_box_new();
                    gtk_widget_modify_bg(eventBox, GTK_STATE_NORMAL, &color);
                    gtk_container_add((GtkContainer *)eventBox, label);
                    gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 5);
            }
            else {
                    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 5);
            }
    }
    vbox = gtk_vbox_new(FALSE, 0);
    
    gtk_box_pack_start((GtkBox *)hbox,
                        vbox, FALSE, FALSE, 0);
    button = gtk_button_new();
    for (i = 0; i < 10; ++i) {
            std::stringstream sstr;
            if (i == newScore) {
                    KEYPAD kp;
                    kp.init();
                    sstr << std::setw(20) << std::setfill(' ') << std::setiosflags(std::ios_base::left)
                                    << kp.run();
                    nameEntry = gtk_label_new(sstr.str().c_str());
                    eventBox = gtk_event_box_new();
                    gtk_widget_modify_bg(eventBox, GTK_STATE_NORMAL, &color);
                    
                    gtk_misc_set_alignment((GtkMisc *)nameEntry, 0, 0.5);
                    gtk_container_add((GtkContainer *)eventBox, nameEntry);
                    
                    gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 5);
                    
            }
            else {
                    sstr << std::setw(20) << std::setfill(' ') << std::setiosflags(std::ios_base::left)
                                    << scores[i].name;
                    label = gtk_label_new(sstr.str().c_str());
                    gtk_misc_set_alignment((GtkMisc *)label, 0, 0.5);
                    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 5);
            }
    }
    vbox = gtk_vbox_new(FALSE, 0);
    
    gtk_box_pack_start((GtkBox *)hbox,
                        vbox, FALSE, FALSE, 0);
    for (int i = 0; i < 10; ++i) {
        std::stringstream sstr;
            
        if (scoreType == INTEGER || scoreType == INTEGER_DOWN) {
            std::string str;
            sstr << scores[i].score;
            str = sstr.str();
            sstr.str("");
            addCommas(str);
            sstr << std::setw(20) << std::setfill(' ') << std::setiosflags(std::ios_base::right)
                    << str;
        }
        else if (scoreType == FLOAT) {
            sstr << std::setw(20) << std::setfill(' ') 
                << std::setiosflags(std::ios_base::right)
                << std::setprecision(4)
                << scores[i].fscore;
        }
        else if (scoreType == TIME_DOWN) {
            std::stringstream ss;
            ss << std::setw(2) << std::setfill(' ') << (int)scores[i].fscore << ':' 
                << std::setw(2) << std::setfill('0') << (int)(scores[i].fscore * 100.0) % 100;
            sstr << std::setw(20) << std::setfill(' ') 
                << std::setiosflags(std::ios_base::right)
                << ss.str();
        }
        label = gtk_label_new(sstr.str().c_str());
        gtk_misc_set_alignment((GtkMisc *)label, 1, 0.5);
            
        if (i == newScore) {
            eventBox = gtk_event_box_new();
            gtk_widget_modify_bg(eventBox, GTK_STATE_NORMAL, &color);
            gtk_container_add((GtkContainer *)eventBox, label);
            gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 5);
        }
        else {
            gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 5);
        }
    }
    gtk_widget_show_all(hbox);
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    gtk_box_pack_start((GtkBox *)hbox,
                        vbox, FALSE, FALSE, 0);
    for (int i = 0; i < 10; ++i) {
            std::stringstream sstr;
            sstr << std::setw(20) << std::setfill(' ') << std::setiosflags(std::ios_base::right)
                            << scores[i].date;
            label = gtk_label_new(sstr.str().c_str());
            gtk_misc_set_alignment((GtkMisc *)label, 1, 0.5);
            
            if (i == newScore) {
                    eventBox = gtk_event_box_new();
                    gtk_widget_modify_bg(eventBox, GTK_STATE_NORMAL, &color);
                    gtk_container_add((GtkContainer *)eventBox, label);
                    gtk_box_pack_start((GtkBox *)vbox, eventBox, FALSE, FALSE, 5);
            }
            else {
                    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 5);
            }
    }
    gtk_widget_show_all(hbox);
    
    label = gtk_label_new("OK");
    gtk_widget_set_name(label, "bigLightFont");

    gtk_container_add(GTK_CONTAINER(button), label);
    gtk_dialog_add_action_widget(GTK_DIALOG(highScoresDialog), 
            button, GTK_RESPONSE_ACCEPT);
    gtk_widget_show_all(button);
    
    
    gtk_window_set_transient_for((GtkWindow *)&((GtkDialog *)highScoresDialog)->window,
            (GtkWindow *)getWindow());
    gtk_window_set_position((GtkWindow *)&((GtkDialog *)highScoresDialog)->window,
            GTK_WIN_POS_CENTER_ON_PARENT);
    
    if (gtk_dialog_run((GtkDialog *)highScoresDialog) == GTK_RESPONSE_ACCEPT) {
        if (newScore >= 0) {
            scores[newScore].name = std::string(gtk_label_get_text((GtkLabel *)nameEntry));
        }
    }

    gtk_widget_destroy(highScoresDialog);
    // do we really need to return something here?
    return 0;
}


void GAME_PTR::setWavePlayer(TmgWave *wave) {
    tmgWave = wave;
}

void GAME_PTR::playWave(char *waveFile) {
    if (tmgWave != NULL) {
        std::stringstream sstr;
        sstr << INSTALL_DIR << name() << "/sounds/" << waveFile;
        tmgWave->play((char *)sstr.str().c_str());
    }
}

void GAME_PTR::playWaveFullPath(char *waveFile) {
    if (tmgWave != NULL) {
        tmgWave->play(waveFile);
    }
}

GtkWidget *GAME_PTR::loadImage(std::string imageFile) {
    std::string str(INSTALL_DIR);
    GtkWidget *widget;
    char cstr[1000];
    
    str += name();
    str += "/images/";
    str += imageFile;
//     std::cout << "loading image: " << str << std::endl;
//     std::cout.flush();
    strcpy(cstr, str.c_str());
    
//     std::cout << "GAME_PTR::loadImage() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    
    widget = gtk_image_new_from_file(cstr);
//     widget = gtk_image_new_from_file(str.c_str());
    
//     std::cout << "GAME_PTR::loadImage() " << __LINE__ << std::endl;
//     std::cout.flush();
    
    return widget;
}

GtkWidget *GAME_PTR::loadMainImage(std::string imageFile) {
    std::string str(INSTALL_DIR);
    str += "images/";
    str += imageFile;
//    std::cout << "Loading image: " << str << std::endl;
    return gtk_image_new_from_file(str.c_str());
}

void GAME_PTR::reloadImage(GtkWidget *widget, std::string imageFile) {
    std::string str(INSTALL_DIR);
    str += name();
    str += "/images/";
    str += imageFile;
//    std::cout << "Loading image: " << str << std::endl;
    gtk_image_set_from_file((GtkImage *)widget, str.c_str());
}

void GAME_PTR::reloadMainImage(GtkWidget *widget, std::string imageFile) {
    std::string str(INSTALL_DIR);
    str += "images/";
    str += imageFile;
//    std::cout << "Loading image: " << str << std::endl;
    gtk_image_set_from_file((GtkImage *)widget, str.c_str());
}

void GAME_PTR::addCommas(std::string &str) {
    std::string strcopy;
    int len = str.size(), rem = len % 3, commas = len / 3;
    int idx1 = 0, idx2 = rem, count3 = -1;
    int neg(0);
    if (str[0] == '-') {
        neg = 1;
        strcopy = str.substr(1, str.size());
        len = strcopy.size();
        rem = len % 3;
        commas = len / 3;
        idx2 = rem;
    }
    else {
        strcopy = str;
    }
    
    
    if (len < 3) {
        return;
    }
    
    str.clear();
    
    if (neg) {
        str += '-';
    }
    
    if (rem) {
        for (idx1 = 0; idx1 < rem; ++idx1) {
            str += strcopy[idx1];
        }
        str += ',';
        ++idx1;
    }
    else {
        --commas;
    }
    
    len += commas;
    for (; idx1 < len; ++idx1) {
        
        if (++count3 == 3) {
            str += ',';
            ++idx1;
            count3 = 0;
        }
        str += strcopy[idx2++];
    }
    
}

std::string GAME_PTR::addCommas(int number) {
    std::stringstream sstr;
    std::string str;
    sstr << number;
    str = sstr.str();
    addCommas(str);
    return str;
}

void GAME_PTR::remCommas(std::string &str) {
    for (unsigned int i = 0; i < str.size(); ++i) {
        if (str[i] == ',') {
            str.erase(i, 1);
            remCommas(str);
            return;
        }
    }
}

// seed rand will call the srandom function with 
// the gettimeofday value
void GAME_PTR::seedRand() {
    timeval tv;
    gettimeofday(&tv, NULL);
#ifndef WIN32
    srandom(tv.tv_usec * 1000 + tv.tv_sec);
#else
    srand(tv.tv_usec * 1000 + tv.tv_sec);
#endif
}

void GAME_PTR::seedRand(bool showSeed) {
    timeval tv;
    gettimeofday(&tv, NULL);
#ifndef WIN32
    srandom(tv.tv_usec * 1000 + tv.tv_sec);
#else
    srand(tv.tv_usec * 1000 + tv.tv_sec);
#endif
    
}

void GAME_PTR::seedRand(unsigned int seed) {
    
#ifndef WIN32
    srandom(seed);
#else
    srand(seed);
#endif
}

// pass the maximum number you want, i.e.
// if you want 0-9, put max in.  This uses
// the little (int)(max * (random() / (RAND_MAX + 1.0)))
// usage from the man page.
int GAME_PTR::getRand(int max) {
#ifdef __unix__
    return (int)((double)max * (random() / (RAND_MAX + 1.0)));
#else
    return (int)((double)max * (rand() / (RAND_MAX + 1.0)));
#endif
}

void GAME_PTR::selectWidget(GtkWidget *widget) {
    selWidget = widget;
}

GtkWidget *GAME_PTR::selectWidget() {
    return selWidget;
}
