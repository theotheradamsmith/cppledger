#pragma once

#include <list>
#include <Wt/WContainerWidget>

#include "account.h"
#include "database.h"

class content_box {
	public:
		content_box(Wt::WContainerWidget *parent, database *db);
	private:
		database *database_connection;
		list<account *> account_list;
};
