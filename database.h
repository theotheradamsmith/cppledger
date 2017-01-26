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
		sqlite3 *get_database_connection() const;
		int initialize_vault();
		void close_database_connection();

		int load_data(list<account> &acc);

		int save_data(account &acc);
		int save_data(envelope &env, int owner_id);

		vector<account> accounts;

	private:
		const char *database_name;
		sqlite3 *database_connection;
		char *main_database_err_msg;
};
