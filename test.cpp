#include <list>
#include "test.h"

int test_interface_loop(database &db) {
	list<account *> acc_list;
	string name;
	long bal;
	long inc;
	envelope *env;

	db.load_data(acc_list);

	while (1) {
		cout << "\n"
			 << "Options" << endl
			 << "1) Display Grand Total" << endl
			 << "2) Create an envelope" << endl
			 << "3) Display an envelope total" << endl
			 << "4) Increment an envelope" << endl
			 << "5) Print Account" << endl
			 << "8) Initialize main account " << endl
			 << "9) Save account data " << endl
			 << "0 to Quit" << endl
			 << endl;
		int selection;
		cin >> selection;

		switch (selection) {
			case 1:
				cout << "Grand Total: " << acc_list.front()->get_total_account_balance() << endl;
				break;
			case 2:
				cout << "Please provide a title for your test envelope: ";
				cin >> name;
				cout << "Please enter a balance for your test envelope: ";
				cin >> bal;
				cout << "Please enter a default increment amount: ";
				cin >> inc;
				acc_list.front()->create_envelope(name, bal, inc, -1);
				break;
			case 3:
				cout << "Envelope name: ";
				cin >> name;
				env = acc_list.front()->locate_envelope(name);
				if (env) {
					cout << *env << endl;
				} else {
					cout << "No such envelope found" << endl;
				}
				break;
			case 4:
				cout << "Envelope name: ";
				cin >> name;
				acc_list.front()->increment_envelope(name);
				break;
			case 5:
				cout << *(acc_list.front()) << endl;
				break;
			case 8:
				cout << "Please provide a starting balance for the main account: ";
				cin >> bal;
				acc_list.front()->set_total_account_balance(bal);
				acc_list.front()->set_available_balance(bal);
				break;
			case 9:
				cout << "Saving data..." << endl;
				db.save_data(*(acc_list.front()));
				break;
			default:
				return 0;
		}
	}
}
