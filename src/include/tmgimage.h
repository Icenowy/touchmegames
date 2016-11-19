
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

// The purpose of the TmgImage
// is so that I can do things such as
// get a color at x, y, change a color
// at x, y, scale the image, etc without
// having to deal with the mess everytime.

#ifndef TMG_IMAGE_H
#define TMG_IMAGE_H

#include <config.h> // touchmegames config, for INSTALL_DIR
#include <gtk/gtk.h>
#include <vector>

struct TmgImageDiff {
    unsigned long x;
    unsigned long y;
    unsigned char p1;
    unsigned char p2;
    unsigned char p3;
};

class TmgImage {
private:
    GtkWidget *image;
    GdkPixbuf *pixbuf;
    
    int width, height, rowstride, numChannels;
    guchar *pixels, *p;
    
public:
    TmgImage();
    GtkWidget *widget();
    
    // Treat the path as if you are in INSTALL_DIR,
    // I.E. fotohunt/images/1a.png
    GtkWidget *loadImage(const char *path);
    
    // FP for full path, do not use in the games!!
    // This function helps with loading diffs.
    GtkWidget *loadFPImage(const char *path);
    
    void reloadImage(const char *path);
    
    // regardless of the depth, the color will be 
    // 0x00RRGGBB... for now
    unsigned long getColor(int x, int y);
    void setColor(int x, int y, unsigned long color);
    
    // type must be one of 
    // typedef enum {
    //     GDK_INTERP_NEAREST,
    //     GDK_INTERP_TILES,
    //     GDK_INTERP_BILINEAR,
    //     GDK_INTERP_HYPER
    // } GdkInterpType;
    void scale(int width, int height, int type);
    
    
    void getDifference(TmgImage *comp, std::vector<TmgImageDiff> *diff);
    void applyDifference(std::vector<TmgImageDiff> *diff);
    void applyDifference(const char *path);
    
    void repaint();
};


#endif // TMG_IMAGE_H
