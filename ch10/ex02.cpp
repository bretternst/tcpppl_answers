#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#define USE_EPOCH_DATE

namespace Calendar
{
	using std::string;

#ifdef USE_EPOCH_DATE
	// Note:
	// For this implementation, I have chosen not to handle dates before 1 AD. Any year less than
	// 1 will result in a BadDateError exception.
	class Date
	{
	public:
		enum Month { Jan = 1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };
		class BadDateError {};

		Date(int yy = 0, Month mm = Month(0), int dd = 0);
		int get_year() const { return get_friendly_date().y; }
		int get_month() const { return get_friendly_date().m; }
		int get_day() const { return get_friendly_date().d; }
		Date& add_years(int n);
		Date& add_months(int n);
		Date& add_days(int d);
		string to_str() const;

		static void set_default(int y, Month m, int d);
	private:
		int days;

		static int get_days_in_month(int m, int y);
		static bool is_leap_year(int y);
		static Date defaultDate;

		struct FriendlyDate { int y, m, d; };
		int get_days_from_epoch(int y, int m, int d);
		FriendlyDate get_friendly_date() const;
	};

	Date::Date(int yy, Month mm, int dd) : days(get_days_from_epoch(
		yy == 0 ? defaultDate.get_year() : yy, 
		mm == 0 ? defaultDate.get_month() : mm,
		dd == 0 ? defaultDate.get_day() : dd))
	{
	}

	Date& Date::add_years(int n)
	{
		FriendlyDate dt = get_friendly_date();

		if(n != 0)
		{
			if(dt.d == 29 && !is_leap_year(dt.y + n))
			{
				dt.d = 28;
			}

			dt.y += n;

			days = get_days_from_epoch(dt.y, dt.m, dt.d);
		}
		return *this;
	}

	Date& Date::add_months(int n)
	{
		if(n == 0) return *this;

		FriendlyDate dt = get_friendly_date();

		int yDelta = n / 12;
		int mm = dt.m + n % 12;

		if (n < 0)
		{
			if(mm > 12)
			{
				yDelta++;
				mm -= 12;
			}
			dt.m = Month(mm);
		}
		else if (n > 0)
		{
			if(mm < 0)
			{
				yDelta--;
				mm += 12;
			}
		}
		dt.y += yDelta;
		dt.m = Month(mm);
		dt.d = std::min(dt.d, get_days_in_month(dt.m, dt.y));

		days = get_days_from_epoch(dt.y,dt.m,dt.d);

		return *this;
	}

	Date& Date::add_days(int n)
	{
		days += n;
		return *this;
	}

	string Date::to_str() const
	{
		std::stringstream str;
		FriendlyDate dt = get_friendly_date();
		str << dt.y << "-" << dt.m << "-" << dt.d;
		return str.str();
	}

	int Date::get_days_in_month(int m, int y)
	{
		static const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		return m == 2 ? 
			(28 + (is_leap_year(y) ? 1 : 0)) :
			(daysInMonth[m-1]);
	}

	bool Date::is_leap_year(int y)
	{
		return y % 400 == 0 || (y % 100 != 0 && y % 4 == 0);
	}

	Date Date::defaultDate(1970, Date::Jan, 1);

	void Date::set_default(int y, Month m, int d)
	{
		defaultDate = Date(y, m, d);
	}

	int Date::get_days_from_epoch(int y, int m, int d)
	{
		if(m < 0 || m > 12 || d < 1 || y < 1 ||
			d > get_days_in_month(m, y))
		{
			throw BadDateError();
		}

		days = 0;
		int yy;

		days += (y)*365;
		days += (y-1)/4 - (y-1)/100 + (y-1)/400;
		for(int mm = 1; mm < m; mm++)
		{
			days += get_days_in_month(mm, y);
		}
		days += d;

		return days - 719528; // Number of days from 0 to Jan 1, 1970
	}

