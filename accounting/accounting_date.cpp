#include "accounting.h"
#include "accounting_date.h"

#include <iostream>

using namespace accounting;

int date::days_per_month[12]{
	31, //January
	28, //February
	31, //March
	30, //April
	31, //May
	30, //June
	31, //July
	31, //August
	30, //September 
	31, //October
	30, //November
	31  //December
};

date::date()
{
	year = 1970;
	month = 1;
	day = 1;
}

accounting::date::date(int month, int day, int year = 0) : month{month}, day{day}, year{year}
{
}


int date::getMonth() const { return month;  }
int date::getDay() const { return day;  }
int date::getYear() const { return year;  }

date::~date()
{
}

accounting::date::date(const date & d)
{
	day = d.day;
	month = d.month;
	year = d.year;
}

date & accounting::date::operator=(const date & d)
{
	month = d.month;
	day = d.day;
	year = d.year; 
	return *this;
}

date accounting::date::operator+(const date & d)
{
	date new_date(*this);
	new_date.day += d.day;
	while (new_date.day > days_per_month[new_date.month-1]) {
		//Need to cut it off
		new_date.day -= days_per_month[new_date.month -1];
		new_date.month++;
		if (new_date.month > 12) {
			new_date.month -= 12;
			new_date.year++;
		}
	}
	new_date.month += d.month;
	if (new_date.month > 12) {
		new_date.month -= 12;
		new_date.year++;
	}
	new_date.year += d.year;
	return new_date;
}

date accounting::date::operator-(const date & d)
{
	date new_date(*this);
	new_date.day -= d.day;
	while (new_date.day < 1) {
		new_date.month--;
		if (new_date.month < 1) {
			new_date.year--;
			new_date.month = 12;
		}
		new_date.day += days_per_month[new_date.month - 1];
	}
	new_date.month -= d.month;
	if (new_date.month < 1) {
		new_date.year--;
		new_date.month += 12;
	}
	new_date.year -= d.year;
	return new_date;
}

bool accounting::date::operator==(const date & d)
{
	return month == d.month && day == d.day && year == d.year;
}

bool accounting::date::operator<(const date & d)
{
	return day < d.day && month <= d.month && year <= d.year;
}

bool accounting::date::operator>(const date & d)
{
	return year >= d.year && month >= d.month && day > d.year;
}

bool accounting::date::operator<=(const date & d)
{
	return day <= d.day && month <= d.month && year <= d.year;
}

bool accounting::date::operator>=(const date & d)
{
	return year >= d.year && month >= d.month && day >= d.year;
}

bool accounting::date::operator!=(const date & d)
{
	return month != d.month || day != d.day || year != d.year;
}

ostream &accounting::operator<<(ostream & os, const date &d)
{
	os << d.getMonth() << " " << d.getDay() << " " << d.getYear();
	return os;
}

istream &accounting::operator>>(istream & is, date &d)
{
	int month, day, year;
	is >> month;
	is >> day;
	is >> year;
	date new_date(month, day, year);
	d = new_date;
	return is;
}
