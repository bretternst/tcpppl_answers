#include <locale>
#include <iostream>

namespace xxd {
    class collate_dict_es : public std::collate<wchar_t> {
    public:
        std::wstring do_transform(const wchar_t* beg, const wchar_t* end) const;
        int do_compare(const wchar_t* beg1, const wchar_t* end1, const wchar_t* beg2, const wchar_t* end2) const;
    };

    std::wstring collate_dict_es::do_transform(const wchar_t* beg, const wchar_t* end) const {
        std::wstring tmp;
        while(beg != end) {
            wchar_t c = *beg++;
            switch(c) {
            case L'á':
                c = 'a';
                break;
            case L'é':
                c = 'e';
                break;
            case L'í':
                c = 'i';
                break;
            case L'ó':
                c = 'o';
                break;
            case L'ú':
            case L'ü':
                c = 'u';
                break;
            }
            tmp.push_back(c);
        }
        return tmp;
    }

    int collate_dict_es::do_compare(const wchar_t* beg1, const wchar_t* end1, const wchar_t* beg2, const wchar_t* end2) const {
        std::wstring s1 = transform(beg1, end1);
        std::wstring s2 = transform(beg2, end2);
        std::wstring::iterator i1 = s1.begin();
        std::wstring::iterator i2 = s2.begin();
        for(; i1 != s1.end() && i2 != s2.end(); i1++, i2++) {
            if(*i1 == L'ñ' && *i2 == L'n')
                return 1;
            else if (*i1 == L'n' && *i2 == L'ñ')
                return -1;
            else if (*i1 < *i2)
                return -1;
            else if (*i2 > *i2)
                return 1;
        }
        if(i1 == s1.end() && i2 == s2.end())
            return 0;
        else if (i1 == s1.end())
            return 1;
        else
            return -1;
    }
}

void test(const std::wstring& s1, const std::wstring& s2) {
    std::locale loc(std::locale(), new xxd::collate_dict_es());
    const std::collate<wchar_t>& col = std::use_facet<std::collate<wchar_t> >(loc);
    std::wcout << col.compare(s1.data(), s1.data()+s1.size(), s2.data(), s2.data()+s2.size()) << std::endl;
}

int main() {
    test(L"aeiou", L"áéíóú"); // 0
    test(L"aeioux", L"áéíóú"); // -1
    test(L"xxxñxxx", L"xxxnxxx"); // 1
}
