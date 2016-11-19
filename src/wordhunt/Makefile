
# Makefile for wordhunt 

INC=-I../lib -I. -Iwhxmlparser

GENERAL_CFLAGS=-DINSTALL_DIR="\"$(INSTALL_DIR)\""
GTK_CFLAGS = `pkg-config --cflags gtk+-2.0`  -DGTK_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1
GTK_LFLAGS = `pkg-config --cflags --libs gtk+-2.0`  -DGTK_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1
LFLAGS=-lasound
XML_CFLAGS = `xml2-config --cflags`
XML_LFLAGS = `xml2-config --cflags --libs`

GAME_NAME=wordhunt

all: $(GAME_NAME).h events.o interface.o support.o whxmlparser.o lib

lib: events.o interface.o support.o
	ar crv lib$(GAME_NAME).a events.o interface.o support.o whxmlparser/whxmlparser.o
	ranlib lib$(GAME_NAME).a

game: all main.cpp gameptr.o whxmlparser.o
	g++ main.cpp events.o interface.o support.o whxmlparser/whxmlparser.o ../lib/libgameptr.a -o \
	$(GAME_NAME) $(INC) $(GTK_LFLAGS) $(LFLAGS) $(XML_LFLAGS)

debuggame: debug
	g++ main.cpp lib$(GAME_NAME).a -g ../lib/libgameptr.a -o $(GAME_NAME) $(INC) $(GTK_LFLAGS) $(LFLAGS) $(XML_LFLAGS) 

debug: devents.o dinterface.o dsupport.o dwhxmlparser.o
	ar crv lib$(GAME_NAME).a events.o interface.o support.o whxmlparser/whxmlparser.o
	ranlib lib$(GAME_NAME).a

events.o: events.cpp $(GAME_NAME).h
	g++ events.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(XML_CFLAGS)
	
interface.o: interface.cpp $(GAME_NAME).h
	g++ interface.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(XML_CFLAGS)
	
support.o: support.cpp $(GAME_NAME).h
	g++ support.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(XML_CFLAGS)
	
gameptr.o: ../lib/gameptr.cpp ../lib/gameptr.h
	g++ ../lib/gameptr.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(XML_CFLAGS)
	
whxmlparser.o:
	(cd whxmlparser; $(MAKE))

devents.o: events.cpp $(GAME_NAME).h
	g++ events.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(XML_CFLAGS)
	
dinterface.o: interface.cpp $(GAME_NAME).h
	g++ interface.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(XML_CFLAGS)
	
dsupport.o: support.cpp $(GAME_NAME).h
	g++ support.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(XML_CFLAGS)
	
dgameptr.o: ../lib/gameptr.cpp ../lib/gameptr.h
	g++ ../lib/gameptr.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)

dkeypad.o: ../lib/keypad.cpp ../lib/keypad.h
	g++ ../lib/keypad.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)
	
dwhxmlparser.o:
	(cd whxmlparser; $(MAKE) debug);

clean:
	(cd whxmlparser; $(MAKE) clean);
	rm -rf *.o *~ *.a $(GAME_NAME)

install:
	if [ ! -e /usr/local/share/touchmegames/$(GAME_NAME) ]; \
	then mkdir /usr/local/share/touchmegames/$(GAME_NAME); \
	fi
	if [ ! -e /usr/local/share/touchmegames/$(GAME_NAME)/sounds ]; \
	then mkdir /usr/local/share/touchmegames/$(GAME_NAME)/sounds; \
	fi
	if [ ! -e /usr/local/share/touchmegames/$(GAME_NAME)/images ]; \
	then mkdir /usr/local/share/touchmegames/$(GAME_NAME)/images; \
	fi
	if [ ! -e /usr/local/share/touchmegames/$(GAME_NAME)/$(GAME_NAME).scores ]; \
	then touch /usr/local/share/touchmegames/$(GAME_NAME)/$(GAME_NAME).scores; \
	fi
	cp lists /usr/local/share/touchmegames/$(GAME_NAME)
	cp $(GAME_NAME).gif /usr/local/share/touchmegames/$(GAME_NAME)
	cp images/*.* /usr/local/share/touchmegames/$(GAME_NAME)/images
	cp sounds/*.* /usr/local/share/touchmegames/$(GAME_NAME)/sounds
	if [ -e $(GAME_NAME) ]; \
	then cp $(GAME_NAME) /usr/bin; \
	fi

uninstall:
	rm -f /usr/bin/$(GAME_NAME)
