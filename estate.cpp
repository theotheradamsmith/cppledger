#include <Wt/WAnchor>
#include <Wt/WAnimation>
#include <Wt/WNavigationBar>
#include <Wt/WMenu>
#include <Wt/WText>
#include <Wt/WStackedWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WApplication>

#include "estate.h"
#include "menu_item.h"

using namespace Wt;

estate::estate() {
	WText *title = new WText("<h1>Ledger of Your Estate</h1>");
	addWidget(title);

	contentsStack_ = new WStackedWidget();
	WAnimation animation(WAnimation::Fade, WAnimation::Linear, 200);
	contentsStack_->setTransitionAnimation(animation, true);

	navigation_ = new WNavigationBar();
	navigation_->addStyleClass("navigation");
	navigation_->setTitle("Estate Ledger", "http://localhost:9090");
	navigation_->setResponsive(true);

	WMenu *menu = new WMenu(contentsStack_);
	menu->setInternalPathEnabled();
	menu->setInternalBasePath("/");

	addToMenu(menu, "Accounts", new MenuItem());
	addToMenu(menu, "Envelopes", new MenuItem());
	addToMenu(menu, "Summary", new MenuItem());

	navigation_->addMenu(menu);

	WVBoxLayout *layout = new WVBoxLayout(this);
	layout->addWidget(navigation_);
	layout->addWidget(contentsStack_, 1);
	layout->setContentsMargins(0, 0, 0, 0);

	WApplication::instance()->internalPathChanged().connect(this, &estate::handleInternalPath);

}

WMenuItem *estate::addToMenu(WMenu *menu, const WString &name, MenuItem *topic) {
	WContainerWidget *result = new WContainerWidget();
	WContainerWidget *pane = new WContainerWidget();
	WVBoxLayout *vLayout = new WVBoxLayout(result);
	vLayout->setContentsMargins(0, 0, 0, 0);
	vLayout->addWidget(topic);
	vLayout->addWidget(pane, 1);

	WHBoxLayout *hLayout = new WHBoxLayout(pane);
	WMenuItem *item = new WMenuItem(name, result);
	menu->addItem(item);

	WStackedWidget *subStack = new WStackedWidget();
	subStack->addStyleClass("contents");
	subStack->setOverflow(WContainerWidget::OverflowAuto);

	WMenu *subMenu = new WMenu(subStack);
	subMenu->addStyleClass("nav-pills");
	subMenu->setWidth(200);
	hLayout->addWidget(subMenu);
	hLayout->addWidget(subStack, 1);

	subMenu->setInternalPathEnabled();
	subMenu->setInternalBasePath("/" + item->pathComponent());

	topic->populateSubMenu(subMenu);

	return item;
}

void estate::handleInternalPath(const string &internalPath) {
}
