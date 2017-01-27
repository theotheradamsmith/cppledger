#pragma once

#include <Wt/WContainerWidget>

using namespace std;

namespace Wt {
	class WStackedWidget;
	class WAnchor;
}

class Session {};
class MenuItem;

class estate : public Wt::WContainerWidget {
	public:
		estate();
		void handleInternalPath(const string &internalPath);

	private:
		Wt::WStackedWidget *contentsStack_;
		Wt::WNavigationBar *navigation_;
		Wt::WAnchor *back_to_accounts_;
		Wt::WAnchor *envelopes_;

		Session session_;

		Wt::WMenuItem *addToMenu(Wt::WMenu *menu, const Wt::WString &name, MenuItem *topic);
};
