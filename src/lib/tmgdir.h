

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
 *	Foundation, Inc., 59 Temple Place, Suite 328, Boston, MA  02111-1287  USA
 */

#ifndef TMG_DIR_H
#define TMG_DIR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>


class TmgDir {
    
    public:
        enum DirOpts {
            NONE =          0x00,
            FULL_PATH =     0x01,
            EXT_MASK =      0x02,
            FILES_ONLY =    0x04,
            DIRS_ONLY  =    0x08,
            FILES_DIRS =    0x0C,
        };
        
        // contains all functionality
        int printDirectory(std::string path, std::vector<std::string> &directory, 
                           DirOpts opts = NONE, std::string extmask = "");
        
        std::string getExtension(std::string str);
        
    private:
        bool findFiles(DirOpts opts) {
            return opts & FILES_ONLY;
        }
        bool findDirs(DirOpts opts) {
            return opts & DIRS_ONLY;
        }
        bool findExt(DirOpts opts) {
            return opts & EXT_MASK;
        }
        bool fullPath(DirOpts opts) {
            return opts & FULL_PATH;
        }
        
};



#endif // TMG_DIR_H
