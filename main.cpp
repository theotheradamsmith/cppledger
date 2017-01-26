#include "account.h"
#include "main.h"
#include "database.h"
#include "test.h"

using namespace std;

char *main_database_err_msg = nullptr;

int main(int argc, char **argv) {
	/* Check to see if the ROOT_DB file exists; if not, create it.
	 * Exit on fatal error.
	 */
	FILE *fp;
	if ((fp = fopen(ROOT_DB, "ab+"))) {
		fclose(fp);
	} else {
		fprintf(stderr, "FATAL ERR: Unable to open/create ROOT_DB!\n");
		exit(1);
	}

	/* Attempt to connect to the ROOT_DB and ensure our primary vault table exists */
	database db(ROOT_DB);

	cout << endl;
	cout << "Welcome to your Estate. Initializing..." << endl;

	int rc;

	if ((rc = db.initialize_vault()) != SQLITE_OK) {
		fprintf(stderr, "FATAL ERR: SQL error: %s\n", main_database_err_msg);
		sqlite3_free(main_database_err_msg);
		db.close_database_connection();
		exit(1);
	}

	//db.load_data();

	if (test_interface_loop(db) != 0) {
		fprintf(stderr, "FATAL ERR: Interface Loop failure\n");
		db.close_database_connection();
		exit(1);
	}

	/* Close the database */
	db.close_database_connection();

	return 0;
}
