#!/bin/bash
# Main install script for TouchMe Games

INSTALL_ARRAY=(`grep INSTALL_DIR lib/config.h | awk '{ print $3 }' | sed 's/\"//g'`)


rm -f /usr/bin/touchmegames
rm -rf ${INSTALL_ARRAY[0]}
rm -f /usr/share/applnk/Games/touchmegames

