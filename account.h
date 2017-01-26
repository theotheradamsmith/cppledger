#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class account;

class monetary_container {
	public:
		string get_name() const;
		long get_available_balance() const;
	protected:
		monetary_container(string n, long b) : name(n), balance(b) {}
		virtual ~monetary_container() {}
		void modify_balance(long b);
		string name;
		long balance;
};

class envelope : public monetary_container {
	friend ostream &operator<<(ostream &out, envelope &env);
	public:
		envelope(string n, long b, long i)
			: monetary_container(n, b), increment_value(i) {}
		~envelope() {}

		/**
		 * Change an envelope's increment value to specificed value
		 * @param i New increment value
		 */
		void set_envelope_increment_value(long i);

		/**
		 * Change an envelope's name to specified value
		 * @param n New name
		 */
		void name_envelope(string n);

		/**
		 * Change the value of an envelope's balance
		 * @param v The amount by which to increment the envelope balance
		 */
		void adjust_envelope_balance(long v);

		long get_increment_value() const;
		string get_category() const;

	private:
		friend class account;

		/// The amount by which to change an envelope's value
		long increment_value = 0;

		/// The envelope's "investment category"
		string category;
		
};

class account : public monetary_container {
	/// Operator overloader to facilitate easy printing of accounts
	friend ostream &operator<<(ostream &, account &);

	public:
		/// Initialize a new account by providing an initial balance
		account(string n, long b, int i)
			: monetary_container(n, b), total_account_balance(b), id(i) {}
		~account() {}

		/**
		 * Function to create envelope and push it to the account's envelopes vector
		 * @param n Name for the envelope
		 * @param b A starting balance for the account
		 * @param i A default increment value for the account
		 */
		void create_envelope(string n, long b, long i);

		/**
		 * Helper function to adjust the working balance available in the account
		 * @param v The amount by which to adjust the available account balance
		 */
		void adjust_account_balance(long v);

		/**
		 * Find a specified envelope in an object's envelopes vector and modify it
		 * by its set increment amount
		 * @param n The name of the envelope to increment or decrement
		 */
		void increment_envelope(string n);

		/**
		 * Helper function to find envelopes in account envelopes vectors
		 * @param n The name of the account to search for
		 * @return Returns a pointer to the envelope or nullptr upon failure
		 */
		envelope *locate_envelope(string n);

		/**
		 * Get the total account balance
		 */
		long get_total_account_balance() const;
		/**
		 * Get the number of envelopes
		 */
		int get_number_of_envelopes() const;
		/**
		 * Get the account id
		 */
		int get_id() const;

		/**
		 * Assign a new name to an account
		 * @param n A string representing the new account name
		 */
		void rename_account(string n);

		/**
		 * Change an account's total account balance
		 * @param v The new total account balance
		 */
		void set_total_account_balance(long v);

		/**
		 * Set a new available balance for an account
		 * @param v Long representing the new balance
		 */
		void set_available_balance(long val);

		/// A vector of envelopes that subdivide the account
		vector<envelope> envelopes;

	private:
		/// Shared grand total
		long total_account_balance = 0;

		/// Unique id number
		int id;

		/// The number of unique envelopes associated with this account
		int number_of_envelopes = 0;
};
