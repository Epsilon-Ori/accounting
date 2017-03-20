#include "accounting_account.h"

using namespace accounting;

accounting::account::account(string name, account_type type) :
	type{type}, name{ name }, debits{0}, credits{0}, amount{0}, lines() {}

accounting::account::account(const account & rhs)
{
	lines = rhs.lines;
	name = rhs.name;
	type = rhs.type;
	debits = rhs.debits;
	credits = rhs.credits;
	amount = rhs.amount;
}

accounting::account::account() :
	type{ INVALID }, name{ "default_account_name" }, debits{ 0 }, credits{ 0 }, amount{ 0 }, lines() {}

void accounting::account::addLine(const line & ln)
{
	if (ln.getAccountName() == name) {
		this->lines.push_back(ln);
		if (ln.getLineType() == DEBIT) {
			debit(ln.getAmount());
		}
		else if (ln.getLineType() == CREDIT) {
			credit(ln.getAmount());
		}
	}
}


void accounting::account::debit(double amount)
{
	//assets = liabilities + equity + revenue - expenses
	switch (type) {
	case ASSET:
		this->amount += amount;
		debits += amount;
		break;
	case LIABILITY:
		this->amount -= amount;
		debits -= amount;
		break;
	case EQUITY:
		this->amount -= amount;
		debits -= amount;
		break;
	case REVENUE:
		this->amount -= amount;
		debits -= amount;
		break;
	case EXPENSE:
		this->amount += amount;
		debits += amount;
		break;
	default:
		cout << "INVALID ACCOUNT TYPE; CANNOT DEBIT" << endl;
	}
}

void accounting::account::credit(double amount)
{
	switch (type) {
	case ASSET:
		this->amount -= amount;
		credits -= amount;
		break;
	case LIABILITY:
		this->amount += amount;
		credits += amount;
		break;
	case EQUITY:
		this->amount += amount;
		credits += amount;
		break;
	case REVENUE:
		this->amount += amount;
		credits += amount;
		break;
	case EXPENSE:
		this->amount -= amount;
		credits -= amount;
		break;
	default:
		cout << "INVALID ACCOUNT TYPE; CANNOT CREDIT" << endl;
	}
}

void accounting::account::write(ostream & os)
{
	os << name << " ";
	os << type << " ";
	os << debits << " ";
	os << credits << " ";
	os << amount << " ";
	os << lines.size() << " ";
	for (list<line>::iterator itr = lines.begin(); itr != lines.end(); itr++) {
		itr->write(os);
	}
}

void accounting::account::read(istream & os)
{
	os >> name;
	int type_int;
	os >> type_int;
	type = static_cast<account_type>(type_int);
	os >> debits;
	os >> credits;
	os >> amount;
	int amt;
	os >> amt;
	lines.clear();
	for (int i = 0; i < amt; i++) {
		line ln("", DEBIT, 0); //duplicate lines, but that should not be a problem
		ln.read(os);
		lines.push_back(ln);
	}
}

string accounting::account::getName() const
{
	return name;
}

account_type accounting::account::getType() const
{
	return type;
}

account & accounting::account::operator=(const account & rhs)
{
	lines = rhs.lines;
	name = rhs.name;
	type = rhs.type;
	debits = rhs.debits;
	credits = rhs.credits;
	amount = rhs.amount;
	return *this;
}

list<line> &accounting::account::getLines() {
	return lines;
}