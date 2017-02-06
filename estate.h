#pragma once

#include <list>
#include <Wt/WContainerWidget>
#include "database.h"

using namespace std;

namespace Wt {
	class WStackedWidget;
	class WAnchor;
}

class Session {};
class MenuItem;

class estate : public Wt::WContainerWidget {
	public:
		estate(database *db);
		void handleInternalPath(const string &internalPath);

	private:
		Wt::WStackedWidget *contents_stack;
		//Wt::WStackedWidget *contentsStack_;
		Wt::WNavigationBar *navigation_;
		Wt::WAnchor *back_to_accounts_;
		Wt::WAnchor *envelopes_;

		Session session_;

		Wt::WMenuItem *addToMenu(Wt::WMenu *menu, const Wt::WString &name, MenuItem *topic);

		// Objects to hold the database connection and account list
		database *database_connection;
		list<account *> account_list;
};
