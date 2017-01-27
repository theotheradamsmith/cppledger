#include <sstream>
#include <boost/algorithm/string.hpp>

#include <Wt/WText>

#include "menu_item.h"

using namespace Wt;

namespace {
	int countSpaces(const string &line) {
		for (unsigned int pos=0; pos < line.length(); ++pos) {
			if (line[pos] != ' ') {
				return pos;
			}
		}
		return line.length();
	}

	string skipSpaces(const string &line, unsigned int count) {
		if (line.length() >= count) {
			return line.substr(count);
		} else {
			return string();
		}
	}
}

MenuItem::MenuItem()
	: WContainerWidget()
{
}

void MenuItem::populateSubMenu(WMenu *menu) {
}
