#pragma once

#include <Wt/WContainerWidget>

using namespace std;

namespace Wt {
	class WStackedWidget;
	class WAnchor;
}

class Session {};

class estate : public Wt::WContainerWidget {
	public:
		estate();
		void handleInternalPath(const string &internalPath);

	private:
		Wt::WStackedWidget *mainStack_;
		Wt::WContainerWidget *links_;
		Wt::WAnchor *back_to_accounts_;
		Wt::WAnchor *envelopes_;

		Session session_;
};
