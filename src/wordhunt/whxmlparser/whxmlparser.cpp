
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

// xmlparser for wordhunt list file

#include <whxmlparser.h>


bool WhXmlParser::openXmlFile(std::string file, std::string rootname, std::string &errStr) {
	doc = xmlParseFile(file.c_str());

	if (doc == NULL) {
		errStr = "WhXmlParser Error: could not parse file ";
		errStr += file;
		errStr += ".";
		return false;
	}
	
	// check to see if there is a root called foxyboxyboard
	root = xmlDocGetRootElement(doc);

	if (!root || !root->name || xmlStrcmp(root->name, (const xmlChar *)rootname.c_str())) {
		xmlFreeDoc(doc);
		errStr = "WhXmlParser Error: Not a wordhunt file.";
		return false;
	}
	
	getNumEntries();
	
	return true;
}

void WhXmlParser::getNumEntries() {
	xmlNode *curNode;
	char *text;
	numEntries = 0;
	for (curNode = root->children; curNode != NULL; curNode = curNode->next) {
		// check for the different Children
		if (curNode->type == XML_ELEMENT_NODE  &&
				  !xmlStrcmp(curNode->name, (const xmlChar *)"numberOfLists")) {
			text = (char *)xmlNodeGetContent(curNode);
			if (!text) {
				std::cerr << "Cannot get number of numberOfLists" << std::endl;
				return;
			}
			numEntries = atoi(text);
			xmlFree(text);
		}
	}
}

//bool WhXmlParser::parsePhrase(std::string &descriptor, std::string &phrase, xmlNode *curNode, std::string &errStr) {
bool WhXmlParser::parseList(std::string &listName, std::string *words, xmlNode *curNode, std::string &errStr) {
	xmlNode *childNode;
	char *text;
	int cntr(0);
	
	for (childNode = curNode->children; childNode != NULL; childNode = childNode->next) {
		if (childNode->type == XML_ELEMENT_NODE  &&
				   !xmlStrcmp(childNode->name, (const xmlChar *)"title")) {
			
			text = (char *)xmlNodeGetContent(childNode);
			if (!text) {
				errStr = "WhXmlParser Error: No content found for descriptor.";
				return false;
			}
			listName = text;
			xmlFree(text);
		}
		else if (childNode->type == XML_ELEMENT_NODE  &&
				       !xmlStrcmp(childNode->name, (const xmlChar *)"word")) {
			
			text = (char *)xmlNodeGetContent(childNode);
			if (!text) {
				errStr = "WhXmlParser Error: No content found for text.";
				return false;
			}
			words[cntr++] = text;
			xmlFree(text);
		}
	}
	
	return true;
}



/*bool WhXmlParser::parseXmlFile(std::vector<Phrase> *phrases, std::string &errStr) {
	xmlNode *curNode;
	
	for (curNode = root->children; curNode != NULL; curNode = curNode->next) {
		// check for the different Children
		if (curNode->type == XML_ELEMENT_NODE  &&
				!xmlStrcmp(curNode->name, (const xmlChar *)"phrase")) {
			Phrase phrase;
			
//			if (!parsePhrase(phrase, curNode, errStr)) {
//				return false;
//			}
			phrases->push_back(phrase);
		}
	}
	return true;
}*/

//bool WhXmlParser::getNthPhrase(int n, std::string &descriptor, std::string &phrase, std::string &errStr) {
bool WhXmlParser::getNthList(int n, std::string &listName, std::string *words, std::string &errStr) {
	xmlNode *curNode;
	if (n > numEntries) {
		errStr = "n > numEntries";
		return false;
	}
	++n;
	for (curNode = root->children; curNode != NULL; curNode = curNode->next) {
		// check for the different Children
		if (curNode->type == XML_ELEMENT_NODE  &&
				!xmlStrcmp(curNode->name, (const xmlChar *)"list")
				 && !(--n)) {
			if (parseList(listName, words, curNode, errStr)) {
				return true;
			}
			
		}
	}
	
	return false;
}



int WhXmlParser::getNumberOfEntries() {
	return numEntries;
}

bool WhXmlParser::closeXmlFile() {
	xmlFreeDoc(doc);


	// Free the global variables that may
	// have been allocated by the parser.

	xmlCleanupParser();
	return true;
}
