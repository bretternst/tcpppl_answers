#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

int main() {
	using namespace std;
	cout << "Enter name value pairs (blank to finish)" << endl;

	typedef map<string,vector<int> > DataList;
	DataList data;

	for(;;)
	{
		string s;
		getline(cin, s);
		if(s.size() == 0)
			break;
		istringstream parts(s);
		string name;
		int val;
		parts >> name;
		parts >> val;
		data[name].push_back(val);
	}

	for(DataList::const_iterator i = data.begin(); i != data.end(); i++) {
		vector<int> v = i->second;
		sort(v.begin(), v.end());
		int sum = 0;
		for(vector<int>::const_iterator j = v.begin(); j != v.end(); j++) {
			sum += *j;
			cout << *j << endl;
		}
		double mean = (double)sum / v.size();
		double median;
		if(v.size() % 2 == 0)
			median = (v[v.size() / 2] + v[v.size() / 2 + 1]) / 2.0;
		else
			median = v[v.size() / 2];
		cout << "name: " << i->first
			<< " total: " << sum
			<< " mean: " << mean
			<< " median: " << median
			<< endl;
	}
}