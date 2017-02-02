#include <list>
#include <Wt/WText>

#include "content_box.h"
#include "test.h"

using namespace Wt;

content_box::content_box(WContainerWidget *parent, database *db) {
	WContainerWidget *container = new WContainerWidget(parent);
	container->setStyleClass("green-box");

	list<account *> acc_list;
	db->load_data(acc_list);

	const char *acc_info = "<p>Account: {1}</p>"
						   "<p>Total Balance: {2}</p>"
						   "<p>Avail Balance: {3}</p>"
						   "<p>Envelopes: {4}</p>";

	account acc = *(acc_list.front());
	WText *item = new WText(WString(acc_info).arg(acc.get_name()).arg(std::to_string(acc.get_total_account_balance())).arg(std::to_string(acc.get_available_balance())).arg(std::to_string(acc.get_number_of_envelopes())));
	container->addWidget(item);

}
