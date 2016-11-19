#!/bin/bash

INSTALL_DIR=$1
GAME_NAME=$2

echo Install Dir: $INSTALL_DIR
echo Game Name: $GAME_NAME

if [ ! -e $INSTALL_DIR$GAME_NAME ]; \
    then mkdir $INSTALL_DIR$GAME_NAME; \
fi
if [ ! -e $INSTALL_DIR$GAME_NAME/sounds ]; \
    then mkdir $INSTALL_DIR$GAME_NAME/sounds; \
fi
if [ ! -e $INSTALL_DIR$GAME_NAME/images ]; \
    then mkdir $INSTALL_DIR$GAME_NAME/images; \
fi
if [ ! -e $INSTALL_DIR$GAME_NAME.scores ]; \
    then touch $INSTALL_DIR$GAME_NAME.scores; \
fi
cp $GAME_NAME.gif $GAME_NAME.png $INSTALL_DIR$GAME_NAME > /dev/null 2>&1
cp images/*.* $INSTALL_DIR$GAME_NAME/images
if [ -e $GAME_NAME ]; \
then cp $GAME_NAME /usr/bin; \
fi