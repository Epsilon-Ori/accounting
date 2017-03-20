#ifndef __ACCOUNTING_DATE__
#define __ACCOUNTING_DATE__

#include <iostream>

namespace accounting {
	class date
	{
	public:
		date();
		date(int month, int day, int year);
		int getMonth() const;
		int getDay() const;
		int getYear() const;
		~date();
		date(const date& d);
		date &operator=(const date &d);
		date operator+(const date &d);
		date operator-(const date &d);
		bool operator==(const date &d);
		bool operator<(const date &d);
		bool operator>(const date &d);
		bool operator<=(const date &d);
		bool operator>=(const date &d);
		bool operator!=(const date &d);
		static int days_per_month[12];
	private:
		int month;
		int day;
		int year;
	};
	std::ostream &operator<<(std::ostream &os, const date &d);
	std::istream &operator>>(std::istream &is, date &d);
}

#endif