
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

// support functions for Compile4

#include <compile4.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <sys/time.h>

// german defines

#define NOOFPLAYER 2/* Zwei Spieler */
#define MAXANZAHL 6 /* Maximal 6 Steine pro Reihe */
#define WIN4 4      /* 4er Reihe hat gewonnen */
#define MAXZUG 42   /* Soviele Zuege moeglich */
#define FELD_OFF 10
#define LOWERT 	-999999999L
#define SIEG_WERT 9999999L
#define START_REK 1    // (0) 1:Nur Stellungsbewertung bei Level 1

// end german defines

Compile4::Compile4() {
    bgcolor.red = 0xa400;
    bgcolor.green = 0x0000;
    bgcolor.blue = 0x0000;
}

std::string Compile4::name() const {
    return "compile4";
}

std::string Compile4::imageName() const {
    return "compile4.gif";
}

GdkColor *Compile4::backgroundColor() {
    return &bgcolor;
}


void Compile4::startRound() {
    const char *s1="7777776666666123456654321123456654321";
    const char *s2="0000000000000000000123456000000123456";
    std::stringstream sstr;
    gameState = PLAYER_TURN;
    for (int i = 0; i < 21; ++i) {
        gtk_fixed_move((GtkFixed *)fixed, p1Circs[i], -100, -100);
        gtk_widget_show(p1Circs[i]);
        gtk_fixed_move((GtkFixed *)fixed, p2Circs[i], -100, -100);
        gtk_widget_show(p2Circs[i]);
    }
    for (int x = 0; x < 7; ++x) {
        for (int y = 0; y < 6; ++y) {
            grid[x][y] = 0;
            widgetGrid[x][y] = NULL;
        }
    }

    for (int i = 0; i < strlen(s1); i++) {
        lenofrow[i] = s1[i] - '0';
    }
    for (int i = 0; i < strlen(s2); i++) {
        startofrow[i] = s2[i] - '0';
    }
    ++round;
    sstr << round;
    gtk_label_set_text((GtkLabel *)roundLabel, sstr.str().c_str());
    move = 0;
    timeForMoves = 0;
    gtk_fixed_move((GtkFixed *)fixed, scoreWindow, -400, -400);
}

GtkWidget *Compile4::getHelp() {
    GtkWidget *vbox;
    GtkWidget *label;
    GdkColor color;
    std::stringstream sstr;
    
    color.red = 0xFFFF;
    color.green = 0xFFFF;
    color.blue = 0xFFFF;
    
    vbox = gtk_vbox_new(FALSE, 0);
    
    sstr << "compile4 is a game which you\n"
            << "play against the computer and try to \n"
            << "compile four in a row, horizontally,\n"
            << "vertically, or diagonally.  It has 3\n"
            << "rounds against the computer with each\n"
            << "round harder than the last round.\n"
            << "Points are based on the number of\n"
            << "moves, the outcome of the game, and\n"
            << "how long you take for each turn.";
    
    label = gtk_label_new(sstr.str().c_str());
    gtk_widget_modify_fg(label, (GtkStateType)0, &color);
    gtk_box_pack_start((GtkBox *)vbox, label, FALSE, FALSE, 10);
    return vbox;
}

int Compile4::gameType() const {
    return STRATEGY_GAME;
}


void Compile4::cleanAndShowScore() {
    std::stringstream sstr;
    if (round & 1) { // human went first
        move = (move & 1) ? (move / 2 + 1): (move / 2);
    }
    else {
        move = move / 2;
    }
    
    
    sstr << move;
    gtk_label_set_text((GtkLabel *)moveLabel, sstr.str().c_str());
    sstr.str("");
    
    timeForMoves = (timeForMoves / move) / 20; // cntr @ 20/sec
    
    
    winner = (winner == 1 ? 40: winner == 2 ? 1: 10);
    
    if (winner != 1) {
        move = (19 - (move - 4));
    }
    else {
        move = move - 3;
    }
    sstr << move;
    gtk_label_set_text((GtkLabel *)moveValueLabel, sstr.str().c_str());
    sstr.str("");
    
    sstr << winner;
    gtk_label_set_text((GtkLabel *)winValueLabel, sstr.str().c_str());
    sstr.str("");
    
    if (timeForMoves > 60) {
        timeForMoves = 60;
    }
    else if (timeForMoves< 1) {
        timeForMoves = 1;
    }
    sstr << timeForMoves;
    gtk_label_set_text((GtkLabel *)timePerMoveLabel, sstr.str().c_str());
    sstr.str("");
    
    timeForMoves = 61 - timeForMoves;
    
    sstr << timeForMoves;
    gtk_label_set_text((GtkLabel *)timePerMoveValueLabel, sstr.str().c_str());
    sstr.str("");
    
    gtk_label_set_text((GtkLabel *)totalValue1Label, "0");
    gtk_label_set_text((GtkLabel *)totalValue2Label, "0");
    
    
    sstr << (winner == 40 ? "Win": winner == 10 ? "Tie": "Loss");
    gtk_label_set_text((GtkLabel *)winLabel, sstr.str().c_str());
    gtk_label_set_text((GtkLabel *)topScoreLabel, sstr.str().c_str());
    
    gtk_fixed_move((GtkFixed *)fixed, scorebg, 150, 90);
    gtk_fixed_move((GtkFixed *)fixed, scoreWindow, 170, 110);
    gtk_widget_show(scorebg);
    gtk_widget_show_all(scoreWindow);
    
    total1 = 0;
    total2 = 0;
}

