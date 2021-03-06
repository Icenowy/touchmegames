
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

#include <iostream>
#include <fstream>
#include <cstring>
#include <tmgimage.h>

TmgImage::TmgImage() : image(NULL), pixbuf(NULL) {

}

GtkWidget *TmgImage::widget() {
    return image;
}

GtkWidget *TmgImage::loadImage(const char *path) {
    
    std::string str(INSTALL_DIR);
    str += path;
    image = gtk_image_new_from_file(str.c_str());
    
    if (gtk_image_get_storage_type((GtkImage *)image) == GTK_IMAGE_PIXBUF) {
    
        pixbuf = gtk_image_get_pixbuf((GtkImage *)image);
        if (pixbuf == NULL) {
            std::cout << "Could not get pixbuf from image!!" << std::endl;
        }
        else {
            numChannels = gdk_pixbuf_get_n_channels(pixbuf);

            g_assert(gdk_pixbuf_get_colorspace(pixbuf) == GDK_COLORSPACE_RGB);
            g_assert(gdk_pixbuf_get_bits_per_sample(pixbuf) == 8);
        
            width = gdk_pixbuf_get_width (pixbuf);
            height = gdk_pixbuf_get_height (pixbuf);
        
            rowstride = gdk_pixbuf_get_rowstride (pixbuf);
            pixels = gdk_pixbuf_get_pixels (pixbuf);
        }
    }
    else {
        std::cerr << "TmgImage Error: Image storage type != GTK_IMAGE_PIXBUF" << std::endl;
        std::cerr << "\tSome features of TmgImage will not be available" << std::endl;
    }
    return image;
}

// FP for full path, do not use in the games!!
// This function helps with loading diffs.
GtkWidget *TmgImage::loadFPImage(const char *path) {
    std::cout << "TmgImage::loadFPImage Warning: This function should not be used in games" << std::endl;
    if (image == NULL) {
        image = gtk_image_new_from_file(path);
        
        if (gtk_image_get_storage_type((GtkImage *)image) == GTK_IMAGE_PIXBUF) {
        
            pixbuf = gtk_image_get_pixbuf((GtkImage *)image);
            if (pixbuf == NULL) {
                std::cout << "Could not get pixbuf from image!!" << std::endl;
            }
            else {
                numChannels = gdk_pixbuf_get_n_channels(pixbuf);

                g_assert(gdk_pixbuf_get_colorspace(pixbuf) == GDK_COLORSPACE_RGB);
                g_assert(gdk_pixbuf_get_bits_per_sample(pixbuf) == 8);
            
                width = gdk_pixbuf_get_width (pixbuf);
                height = gdk_pixbuf_get_height (pixbuf);
            
                rowstride = gdk_pixbuf_get_rowstride (pixbuf);
                pixels = gdk_pixbuf_get_pixels (pixbuf);
            }
        }
        else {
            std::cerr << "TmgImage Error: Image storage type != GTK_IMAGE_PIXBUF" << std::endl;
            std::cerr << "\tSome features of TmgImage will not be available" << std::endl;
        }
    }
    return NULL;
}

void TmgImage::reloadImage(const char *path) {
    std::string str(INSTALL_DIR);
    str += path;
    gtk_image_set_from_file((GtkImage *)image, str.c_str());
        
    if (gtk_image_get_storage_type((GtkImage *)image) == GTK_IMAGE_PIXBUF) {
        
        pixbuf = gtk_image_get_pixbuf((GtkImage *)image);
        if (pixbuf == NULL) {
            std::cout << "Could not get pixbuf from image!!" << std::endl;
        }
        else {
            numChannels = gdk_pixbuf_get_n_channels(pixbuf);

            g_assert(gdk_pixbuf_get_colorspace(pixbuf) == GDK_COLORSPACE_RGB);
            g_assert(gdk_pixbuf_get_bits_per_sample(pixbuf) == 8);
            
            width = gdk_pixbuf_get_width (pixbuf);
            height = gdk_pixbuf_get_height (pixbuf);
            
            rowstride = gdk_pixbuf_get_rowstride (pixbuf);
            pixels = gdk_pixbuf_get_pixels (pixbuf);
        }
    }
    else {
        std::cerr << "TmgImage Error: Image storage type != GTK_IMAGE_PIXBUF" << std::endl;
        std::cerr << "\tSome features of TmgImage will not be available" << std::endl;
    }
}

