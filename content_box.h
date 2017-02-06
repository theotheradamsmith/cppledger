#pragma once

#include <list>
#include <Wt/WContainerWidget>

#include "account.h"
#include "account_display.h"
#include "database.h"

typedef struct envelope_box {
	string name;
	Wt::WVBoxLayout *env_box;
	envelope_display *ed;
	Wt::WPushButton *button;
} envelope_box;

class content_box {
	public:
		content_box(Wt::WContainerWidget *parent, database *db, account *acc);
		~content_box() {}
	private:
		database *database_connection = nullptr;
		account *working_account = nullptr;
		account_display *acc_display = nullptr;
		vector<envelope_box *> envelope_boxes;

		void create_envelope_hbox(Wt::WHBoxLayout *hbox, list<envelope *> envelopes);
};
