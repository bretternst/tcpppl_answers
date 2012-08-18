#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <iterator>
#include <set>

// File must have this format:
// year:author:title:publisher
// e.g.
// 1840:Victor Hugo:The Hunchback of Notre Dame:Gosselin
//
// ./ex12 books.txt title "the" year
// Prints all books with 'the' in the title, ordered by year
namespace ch18 {
    using namespace std;

    struct Book {
        int year;
        string author;
        string title;
        string publisher;

        string to_str() const {
            ostringstream ostr;
            ostr << "Title=" << title << ", Author=" << author << ", Year=" << year << ", Publisher=" << publisher;
            return ostr.str();
        }
    };

    void print_book(const Book& book) {
        cout << book.to_str() << endl;
    }

    class Filter_year : public unary_function<Book,bool> {
        int year;
    public:
        Filter_year(int year) : year(year) { }
        bool operator()(const Book& b) {
            return b.year != year;
        }
    };

    char tolower(char c) {
        return ::tolower(c);
    }

    string lower_str(string& s) {
        transform(s.begin(), s.end(), s.begin(), ch18::tolower);
        return s;
    }

    class Filter_title : public unary_function<Book, bool> {
        vector<string> keywords;
    public:
        Filter_title(string s) {
            vector<string> tmp;
            istringstream in(s);
            copy(istream_iterator<string>(in), istream_iterator<string>(), back_inserter(keywords));
            transform(keywords.begin(), keywords.end(), keywords.begin(), lower_str);
            sort(keywords.begin(), keywords.end());
        }
        bool operator()(const Book& b) {
            vector<string> words;
            istringstream in(b.title);
            copy(istream_iterator<string>(in), istream_iterator<string>(), back_inserter(words));
            transform(words.begin(), words.end(), words.begin(), lower_str);
            sort(words.begin(), words.end());
            return !includes(words.begin(), words.end(), keywords.begin(), keywords.end());
        }
    };

    class Filter : public unary_function<Book, bool> {
        string Book::*p;
        string s;
    public:
        Filter(const string& s, string Book::*p) : s(s), p(p) { }
        bool operator()(const Book& b) {
            return s != b.*p;
        }
    };

    template <class T>
    class Sort : public binary_function<Book, Book, bool> {
        T Book::*p;
    public:
        Sort(T Book::*p) : p(p) { }
        bool operator()(const Book& x, const Book& y) {
            return x.*p < y.*p;
        }
    };
}

int main(int argc, char** argv) {
    using namespace std;
    using namespace ch18;

    if(argc < 4) {
        cout << "Usage: ex12 <file> <searchfield> <value> [sort]" << endl;
        cout << "   searchfield = year | author | title | publisher" << endl;
        return 1;
    }

    ifstream in(argv[1]);
    if(!in) {
        cout << "File not found." << endl;
        return 1;
    }

    vector<Book> books;
    while(in) {
        string s;
        if(!getline(in, s))
            break;
        istringstream ins(s);
        Book b;
        string tmp;
        getline(ins, tmp, ':');
        istringstream yr(tmp);
        yr >> b.year;

        getline(ins, b.author, ':');
        getline(ins, b.title, ':');
        getline(ins, b.publisher, ':');
        books.push_back(b);
    }

    vector<Book> result;

    string field(argv[2]);
    string value(argv[3]);
    if(field == "year") {
        int year;
        istringstream yr(value);
        yr >> year;
        remove_copy_if(books.begin(), books.end(), back_inserter(result), Filter_year(year));
    }
    else if (field == "author") {
        remove_copy_if(books.begin(), books.end(), back_inserter(result), Filter(value, &Book::author));
    }
    else if (field == "publisher") {
        remove_copy_if(books.begin(), books.end(), back_inserter(result), Filter(value, &Book::publisher));
    }
    else if (field == "title") {
        remove_copy_if(books.begin(), books.end(), back_inserter(result), Filter_title(value));
    }

    if(argc > 4) {
        string orderby(argv[4]);
        if(orderby == "year") {
            sort(result.begin(), result.end(), Sort<int>(&Book::year));
        }
        else if (orderby == "author") {
            sort(result.begin(), result.end(), Sort<string>(&Book::author));
        }
        else if (orderby == "publisher") {
            sort(result.begin(), result.end(), Sort<string>(&Book::publisher));
        }
        else if (orderby == "title") {
            sort(result.begin(), result.end(), Sort<string>(&Book::title));
        }
    }

    for_each(result.begin(), result.end(), print_book);
}