// regardless of the depth, the color will be 
// 0x00RRGGBB... for now
unsigned long TmgImage::getColor(int x, int y) {
    g_assert(x >= 0 && x < width);
    g_assert(y >= 0 && y < height);
            
    rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);

    p = pixels + y * rowstride + x * numChannels;

    return (((unsigned long)p[0]) << 16) | (((unsigned long)p[1]) << 8) | (unsigned long)p[2];
}

void TmgImage::setColor(int x, int y, unsigned long color) {
    g_assert(x >= 0 && x < width);
    g_assert(y >= 0 && y < height);
            
    rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);

    p = pixels + y * rowstride + x * numChannels;
    p[0] = color >> 16;
    p[1] = (color >> 8) & 0xFF;
    p[2] = color & 0xFF;
}


void TmgImage::scale(int width, int height, int type) {
    
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, width, height, (GdkInterpType)type);
    
    gtk_image_set_from_pixbuf((GtkImage *)image, pixbuf);
    
    numChannels = gdk_pixbuf_get_n_channels(pixbuf);

    g_assert(gdk_pixbuf_get_colorspace(pixbuf) == GDK_COLORSPACE_RGB);
    g_assert(gdk_pixbuf_get_bits_per_sample(pixbuf) == 8);
            
    this->width = gdk_pixbuf_get_width(pixbuf);
    this->height = gdk_pixbuf_get_height(pixbuf);

            
    rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);
}

void TmgImage::getDifference(TmgImage *comp, std::vector<TmgImageDiff> *diff) {
    if (width != comp->width) {
        std::cerr << "TmgImage::getDifference error: width is not the same" << std::endl;
        return;
    }
    if (height != comp->height) {
        std::cerr << "TmgImage::getDifference error: width is not the same" << std::endl;
        return;
    }
    
    for (long x = 0; x < width; ++x) {
        for (long y = 0; y < height; ++y) {
            p = pixels + y * rowstride + x * numChannels;
            comp->p = comp->pixels + y * rowstride + x * numChannels;
            if (memcmp(p, comp->p, 3) != 0) {
                TmgImageDiff d;
                std::cout << "We have a diff at: " << x << ' ' << y << std::endl;
                d.x = x;
                d.y = y;
                d.p1 = comp->p[0];
                d.p2 = comp->p[1];
                d.p3 = comp->p[2];
                diff->push_back(d);
            }
        }
    }
}

void TmgImage::applyDifference(std::vector<TmgImageDiff> *diff) {
    for (unsigned int idx = 0; idx < diff->size(); ++idx) {
        TmgImageDiff d = (*diff)[idx];
        
        p = pixels + d.y * rowstride + d.x * numChannels;
        p[0] = d.p1;
        p[1] = d.p2;
        p[2] = d.p3;
    }
}

void TmgImage::applyDifference(const char *path) {
    std::vector<TmgImageDiff> diff;
    std::ifstream is;
    TmgImageDiff d;
    
    is.open(path);
    if (!is) {
        std::cerr << "TmgImage::applyDifference error: could not open " << path << std::endl;
        return;
    }
    
    while (is.read((char *)&d, sizeof(TmgImageDiff))) {
        diff.push_back(d);
    }
    
    applyDifference(&diff);
    
    is.close();
}

void TmgImage::repaint() {
    gtk_image_set_from_pixbuf((GtkImage *)image, pixbuf);
}


