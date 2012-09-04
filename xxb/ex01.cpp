/* C program from here: http://www.programmingsimplified.com/c/source-code/c-anagram-program

#include <stdio.h>
 
int check_anagram(char [], char []);
 
int main()
{
   char a[100], b[100];
   int flag;
 
   printf("Enter first string\n");
   gets(a);
 
   printf("Enter second string\n");
   gets(b);
 
   flag = check_anagram(a, b);
 
   if (flag == 1)
      printf("\"%s\" and \"%s\" are anagrams.\n", a, b);
   else
      printf("\"%s\" and \"%s\" are not anagrams.\n", a, b);
 
   return 0;
}
 
int check_anagram(char a[], char b[])
{
   int first[26] = {0}, second[26] = {0}, c = 0;
 
   while (a[c] != '\0')
   {
      first[a[c]-'a']++;
      c++;
   }
 
   c = 0;
 
   while (b[c] != '\0')
   {
      second[b[c]-'a']++;
      c++;
   }
 
   for (c = 0; c < 26; c++)
   {
      if (first[c] != second[c])
         return 0;
   }
 
   return 1;
}
*/

#include <iostream>
#include <string>
#include <map>
#include <cctype>

bool check_anagram(const std::string& a, const std::string& b) {
    std::map<char, int> ma;
    std::map<char, int> mb;

    for(std::string::const_iterator i = a.begin(); i != a.end(); i++) {
        if(isalnum(*i))
            ma[*i]++;
    }
    for(std::string::const_iterator i = b.begin(); i != b.end(); i++) {
        if(isalnum(*i))
            mb[*i]++;
    }
    return ma == mb;
}

int main()
{
    using namespace std;
    string a, b;
    cout << "Enter first string" << endl;
    getline(cin, a);
    cout << "Enter second string" << endl;
    getline(cin, b);
    cout << a << " and " << b << " are " << (check_anagram(a, b) ? "" : "not ") << "anagrams." << endl;
}
