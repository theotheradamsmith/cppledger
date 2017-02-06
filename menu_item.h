#pragma once

#include <Wt/WContainerWidget>
#include <Wt/WMenu>
#include <string>

using namespace std;

class MenuItem : public Wt::WContainerWidget {
	public:
		MenuItem();

	protected:
		static Wt::WText *addText(const Wt::WString &text, Wt::WContainerWidget *parent = 0);

	private:
		string docAnchor(const string &classname) const;
		string title(const string &classname) const;
};
