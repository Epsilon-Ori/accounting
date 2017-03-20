#include "accounting_entry.h"
#include <iostream>

using namespace accounting;

accounting::entry::entry(const date & d) :
	dat{ d }, lines{ list<line>() }
{
}

entry::~entry()
{
}

list<line>& accounting::entry::getLines()
{
	return lines;
}

const date & accounting::entry::getDate() const
{
	return dat;
}

bool accounting::entry::isValid()
{
	if (lines.empty())
		return false;

	double debit{ 0 };
	double credit{ 0 };
	list<line>::iterator itr;
	for (itr = lines.begin(); itr != lines.end(); itr++) {
		if (itr->getLineType() == DEBIT) {
			debit += itr->getAmount();
		}
		else if (itr->getLineType() == CREDIT) {
			credit += itr->getAmount();
		}
	}
	//cout << "Debits counted to " << debit << " and credits counted to " << credit << endl;
	return /*Approximately equal */ abs(debit-credit) < 0.001; //Equal to the first few decimal points  
}

void accounting::entry::print()
{
	std::cout << "Need to write print for entries" << endl;
}

void accounting::entry::addLineToEntry(line & ln)
{
	ln.setAssociated(dat);
	lines.push_back(ln);
}

void accounting::entry::write(ostream & os)
{
	os << dat << " ";
	os << lines.size() << " ";
	for (list<line>::iterator itr = lines.begin(); itr != lines.end(); itr++) {
		itr->write(os);
	}
}

void accounting::entry::read(istream & is)
{
	is >> dat;
	int size;
	is >> size;
	for (int i = 0; i < size; i++) {
		line ln("", DEBIT, 0);
		ln.read(is);
		lines.push_back(ln);
	}
}

