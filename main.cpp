#include "account.h"
#include "main.h"
#include "database.h"
#include "test.h"

using namespace std;

char *main_database_err_msg = nullptr;

int main(int argc, char **argv) {
	/* Attempt to connect to the ROOT_DB and ensure our primary vault table exists */
	database *db = new database(ROOT_DB);

	cout << endl;
	cout << "Welcome to your Estate. Initializing..." << endl;

	//db.load_data();

	if (test_interface_loop(*db) != 0) {
		fprintf(stderr, "FATAL ERR: Interface Loop failure\n");
		db->close_database_connection();
		exit(1);
	}

	/* Close the database */
	db->close_database_connection();
	delete db;

	return 0;
}
