#include "database.h"

#define DEBUG 0

ostream &operator<<(ostream &out, database &database_obj) {
	out << database_obj.get_database_connection();
	return out;
}

database::database(const char *n) {
	/* Check to see if the proposed database file exists; if not, create it.
	 * Exit on fatal error.
	 */
	FILE *fp;
	if ((fp = fopen(n, "ab+"))) {
		fclose(fp);
	} else {
		fprintf(stderr, "FATAL ERROR: Unable to open/create ROOT_DB!\n");
		exit(1);
	}

	// Attempt to connect to the database, saving connection in database_connection
	int rc;
	if ((rc = sqlite3_open(database_name, &database_connection)) != SQLITE_OK) {
		fprintf(stderr, "ERROR: Cannot open database: %s\n", sqlite3_errmsg(database_connection));
		exit(1);
	}
	database_name = n;
}

database::~database() {
}

sqlite3 *database::get_database_connection() const {
	return database_connection;
}

void database::close_database_connection() {
	sqlite3_close(database_connection);
}

int database::initialize_vault() {
	const char *sql =
		"BEGIN TRANSACTION;"
		"CREATE TABLE IF NOT EXISTS accounts("
			"id INTEGER PRIMARY KEY,"
			"name TEXT,"
			"total_account_balance INTEGER,"
			"balance INTEGER,"
			"number_of_envelopes INTEGER,"
			"UNIQUE(name)"
		");"
		"INSERT OR IGNORE INTO accounts VALUES(1, 'root_account', 0, 0, 0);"
		"CREATE TABLE IF NOT EXISTS envelopes("
			"id INTEGER PRIMARY KEY,"
			"owner_account INTEGER,"
			"name TEXT,"
			"balance INTEGER,"
			"increment_value INTEGER,"
			"category TEXT,"
			"UNIQUE(name)"
		");"
		"COMMIT;";
	int rc = sqlite3_exec(database_connection, sql, 0, 0, &main_database_err_msg);
	return(rc);
}

int database::load_data(list<account> &acc_list) {
	sqlite3_stmt *res;
	int rc;
	const char *accounts_sql =
		"SELECT "
			"name, "
			"total_account_balance, "
			"balance, "
			"number_of_envelopes "
		"FROM "
			"accounts;";
	if ((rc = sqlite3_prepare_v2(database_connection, accounts_sql, -1, &res, 0)) == SQLITE_OK) {
	} else {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database_connection));
		return(-1);
	}

	while (1) {
		int s = sqlite3_step(res);
		if (s == SQLITE_ROW) {
			string row_name = (const char *)sqlite3_column_text(res, 0);
			long row_total_account_balance = sqlite3_column_int(res, 1);
			long row_balance = sqlite3_column_int(res, 2);
			int row_number_of_envelopes = sqlite3_column_int(res, 3);
			if (DEBUG) {
				cout << "Account " << row_name << ": " << row_total_account_balance;
				cout << " <=> " << row_balance << " ... " << row_number_of_envelopes;
			}
			account new_account(row_name, row_total_account_balance);
			new_account.set_available_balance(row_balance);
			for (int i = 0; i < row_number_of_envelopes; ++i) {
				cout << "push back an envelope here" << endl;
			}
			acc_list.push_back(new_account);
		} else if (s == SQLITE_DONE) {
			break;
		} else {
			fprintf(stderr, "Failed to read row.\n");
			sqlite3_close(database_connection);
			exit(1);
		}
	}

	//const char *envelopes_sql = "";
	return 0;
}

int database::save_data(account &acc) {
	sqlite3_stmt *res;
	int rc;
	const char *accounts_sql = "INSERT OR UPDATE INTO accounts("
		"name, total_account_balance, balance, number_of_envelopes)"
		"VALUES (@nam, @tot_bal, @bal, @num_env);";
	if ((rc = sqlite3_prepare_v2(database_connection, accounts_sql, -1, &res, 0)) == SQLITE_OK) {
		int nam_idx = sqlite3_bind_parameter_index(res, "@nam");
		int tot_bal_idx = sqlite3_bind_parameter_index(res, "@tot_bal");
		int bal_idx = sqlite3_bind_parameter_index(res, "@bal");
		int num_env_idx = sqlite3_bind_parameter_index(res, "@num_env_idx");

		sqlite3_bind_text(res, nam_idx, acc.get_name().c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_int(res, tot_bal_idx, acc.get_total_account_balance());
		sqlite3_bind_int(res, bal_idx, acc.get_available_balance());
		sqlite3_bind_int(res, num_env_idx, acc.get_number_of_envelopes());
	} else {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database_connection));
		return -1;
	}

	if ((rc = sqlite3_step(res)) != SQLITE_DONE) {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database_connection));
		return -1;
	}

	sqlite3_finalize(res);

	return 0;
}

int database::save_data(envelope &env, int owner_id) {
	sqlite3_stmt *res;
	int rc;
	const char *envelopes_sql = "INSERT OR UPDATE INTO envelopes("
		"owner_account, name, balance, increment_value, category)"
		"VALUES (@owner, @nam, @bal, @inc, @cat);";
	if ((rc = sqlite3_prepare_v2(database_connection, envelopes_sql, -1, &res, 0)) == SQLITE_OK) {
		int owner_idx = sqlite3_bind_parameter_index(res, "@owner");
		int nam_idx = sqlite3_bind_parameter_index(res, "@nam");
		int bal_idx = sqlite3_bind_parameter_index(res, "@bal");
		int inc_idx = sqlite3_bind_parameter_index(res, "@inc");
		int cat_idx = sqlite3_bind_parameter_index(res, "@cat");

		sqlite3_bind_int(res, owner_idx, owner_id);
		sqlite3_bind_text(res, nam_idx, env.get_name().c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_int(res, bal_idx, env.get_available_balance());
		sqlite3_bind_int(res, inc_idx, env.get_increment_value());
		sqlite3_bind_text(res, cat_idx, env.get_category().c_str(), -1, SQLITE_STATIC);
	} else {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database_connection));
		return -1;
	}

	if ((rc = sqlite3_step(res)) != SQLITE_DONE) {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database_connection));
		return -1;
	}

	sqlite3_finalize(res);

	return 0;
}
