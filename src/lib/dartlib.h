
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
 
#ifndef DART_LIB_H
#define DART_LIB_H

#include <string>
#include <iostream>

#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <usb.h>



// To use this library
// 1.  init
// 2.  set the double bull, when you set it on(true) single bulls count as 25
// 3.  poll the class for darts, gotDart
// 	gotDart will return true after a dart has been received until readDart has been called
// 4.  read the dart.  readDart returns true if gotDart is true


// protocol
// Byte 1: new dart? 0 = false, else true
// the next two bytes will be the dart information
//


class DartLib {
private:
    usb_dev_handle *usb;
    bool doubleBull;
    bool isOpen;
    char buff[64]; // no allowing for overflow
    int pos;
    bool haveDart;
    bool paused;

    usb_dev_handle *checkForBoard(struct usb_device *dev, int level);
    usb_dev_handle *getBoard();

    bool readPacket();
public:
    DartLib();
    ~DartLib();
    bool init();
    bool init(char *); // backwards compatibility
    bool dartBoardPresent();
    void closeDart();
    void pause();
    void unpause();
    void setDoubleBull(bool on);
    bool getDoubleBull();
    bool gotDart();
    bool readDart(int &number, int &mult);
    bool readDart(int &number, int &mult, int &buff0, int &buff1, int &buff2);
    void clear(); // clears any info in queue.
};


#endif // DART_LIB_H
