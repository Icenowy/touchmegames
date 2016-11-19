
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

#include <dartlib.h>
#include <errno.h>
 
DartLib::DartLib() : doubleBull(false), isOpen(false), pos(0), haveDart(false), paused(false) {

}

DartLib::~DartLib() {
    closeDart();
}

bool DartLib::readPacket() {
    int ret(usb_interrupt_read(usb, 1, buff, 8, 100));
    if (ret < 0) {
        std::cerr << "Error reading DartBoard in unpause: " << strerror(errno) << std::endl;
        // Should we clean up and exit???
        return false;
    }
    return true;
}

bool DartLib::init() {
    usb = getBoard();
    if (usb != NULL) {
        isOpen = true;
        return true;
    }
    // clear out the current darts
    readPacket();
    readPacket();
    while (buff[0] != 0) {
        readPacket();
    }
    
    return false;
}

bool DartLib::init(char *) {
    return init();
}

void DartLib::closeDart() {
    if (isOpen && usb != NULL) {
        usb_release_interface(usb, 0);
    }
}

void DartLib::pause() {
	paused = true;
}

void DartLib::unpause() {
    if (paused && isOpen && usb != NULL) {
        readPacket();
        readPacket();
        while (buff[0] != 0) {
            readPacket();
        }
    }
    paused = false;
}

bool DartLib::dartBoardPresent() {
    if (usb == NULL) {
        usb = getBoard();
        // we didn't check to see if the board was there before
        if (usb == NULL) {
            return false;
        }
        usb_close(usb);
        usb = NULL;
        return true;
    }
    return true;
}

void DartLib::setDoubleBull(bool on) {
    doubleBull = on;
}

bool DartLib::getDoubleBull() {
    return doubleBull;
}

bool DartLib::gotDart() {
    if (!isOpen) {
            return false;
    }
    
    if (haveDart) { // don't read in anything else until this dart has been cleared
            return true;
    }
    readPacket();
    if (buff[0] != 0) {
        
        haveDart = true;
        return true;
    }
    return false;
}

bool DartLib::readDart(int &number, int &mult) {
    return readDart(number, mult);
}

bool DartLib::readDart(int &number, int &mult, int &buff0, int &buff1, int &buff2) {
    int values[2][10] = {
        { 1, 18, 4, 13, 6, 10, 20, 5, 12, 9  },
        { 7, 19, 3, 17, 2, 15, 16, 8, 11, 14 }
    };
    if (!isOpen || !haveDart) {
            return false;
    }
    number = 0;
    haveDart = false;
    buff0 = buff[0];
    buff1 = buff[1];
    buff2 = buff[2];
    // ok, now decode dart buffer
    if (buff[1] == 9 && buff[2] == 3) {
        // single bull
        mult = 2;
        number = 25;
    }
    else if (buff[1] == 8 && buff[2] == 3) {
        if (doubleBull) {
            mult = 1;
            number = 25;
        }
        else {
            mult = 1;
            number = 50;
        }
    }
    else if (buff[2] == 0 || buff[2] == 6) {
        mult = 3;
    }
    else if (buff[2] == 1 || buff[2] == 5) {
        mult = 2;
    }
    else if (buff[2] == 2 || buff[2] == 4) {
        mult = 1;
    }
    if (buff[2] >= 10 || buff[2] < 0) {
        // this should clear out some of the "junk" darts
        return false;
    }
    if (!number) {
        number = values[buff[2] < 4 ? (int)0: (int)1][(int)buff[1]];
    }
    return true;
}

void DartLib::clear() {
    haveDart = false;
    buff[0] = 0;
    readPacket();
    while (buff[0] != 0) {
        readPacket();
    }
}


usb_dev_handle *DartLib::checkForBoard(struct usb_device *dev, int level) {
    usb_dev_handle *udev;
    int i;

    udev = usb_open(dev);
    if (udev) {
        // Vendor Id for the 320 is 0x10c4
        if (dev->descriptor.idVendor == 0x10c4) {
            if (usb_claim_interface(udev, 0) < 0) {
                perror("Claim");
                usb_close(udev);
                return NULL;
            }
            else {
                return udev;
            }
        }
    }

    usb_close(udev);
	
    for (i = 0; i < dev->num_children; i++) {
        udev = checkForBoard(dev->children[i], level + 1);
        if (udev != NULL) {
            return udev;
        }
    }
	
    return NULL;
}


usb_dev_handle *DartLib::getBoard() {
    struct usb_bus *bus;
    usb_dev_handle *udev;
	
    usb_init();
    usb_find_busses();
    usb_find_devices();
	
    // Now that the USB info is in memory, look for the 320 DK
    for (bus = usb_busses; bus; bus = bus->next) {
        if (bus->root_dev) {
            udev = checkForBoard(bus->root_dev, 0);
            if (udev != NULL) {
                return udev;
            }
        }
        else {
            struct usb_device *dev;
	
            for (dev = bus->devices; dev; dev = dev->next) {
                udev = checkForBoard(dev, 0);
                if (udev != NULL) {
                    return udev;
                }
            }
        }
    }
	
    return NULL;
}

