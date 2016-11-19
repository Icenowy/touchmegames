
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

// support functions for FLOG_THE_DOLPHIN

#include <flogthedolphin.h>
#include <sys/time.h>
#include <sstream>

FLOG_THE_DOLPHIN::FLOG_THE_DOLPHIN()  {
    bgcolor.red = 0xFFFF;
    bgcolor.green = 0xFFFF;
    bgcolor.blue = 0xFFFF;
    soundOn = true;
}

FLOG_THE_DOLPHIN::~FLOG_THE_DOLPHIN() {
    
}

std::string FLOG_THE_DOLPHIN::name() const {
    return "flogthedolphin";
}

std::string FLOG_THE_DOLPHIN::imageName() const {
    return "flogthedolphin.gif";
}

GdkColor *FLOG_THE_DOLPHIN::backgroundColor() {
    return &bgcolor;
}

GtkWidget *FLOG_THE_DOLPHIN::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "FlogTheDolphin is a game where you get to\n"
                    << "flog the dolphin.  At the start, the dolphin\n"
                    << "jumps up and down on a raft.  When you touch\n"
                    << "or click on the screen, the dolphin will go\n"
                    << "flying to the right.  Watch when you flog him\n"
                    << "though, some times are better than others.\n\n"
                    << "Chair rafts will make him go faster and higher\n"
                    << "and flat rafts will make him go sky high.  Just\n"
                    << "watch out for the buoys, as they tend to give\n"
                    << "him a headache.";
    
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int FLOG_THE_DOLPHIN::gameType() const {
    return STRATEGY_GAME;
}

void FLOG_THE_DOLPHIN::startRound() {
    timeval tv;
    struct timezone tz;
    std::stringstream sstr;
    //GAME_PTR::loadScores(scores);
    tz.tz_minuteswest = 0;
    tz.tz_dsttime = 0;

    
    gtk_label_set_text((GtkLabel *)currentLabel, "0");
    gettimeofday(&tv, &tz);
    seedRand();
    
    gtk_fixed_move(GTK_FIXED(fixed), bgImage1, 0, 400);
    bgImage1x = 0;
    gtk_fixed_move(GTK_FIXED(fixed), bgImage2, 1200, 400);
    bgImage2x = 1200;
    
    gtk_fixed_move(GTK_FIXED(fixed), raftImage1, 100, 567);
    //raft1x = -120;
    raft1hit = false;
    
    gtk_fixed_move(GTK_FIXED(fixed), raftImage2, 1200, 550);
    raft2x = -120;
    raft2hit = false;
    
    gtk_fixed_move(GTK_FIXED(fixed), chairImage, 1200, 500);
    chairx = -120;
    chairhit = false;
    
    gtk_fixed_move(GTK_FIXED(fixed), buoyImage, 1200, 467);
    buoyx = -120;
    
    newhvelocity = 0;
    rate = 15;
    hvelocity = 50;//1000;
    dolphinTilt = 0;
    initialy = 0;
    angle = 45;
    t = 0;
    gameState = startOfRound;
    currentDolphin = 0;
    dolphinTiltReload = 0.142727;
    length = 0;
}


