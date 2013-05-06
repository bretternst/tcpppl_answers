#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

void display(const vector<string>& v ) {
    typedef vector<string>::const_iterator vi;
	
	for(vi i=v.begin(); i!=v.end(); i++) cout << *i << "\t";
	cout << endl;
	}

int main(void ) {
	
	string s;
	vector<string> vs;
	
	cin >> s;
	while(s != "quit" ) {
		vs.push_back(s );
		cin >> s;
		}
	
	sort(vs.begin(), vs.end() );	
	display(vs);
	
	vector<string> tmp;
	unique_copy(vs.begin(), vs.end(), back_inserter(tmp) );
	
	display(tmp);
	
	return 0;
	}
