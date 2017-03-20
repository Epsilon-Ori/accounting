#ifndef __ACCOUNTING_ACCOUNT__
#define __ACCOUNTING_ACCOUNT__

#include "accounting_line.h"
#include "accounting_entry.h"
#include "accounting_account_type.h"
#include "accounting_io.h"
#include <string>
#include <list>

using namespace std;

namespace accounting {
	class account : accounting_io
	{
	public:
		explicit account(string name, account_type type);
		account(const account &rhs);
		account();
		~account() = default;

		//Lines in the entry that affect this account will be applied
		void addLine(const line &ln);
		list<line> &getLines();
		void debit(double amount);
		void credit(double amount);
		void write(ostream & os) override;
		void read(istream & os) override;
		string getName() const;
		account_type getType() const;
		double getTotalDebits() const { return debits; }
		double getTotalCredits() const { return credits; }
		double getTotal() const { return amount;  }
		//operators
		account &operator=(const account &rhs);
	private:
		list<line> lines;
		string name;
		account_type type;
		double debits;
		double credits;
		double amount;
	};
}

#endif