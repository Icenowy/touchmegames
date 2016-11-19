
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

#include <tmgdir.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifndef WIN32
#include <sys/vfs.h>
#include <sys/mount.h>
#endif
#include <errno.h>

int TmgDir::printDirectory(std::string path, std::vector<std::string> &directory, 
                           DirOpts opts, std::string extmask) {
    DIR *dp;
    char cwd[4096];
    struct dirent *entry;
    struct stat statbuf;

    if((dp = opendir(path.c_str())) == NULL) {
        return -1;
    }
    
    if (path[path.size() - 1] != '/') {
        path += "/";
    }

    getcwd(cwd, 4096);
    chdir(path.c_str());

    // make sure the user doesn't give us a 
    // vector with data in already
    directory.clear();
#ifndef WIN32    
    // set the bit in opts, rather than checking the size
    // each time
    if (extmask.size()) {
        opts = (DirOpts)(opts | EXT_MASK);
    }
    
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);

        if (S_ISDIR(statbuf.st_mode) && findDirs(opts)) {
            
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                // don't include these, they're in every dir
            }
            else if (fullPath(opts)) {
                directory.push_back(path + std::string(entry->d_name));
            }
            else {
                directory.push_back(entry->d_name);
            }
        }
        else if (findFiles(opts)) {
            if (fullPath(opts)) {
                if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                // don't include these, they're in every dir
                }
                else if (findExt(opts) && std::string(entry->d_name).find(extmask) 
                        < std::string(entry->d_name).size()) {
                    directory.push_back(path + std::string(entry->d_name));
                }
                else {
                    directory.push_back(path + std::string(entry->d_name));
                }
            }
            else {
                if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                // don't include these, they're in every dir
                }
                else if (extmask.size() && std::string(entry->d_name).find(extmask) 
                        < std::string(entry->d_name).size()) {
                    directory.insert(directory.end(), entry->d_name);
                }
                else if (!extmask.size()) {
                    directory.insert(directory.end(), entry->d_name);
                }
            }
        }
    }

    // sort the values
    std::sort(directory.begin(), directory.end());

    // set the directory back to the previous
    chdir(cwd);
    closedir(dp);
#endif // WIN32
    return 0;
}


std::string TmgDir::getExtension(std::string str) {
    int pos(str.size() - 1);
    // start at the back and work front
    while (pos > 0 && str[--pos] != '.');
    if (pos == 0) {
        return "";
    }
    return str.substr(pos, str.size());
}