// code below comes from kwin4 by Martin Heni, slightly modified
// for this game.
long Compile4::think(bool hint) {
    int pl;
    int mymove;
    int tmp;
    // player
    pl = 1;
    
    aktzug = move;
    
    // We need all the +1 because the main programm has different defines
    // for the colours. And chaning it here seems not to work....
    //amZug=(Farbe)(tmp + 1);
    amZug = 2;
    
    // beginner
    beginner = round & 1 ? 1: 2;
    
    // second player
    second = round & 1 ? 2: 1;
    
    // level
    mymaxreklev = round * 2 - 1;
    
    for (int y = 0;y <= SIZE_Y_ALL; y++) {
        anzahlmatrix[y] = 0;
    }
    for (int y = 0;y <= SIZE_Y; y++) {
        for (int x = 0;x <= SIZE_X; x++) {
            feldmatrix[y][x] = (FARBE)(y + FELD_OFF);
            feldmatrix[6+x][y] = (FARBE)(y + FELD_OFF);
            feldmatrix[13+x+y][x] = (FARBE)(y + FELD_OFF);
            feldmatrix[30+x-y][x] = (FARBE)(y + FELD_OFF);
        }
    }/* next y */
    
    for (int i = SIZE_Y; i >= 0; i--) {
        for (int j = 0; j <= SIZE_X; j++) {
            char col;
            Farbe colour;
            col = grid[j][i];
            colour = (Farbe)col;
            DoMove(j, colour, feldmatrix, anzahlmatrix);
        }
    }
    
    
    for (int i = 0; i <= SIZE_Y; i++) {
        char tstr[1024];
        tstr[0] = 0;
        for (int j = 0;j <= SIZE_X; j++) {
            sprintf(tstr+strlen(tstr),"%02d ", feldmatrix[i][j]);
        }
    }

    mymove = GetCompMove();
    
    return mymove;
}

/* --------------------------------------------------------------------------- */
/* Computer Routine							       */
/* --------------------------------------------------------------------------- */
int Compile4::GetCompMove() {
    int cmove;
    long cmax,wert;
    int x;
    FARBE lfeld[SIZE_Y_ALL+1][SIZE_X+1];
    char lanzahl[SIZE_Y_ALL+1];
    Farbe farbe;


    farbe = (e_farbe)amZug;
    cmove = -1;		/* Kein Zug */
    cmax = LOWERT;
    for (x = 0; x <= SIZE_X; x++) {
        if (anzahlmatrix[6+x] >= MAXANZAHL) {
            continue;
        }
        memcpy(lanzahl, anzahlmatrix, sizeof(lanzahl));
        memcpy(lfeld, feldmatrix, sizeof(lfeld));

        DoMove(x, farbe, lfeld, lanzahl);
        wert = Wertung(farbe, lfeld, lanzahl, START_REK, aktzug + 1);

        if (wert >= cmax) {
            cmax = wert;
            cmove = x;
            if (cmax >= SIEG_WERT) {
                break;
            }
        }
    } /*next x*/
    aktwert = cmax;
    amZug = farbe;	// Wertung changes amZug !!!
    return cmove;
}/* end getCompMove */

