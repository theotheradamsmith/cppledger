#include <Wt/WAnchor>
#include <Wt/WAnimation>
#include <Wt/WNavigationBar>
#include <Wt/WMenu>
#include <Wt/WText>
#include <Wt/WStackedWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WApplication>
#include <Wt/WBorderLayout>

#include "estate.h"
#include "menu_item.h"

using namespace Wt;

estate::estate() {
	WText *title = new WText("<h1>Ledger of Your Estate</h1>");
	addWidget(title);

	WContainerWidget *container = new WContainerWidget(this);
	container->setHeight(600);
	container->setStyleClass("backdrop");

	WBorderLayout *layout = new WBorderLayout();
	container->setLayout(layout);

	const char *cell = "{1} item";
	Wt::WText *item = new Wt::WText(Wt::WString(cell).arg("North"));
	item->setStyleClass("blue-box");
	layout->addWidget(item, Wt::WBorderLayout::North);

	item = new Wt::WText(Wt::WString(cell).arg("East"));
	item->setStyleClass("blue-box");
	layout->addWidget(item, Wt::WBorderLayout::East);

	item = new Wt::WText(Wt::WString(cell).arg("South"));
	item->setStyleClass("blue-box");
	layout->addWidget(item, Wt::WBorderLayout::South);

	item = new Wt::WText(Wt::WString(cell).arg("Center"));
	item->setStyleClass("green-box");
	layout->addWidget(item, Wt::WBorderLayout::Center);

	contentsStack_ = new WStackedWidget();
	WAnimation animation(WAnimation::Fade, WAnimation::Linear, 200);
	contentsStack_->setTransitionAnimation(animation, true);

	WMenu *menu = new WMenu(contentsStack_);
	menu->setInternalPathEnabled();
	menu->setInternalBasePath("/");
	addToMenu(menu, "Accounts", new MenuItem());
	addToMenu(menu, "Envelopes", new MenuItem());
	addToMenu(menu, "Summary", new MenuItem());

	navigation_ = new WNavigationBar();
	navigation_->addStyleClass("navigation");
	navigation_->setTitle("Estate Ledger", "http://localhost:9090");
	navigation_->setResponsive(true);
	navigation_->addMenu(menu);

	WVBoxLayout *nav_layout = new WVBoxLayout();
	nav_layout->addWidget(navigation_);
	nav_layout->addWidget(contentsStack_, 1);
	nav_layout->setContentsMargins(0, 0, 0, 0);

	WContainerWidget *nav_container = new WContainerWidget(this);
	nav_container->setStyleClass("blue-box");
	nav_container->setLayout(nav_layout);
	layout->addWidget(nav_container, Wt::WBorderLayout::West);

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
