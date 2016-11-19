
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


#ifndef PHRASE_GENERATOR_H
#define PHRASE_GENERATOR_H


#include <iostream>
#include <xmlparser.h>
#include <vector>

class PhraseGenerator {
	private:
		std::vector<std::string> descriptors;
		std::string filename;
		std::string rootname;
		std::string descriptor;
		std::string phrase;
		
		int showMainMenu(); // what file to add to
		int showFileMenu(); // what type of entry to add
		void addPhrase(int numberOfPhrases);
		
	public:
		void run();
};


#endif // PHRASE_GENERATOR_H
