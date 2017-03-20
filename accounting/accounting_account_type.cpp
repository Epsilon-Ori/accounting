#include "accounting_account_type.h"

using namespace std;

string accounting::accountTypeToString(account_type type)
{
	if (type == ASSET) {
		return "asset";
	}
	else if (type == LIABILITY) {
		return "liability";
	}
	else if (type == EQUITY) {
		return "equity";
	}
	else if (type == REVENUE) {
		return "revenue";
	}
	else if (type == EXPENSE) {
		return "expense";
	}
	else {
		return "invalid";
	}
}