/* ---------------------------------------------------------------- */
long Compile4::Wertung(Farbe farbe,FARBE feld[][SIZE_X+1],char anzahl[],int reklev,int zug) {
    static long gaus[]={10,50,300,500,300,50,10};
    FARBE lfeld[SIZE_Y_ALL+1][SIZE_X+1];
    char lanzahl[SIZE_Y_ALL+1];
    long max,wert;
    int x;
    Farbe winner;
    
    winner = (e_farbe)GameOver(feld, anzahl);
    if (winner != Niemand) {
        if (winner == farbe) {
            return (SIEG_WERT);
        }
        else {
            return (-SIEG_WERT);
        }
    }
    if (zug >= MAXZUG) {
        return(0); /* Remis */
    }
    if (reklev >= mymaxreklev) {
        return Bewertung(farbe, feld);
    }


    farbe = (Farbe)SwitchPlayer(farbe);
    
    max = LOWERT;
    for (x = 0;x <= SIZE_X; x++) {
        if (anzahl[6+x] >= MAXANZAHL) continue;
        memcpy(lfeld, feld, sizeof(lfeld));
        memcpy(lanzahl, anzahl, sizeof(lanzahl));
        DoMove(x, farbe, lfeld, lanzahl);
        wert = Wertung(farbe, lfeld, lanzahl, reklev + 1, zug + 1) + gaus[x];
        if (wert >= max) {
            max = wert;
            if (max >= SIEG_WERT) break;
        }
    }/*next x*/
    return(-max);
}/*end wertung*/
/* ---------------------------------------------------------------- */
long Compile4::Bewertung(Farbe farbe,FARBE feld[][SIZE_X+1]) {
    /* Abstand:              0    1    2    3    4    5    */
    static long myWERT[]={2200,600, 300,  75,  20,   0};
    //static long myWERT[]={0,0,0,0,0,0};
    /* Wieviele von Farbe:       0     1     2     3    4 */
    static long steinWERT[4][5]= {
            {     0,  500L, 40000L,200000L,SIEG_WERT}, // Leerfelder=0
            {     0,  500L,  8000L, 40000L,SIEG_WERT}, //           =1
            {     0,   00L,  4000L, 25000L,SIEG_WERT}, //			=2
            {     0,   00L,  2000L, 12500L,SIEG_WERT}, //           =3
    };
    long gelb_wert,rot_wert,wert;
    int cntcol,cnt;
    Farbe color;
    FARBE field;
    int y,i,j;
    
    gelb_wert = getRand(2501);
    rot_wert = getRand(2501);
    for (y=0;y<=SIZE_Y_ALL;y++) {
        if (lenofrow[y]<WIN4) { 
            continue;
        }
        for (i=0;i<=(lenofrow[y]-WIN4);i++) {
            color=Niemand;
            wert=0;
            cntcol=0;
            cnt=0;
            for (j=0;j<WIN4;j++) {
                field=feld[y][i+j+startofrow[y]];
                if ((Farbe)field==Rot) {
                    if (color==Gelb) {
                        color=Niemand;
                        break;
                    }
                    cntcol++;
                    color=Rot;
                }
                else if ((Farbe)field==Gelb) {
                    if (color == Rot) {
                        color=Niemand;
                        break;
                    }
                    cntcol++;
                    color=Gelb;
                }
                else {
                    cnt+=field-FELD_OFF;
                    wert+=myWERT[field-FELD_OFF];
                }
            }/*next j */
            if (cnt>3) {
                cnt=3;
            }
            if (color==Rot) {
                rot_wert+=(wert+steinWERT[cnt][cntcol]);
            }
            else if (color==Gelb) {
                gelb_wert+=(wert+steinWERT[cnt][cntcol]);
            }
        }/*next i*/
    }/*next y*/
    if (farbe==Rot) {
        wert=rot_wert-gelb_wert;
    }
    else {
        wert=gelb_wert-rot_wert;
    }
    return(wert);
}/* end bewertung */
/* ---------------------------------------------------------------- */

char Compile4::GameOver(FARBE feld[][SIZE_X+1],char anzahl[]) {
    Farbe thiscolor,field;
    int x,y,cnt;
    for (y=0;y<=SIZE_Y_ALL;y++) {
        if (anzahl[y]<WIN4) {
            continue;
        }
        if ( lenofrow[y]<WIN4 ) {
            continue;
        }
        cnt=0;
        thiscolor=Niemand;
        for (x=0;x<lenofrow[y];x++) {
            field=(Farbe)feld[y][x+startofrow[y]];
            if (field==thiscolor) {
                cnt++;
            }
            else {
                cnt=1;
                thiscolor=field;
            }
            if ( (cnt>=WIN4)&&( (thiscolor==Gelb)||(thiscolor==Rot) ) ) {
                return(thiscolor);
            }
        }/*next x */
    }/*next y*/
    return(Niemand);
}/* end gameOver */

