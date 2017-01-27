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
		out << '\t' << *env << endl;
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
int envelope::id_gen = 0;

envelope::envelope(string nam, long bal, long inc, int env_id, int own) :
	monetary_container(nam, bal),
	increment_value(inc),
	owner_id(own)
{
	id_gen++;
	if (env_id == -1) {
		id = id_gen;
		cout << "Creating envelope with id " << id << endl;
	} else {
		id = env_id;
	}
}

envelope::~envelope() {}
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

int envelope::get_id() const {
	return id;
}

/**
 * Account Functions
 */

account::account(string n, long b, int i) :
	monetary_container(n, b),
	total_account_balance(b),
	id(i)
{
}

account::~account() {
}

void account::create_envelope(string n, long b, long i, int env_id=-1) {
	envelope *env = new envelope(n, b, i, env_id, id);
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
		if (n == env->name) {
			return env;
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

int account::get_id() const {
	return id;
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