	Date::FriendlyDate Date::get_friendly_date() const
	{
		const static int DAYS400 = 400 * 365 + 4*24 + 1;
		const static int DAYS100 = 100 * 365 + 24;
		const static int DAYS4 = 4*365 + 1;

		int days = this->days + 719528; // Number of days from 0 until Jan 1, 1970

		int interval400 = days / DAYS400;
		int interval100 = days % DAYS400 / DAYS100;
		int interval4 = days % DAYS400 % DAYS100 / DAYS4;
		int remainder = days % DAYS400 % DAYS100 % DAYS4;

		int year = 400 * interval400 + 100 * interval100 + 4 * interval4;
		if(is_leap_year(year) && (remainder < 365))
		{
			remainder++;
		}
		else if (is_leap_year(year))
		{
			year = year + 1 + (remainder-365)/365;
			remainder %= 365;
		}
		else
		{
			year = year + remainder/365;
			remainder %= 365;
		}

		if(remainder==0) {year--;remainder=is_leap_year(year) ? 366 : 365;}

		int max, mm;
		mm = 1;
		while(remainder > (max = get_days_in_month(mm, year)))
		{
			mm++;
			remainder -= max;
		}

		FriendlyDate dt = {year, mm, remainder};

		return dt;
	}

#else

	class Date
	{
	public:
		enum Month { Jan = 1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };
		class BadDateError {};

		Date(int yy = 0, Month mm = Month(0), int dd = 0);
		int get_year() const { return y; }
		int get_month() const { return m; }
		int get_day() const { return d; }
		Date& add_years(int n);
		Date& add_months(int n);
		Date& add_days(int d);
		string to_str() const;

		static void set_default(int y, Month m, int d);
	private:
		int y, m, d;

		static int get_days_in_month(int m, int y);
		static bool is_leap_year(int y);
		static Date defaultDate;
	};

	Date::Date(int yy, Month mm, int dd) : y(yy), m(mm), d(dd)
	{
		if(y == 0) y = defaultDate.y;
		if(m == 0) m = defaultDate.m;
		if(d == 0) d = defaultDate.d;

		if(m < 0 || m > 12 || d < 1 ||
			d > get_days_in_month(m, y))
		{
			throw BadDateError();
		}
	}

	Date& Date::add_years(int n)
	{
		if(n != 0)
		{
			if(d == 29 && !is_leap_year(y + n))
			{
				d = 28;
			}

			y += n;
		}
		return *this;
	}

	Date& Date::add_months(int n)
	{
		if(n == 0) return *this;

		int yDelta = n / 12;
		int mm = m + n % 12;

		if (n < 0)
		{
			if(mm > 12)
			{
				yDelta++;
				mm -= 12;
			}
			m = Month(mm);
		}
		else if (n > 0)
		{
			if(mm < 0)
			{
				yDelta--;
				mm += 12;
			}
		}
		y += yDelta;
		m = Month(mm);
		d = std::min(d, get_days_in_month(m, y));

		return *this;
	}

	Date& Date::add_days(int n)
	{
		d += n;
		int max;
		while(d < 1)
		{
			m--;
			if(m < 1) { m = Month(12); y--; if(y==0) y--; }
			d += get_days_in_month(m, y);
		}
		while(d > (max = get_days_in_month(m, y)))
		{
			m++;
			if(m > 12) { m = Month(1); y++; if(y==0) y++; }
			d -= max;
		}

		d = std::min(d, get_days_in_month(m, y));

		return *this;
	}

	string Date::to_str() const
	{
		std::stringstream str;
		str << y << "-" << m << "-" << d;
		return str.str();
	}

	int Date::get_days_in_month(int m, int y)
	{
		static const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		return m == 2 ? 
			(28 + (is_leap_year(y) ? 1 : 0)) :
			(daysInMonth[m-1]);
	}

	bool Date::is_leap_year(int y)
	{
		return y % 400 == 0 || (y % 100 != 0 && y % 4 == 0);
	}

	Date Date::defaultDate(1970, Date::Jan, 1);

