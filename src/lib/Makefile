
# Makefile for libs

GENERAL_CFLAGS=-DINSTALL_DIR="\"$(INSTALL_DIR)\""
GTK_CFLAGS = `pkg-config --cflags gtk+-2.0` -Wall -DGTK_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1
GTK_LFLAGS = `pkg-config --cflags --libs gtk+-2.0` -DGTK_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1
GTHREAD_CFLAGS = `pkg-config --cflags gthread-2.0`


INC=-I. -I../include

all: gameptr.a

debug:
	g++ gameptr.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)
	g++ keypad.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)
	g++ tmgdir.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)
	g++ tmgglist.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)
	g++ altprogressbar.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) -Wall
	g++ tmgwave.cpp -c -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) -Wall
	ar crv libgameptr.a gameptr.o keypad.o altprogressbar.o tmgdir.o tmgglist.o tmgwave.o tmgimage.o
	ranlib libgameptr.a

gameptr.a: gameptr.o keypad.o altprogressbar.o tmgdir.o tmgglist.o tmgglist.o tmgwave.o tmgtogglebutton.o tmgimage.o
	ar crv libgameptr.a gameptr.o keypad.o altprogressbar.o tmgdir.o tmgglist.o tmgwave.o tmgtogglebutton.o tmgimage.o
	ranlib libgameptr.a
	
gameptr.o: gameptr.cpp gameptr.h
	g++ gameptr.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)

keypad.o: keypad.cpp keypad.h
	g++ keypad.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)

tmgdir.o: tmgdir.cpp tmgdir.h
	g++ tmgdir.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)
	
tmgglist.o: tmgglist.cpp tmgglist.h
	g++ tmgglist.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)

tmgimage.o: tmgimage.cpp ../include/tmgimage.h
	g++ tmgimage.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS)

keypad: keypad.h keypad.cpp
	g++ keypad.cpp kpmain.cpp -o keypad $(INC) $(GTK_LFLAGS)

altprogressbar.o: altprogressbar.h altprogressbar.cpp
	g++ altprogressbar.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) -Wall
	
tmgwave.o: tmgwave.cpp tmgwave.h
	g++ tmgwave.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) -Wall $(GTHREAD_CFLAGS)
	
tmgtogglebutton.o: tmgtogglebutton.cpp tmgtogglebutton.h
	g++ tmgtogglebutton.cpp -c $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) -Wall $(GTHREAD_CFLAGS)
	
tmgglisttest: tmgglist.o
	g++ tmgglisttest.cpp tmgglist.o -o tgltest -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(GTK_LFLAGS) -Wall
	
tmgtogglebuttontest: tmgtogglebutton.o
	g++ tmgtogglebuttontest.cpp tmgtogglebutton.o -o ttbtest -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(GTK_LFLAGS) -Wall
	
tmgwavetest: tmgwave.o
	g++ tmgwavetest.cpp tmgwave.o -o tgwtest -g $(INC) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(GTK_LFLAGS) -Wall -lasound
	
clean:
	rm -rf *.o *~ *.a keypad *test



