# makefile for testxml.c

INC=-I. -I../ -I../../lib

XML_CFLAGS=`xml2-config --cflags`
XML_LFLAGS=`xml2-config --cflags --libs`
GTK_CFLAGS = -Wall `pkg-config --cflags gtk+-2.0`
GTK_LFLAGS = -Wall `pkg-config --cflags --libs gtk+-2.0`

all: whxmlparser.o
	

debug: dwhxmlparser.o

whxmlparser.o: whxmlparser.cpp whxmlparser.h
	g++ whxmlparser.cpp $(XML_CFLAGS) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(INC) -c
	
	
dwhxmlparser.o: whxmlparser.cpp whxmlparser.h
	g++ whxmlparser.cpp $(XML_CFLAGS) $(GTK_CFLAGS) $(GENERAL_CFLAGS) $(INC) -c -g
	
clean:
	rm -rf *.o *~ 
