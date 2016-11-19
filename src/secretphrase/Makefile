
# Makefile for secretphrase 

INC=-I../lib -I. -Ixmlparser

GENERAL_CFLAGS=-DINSTALL_DIR="\"$(INSTALL_DIR)\""
GTK_CFLAGS = `pkg-config --cflags gtk+-2.0`  -DGTK_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1
GTK_LFLAGS = `pkg-config --cflags --libs gtk+-2.0`  -DGTK_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1
XML_CFLAGS = `xml2-config --cflags`
XML_LFLAGS = `xml2-config --cflags --libs`

GAME_NAME=secretphrase

all: $(GAME_NAME).h events.o interface.o support.o xmlparser.o lib

lib:
	ar crv lib$(GAME_NAME).a events.o interface.o support.o xmlparser/xmlparser.o
	ranlib lib$(GAME_NAME).a

game: all main.cpp gameptr.o keypad.o altprogressbar.o
	g++ main.cpp events.o interface.o support.o gameptr.o keypad.o altprogressbar.o xmlparser/xmlparser.o -o $(GAME_NAME) $(INC) $(GTK_LFLAGS) $(XML_LFLAGS)

debug: devents.o dinterface.o dsupport.o 
	(cd xmlparser; $(MAKE) "INSTALL_DIR=${INSTALL_DIR}" debug;)
	ar crv lib$(GAME_NAME).a events.o interface.o support.o xmlparser/xmlparser.o
	ranlib lib$(GAME_NAME).a

events.o: events.cpp $(GAME_NAME).h
	g++ events.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(XML_CFLAGS)
	
interface.o: interface.cpp $(GAME_NAME).h
	g++ interface.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(XML_CFLAGS)
	
support.o: support.cpp $(GAME_NAME).h
	g++ support.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(XML_CFLAGS)
	
gameptr.o: ../lib/gameptr.cpp ../lib/gameptr.h
	g++ ../lib/gameptr.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)

keypad.o: ../lib/keypad.cpp ../lib/keypad.h
	g++ ../lib/keypad.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)

altprogressbar.o: ../lib/altprogressbar.cpp ../lib/altprogressbar.h
	g++ ../lib/altprogressbar.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)

xmlparser.o:
	(cd xmlparser; $(MAKE) "INSTALL_DIR=${INSTALL_DIR}")

devents.o: events.cpp $(GAME_NAME).h
	g++ events.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)  $(XML_CFLAGS)
	
dinterface.o: interface.cpp $(GAME_NAME).h
	g++ interface.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)  $(XML_CFLAGS)
	
dsupport.o: support.cpp $(GAME_NAME).h
	g++ support.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)  $(XML_CFLAGS)
	
dgameptr.o: ../lib/gameptr.cpp ../lib/gameptr.h
	g++ ../lib/gameptr.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)

dkeypad.o: ../lib/keypad.cpp ../lib/keypad.h
	g++ ../lib/keypad.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)

clean:
	rm -rf *.o *~ *.a $(GAME_NAME)
	(cd xmlparser; $(MAKE) clean;)
	(cd phrasegenerator; $(MAKE) clean;)

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
	if [ ! -e /usr/local/share/touchmegames/$(GAME_NAME)/phrases ]; \
	then mkdir /usr/local/share/touchmegames/$(GAME_NAME)/phrases; \
	fi
	cp $(GAME_NAME).gif /usr/local/share/touchmegames/$(GAME_NAME)
	cp images/*.* /usr/local/share/touchmegames/$(GAME_NAME)/images
	cp phrases/*.* /usr/local/share/touchmegames/$(GAME_NAME)/phrases
	if [ -e $(GAME_NAME) ]; \
	then cp $(GAME_NAME) /usr/bin; \
	fi

uninstall:
	rm -f /usr/bin/$(GAME_NAME)
