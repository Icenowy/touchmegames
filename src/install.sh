#!/bin/bash
# Main install script for TouchMe Games

GAMES=`echo */`

if [ ! -e touchmegames ]; then make; fi
test -d ${INSTALL_DIR} || mkdir -p ${INSTALL_DIR}

echo installDir: ${INSTALL_DIR}

for i in ${GAMES}; do \
    if [ "$i" != "CVS/" ]; then
        echo Installing: ${i%\/}; \
        cd $i; ../installgame.sh ${INSTALL_DIR} ${i%\/};
        test -e install.sh && ./install.sh ${INSTALL_DIR} ${i%\/} > /dev/null
        cd ..; \
    fi
done
# (cd images; make install;);
cd images; ./install.sh ${INSTALL_DIR}; cd ..
test -d ${PREFIX}/bin || mkdir -p ${PREFIX}/bin
cp touchmegames ${PREFIX}/bin/
test -d ${PREFIX}/share/applications || mkdir -p ${PREFIX}/share/applications

head -n 12 touchmegames.desktop > ${PREFIX}/share/applications/touchmegames.desktop
echo Icon=${INSTALL_DIR}images/tmgicon.png >> ${PREFIX}/share/applications/touchmegames.desktop
