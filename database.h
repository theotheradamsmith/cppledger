#pragma once

#include <iostream>
#include <vector>
#include <list>

#include "account.h"
#include "sqlite3.h"

using namespace std;

class database {
	friend ostream &operator<<(ostream &out, database &database_obj);
	public:
		database(const char *n);
		~database();

		/**
		 * Return a pointer to the sqlite3 database connection object
		 */
		sqlite3 *get_database_connection() const;

		/**
		 * Cleanly terminate the connection represented by this database object
		 */
		void close_database_connection();

		/**
		 * Load all saved database data into memory
		 * @param acc A linked list of account pointers that will contain loaded information
		 */
		int load_data(list<account *> &acc);

		/**
		 * Write data to the local database file
		 * @param acc An account object that we intend to save
		 * @return Returns 0 on success
		 */
		int save_data(account &acc);

		/**
		 * Write data to the local database file
		 * @param env An envelope object that we intend to save
		 * @param owner_id The account that 'owns' this envelope
		 * @return Returns 0 on success
		 */
		int save_data(envelope &env, int owner_id);

		vector<account> accounts;

	private:
		/// The unique database name
		const char *database_name;

		/// The pointer to the actual database connection object used by sqlite
		sqlite3 *database_connection;

		/// A field to store error message associated with the database
		char *main_database_err_msg;

		/**
		 * Helper function to create the root account table & "root_account" entry
		 * and the empty envelopes table
		 */
		int initialize_vault();

		/**
		 * Helper function to load information from the envelopes table
		 */
		int load_envelope_data(account &acc);
};
