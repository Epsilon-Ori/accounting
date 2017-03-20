#ifndef __ACCOUNTING_ACCOUNT_TYPE__
#define __ACCOUNTING_ACCOUNT_TYPE__

#include <string>

namespace accounting {
	enum account_type {
		ASSET,
		LIABILITY,
		EQUITY,
		REVENUE,
		EXPENSE,
		INVALID
	};
	std::string accountTypeToString(account_type type);
}

#endif