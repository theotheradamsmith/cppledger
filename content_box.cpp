#include <list>
#include <Wt/WHBoxLayout>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WVBoxLayout>

#include "account_display.h"
#include "content_box.h"

using namespace Wt;

content_box::content_box(WContainerWidget *parent, database *db)
	: database_connection(db)
{
	// Load account data
	database_connection->load_data(account_list);
	account *acc = account_list.front();

	// Create background container
	WContainerWidget *container = new WContainerWidget(parent);
	container->setStyleClass("green-box");

	// Create vertical layout
	WVBoxLayout *vbox = new WVBoxLayout();
	container->setLayout(vbox);

	// Insert account display
	account_display *ad = new account_display(acc, "blue-box");
	vbox->addWidget(ad->get_display(), 1);

	// Create horizontal layout & place below account display
	WHBoxLayout *hbox = new WHBoxLayout();
	vbox->addLayout(hbox);

	for (auto env : acc->envelopes) {
		// Create a vertical box to hold envelope display & a button
		WVBoxLayout *env_box = new WVBoxLayout();

		// Create envelope display & button for env
		envelope_display *ed = new envelope_display(env, "blue-box");
		env_box->addWidget(ed->get_display());
		WPushButton *inc_button = new Wt::WPushButton("Increment");
		inc_button->clicked().connect(std::bind([=] () {
			acc->increment_envelope(env->get_name());
			database_connection->save_data(*acc);
			ad->refresh();
			ed->refresh();
		}));
		env_box->addWidget(inc_button);

		// Add vertical envelope box to horizontal layout
		hbox->addLayout(env_box);
	}
}
