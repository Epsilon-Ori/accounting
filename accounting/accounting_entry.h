#ifndef __ACCOUNTING_ENTRY__
#define __ACCOUNTING_ENTRY__

#include "accounting_date.h"
#include "accounting_io.h"
#include "accounting_line.h"
#include <list>

using namespace std;

namespace accounting {
	class entry : accounting_io
	{
	public:
		explicit entry(const date &d);
		virtual ~entry();
		list<line> &getLines();
		const date &getDate() const;
		//Checks to see if it follows the rule of credits equalling debits
		bool isValid();
		//Display
		void print();
		//Add line to entry as constructing it
		void addLineToEntry(line& ln);
		void write(ostream & os) override;
		void read(istream & os) override;
		
	private:
		date dat;
		list<line> lines;
		//Lines in the entry are in order from min-ref to max-ref
	};
}

#endif