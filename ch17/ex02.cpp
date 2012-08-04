#include <iostream>
#include <vector>
#include <algorithm>

namespace Exercises
{
    class phone_number
    {
        unsigned short countryCode;
        unsigned short areaCode;
        unsigned int number;
        unsigned int extension;

    public:
        unsigned short CountryCode() { return countryCode; }
        unsigned short AreaCode() { return areaCode; }
        unsigned int Number() { return number; }
        unsigned int Extension() { return extension; }

        // uses default copy constructor

        phone_number() : countryCode(1), areaCode(0), number(0), extension(0) {}
        phone_number(unsigned short areaCode, unsigned int number, unsigned short countryCode = 1, 
            unsigned int extension = 0)
            : areaCode(areaCode), number(number), extension(extension), countryCode(countryCode) {}

        void Print(std::ostream& output) const;
        bool operator<(const phone_number& x) const;
    };

    void phone_number::Print(std::ostream& output) const
    {
        output << '+' << countryCode << ' ' << '(' << areaCode << ") " << number;
        if(extension>0) output << " x" << extension;
    }

    bool phone_number::operator<(const phone_number& x) const
    {
        if(countryCode != x.countryCode) return countryCode < x.countryCode;
        if(areaCode != x.areaCode) return areaCode < x.areaCode;
        if(number != x.number) return number < x.number;
        if(extension != x.extension) return extension < x.extension;
        return false;
    }

    std::ostream& operator<<(std::ostream& output, const phone_number& x)
    {
        x.Print(output);
        return output;
    }

    template<class Tcontainer> class phone_number_container
    {
        Tcontainer& c;

    public:
        phone_number_container(Tcontainer& cc) : c(cc) {}

        typedef typename Tcontainer::iterator iterator;

        void PrintAll();
        iterator GetLowerBound(unsigned short countryCode, unsigned short areaCode);
        iterator GetUpperBound(unsigned short countryCode, unsigned short areaCode);
        std::pair<iterator,iterator> GetEqualBound(unsigned short countryCode, unsigned short areaCode);
    };

    template<class Tcontainer>
    void phone_number_container<Tcontainer>::PrintAll()
    {
        for(iterator i = c.begin(); i != c.end(); i++)
        {
            std::cout << *i << std::endl;
        }
    }

    template<class Tcontainer> typename phone_number_container<Tcontainer>::iterator
    phone_number_container<Tcontainer>::GetLowerBound(unsigned short countryCode, unsigned short areaCode)
    {
        iterator i = c.begin();
        while(i != c.end())
        {
            if(i->CountryCode() == countryCode && i->AreaCode() == areaCode) break;
            i++;
        }
        return i;
    }

    template<class Tcontainer> typename phone_number_container<Tcontainer>::iterator
    phone_number_container<Tcontainer>::GetUpperBound(unsigned short countryCode, unsigned short areaCode)
    {
        iterator i = GetLowerBound(countryCode, areaCode);
        while(i != c.end())
        {
            if(i->CountryCode() != countryCode || i->AreaCode() != areaCode) break;
            i++;
        }
        return i;
    }

    template<class Tcontainer>
    std::pair<typename phone_number_container<Tcontainer>::iterator,typename phone_number_container<Tcontainer>::iterator>
    phone_number_container<Tcontainer>::GetEqualBound(unsigned short countryCode, unsigned short areaCode)
    {
        iterator i1 = GetLowerBound(countryCode, areaCode);
        iterator i2 = i1;
        while(i2 != c.end())
        {
            if(i2->CountryCode() != countryCode || i2->AreaCode() != areaCode) break;
            i2++;
        }
        return std::make_pair(i1, i2);
    }
}

int main()
{
    using namespace std;
    using namespace Exercises;

    vector<phone_number> v;
    v.push_back(phone_number(425, 5551111, 1, 0));
    v.push_back(phone_number(425, 5551112, 1, 1234));
    v.push_back(phone_number(360, 5551111, 1, 0));
    v.push_back(phone_number(206, 5551112, 1, 0));
    v.push_back(phone_number(206, 5551111, 1, 0));
    v.push_back(phone_number(206, 5551113, 1, 0));

    sort(v.begin(),v.end());

    phone_number_container<vector<phone_number> > c(v);

    cout << "printing all 206 numbers:" << endl;
    pair<vector<phone_number>::iterator,vector<phone_number>::iterator> pair = c.GetEqualBound(1, 206);
    for(vector<phone_number>::iterator i = pair.first; i != pair.second; i++)
        cout << *i << endl;

    cout << "printing all 360 numbers:" << endl;
    pair = c.GetEqualBound(1, 360);
    for(vector<phone_number>::iterator i = pair.first; i != pair.second; i++)
        cout << *i << endl;

    cout << "printing all 425 numbers:" << endl;
    pair = c.GetEqualBound(1, 425);
    for(vector<phone_number>::iterator i = pair.first; i != pair.second; i++)
        cout << *i << endl;

    return 0;
}
