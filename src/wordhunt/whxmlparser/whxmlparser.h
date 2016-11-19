
/*	Copyright (C) 2005, 2006 by Andy Grebe a.k.a. tecknophreak(tecknophreak@gmail.com)
 *
 *	This file is part of TouchMe Games.
 *
 *	TouchMe Games is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	TouchMe Games is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with TouchMe Games; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef WHXMLPARSER_H
#define WHXMLPARSER_H

#include <gtk/gtk.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <iostream>
#include <string>
#include <vector>


class WhXmlParser {
private:
	xmlDocPtr doc;
	xmlNode *root;
	int numEntries;

	bool parseList(std::string &listName, std::string *words, xmlNode *curNode, std::string &errStr);
	void getNumEntries();
public:
	
	bool openXmlFile(std::string file, std::string rootname, std::string &errStr);
	//bool parseXmlFile(std::vector<Phrase> *phrases, std::string &errStr);
	bool getNthList(int n, std::string &listName, std::string *words, std::string &errStr);
	bool closeXmlFile();
	int getNumberOfEntries();
};

#endif // WHXMLPARSER_H
