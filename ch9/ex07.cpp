// In aggregate, it takes about 10 times longer to open/read the 100
// files than it does to open the one file, even though the total number
// of bytes is the same in each case.
//
// This is likely due to the operating system overhead of opening and closing
// file resources as compared to reading a stream of bytes into a buffer. It
// is likely that, at least for everything but the most complex include files,
// it is faster to have a small number of large include files than a large number
// of small include files.
//
// In linux/*nix operating systems, the maximum number of open files is limited
// by the operating system policies (managed with ulimit). Files and sockets are
// both file descriptors, so they count towards the same total. To see the max
// allowed files for the current user:
// ulimit -n

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <sstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>

using std::cout;
using std::cerr;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::string;
using std::numeric_limits;
using std::ostringstream;
using std::vector;

const char* PATH = "tmp/";
const char* PREFIX = "file";
const char* SUFFIX = ".txt";
const char* BIG_FILE_NAME = "tmp/bigfile.txt";
const int SIZE_OF_BIG_FILE = 150000;
const int SIZE_OF_FILE = 1500;
const int NUMBER_OF_FILES = 100;

struct FileWriteFailed {};
struct FileReadFailed {};

string create_file_name(int i)
{
	ostringstream sstr;
	sstr << PATH << PREFIX << i << SUFFIX;
	return sstr.str();
}

void create_file_contents(string fileName, int numChars)
{
	ofstream file(fileName.c_str(), ofstream::out);
	if(!file.good()) throw FileWriteFailed();

	for(int i = 0; i < numChars; i++)
	{
		char c = (rand() % 26) + 'a';
		file << c;
	}
	file.close();
}

void read_file_contents(string fileName)
{
	char c[1024];

	ifstream file(fileName.c_str());
	while(file.good())
	{
		file.read(c, 1024);
	}
	file.close();
}

int main()
{
	// Make a temporary directory
	if (mkdir(PATH, 0777))
	{
		cerr << "Failed to create folder." << endl;
		return 1;
	}

	vector<string> fileNames;

	// Create full file paths.
	for(int i = 0; i < NUMBER_OF_FILES; i++)
	{
		fileNames.push_back(create_file_name(i));
	}

	try
	{
		// Create many files
		for(vector<string>::const_iterator i = fileNames.begin(); i != fileNames.end(); i++)
		{
			create_file_contents(*i, SIZE_OF_FILE);
		}

		// Create one big file
		create_file_contents(BIG_FILE_NAME, SIZE_OF_BIG_FILE);

		// Read many files - repeat this 500 times to get a better time sample
		int start = clock();
		for(int j = 0; j < 500; j++)
		{
			for(vector<string>::const_iterator i = fileNames.begin(); i != fileNames.end(); i++)
			{
				read_file_contents(*i);
			}
		}
		int end = clock();
		cout << "Time elapsed to read many files: " << end - start << endl;

		// Read one big file - again, repeat 500 times
		start = clock();
		for(int j = 0; j < 500; j++)
		{
			read_file_contents(BIG_FILE_NAME);
		}
		end = clock();
		cout << "Time elapsed to read one big file: " << end - start << endl;

		// Delete all files
		for(vector<string>::const_iterator i = fileNames.begin(); i != fileNames.end(); i++)
		{
			remove(i->c_str());
		}
		remove(BIG_FILE_NAME);
	}
	catch(FileWriteFailed)
	{
		cerr << "Error writing file." << endl;
		return 1;
	}
	catch(FileReadFailed)
	{
		cerr << "Error reading file." << endl;
		return 1;
	}

	rmdir(PATH);
    return 0;
}
