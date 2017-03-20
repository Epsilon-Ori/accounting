#include "accounting_journal.h"
#include <fstream>

using namespace accounting;


journal::journal() : entries{list<entry*>()}
{
}


journal::~journal()
{
	clearEntries();
}

void accounting::journal::addEntry(entry * entry)
{
	entries.push_back(entry);
}

const list<entry*> &accounting::journal::getEntries() const { return entries;  }

void accounting::journal::write(ostream & os)
{
	os << getEntries().size() << " ";
	for (list<entry*>::const_iterator itr = getEntries().cbegin(); itr != getEntries().cend(); itr++) {
		(*itr)->write(os);
	}
}

void accounting::journal::read(istream & os)
{
	clearEntries();
	int size;
	os >> size;
	for (int i = 0; i < size; i++) {
		entry *e = new entry(date());
		e->read(os);
		addEntry(e);
	}
}

void accounting::journal::analyze_journal(string fileName)
{
	ofstream jourFile(fileName);
	//Create a CSV file that lists the entries in the current journal
	//The 4 columns are:
	//Date, debit, credit, and amount
	//
	//The journal is suppose to tell a story about how the accounts have
	//changed over time
	
	//Create header
	jourFile << "DATE,DEBIT,CREDIT,AMOUNT" << endl;
	for (list<entry*>::iterator itr = entries.begin(); itr != entries.end(); itr++) {
		//There is at least one line in the entry
		list<line>::iterator line_itr = (*itr)->getLines().begin();

		jourFile << (*itr)->getDate().getMonth() << "/" << (*itr)->getDate().getDay() << "/" << (*itr)->getDate().getYear() << ",";
		//Do the rest of it for the first row
		if (line_itr->getLineType() == DEBIT) {
			jourFile << line_itr->getAccountName() << "," /* Credit Column */ << "," << line_itr->getAmount() << endl;
		}
		else if (line_itr->getLineType() == CREDIT) {
			jourFile << "," /* Debit Column */ << "," << line_itr->getAccountName() << "," << line_itr->getAmount() << endl;
		}
		line_itr++;
		while (line_itr != (*itr)->getLines().end()) {
			/*Relatively bad coding with the amount of copy-paste here */
			jourFile << ","; //Skip date column
			if (line_itr->getLineType() == DEBIT) {
				jourFile << line_itr->getAccountName() << "," /* Credit Column */ << "," << line_itr->getAmount() << endl;
			}
			else if (line_itr->getLineType() == CREDIT) {
				jourFile << /* Debit Column */ "," << line_itr->getAccountName() << "," << line_itr->getAmount()  << endl;
			}
			line_itr++;
		}
		
		//End of writing the entry, onto the next one
	}
	//Finished writing all entries
	jourFile.flush();
	jourFile.close();
}

void accounting::journal::display_journal()
{
	cout << "-------< JOURNAL >-------" << endl;
	entry *cur = nullptr;
	list<entry*>::iterator itr;
	for (itr = entries.begin(); itr != entries.end(); itr++) {
		cur = static_cast<entry*>(*itr);
		for (list<line>::iterator l_itr = cur->getLines().begin(); l_itr != cur->getLines().end(); l_itr++) {
			if (l_itr->getLineType() == DEBIT) {
				cout << "DEBIT   " <<  l_itr->getAccountName() << "  " << l_itr->getAmount() << endl;
			}
			else if (l_itr->getLineType() == CREDIT) {
				cout << "CREDIT  " << l_itr->getAccountName() << "  " << l_itr->getAmount() << endl;
			}
		}
		cout << "-------------------------" << endl;
	}
}

void accounting::journal::clearEntries()
{
	for (list<entry*>::const_iterator itr = getEntries().cbegin(); itr != getEntries().cend(); itr++) {
		delete *itr;
	}
	entries.clear();
}
