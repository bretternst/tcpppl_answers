#include <iostream>
#include <vector>
#include <bitset>
#include <limits>
#include <string>

int main()
{
	using namespace std;

	const int ClassSize = 10;

	typedef bitset<ClassSize> AttendanceDay;
	typedef vector<AttendanceDay> AttendanceHistory;

	AttendanceHistory days;
	days.push_back(AttendanceDay(string("1011011111")));
	days.push_back(AttendanceDay(string("0111010110")));
	days.push_back(AttendanceDay(string("1101111111")));
	days.push_back(AttendanceDay(string("1101110111")));
	days.push_back(AttendanceDay(string("0101111111")));
	days.push_back(AttendanceDay(string("0111100111")));
	days.push_back(AttendanceDay(string("1011101111")));
	days.push_back(AttendanceDay(string("1111110111")));
	days.push_back(AttendanceDay(string("0111111111")));
	days.push_back(AttendanceDay(string("1111100111")));
	days.push_back(AttendanceDay(string("0101111111")));
	days.push_back(AttendanceDay(string("1101110111")));

	AttendanceHistory::const_iterator i = days.begin();
	AttendanceDay b = *i;
	for(i++; i != days.end(); i++)
		b &= *i;

	cout << "the students who were present every day are: ";
	for(int i = 0; i < ClassSize; i++)
		if(b[i]) cout << '#' << i+1 << ' ';
	cout << endl;

	vector<int> v(ClassSize);
	for(AttendanceHistory::const_iterator i = days.begin(); i != days.end(); i++)
	{
		for(int j = 0; j < ClassSize; j++)
			v[j] += (*i)[j];
	}

	cout << "the students who were present at least 8 days are: ";
	for(unsigned int i = 0; i < v.size(); i++)
		if(v[i] >= 8) cout << '#' << i+1 << ' ';
	cout << endl;
}