char Compile4::roundOver() {
    int cnt(0);
    char color(0);
            
    // up and down
    for (int x = 0; x <= SIZE_X; ++x) {
        for (int y = 0; y <= SIZE_Y; ++y) {
            if (color != grid[x][y]) {
                cnt = 1;
                blinky[cnt - 1][0] = x;
                blinky[cnt - 1][1] = y;
                color = grid[x][y];
            }
            else {
                blinky[cnt][0] = x;
                blinky[cnt][1] = y;
                if (++cnt == 4 && color != 0) {
                    return color;
                }
                else if (color == 0) {
                    cnt = 0;
                }
            }
        }
            cnt = 0;
    }
    // left to right
    for (int y = 0; y <= SIZE_Y; ++y) {
        for (int x = 0; x <= SIZE_X; ++x) {
            if (color != grid[x][y]) {
                cnt = 1;
                blinky[cnt - 1][0] = x;
                blinky[cnt - 1][1] = y;
                color = grid[x][y];
            }
            else {
                blinky[cnt][0] = x;
                blinky[cnt][1] = y;
                if (++cnt == 4 && color != 0) {
                        
                    return color;
                }
                else if (color == 0) {
                    cnt = 0;
                }
            }
        }
        cnt = 0;
    }
    color = 0;
    // diagonal down right
    for (int x = 0; x <= 4; ++x) {
        for (int y = 0; y < 3; ++y) {
            int x1(x);
            for (int y1 = y; y1 <= SIZE_Y; ++y1) {
                
                blinky[cnt][0] = x1;
                blinky[cnt][1] = y1;
                
                
                if (x1 > SIZE_X) {
                    y1 = SIZE_Y + 1;
                }
                else if (color != grid[x1][y1]) {
                    cnt = 1;
                    color = grid[x1][y1];
                    blinky[cnt - 1][0] = x1;
                    blinky[cnt - 1][1] = y1;
                }
                else if (++cnt == 4 && color != 0) {
                        
                    return color;
                }
                else if (color == 0) {
                    cnt = 0;
                }
                ++x1;
            }
            cnt = 0;
            color = 0;
        }
    }
    // diagonal down left
    for (int x = SIZE_X; x > 2; --x) {
        for (int y = 0; y < 3; ++y) {
            int x1(x);
            for (int y1 = y; y1 <= SIZE_Y && x1 >= 0; ++y1) {
                    
                    
                blinky[cnt][0] = x1;
                blinky[cnt][1] = y1;
                
                if (x1 < 0) {
                    y1 = SIZE_Y + 1;
                }
                else if (color != grid[x1][y1]) {
                    cnt = 1;
                    color = grid[x1][y1];
                    blinky[cnt - 1][0] = x1;
                    blinky[cnt - 1][1] = y1;
                    
                }
                else if (++cnt == 4 && color != 0) {
                    
                    return color;
                }
                else if (color == 0) {
                    cnt = 0;
                }
                --x1;
            }
            cnt = 0;
            color = 0;
        }
    }
    
    // check tie
    for (int x = 0; x <= SIZE_X; ++x) {
        for (int y = 0; y < SIZE_Y; ++y) {
            if (grid[x][y] == 0) {
                return 0;
            }
        }
    }
    return -1;
}

char Compile4::SwitchPlayer(Farbe m_amZug) {
    if (m_amZug==Niemand) {
        m_amZug=(Farbe)amZug;
    }
    if (m_amZug==Rot) {
        amZug=Gelb;
    }
    else if (m_amZug==Gelb) {
        amZug=Rot;
    }
    else {
        amZug=beginner;
    }
    return amZug;
}

void Compile4::DoMove(char move,Farbe farbe,FARBE feld[][SIZE_X+1],char anzahl[]) {
    int x,i,y;

    if (farbe==Tip || farbe==Niemand) {
        return ;  // no real move
    }
    x=move;
    y=anzahl[6+move];
    feld[y][x]=farbe;


    feld[6+x][y]=farbe;
    feld[13+x+y][x]=farbe;
    feld[30+x-y][x]=farbe;
    anzahl[y]++;
    anzahl[6+x]++;
    anzahl[13+x+y]++;
    anzahl[30+x-y]++;
    for (i=y+1;i<=SIZE_Y;i++) {
        feld[i][x]--;
        feld[6+x][i]--;
        feld[13+x+i][x]--;
        feld[30+x-i][x]--;
    }
}/* end doMove */

