#include <iostream>
#include <string>

using namespace std;

string encrypt(string key, string s)
{
	if(key.size()<1) return 0;
	string ret;
	int k = 0;
	for(int i = 0; i < s.size(); i++)
	{
		ret.push_back(s[i] ^ key[k++]);
		if (k >= key.size()) k = 0;
	}
	return ret;
}

int main(int argc, char* argv[])
{
	if(argc < 2) return -1;

    string s(argv[1]);
    if (argc < 3) {
        cout << s << endl;
        return 0;
    }
    string key(argv[2]);

	string enc = encrypt(key,s);
	cout << enc << endl;
	enc = encrypt(key,enc);
	cout << enc << endl;
}