	void Date::set_default(int y, Month m, int d)
	{
		defaultDate = Date(y, m, d);
	}

#endif
}

void test(std::string a, std::string b)
{
	using namespace std;
	cout << (a.compare(b) == 0 ? "PASS: " : "FAIL: ") << a << " == " << b << endl;
}

int main()
{
	using Calendar::Date;

	try
	{
		Date d(2009, Date::Jan, 1);
		test(d.to_str(),"2009-1-1");
		d = Date(2008, Date::Jan, 31);
		test(d.to_str(),"2008-1-31");
		d.add_years(1);
		test(d.to_str(),"2009-1-31");
		d = Date(2008, Date::Feb, 29);
		d.add_years(1);
		test(d.to_str(),"2009-2-28");
		d = Date(2008, Date::Feb, 29);
		d.add_years(-2);
		test(d.to_str(),"2006-2-28");
		d = Date(2008, Date::Feb, 29);
		d.add_years(-4);
		test(d.to_str(),"2004-2-29");
		d = Date(2008, Date::Feb, 29);
		d.add_months(1);
		test(d.to_str(),"2008-3-29");
		d = Date(2008, Date::Feb, 29);
		d.add_months(-1);
		test(d.to_str(),"2008-1-29");
		d = Date(2008, Date::Mar, 31);
		d.add_months(1);
		test(d.to_str(),"2008-4-30");
		d = Date(2008, Date::Mar, 31);
		d.add_months(-1);
		test(d.to_str(),"2008-2-29");
		d = Date(2008, Date::Mar, 31);
		d.add_months(-12);
		test(d.to_str(),"2007-3-31");
		d = Date(2008, Date::Mar, 31);
		d.add_months(-13);
		test(d.to_str(),"2007-2-28");
		d = Date(2008, Date::Mar, 31);
		d.add_months(13);
		test(d.to_str(),"2009-4-30");
		d = Date(2008, Date::Feb, 29);
		d.add_days(1);
		test(d.to_str(),"2008-3-1");
		d = Date(2008, Date::Feb, 29);
		d.add_days(-1);
		test(d.to_str(),"2008-2-28");
		d = Date(2008, Date::Feb, 29);
		d.add_days(31);
		test(d.to_str(),"2008-3-31");
		d = Date(2008, Date::Feb, 29);
		d.add_days(-31);
		test(d.to_str(),"2008-1-29");
		d = Date(2008, Date::Mar, 2);
		d.add_days(-5);
		test(d.to_str(),"2008-2-26");
		d = Date(2008, Date::Feb, 28);
		d.add_days(5);
		test(d.to_str(),"2008-3-4");
		d = Date(2008, Date::Feb, 29);
		d.add_days(365);
		test(d.to_str(),"2009-2-28");
		d = Date(2008, Date::Feb, 29);
		d.add_days(366);
		test(d.to_str(),"2009-3-1");
		d = Date(2008, Date::Feb, 29);
		d.add_days(-365);
		test(d.to_str(),"2007-3-1");
		d = Date(2008, Date::Feb, 29);
		d.add_days(-366);
		test(d.to_str(),"2007-2-28");
		d = Date(1971, Date::Jan, 1);
		test(d.to_str(),"1971-1-1");
		d = Date(1970, Date::Jan, 1);
		test(d.to_str(),"1970-1-1");
		d = Date(1969, Date::Dec, 31);
		test(d.to_str(),"1969-12-31");
		d = Date(1969, Date::Dec, 30);
		test(d.to_str(),"1969-12-30");
		d = Date(2008, Date::Dec, 31);
		test(d.to_str(),"2008-12-31");
		d.add_years(5);
		test(d.to_str(),"2013-12-31");
		Date::set_default(2008, Date::Month(12), 31);
		d = Date();
		test(d.to_str(),"2008-12-31");
	}
	catch(Date::BadDateError)
	{
		std::cout << "bad date!" << std::endl;
	}
}
