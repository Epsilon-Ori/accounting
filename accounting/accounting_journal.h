#ifndef __ACCOUNTING_JOURNAL__
#define __ACCOUNTING_JOURNAL__

#include "accounting_entry.h"
#include "accounting_io.h"
#include <list>

namespace accounting {
	class journal : accounting_io
	{
	public:
		journal();
		virtual ~journal();
		void addEntry(entry *entry);
		const list<entry*> &getEntries() const;
		void write(ostream & os) override;
		void read(istream & os) override;
		void analyze_journal(string fileName);
		void display_journal();
	private:
		list<entry*> entries; //List of pointers to entities
		void clearEntries();
	};
}

#endif