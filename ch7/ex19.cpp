#include <iostream>
#include <string>

using namespace std;

const int DAYS_INm_month[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const string WEEKDAY[7] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

struct Date {
private:
	int m_dt;
	int m_year;
	int m_month;
	int m_day;

	int isLeapYear(int y)
	{
		return y % 400 == 0 || (y % 100 != 0 && y % 4 == 0);
	}

	int daysInYear(int y)
	{
		return isLeapYear(y) ? 366 : 365;
	}

	int daysInMonth(int y, int m)
	{
		if(m==2 && isLeapYear(y)) return 29;
		else return DAYS_INm_month[m];
	}

	int daysFromEpoch()
	{
		int dt = 0;
		for(int i = 1900; i < m_year; i++)
			dt += daysInYear(m_year);
		for(int i = 1; i < m_month; i++)
			dt += daysInMonth(m_year,m_month);
		dt += m_day - 1;
		cout << "days since epoch" << dt << endl;
		return dt;
	}

public:
	Date(int y, int m, int d)
	{
		m_year = y;
		m_month = m;
		m_day = d;
	}

	void addYear()
	{
		m_year++;
		if(m_month == 2)
		{
			if(m_day == 29) m_day = 28;
			else if (m_day == 28 && isLeapYear(m_year)) m_day = 29;
		}
	}

	void addMonth()
	{
		m_month++;
		if(m_month > 12)
		{
			m_month = 1;
			m_year++;
		}
		if(daysInMonth(m_year, m_month) < m_day)
			m_day = daysInMonth(m_year,m_month);
	}

	void addDay()
	{
		m_day++;
		if(daysInMonth(m_year, m_month) < m_day)
		{
			addMonth();
			m_day = 1;
		}
	}

	int dayOfWeek()
	{
        // found somewhere on the intenet
		int x = m_year - 1900;
		x += x / 4;
		if(isLeapYear(m_year) && m_month < 3) x -= 1;
		x += m_day;
		if(m_month==5) x += 1;
		if(m_month==8) x += 2;
		if(m_month==2 || m_month==3 || m_month==11) x += 3;
		if(m_month==6) x += 4;
		if(m_month==9 || m_month==12) x += 5;
		if(m_month==4 || m_month==7) x += 6;
		return x % 7;
	}

	Date nextMonday()
	{
        int dow = dayOfWeek();
        int addDays = (dow == 0 ? 1 : (7-dow+1));
        Date d(m_year, m_month, m_day);
        for(int i = 0; i < addDays; i++)
            d.addDay();
        return d;
	}

	int year() { return m_year; }
	int month() { return m_month; }
	int day() { return m_day; }
};

void printDate(Date d)
{
	cout << d.year() << "-" << d.month() << "-" << d.day() << endl;
}

Date readDate()
{
	int y, m, d;
	cin >> y >> m >> d;
	return Date(y, m, d);
}

int main()
{
	while(cin.good())
	{
		Date d = readDate();
		Date d2 = d;
		cout << "Adding one year: ";
		d2.addYear();
		printDate(d2);
		d2 = d;
		cout << "Adding one month: ";
		d2.addMonth();
		printDate(d2);
		d2 = d;
		cout << "Adding one day: ";
		d2.addDay();
		printDate(d2);
		cout << "Day of week: " << WEEKDAY[d.dayOfWeek()] << endl;
        cout << "Next monday: ";
        printDate(d.nextMonday());
	}
}
