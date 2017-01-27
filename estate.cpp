#include <Wt/WAnchor>
#include <Wt/WText>
#include <Wt/WStackedWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WApplication>

#include "estate.h"

using namespace Wt;

estate::estate() {
	WText *title = new WText("<h1>Ledger of Your Estate</h1>");
	addWidget(title);

	mainStack_ = new WStackedWidget();
	//mainStack_->setStyleClass("");
	addWidget(mainStack_);

	links_ = new WContainerWidget();
	//links_->setStyleClass("links");
	addWidget(links_);

	WApplication::instance()->internalPathChanged().connect(this, &estate::handleInternalPath);

}

void estate::handleInternalPath(const string &internalPath) {
}
