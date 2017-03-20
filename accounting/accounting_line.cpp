#include "accounting_line.h"

using namespace accounting;

accounting::line::line(string account, line_type type, double amount) :
	account{account}, type{type}, amount{amount}, associated{date()} {}

line::line(const line &ln) {
	account = ln.account;
	type = ln.type;
	amount = ln.amount;
	associated = ln.associated; //Copying the dates
}

void accounting::line::write(ostream & os)
{
	os << account << " ";
	os << type << " ";
	os << amount << " ";
	os << associated << " ";
}

void accounting::line::read(istream & is)
{
	is >> account;
	int type_int;
	is >> type_int;
	type = static_cast<line_type>(type_int);
	is >> amount;
	is >> associated;
}

date line::getAssociated() const {
	return associated;
}

void line::setAssociated(date &ent) {
	associated = ent;
}