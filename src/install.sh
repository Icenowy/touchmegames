#!/bin/bash
# Main install script for TouchMe Games

INSTALL_DIR_STR=`grep INSTALL_DIR lib/config.h | head -n 1 | awk '{ print $3 }'`
INSTALL_DIR=${INSTALL_DIR_STR%\"*}

GAMES=`echo */`

if [ ! -e touchmegames ]; then make; fi
test -d ${INSTALL_DIR:1} || mkdir -p ${INSTALL_DIR:1}

echo installDir: ${INSTALL_DIR:1}

for i in ${GAMES}; do \
    if [ "$i" != "CVS/" ]; then
        echo Installing: ${i%\/}; \
        cd $i; ../installgame.sh ${INSTALL_DIR:1} ${i%\/};
        test -e install.sh && ./install.sh ${INSTALL_DIR:1} ${i%\/} > /dev/null
        cd ..; \
    fi
done
# (cd images; make install;);
cd images; ./install.sh ${INSTALL_DIR:1}; cd ..
cp touchmegames /usr/bin
test -d /usr/share/applnk/Games || mkdir -p /usr/share/applnk/Games

head -n 12 touchmegames.desktop > /usr/share/applnk/Games/touchmegames.desktop
echo Icon=${INSTALL_DIR:1}images/tmgicon.png >> /usr/share/applnk/Games/touchmegames.desktop
