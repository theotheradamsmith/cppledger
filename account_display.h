#pragma once

#include <Wt/WText>
#include "account.h"

class basic_display {
	public:
		virtual void refresh() = 0;
		virtual Wt::WText *get_display() const = 0;
		void set_style(string s) {
			style = s;
		}
	protected:
		string style;
		Wt::WText *display = nullptr;
};

class account_display : public basic_display {
	public:
		account_display(account *a, string s);
		Wt::WText *get_display() const override;
		void refresh() override;
	private:
		account *acc;
};

class envelope_display : public basic_display {
	public:
		envelope_display(envelope *e, string s);
		Wt::WText *get_display() const override;
		void refresh() override;
	private:
		envelope *env;
};
