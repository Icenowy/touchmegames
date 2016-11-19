# makefile for testxml.c

INC=-I. -I../ -I../../lib

XML_CFLAGS=`xml2-config --cflags`
XML_LFLAGS=`xml2-config --cflags --libs`
GTK_CFLAGS = -Wall `pkg-config --cflags gtk+-2.0`  -DGTK_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1
GTK_LFLAGS = -Wall `pkg-config --cflags --libs gtk+-2.0`  -DGTK_DISABLE_DEPRECATED=1 -DGTK_MULTIHEAD_SAFE=1

all: xmlparser.o xmltest.o
	g++ xmltest.o xmlparser.o $(XML_LFLAGS) $(GTK_LFLAGS) -o xmltest

debug: dxmlparser.o

xmlparser.o: xmlparser.cpp xmlparser.h
	g++ xmlparser.cpp $(XML_CFLAGS) $(GTK_CFLAGS) $(INC) -c
	
xmltest.o: xmltest.cpp
	g++ xmltest.cpp $(XML_CFLAGS) $(GTK_CFLAGS) $(INC) -c
	
dxmlparser.o: xmlparser.cpp xmlparser.h
	g++ xmlparser.cpp $(XML_CFLAGS) $(GTK_CFLAGS) $(INC) -c -g
	
clean:
	rm -rf *.o *~ xmltest 
