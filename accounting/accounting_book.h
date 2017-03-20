#ifndef __ACCOUNTING_BOOK__
#define __ACCOUNTING_BOOK__

#include "accounting_journal.h"
#include "accounting_general_ledger.h"
#include "accounting_io.h"
#include <iostream>

using namespace std;

namespace accounting {

	class accounting_book : accounting_io
	{
	public:
		accounting_book();
		virtual ~accounting_book();
		//Don't copy or set the accounting book object
		accounting_book(const accounting_book &ac) = delete;
		accounting_book &operator=(const accounting_book &rhs) = delete;
		void closeBooks(date &d);
		void post(entry *ent);
		journal *getJournal() const;
		ledger *getGeneralLedger() const;
		void write(ostream & os) override;
		void read(istream & os) override;
	private:
		journal *j;
		ledger *l;
	};

}

#endif