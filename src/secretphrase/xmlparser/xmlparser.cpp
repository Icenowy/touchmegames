
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



#include <xmlparser.h>


bool XmlParser::openXmlFile(std::string file, std::string rootname, std::string &errStr) {
	doc = xmlParseFile(file.c_str());

	if (doc == NULL) {
		errStr = "XmlParser Error: could not parse file ";
		errStr += file;
		errStr += ".";
		return false;
	}
	
	// check to see if there is a root called foxyboxyboard
	root = xmlDocGetRootElement(doc);

	if (!root || !root->name || xmlStrcmp(root->name, (const xmlChar *)rootname.c_str())) {
		xmlFreeDoc(doc);
		errStr = "XmlParser Error: Not a secretphrase file.";
		return false;
	}
	
	getNumEntries();
	
	return true;
}

void XmlParser::getNumEntries() {
	xmlNode *curNode;
	char *text;
	numEntries = 0;
	for (curNode = root->children; curNode != NULL; curNode = curNode->next) {
		// check for the different Children
		if (curNode->type == XML_ELEMENT_NODE  &&
				  !xmlStrcmp(curNode->name, (const xmlChar *)"entries")) {
			text = (char *)xmlNodeGetContent(curNode);
			if (!text) {
				std::cerr << "Cannot get number of entries" << std::endl;
				return;
			}
			numEntries = atoi(text);
			xmlFree(text);
		}
	}
}

bool XmlParser::parsePhrase(std::string &descriptor, std::string &phrase, xmlNode *curNode, std::string &errStr) {
	xmlNode *childNode;
	char *text;
	
	for (childNode = curNode->children; childNode != NULL; childNode = childNode->next) {
		if (childNode->type == XML_ELEMENT_NODE  &&
				   !xmlStrcmp(childNode->name, (const xmlChar *)"descriptor")) {
			
			text = (char *)xmlNodeGetContent(childNode);
			if (!text) {
				errStr = "XmlParser Error: No content found for descriptor.";
				return false;
			}
			descriptor = text;
			xmlFree(text);
		}
		else if (childNode->type == XML_ELEMENT_NODE  &&
				       !xmlStrcmp(childNode->name, (const xmlChar *)"text")) {
			
			text = (char *)xmlNodeGetContent(childNode);
			if (!text) {
				errStr = "XmlParser Error: No content found for text.";
				return false;
			}
			phrase = text;
			xmlFree(text);
		}
	}
	
	return true;
}



/*bool XmlParser::parseXmlFile(std::vector<Phrase> *phrases, std::string &errStr) {
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

bool XmlParser::getNthPhrase(int n, std::string &descriptor, std::string &phrase, std::string &errStr) {
	xmlNode *curNode;
	if (n > numEntries) {
		errStr = "n > numEntries";
		return false;
	}
	++n;
	for (curNode = root->children; curNode != NULL; curNode = curNode->next) {
		// check for the different Children
		if (curNode->type == XML_ELEMENT_NODE  &&
				!xmlStrcmp(curNode->name, (const xmlChar *)"phrase")
				 && !(--n)) {
			if (parsePhrase(descriptor, phrase, curNode, errStr)) {
				return true;
			}
			
		}
	}
	
	return false;
}

bool XmlParser::getTypes(std::vector<std::string> &v, std::string &errStr) {
	xmlNode *curNode;
	char *text;
	
	for (curNode = root->children; curNode != NULL; curNode = curNode->next) {
		// check for the different Children
		if (curNode->type == XML_ELEMENT_NODE  &&
				  !xmlStrcmp(curNode->name, (const xmlChar *)"descriptortype")) {
			
			
			text = (char *)xmlNodeGetContent(curNode);
			if (!text) {
				errStr = "XmlParser Error: No content found for descriptortype.";
				return false;
			}
			v.push_back(text);
			xmlFree(text);
		}
		else if (curNode->type == XML_ELEMENT_NODE  &&
				       !xmlStrcmp(curNode->name, (const xmlChar *)"entries")) {
		
			// we're at the end here, so exit
			return true;
		}
	}
	
	return true;
}


int XmlParser::getNumberOfEntries() {
	return numEntries;
}

bool XmlParser::closeXmlFile() {
	xmlFreeDoc(doc);


	// Free the global variables that may
	// have been allocated by the parser.

	xmlCleanupParser();
	return true;
}
