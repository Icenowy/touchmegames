
# Makefile for secretphrase 

INC=-I. -I../xmlparser -I../

GTK_CFLAGS = `pkg-config --cflags gtk+-2.0`
GTK_LFLAGS = `pkg-config --cflags --libs gtk+-2.0`
XML_CFLAGS = `xml2-config --cflags`
XML_LFLAGS = `xml2-config --cflags --libs`



all: phrasegenerator ../xmlparser/xmlparser.o



phrasegenerator: phrasegenerator.o ../xmlparser/xmlparser.o
	g++ main.cpp phrasegenerator.o ../xmlparser/xmlparser.o -o phrasegenerator $(INC) $(XML_LFLAGS) $(GTK_LFLAGS)
	cp phrasegenerator pg

phrasegenerator.o: phrasegenerator.cpp phrasegenerator.h
	g++ phrasegenerator.cpp -c -Wall $(GTK_CFLAGS) $(XML_CFLAGS) $(INC)

xmlparser.o:
	(cd xmlparser; $(MAKE))

clean:
	rm -rf *.o *~ *.a pg phrasegenerator
	
