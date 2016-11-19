
# Makefile for consume21

INC=-I../lib -I.

GENERAL_CFLAGS=-DINSTALL_DIR="\"$(INSTALL_DIR)\""
GTK_CFLAGS = `pkg-config --cflags gtk+-2.0` -DGTK_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1
GTK_LFLAGS = `pkg-config --cflags --libs gtk+-2.0` -DGTK_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1

GAME_NAME=consume21

all: $(GAME_NAME).h events.o interface.o support.o lib

lib: events.o interface.o support.o
	ar crv lib$(GAME_NAME).a events.o interface.o support.o
	ranlib lib$(GAME_NAME).a

game: all main.cpp gameptr.o keypad.o altprogressbar.o
	g++ main.cpp events.o interface.o support.o gameptr.o altprogressbar.o keypad.o -o $(GAME_NAME) $(INC) $(GTK_LFLAGS)

debug: devents.o dinterface.o dsupport.o 
	ar crv lib$(GAME_NAME).a events.o interface.o support.o
	ranlib lib$(GAME_NAME).a

events.o: events.cpp $(GAME_NAME).h
	g++ events.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)
	
interface.o: interface.cpp $(GAME_NAME).h
	g++ interface.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)
	
support.o: support.cpp $(GAME_NAME).h
	g++ support.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)
	
gameptr.o: ../lib/gameptr.cpp ../lib/gameptr.h
	g++ ../lib/gameptr.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)

keypad.o: ../lib/keypad.cpp ../lib/keypad.h
	g++ ../lib/keypad.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)
	
altprogressbar.o: ../lib/altprogressbar.cpp ../lib/altprogressbar.h
	g++ ../lib/altprogressbar.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)
	
devents.o: events.cpp $(GAME_NAME).h
	g++ events.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)
	
dinterface.o: interface.cpp $(GAME_NAME).h
	g++ interface.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)
	
dsupport.o: support.cpp $(GAME_NAME).h
	g++ support.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)
	
dgameptr.o: ../lib/gameptr.cpp ../lib/gameptr.h
	g++ ../lib/gameptr.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)

dkeypad.o: ../lib/keypad.cpp ../lib/keypad.h
	g++ ../lib/keypad.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)


clean:
	rm -rf *.o *~ *.a $(GAME_NAME)

install:
	if [ ! -e /usr/local/share/touchmegames/$(GAME_NAME) ]; \
	then mkdir /usr/local/share/touchmegames/$(GAME_NAME); \
	fi
	if [ ! -e /usr/local/share/touchmegames/$(GAME_NAME)/images ]; \
	then mkdir /usr/local/share/touchmegames/$(GAME_NAME)/images; \
	fi
	if [ ! -e /usr/local/share/touchmegames/$(GAME_NAME)/$(GAME_NAME).scores ]; \
	then touch /usr/local/share/touchmegames/$(GAME_NAME)/$(GAME_NAME).scores; \
	fi
	cp $(GAME_NAME).gif /usr/local/share/touchmegames/$(GAME_NAME)
	cp images/*.* /usr/local/share/touchmegames/$(GAME_NAME)/images
	if [ -e $(GAME_NAME) ]; \
	then cp $(GAME_NAME) /usr/bin; \
	fi

uninstall:
	rm -f /usr/bin/$(GAME_NAME)
