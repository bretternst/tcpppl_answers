#include <iostream>

using namespace std;

int main()
{
	const int max_length = 10;
	const char* input_line = "abc?def?gh";
	int quest_count = 0;

	for(int i=0; i < max_length; i++)
	{
		if (input_line[i] == '?') quest_count++;
	}

	cout << quest_count << endl;
	quest_count = 0;

	const char* p = input_line;
	while (*p != 0)
	{
		if (*p++=='?') quest_count++;
	}

	cout << quest_count << endl;
}
