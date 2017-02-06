#include "account_display.h"

using namespace Wt;

/* Account functions */

account_display::account_display(account *a, string s)
	: acc(a)
{
	this->refresh();
	this->set_style(s);
	display->setStyleClass(style);
}

void account_display::refresh() {
	const char *acc_info = "<p>Account: {1}</p>"
						   "<p>Total Balance: {2}</p>"
						   "<p>Avail Balance: {3}</p>"
						   "<p>Envelopes: {4}</p>";
	if (!display) {
		display = new WText(
			WString(acc_info).
			arg(acc->get_name()).
			arg(std::to_string(acc->get_total_account_balance())).
			arg(std::to_string(acc->get_available_balance())).
			arg(std::to_string(acc->get_number_of_envelopes()))
		);
	} else {
		display->setText(
			WString(acc_info).
			arg(acc->get_name()).
			arg(std::to_string(acc->get_total_account_balance())).
			arg(std::to_string(acc->get_available_balance())).
			arg(std::to_string(acc->get_number_of_envelopes()))
		);
	}
}

WText *account_display::get_display() const {
	return display;
}

/* Envelope functions */

envelope_display::envelope_display(envelope *e, string s)
	: env(e)
{
	this->refresh();
	this->set_style(s);
	display->setStyleClass(style);
}

void envelope_display::refresh() {
	const char *env_info = "<p>{1} - Envelope: {2}</p>"
						   "<p>Balance: {3}</p>";
	if (!display) {
		display = new WText(
			WString(env_info).
			arg(std::to_string(env->get_id())).
			arg(env->get_name()).
			arg(std::to_string(env->get_available_balance()))
		);
	} else {
		display->setText(
			WString(env_info).
			arg(std::to_string(env->get_id())).
			arg(env->get_name()).
			arg(std::to_string(env->get_available_balance()))
		);
	}
}

WText *envelope_display::get_display() const {
	return display;
}
