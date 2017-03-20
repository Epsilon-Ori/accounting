#ifndef __ACCOUNTING_GENERAL_LEDGER__
#define __ACCOUNTING_GENERAL_LEDGER__

#include "accounting_account.h"
#include "accounting_io.h"
#include <string>
#include <map>
#include <iterator>

using namespace std;

namespace accounting {
	class ledger : accounting_io
	{
	public:
		ledger();
		virtual ~ledger();

		void createAccount(string name, account_type type);
		account *findAccount(string name);
		const map<string, account> &getAccounts() const;
		map<string, account> &getAccounts();
		void postLine(line &l);
		void write(ostream & os) override;
		void read(istream & os) override;
		void analyze_ledger(string fileName);
		void display_ledger();
	private:
		map<string, account> accounts;
	};

}

#endif