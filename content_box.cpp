#include <list>
#include <Wt/WHBoxLayout>
#include <Wt/WText>
#include <Wt/WVBoxLayout>

#include "content_box.h"

using namespace Wt;

content_box::content_box(WContainerWidget *parent, database *db)
	: database_connection(db)
{
	WContainerWidget *container = new WContainerWidget(parent);
	container->setStyleClass("green-box");

	WVBoxLayout *vbox = new WVBoxLayout();
	container->setLayout(vbox);

	db->load_data(account_list);

	const char *acc_info = "<p>Account: {1}</p>"
						   "<p>Total Balance: {2}</p>"
						   "<p>Avail Balance: {3}</p>"
						   "<p>Envelopes: {4}</p>";

	account acc = *(account_list.front());
	WText *item = new WText(WString(acc_info).arg(acc.get_name()).arg(std::to_string(acc.get_total_account_balance())).arg(std::to_string(acc.get_available_balance())).arg(std::to_string(acc.get_number_of_envelopes())));
	item->setStyleClass("blue-box");
	vbox->addWidget(item, 1);

	WHBoxLayout *hbox = new WHBoxLayout();
	vbox->addLayout(hbox);

	const char *env_info = "<p>{1} - Envelope: {2}</p>"
						   "<p>Balance: {3}</p>";

	for (auto env : acc.envelopes) {
		item = new WText(WString(env_info).arg(std::to_string(env->get_id())).arg(env->get_name()).arg(std::to_string(env->get_available_balance())));
		item->setStyleClass("blue-box");
		hbox->addWidget(item);
	}
}
