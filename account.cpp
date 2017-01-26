#include "account.h"

ostream &operator<<(ostream &out, envelope &env) {
	out << env.name << ": " << env.balance;
	return out;
}

ostream &operator<<(ostream &out, account &acc) {
	out << "Account: " << acc.name << endl;
	out << "Total Balance: " << acc.total_account_balance << endl;
	out << "Avail Balance: " << acc.balance << endl;
	out << "\tEnvelopes (" << acc.number_of_envelopes << "):" << endl;
	for (auto env : acc.envelopes) {
		out << '\t' << env << endl;
	}
	
	return out;
}

/**
 * Monetary Container Functions
 */
string monetary_container::get_name() const {
	return name;
}

long monetary_container::get_available_balance() const {
	return balance;
}

/**
 * Envelope Functions
 */
void envelope::set_envelope_increment_value(long i) {
	increment_value = i;
}

void envelope::name_envelope(string n) {
	name = n;
}

void envelope::adjust_envelope_balance(long v) {
	balance += v;
}

long envelope::get_increment_value() const {
	return increment_value;
}

string envelope::get_category() const {
	return category;
}

/**
 * Account Functions
 */

void account::create_envelope(string n, long b, long i) {
	envelope env(n, b, i);
	envelopes.push_back(env);
	adjust_account_balance(-b);
	number_of_envelopes++;
}

void account::increment_envelope(string n) {
	envelope *env = locate_envelope(n);
	if (env) {
		if (balance - env->increment_value >= 0) {
			env->adjust_envelope_balance(env->increment_value);
			balance -= env->increment_value;
		} else {
			cout << "Insufficient funds. No action taken." << endl;
		}
	}
}

void account::adjust_account_balance(long v) {
	balance += v;
}

envelope *account::locate_envelope(string n) {
	for (auto &env : envelopes) {
		if (n == env.name) {
			return &env;
		}
	}
	return nullptr;
}

/**
 * Getters
 */
long account::get_total_account_balance() const {
	return total_account_balance;
}

int account::get_number_of_envelopes() const {
	return number_of_envelopes;
}

/**
 * Setters
 */
void account::rename_account(string n) {
	name = n;
}

void account::set_total_account_balance(long val) {
	total_account_balance = val;
}

void account::set_available_balance(long val) {
	balance = val;
}
