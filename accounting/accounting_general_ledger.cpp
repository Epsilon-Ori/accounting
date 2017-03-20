#include "accounting_general_ledger.h"
#include <iostream>
#include <fstream>

using namespace accounting;

ledger::ledger() : accounts{map<string, account>()}
{

}


ledger::~ledger()
{
	accounts.clear();
}

void accounting::ledger::createAccount(string name, account_type type)
{
	accounts[name] = account(name, type);
}

account *accounting::ledger::findAccount(string name)
{
	if (accounts.empty())
		return nullptr;

	if (accounts.count(name) == 1) {
		return &(accounts[name]);
	}
	else {
		return nullptr;
	}
}

const map<string, account> &accounting::ledger::getAccounts() const {
	return accounts;
}

map<string, account> &accounting::ledger::getAccounts() {
	return accounts;
}

//Presuppose that all accounts mentioned in the entry exists
//Accounts should be changed through the ledger functions
void accounting::ledger::postLine(line & l)
{
	account *found = this->findAccount(l.getAccountName());
	if (found) {
		found->addLine(l);
	}
}

void ledger::write(ostream & os)
{
	os << accounts.size() << " ";
	for (map<string, account>::iterator itr = accounts.begin(); itr != accounts.end(); itr++) {
		itr->second.write(os);
	}
}

void ledger::read(istream & os)
{
	accounts.clear();
	int size;
	os >> size;
	account acc;
	for (int i = 0; i < size; i++) {
		acc.read(os);
		accounts[acc.getName()] = acc;
	}
}

void accounting::ledger::analyze_ledger(string fileName)
{
	ofstream ledgerFile(fileName);

	for (map<string, account>::iterator itr = accounts.begin(); itr != accounts.end(); itr++) {
		ledgerFile << itr->first << endl;
		ledgerFile << "DATE,DEBITS,CREDITS" << endl; //Column headers for this account
		for (list<line>::iterator line_itr = itr->second.getLines().begin(); line_itr != itr->second.getLines().end(); line_itr++) {
			//Use the associated entry. All lines in the ledger must have an associated entry
			date d = line_itr->getAssociated();
			ledgerFile << d.getMonth() << "/" << d.getDay() << "/" << d.getYear() << ",";
			if (line_itr->getLineType() == DEBIT) {
				ledgerFile << line_itr->getAmount() << "," << endl;
			}
			else if (line_itr->getLineType() == CREDIT) {
				ledgerFile << "," << line_itr->getAmount() << endl;
			}
			
		}
		//Need to do summary on the bottom
		ledgerFile << "TOTAL DEBITS," << itr->second.getTotalDebits() << "," << endl;
		ledgerFile << "TOTAL CREDITS,," << itr->second.getTotalCredits() << endl;
		ledgerFile << "NET VALUE,";
		if (abs(itr->second.getTotalDebits()) > abs(itr->second.getTotalCredits())) {
			ledgerFile << itr->second.getTotal() << "," << endl;
		}
		else if (abs(itr->second.getTotalDebits()) < abs(itr->second.getTotalCredits())) {
			ledgerFile << "," << itr->second.getTotal() << endl;
		}
		else {
			//Equal
			ledgerFile << "," << endl;
		}
	}

	ledgerFile.flush();
	ledgerFile.close();
}

void accounting::ledger::display_ledger()
{
	for (map<string, account>::iterator itr = accounts.begin(); itr != accounts.end(); itr++) {
		cout << itr->first << endl;
		cout << "DATE,DEBITS,CREDITS" << endl; //Column headers for this account
		for (list<line>::iterator line_itr = itr->second.getLines().begin(); line_itr != itr->second.getLines().end(); line_itr++) {
			//Use the associated entry. All lines in the ledger must have an associated entry
			date d = line_itr->getAssociated();
			cout << d.getMonth() << "/" << d.getDay() << "/" << d.getYear() << ",";
			if (line_itr->getLineType() == DEBIT) {
				cout << line_itr->getAmount() << "," << endl;
			}
			else if (line_itr->getLineType() == CREDIT) {
				cout << "," << line_itr->getAmount() << endl;
			}
			
		}
	}
}
