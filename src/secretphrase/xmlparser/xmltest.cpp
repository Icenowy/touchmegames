
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
#include <string>

XmlParser parser;

bool getPhrase(int n, std::string &descriptor, std::string &phrase) {
	std::string str;
	bool ret;
	ret = parser.getNthPhrase(n, descriptor, phrase, str);
	return ret;
}

int main() {
	int nums[3] = { -1, -1, -1 };
	std::string descriptor, phrase, str;
	
	if (!parser.openXmlFile("../phrases/entertainment.xml", "Entertainment", str)) {
		std::cout << "Fook " << str << std::endl;
	}
	std::cout << "NumberOf entries " << parser.getNumberOfEntries() << std::endl;
	for (int i = 0; i < 3; ++i) {
		int n(-1);
		while (n == nums[0] || n == nums[1] || n == nums[2]) {
			n = random() % parser.getNumberOfEntries();
			std::cout << "n " << n << std::endl;
			if (!getPhrase(n, descriptor, phrase)) {
				std::cout << "Could not get phrase " << n << std::endl;
				n = nums[0];
			}
			else {
				std::cout << "Got phrase" << std::endl;
			}
			
		}
		nums[i] = n;
		
		std::cout << descriptor << ' ' << phrase << std::endl;
	}
	parser.closeXmlFile();
}
