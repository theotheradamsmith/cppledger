#include <signal.h>
#include <thread>

#include <Wt/WApplication>
#include <Wt/WBootstrapTheme>
#include <Wt/WHBoxLayout>
#include <Wt/WServer>

#include "account.h"
#include "estate.h"
#include "main.h"
#include "database.h"
#include "test.h"

using namespace std;

char *main_database_err_msg = nullptr;

Wt::WApplication *createApplication(const Wt::WEnvironment &env) {
	Wt::WApplication *app = new Wt::WApplication(env);

	if (app->appRoot().empty()) {
		cerr << "FATAL ERROR: unable to generate approot" << endl;
	}

	Wt::WBootstrapTheme *bootstrapTheme = new Wt::WBootstrapTheme(app);
	app->setTheme(bootstrapTheme);

	Wt::WHBoxLayout *layout = new Wt::WHBoxLayout(app->root());
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(new estate());

	app->setTitle("The Estate");
	/*
	app->messageResourceBundle().use(app->appRoot() + "strings");
	app->messageResourceBundle().use(app->appRoot() + "templates");
	*/
	app->useStyleSheet("css/estate.css");

	return app;
}

void init_server(int argc, char **argv) {
	try {
		Wt::WServer server(argv[0]);
		server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
		server.addEntryPoint(Wt::Application, createApplication);

		if (server.start()) {
			int sig = Wt::WServer::waitForShutdown(argv[0]);
			cerr << "Shutdown (signal = " << sig << ")" << endl;
			server.stop();

			if (sig == SIGHUP) {
				Wt::WServer::restart(argc, argv, environ);
			}
		}
	} catch (Wt::WServer::Exception &e) {
		cerr << e.what() << endl;
	} catch (exception &e) {
		cerr << "exception: " << e.what() << endl;
	}
}

int main(int argc, char **argv) {
	/* Attempt to connect to the ROOT_DB and ensure our primary vault table exists */
	database *db = new database(ROOT_DB);

	cout << endl;
	cout << "Welcome to your Estate. Initializing..." << endl;

	thread run_program(init_server, argc, argv);
	run_program.detach();

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
