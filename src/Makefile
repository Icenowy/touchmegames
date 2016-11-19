
# Add your game to this line
GAMES=lib gorillathrilla videopoker flogthedolphin wordhunt threescompany tritowers compile4 foxyboxy secretphrase consume21 sudoku dropzone fotoseek nonogram fortresscrackers memory
#dart01 dartCountUp darttictactoe dartlowball dartcountdown dartcricket dartcricket10

GAME_LIBS=lib/libgameptr.a gorillathrilla/libgorillathrilla.a \
	videopoker/libvideopoker.a flogthedolphin/libflogthedolphin.a wordhunt/libwordhunt.a threescompany/libthreescompany.a \
	tritowers/libtritowers.a compile4/libcompile4.a foxyboxy/libfoxyboxy.a secretphrase/libsecretphrase.a \
	consume21/libconsume21.a sudoku/libsudoku.a dropzone/libdropzone.a fotoseek/libfotoseek.a nonogram/libnonogram.a \
	fortresscrackers/libfortresscrackers.a memory/libmemory.a
	
#dart01/libdart01.a dartCountUp/libdartCountUp.a darttictactoe/libdarttictactoe.a dartlowball/libdartlowball.a dartcountdown/libdartcountdown.a dartcricket10/libdartcricket10.a dartcricket/libdartcricket.a \

INC=-Ilib -Iinclude -I. -Igorillathrilla -Ivideopoker -Iflogthedolphin -Iwordhunt -Ithreescompany -Itritowers \
	 -Icompile4 -Ifoxyboxy -Isecretphrase -Iconsume21 -Isudoku -Idropzone -Ifotoseek -Inonogram -Ifortresscrackers \
	 -Imemory
	
#-Idart01 -IdartCountUp -Idarttictactoe -Idartlowball -Idartcountdown  -Idartcricket -Idartcricket10
INSTALL_DIR=/usr/local/share/touchmegames/
LIB=-lasound lib/libgameptr.a

GTK_CFLAGS = `pkg-config --cflags gtk+-2.0` -DGTK_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1
GTK_LFLAGS = `pkg-config --cflags --libs gtk+-2.0`  -DGTK_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1
XML_LFLAGS=`xml2-config --cflags --libs`
GTHREAD_LFLAGS=`pkg-config --cflags --libs gthread-2.0`

all: interface.o support.o events.o lib
	for i in ${GAMES}; do \
		(cd $$i; ${MAKE}); \
	done
	g++ main.cpp interface.o support.o events.o -o touchmegames lib/gameptr.o lib/tmgimage.o lib/altprogressbar.o lib/tmgtogglebutton.o $(GAME_LIBS) \
		$(XML_LFLAGS) $(INC) $(GTK_LFLAGS) $(LIB) $(GTHREAD_LFLAGS) #lib/dartlib.o

debug:
	g++ interface.cpp -c -g $(INC) $(GTK_CFLAGS)
	g++ support.cpp -c -g $(INC) $(GTK_CFLAGS)
	g++ events.cpp -c -g $(INC) $(GTK_CFLAGS)
	(cd lib; ${MAKE} debug)
	for i in ${GAMES}; do \
		(cd $$i; ${MAKE} debug); \
	done
	g++ main.cpp interface.o support.o events.o -o touchmegames lib/gameptr.o lib/altprogressbar.o -g \
		$(GAME_LIBS) $(XML_LFLAGS) $(INC) $(GTK_LFLAGS) $(LIB) $(GTHREAD_LFLAGS)

interface.o: touchmegames.h interface.cpp
	g++ interface.cpp -c $(INC) $(GTK_CFLAGS)
	
support.o: touchmegames.h support.cpp	
	g++ support.cpp -c $(INC) $(GTK_CFLAGS)
	
events.o: touchmegames.h events.cpp
	g++ events.cpp -c $(INC) $(GTK_CFLAGS)
	
lib:
	(cd lib; ${MAKE})

install:
	./install.sh
#	if [ ! -e touchmegames ]; then make; fi
#	if [ ! -e ${INSTALL_DIR} ]; \
#		then mkdir ${INSTALL_DIR}; \
#	fi
#	
#	for i in ${GAMES}; do \
#		(cd $$i; ${MAKE} install;); \
#	done
#	(cd images; ${MAKE} install;);
#	cp touchmegames /usr/bin
#	test -d /usr/share/applnk/Games || mkdir -p /usr/share/applnk/Games
#	cp -f touchmegames.desktop /usr/share/applnk/Games

uninstall:
	./uninstall.sh
#	rm -rf ${INSTALL_DIR}
#	rm -f /usr/bin/touchmegames
#	rm -f /usr/share/applnk/Games/touchmegames.desktop
	
clean:
	for i in ${GAMES}; do \
		(cd $$i; ${MAKE} clean); \
	done
	rm -f *.o *~ touchmegames
