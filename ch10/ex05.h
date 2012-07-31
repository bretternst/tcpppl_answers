#include <iostream>
#include <set>
#include <map>
#include <string>
#include <stdlib.h>
#include <time.h>

namespace Exercises
{
	typedef std::set<int> IntervalSet;
	typedef std::map<int,int> ValueMap;

	class Histogram
	{
	private:
		int lowerBound;
		int upperBound;
		ValueMap values;

	public:
		class OutOfRangeError {};
		class InvalidIntervalSetError {};

		Histogram(IntervalSet& intervals);
		Histogram& add(int n);
		void print_formatted(std::ostream& output);
	};

	Histogram::Histogram(IntervalSet& intervals)
	{
		if(intervals.size() < 2)
			throw InvalidIntervalSetError();

		IntervalSet::const_iterator i;
		for(i = intervals.begin(); i != intervals.end(); i++)
		{
			values.insert(std::pair<int,int>(*i,0));
			upperBound = *i;
		}
		values.erase(upperBound);
		lowerBound = *(intervals.begin());

		if(upperBound <= lowerBound)
			throw InvalidIntervalSetError();
	};

	Histogram& Histogram::add(int n)
	{
		if(n < lowerBound || n > upperBound)
			throw OutOfRangeError();

		ValueMap::iterator i = values.upper_bound(n);
		i--;
		i->second++;

		return *this;
	}

	void Histogram::print_formatted(std::ostream& output)
	{
		std::ostream::fmtflags oldFlags = output.flags();
		output.flags(std::ostream::left);
		for(ValueMap::const_iterator i = values.begin(); i != values.end(); i++)
		{
			output.width(5);
			output << i->first;
			output.width(0);
			output << "- ";
			output.width(5);
			ValueMap::const_iterator next = i;
			next++;
			output << (next == values.end() ? upperBound : next->first - 1);
			output << ": " << std::string(i->second,'#') << " [" << i->second << "]" << std::endl;
		}
		output.flags(oldFlags);
	}
}
