#!/bin/bash

INSTALL_DIR=$1

DIRS=`echo */`


for i in ${DIRS}; do \
    echo Installing: ${i%\/}; \
    mkdir -p $INSTALL_DIR$i
    cd $i
    SUBDIRS=`echo */`
    for j in ${SUBDIRS}; do
        
        mkdir -p $INSTALL_DIR$i$j
        cd $j
        pwd
        cp *.* $INSTALL_DIR$i$j
        cd ..
    done
    cd ..
    cp *.* $INSTALL_DIR$i
    exit
done
(cd images; make install;);
cp touchmegames /usr/bin
test -d /usr/share/applnk/Games || mkdir -p /usr/share/applnk/Games
cp -f touchmegames.desktop /usr/share/applnk/Games