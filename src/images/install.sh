#!/bin/bash
# Install script for images

INSTALL_DIR=$1

DIRS=`echo */`


for i in ${DIRS}; do \
    if [ "$i" != "CVS/" ]; then
        echo Installing: ${i%\/}; \
        mkdir -p ${INSTALL_DIR}images/$i
        cd $i
        SUBDIRS=`echo */`
        for j in ${SUBDIRS}; do
            if [ "$j" != "CVS/" ]; then
                if [ "`echo */`" != "*/" ]; then
                    mkdir -p ${INSTALL_DIR}images/$i$j
                    cd $j
                    pwd
                    cp *.png *.gif *.xpm *.bmp ${INSTALL_DIR}images/$i$j > /dev/null 2>&1
                    cd ..
                fi
            fi
        done
        
        echo Installing images into ${INSTALL_DIR}images/$i
        cp *.png *.gif *.xpm *.bmp ${INSTALL_DIR}images/$i > /dev/null 2>&1
        cd ..
    fi
done

cp *.png *.gif *.xpm *.bmp ${INSTALL_DIR}images > /dev/null 2>&1
