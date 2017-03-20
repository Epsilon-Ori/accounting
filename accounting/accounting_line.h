#ifndef __ACCOUNTING_LINE__
#define __ACCOUNTING_LINE__

#include "accounting_line_type.h"
#include "accounting_io.h"
#include "accounting_date.h"
#include <string>

using namespace std;

namespace accounting {
	class line : accounting_io {
	public:
		line(string account, line_type type, double amount);
		line(const line &ln);
		~line() = default;
		string getAccountName() const { return account;  }
		line_type getLineType() const { return type;  }
		double getAmount() const { return amount;  }
		void write(ostream &os) override;
		void read(istream &is) override;
		date getAssociated() const;
		void setAssociated(date &ent);
	private:
		string account;
		line_type type;
		double amount;
		date associated;
	};

	
}

#endif