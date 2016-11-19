
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

// This is NOT how I like files to be setup and the code isn't too
// good either.  However, this is just a utility, so it's ok. ALG

#include <phrasegenerator.h>
#include <fstream>
#include <sstream>


int PhraseGenerator::showMainMenu() {
	std::string str;
	std::cout << "Which file do you wish to edit?" << std::endl;
	std::cout << "1. Video" << std::endl;
	std::cout << "2. Music" << std::endl;
	std::cout << "3. Misc" << std::endl;
	std::cout << "4. Sports" << std::endl;
	std::cout << "5. Exit" << std::endl;
	std::cout << "? ";
	std::cin >> str;
	if (str == "1") {
		filename = "../phrases/entertainment.xml";
		rootname = "Entertainment";
		return 1;
	}
	else if (str == "2") {
		filename = "../phrases/music.xml";
		rootname = "Music";
		return 2;
	}
	else if (str == "3") {
		filename = "../phrases/misc.xml";
		rootname = "Misc";
		return 2;
	}
	else if (str == "4") {
		filename = "../phrases/sports.xml";
		rootname = "Sports";
		return 2;
	}
	else if (str == "5") {
		return -1;
	}
	std::cout << "\nInvalid Choice." << std::endl;
	sleep(1);
	std::cout << std::endl;
	return showMainMenu();
}


int PhraseGenerator::showFileMenu() {
	std::string str;
	int i(-1);
	while (i <= 0 || i > (int)descriptors.size()) {
		std::cout << "Which type do you wish to add?" << std::endl;
		for (i = 0; i < (int)descriptors.size(); ++i) {
			std::cout << i + 1 << ". " << descriptors[i] << std::endl;
		}
		std::cout << i + 1 << ". Exit to main menu" << std::endl;
		std::cout << "? ";
		std::cin >> str;
		i = atoi(str.c_str());
		std::cout << "I: " << i << std::endl;
	}
	
	if (i == (int)descriptors.size() + 1) {
		// exit
		return -1;
	}
	
	
	str = "";
	while (!str.size()) {
		std::cout << "Enter Phrase: ";
		getline(std::cin, str);
		
	}
	
	descriptor = descriptors[i - 1];
	phrase = str;
	
	return 0;
}

void PhraseGenerator::addPhrase(int numberOfPhrases) {
	// This isn't how I'd like to do this, but
	// it's just a little helper program
	std::ifstream is(filename.c_str());
	std::ofstream os;
	std::string str, srchstr;
	std::stringstream sstr;
	
	std::cout << "Adding phrase to " << filename << std::endl;
	
	srchstr = "</" + rootname + ">";
	
	while (getline(is, str) && str.find("phrase") > str.size());
	sstr << str << std::endl;
	
	while (getline(is, str) && str.find(srchstr) > str.size()) {
		sstr << str << std::endl;
	}
	is.close();
	
	if (sstr.str().find(phrase) < sstr.str().size()) {
		std::cout << "\nAlready Added.\n" << std::endl;
		return;
	}
	
	os.open(filename.c_str());
	os << "<" << rootname << ">" << std::endl;
	
	for (unsigned int i = 0; i < descriptors.size(); ++i) {
		os << "<descriptortype>" << descriptors[i] << "</descriptortype>" << std::endl;
	}
	os << "<entries>" << numberOfPhrases << "</entries>" << std::endl;
			
	
	os << sstr.str() << std::endl;
	os << " <phrase>" << std::endl;
	os << "\t<descriptor>" << descriptor << "</descriptor>" << std::endl;
	os << "\t<text>" << phrase << "</text>" << std::endl;
	os << " </phrase>" << std::endl;
	os << srchstr << std::endl;
	
	
	os.close();
	
}

void PhraseGenerator::run() {
	XmlParser parser;
	while (showMainMenu() != -1) {
		std::string err;
		int numberOfPhrases;
		if (!parser.openXmlFile(filename, rootname, err)) {
			std::cout << "Parser open error: " << err << std::endl;
			return;
		}
		parser.getTypes(descriptors, err);
		numberOfPhrases = parser.getNumberOfEntries();
		parser.closeXmlFile();
		while (showFileMenu() != -1) {
			++numberOfPhrases;
			addPhrase(numberOfPhrases);
		}
		descriptors.clear();
	}
}
