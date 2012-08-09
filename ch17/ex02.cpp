#include <iostream>
#include <vector>
#include <algorithm>

namespace ch17
{
    class phone_number
    {
        unsigned short m_country_code;
        unsigned short m_area_code;
        unsigned int m_number;
        unsigned int m_extension;

    public:
        unsigned short country_code() const { return m_country_code; }
        unsigned short area_code() const { return m_area_code; }
        unsigned int number() const { return m_number; }
        unsigned int extension() const { return m_extension; }

        // uses default copy constructor

        phone_number() : m_country_code(1), m_area_code(0), m_number(0), m_extension(0) {}
        phone_number(unsigned short area_code, unsigned int number, unsigned short country_code = 1, 
            unsigned int extension = 0)
            : m_area_code(area_code), m_number(number), m_extension(extension), m_country_code(country_code) {}

        bool operator<(const phone_number& x) const;
    };

    std::ostream& operator<<(std::ostream& output, const phone_number& pn)
    {
        output << '+' << pn.country_code() << " (" << pn.area_code() << ") " << pn.number();
        if(pn.extension()>0)
            output << " x" << pn.extension();
        return output;
    }

    bool phone_number::operator<(const phone_number& x) const
    {
        if(m_country_code != x.m_country_code) return m_country_code < x.m_country_code;
        if(m_area_code != x.m_area_code) return m_area_code < x.m_area_code;
        if(m_number != x.m_number) return m_number < x.m_number;
        if(m_extension != x.m_extension) return m_extension < x.m_extension;
        return false;
    }

    template<class Tcontainer> class phone_number_container
    {
        Tcontainer& c;

    public:
        phone_number_container(Tcontainer& cc) : c(cc) {}

        typedef typename Tcontainer::iterator iterator;

        void print_all();
        iterator get_lower_bound(unsigned short m_country_code, unsigned short m_area_code);
        iterator get_upper_bound(unsigned short m_country_code, unsigned short m_area_code);
        std::pair<iterator,iterator> get_equal_bound(unsigned short m_country_code, unsigned short m_area_code);
    };

    template<class Tcontainer>
    void phone_number_container<Tcontainer>::print_all()
    {
        for(iterator i = c.begin(); i != c.end(); i++)
        {
            std::cout << *i << std::endl;
        }
    }

    template<class Tcontainer> typename phone_number_container<Tcontainer>::iterator
    phone_number_container<Tcontainer>::get_lower_bound(unsigned short m_country_code, unsigned short m_area_code)
    {
        iterator i = c.begin();
        while(i != c.end())
        {
            if(i->country_code() == m_country_code && i->area_code() == m_area_code) break;
            i++;
        }
        return i;
    }

    template<class Tcontainer> typename phone_number_container<Tcontainer>::iterator
    phone_number_container<Tcontainer>::get_upper_bound(unsigned short m_country_code, unsigned short m_area_code)
    {
        iterator i = get_lower_bound(m_country_code, m_area_code);
        while(i != c.end())
        {
            if(i->CountryCode() != m_country_code || i->AreaCode() != m_area_code) break;
            i++;
        }
        return i;
    }

    template<class Tcontainer>
    std::pair<typename phone_number_container<Tcontainer>::iterator,typename phone_number_container<Tcontainer>::iterator>
    phone_number_container<Tcontainer>::get_equal_bound(unsigned short m_country_code, unsigned short m_area_code)
    {
        iterator i1 = get_lower_bound(m_country_code, m_area_code);
        iterator i2 = i1;
        while(i2 != c.end())
        {
            if(i2->country_code() != m_country_code || i2->area_code() != m_area_code) break;
            i2++;
        }
        return std::make_pair(i1, i2);
    }
}

int main()
{
    using namespace std;
    using namespace ch17;

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
    pair<vector<phone_number>::iterator,vector<phone_number>::iterator> pair = c.get_equal_bound(1, 206);
    for(vector<phone_number>::iterator i = pair.first; i != pair.second; i++)
        cout << *i << endl;

    cout << "printing all 360 numbers:" << endl;
    pair = c.get_equal_bound(1, 360);
    for(vector<phone_number>::iterator i = pair.first; i != pair.second; i++)
        cout << *i << endl;

    cout << "printing all 425 numbers:" << endl;
    pair = c.get_equal_bound(1, 425);
    for(vector<phone_number>::iterator i = pair.first; i != pair.second; i++)
        cout << *i << endl;

    return 0;
}
