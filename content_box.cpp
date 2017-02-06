#include <list>
#include <Wt/WHBoxLayout>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WVBoxLayout>

#include "content_box.h"

using namespace Wt;

content_box::content_box(WContainerWidget *parent, database *db, account *acc)
	: database_connection(db),
	  working_account(acc)
{
	// Create background container
	WContainerWidget *container = new WContainerWidget(parent);
	container->setStyleClass("green-box");

	// Create vertical layout
	WVBoxLayout *vbox = new WVBoxLayout();
	container->setLayout(vbox);

	// Insert account display
	acc_display = new account_display(working_account, "blue-box");
	vbox->addWidget(acc_display->get_display(), 1);

	// Create horizontal layout & place below account display
	WHBoxLayout *hbox = new WHBoxLayout();
	vbox->addLayout(hbox);
	create_envelope_hbox(hbox, working_account->envelopes);
}

void content_box::create_envelope_hbox(WHBoxLayout *hbox, list<envelope *> envelopes) {
	for (auto env : envelopes) {
		// Create a vertical box to hold envelope display & a button
		WVBoxLayout *env_box = new WVBoxLayout();

		// Create envelope display & button for env
		envelope_display *ed = new envelope_display(env, "blue-box");
		env_box->addWidget(ed->get_display());
		WPushButton *inc_button = new Wt::WPushButton("Increment");
		inc_button->clicked().connect(std::bind([=] () {
			working_account->increment_envelope(env->get_name());
			database_connection->save_data(*working_account);
			acc_display->refresh();
			ed->refresh();
		}));
		env_box->addWidget(inc_button);

		// Add vertical envelope box to horizontal layout
		hbox->addLayout(env_box);

		// Populate an envelope_box and push back to envelope_boxes vector
		envelope_box *eb = new envelope_box;
		eb->name = env->get_name();
		eb->env_box = env_box;
		eb->ed = ed;
		eb->button = inc_button;
		envelope_boxes.push_back(eb);
	}
}
