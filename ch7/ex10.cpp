#include <iostream>
#include <string>
#include <limits>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

string encrypt(string key, string s)
{
    if(key.size()<1) return 0;
    string ret("");
    int k = 0;
    for(int i = 0; i < s.size(); i++)
    {
        ret.push_back(s[i] ^ key[k++]);
        if (k >= key.size()) k = 0;
    }
    return ret;
}

bool cmp(pair<int,char> a, pair<int,char> b)
{
    return a.first > b.first;
}

// Do a very simple frequency analysis, looking for outputs in
// which the letters that result in a high probability vs. the
// English language.
void showPossibleMatch(string key, string match)
{
    map<char,int> hist;
    hist.clear();
    for(string::const_iterator i = match.begin(); i != match.end(); i++)
    {
        // Assume non alphanumeric chars means wrong key.
        if(*i != ' ' && !isalpha(*i)) return;

        hist[*i]++;
    }

    vector<pair<int,char> > pairs;
    pairs.clear();
    for(map<char,int>::const_iterator i = hist.begin(); i != hist.end(); i++)
    {
        pairs.push_back(pair<int,char>((*i).second,(*i).first));
    }

    sort(pairs.begin(), pairs.end(), cmp);

    if(pairs[0].second == 'e' || pairs[0].second == 't' || pairs[0].second == 'a'
        || pairs[0].second == 'o' || pairs[0].second == 'i' || pairs[0].second == 'n')
    {
        cout << "Possible match: key='" << key << "' value='" << match << "'" << endl;
    }
}

// Use brute force to try and decrypt the msg. Output potentials with
// printable characters only. NOTE, this brute force routine assumes that
// the key can only be made of lowercase letters.
void tryDecrypt(string key, string s, int terminate)
{
    key.push_back(0);
    for(char i = 'a'; i < 'z'; i++)
    {
        key[key.size()-1] = i;
        showPossibleMatch(key, encrypt(key, s));
        if (key.size() < terminate)
        {
            tryDecrypt(key, s, terminate);
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc != 3) return -1;

    string key = string(argv[1]);
    string s = string(argv[2]);

    string enc = encrypt(key,s);
    cout << enc << endl;

    tryDecrypt(string(""), enc, 3);
}
