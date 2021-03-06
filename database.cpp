#include <cstring>
#include <assert.h>
#include "database.h"

#define DEBUG 1

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

	sqlite3 *db;
	int rc;
	if ((rc = sqlite3_open(n, &db)) != SQLITE_OK) {
		fprintf(stderr, "FATAL: Cannot open database: %s\n", sqlite3_errmsg(db));
		exit(1);
	}
	database_connection = db;
	database_name = n;

	if ((rc = initialize_vault()) != SQLITE_OK) {
		fprintf(stderr, "FATAL ERR: SQL error: %s\n", main_database_err_msg);
		sqlite3_free(main_database_err_msg); close_database_connection();
		close_database_connection();
		exit(1);
	}
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
		"INSERT OR IGNORE INTO accounts(id, name, total_account_balance, balance, number_of_envelopes) VALUES(1, 'root_account', 0, 0, 0);"
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

int database::load_data(list<account *> &acc_list) {
	sqlite3_stmt *res;
	int rc;
	const char *accounts_sql =
		"SELECT "
			"id, "
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
			int row_id = sqlite3_column_int(res, 0);
			string row_name = (const char *)sqlite3_column_text(res, 1);
			long row_total_account_balance = sqlite3_column_int(res, 2);
			long row_balance = sqlite3_column_int(res, 3);
			int row_number_of_envelopes = sqlite3_column_int(res, 4);

			if (DEBUG) {
				cout << "Account " << row_name << ": " << row_total_account_balance;
				cout << " <=> " << row_balance << " ... " << row_number_of_envelopes;
			}

			// Create a new account, set its available balance, and load envelopes
			account *new_account = new account(row_name, row_total_account_balance, row_id);
			new_account->set_available_balance(row_balance);
			load_envelope_data(*new_account);

			// Add the new account to the account list
			acc_list.push_back(new_account);
		} else if (s == SQLITE_DONE) {
			break;
		} else {
			fprintf(stderr, "Failed to read row.\n");
			sqlite3_close(database_connection);
			exit(1);
		}
	}

	return 0;
}

int database::load_envelope_data(account &acc) {
	sqlite3_stmt *res;
	int rc;
	const char *envelopes_sql =
		"SELECT "
			"id, "
			"owner_account, "
			"name, "
			"balance, "
			"increment_value, "
			"category "
		"FROM "
			"envelopes "
		"WHERE "
			"owner_account = @acc_id"
		";"
	;
	if ((rc = sqlite3_prepare_v2(database_connection, envelopes_sql, -1, &res, 0)) == SQLITE_OK) {
		int acc_idx = sqlite3_bind_parameter_index(res, "@acc_id");
		sqlite3_bind_int(res, acc_idx, acc.get_id());
	} else {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database_connection));
		return -1;
	}

	while (1) {
		int s = sqlite3_step(res);
		if (s == SQLITE_ROW) {
			int row_id = sqlite3_column_int(res, 0);
			int row_owner_account = sqlite3_column_int(res, 1);
			string row_name = (const char *)sqlite3_column_text(res, 2);
			long row_balance = sqlite3_column_int(res, 3);
			long row_increment_value = sqlite3_column_int(res, 4);
			string row_category = (const char *)sqlite3_column_text(res, 5);

			assert(row_owner_account == acc.get_id());

			acc.create_envelope(row_name, row_balance, row_increment_value, false, row_id);

		} else if (s == SQLITE_DONE) {
			break;
		} else {
			fprintf(stderr, "Failed to read row.\n");
			sqlite3_close(database_connection);
			exit(1);
		}
	}

	return 0;
}

int database::save_data(account &acc) {
	sqlite3_stmt *res;
	int rc;
	const char *accounts_sql =
		"INSERT OR REPLACE INTO "
			"accounts( "
				"id, "
				"name, "
				"total_account_balance, "
				"balance, "
				"number_of_envelopes"
			") "
		"VALUES ("
			"@id, "
			"@nam, "
			"@tot_bal, "
			"@bal, "
			"@num_env"
		")"
		";"
	;
	if ((rc = sqlite3_prepare_v2(database_connection, accounts_sql, -1, &res, 0)) == SQLITE_OK) {
		int id_idx = sqlite3_bind_parameter_index(res, "@id");
		int nam_idx = sqlite3_bind_parameter_index(res, "@nam");
		int tot_bal_idx = sqlite3_bind_parameter_index(res, "@tot_bal");
		int bal_idx = sqlite3_bind_parameter_index(res, "@bal");
		int num_env_idx = sqlite3_bind_parameter_index(res, "@num_env");

		sqlite3_bind_int(res, id_idx, acc.get_id());
		char *acc_name = strdup(acc.get_name().c_str());
		sqlite3_bind_text(res, nam_idx, acc_name, -1, SQLITE_STATIC);
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

	for (auto env : acc.envelopes) {
		save_data(*env, acc.get_id());
	}

	return 0;
}

int database::save_data(envelope &env, int owner_id) {
	sqlite3_stmt *res;
	int rc;
	const char *envelopes_sql = "INSERT OR REPLACE INTO envelopes("
		"id, owner_account, name, balance, increment_value, category)"
		"VALUES (@id, @owner, @nam, @bal, @inc, @cat);";
	if ((rc = sqlite3_prepare_v2(database_connection, envelopes_sql, -1, &res, 0)) == SQLITE_OK) {
		int id_idx = sqlite3_bind_parameter_index(res, "@id");
		int owner_idx = sqlite3_bind_parameter_index(res, "@owner");
		int nam_idx = sqlite3_bind_parameter_index(res, "@nam");
		int bal_idx = sqlite3_bind_parameter_index(res, "@bal");
		int inc_idx = sqlite3_bind_parameter_index(res, "@inc");
		int cat_idx = sqlite3_bind_parameter_index(res, "@cat");

		sqlite3_bind_int(res, id_idx, env.get_id());
		sqlite3_bind_int(res, owner_idx, owner_id);
		const char *env_name = strdup(env.get_name().c_str());
		sqlite3_bind_text(res, nam_idx, env_name, -1, SQLITE_STATIC);
		sqlite3_bind_int(res, bal_idx, env.get_available_balance());
		sqlite3_bind_int(res, inc_idx, env.get_increment_value());
		char *env_category = strdup(env.get_category().c_str());
		sqlite3_bind_text(res, cat_idx, env_category, -1, SQLITE_STATIC);
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
