#pragma once

#include <Wt/WContainerWidget>
#include "database.h"

class content_box {
	public:
		content_box(Wt::WContainerWidget *parent, database *db);
};
