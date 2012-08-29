#include <streambuf>
#include <ostream>
#include <iostream>
#include <sstream>

// Simple xor "encryption" -- easily breakable. Use a real encryption library for real work!
// I struggled with a couple of ways to make this work without having to wrap the stream, including
// stacking streambufs, but it got pretty messy. For real work, boost has some facilities for filtering
// streams.
//
// Ultimately, the solution is inspired by a post found here:
// http://www.velocityreviews.com/forums/t946062-homework-help-file-encrpytion-decryption-problem.html
namespace ch21 {
    struct Encrypt {
        std::string k;
        Encrypt(const std::string& k) : k(k) { }
    };

    struct Decrypt {
        std::string k;
        Decrypt(const std::string& k) : k(k) { }
    };

    class NoEncrypt { };
    class NoDecrypt { };

    template<class Os>
    class ostream_enc {
        Os& out;
        std::string key;
        int count;

    public:
        ostream_enc(Os& out, const std::string& key) : out(out), key(key), count(0) { }

        template<class T>
        ostream_enc& operator<<(const T& x) {
            std::ostringstream os;
            os.copyfmt(out);
            os << x;
            std::string tmp = os.str();
            for(int i = 0; i < tmp.size(); i++)
                tmp[i] ^= key[count++ % key.size()];
            out << tmp;
            return *this;
        }

        Os& inner() const { return out; }
    };

    // Unfortunately, this has to eat the whole input stream, so it doesn't make sense to
    // stop decrypting; there's no way to know how many bytes a given >> operator is going
    // to require. Is there a better way to do this?
    template<class Is>
    class istream_enc {
        Is& in;
        std::string key;
        std::stringstream iss;
    public:
        istream_enc(Is& in, const std::string& key) : in(in), key(key) {
            std::ostringstream oss;
            oss << in.rdbuf();
            std::string tmp = oss.str();
            for(int i = 0; i < tmp.size(); i++)
                tmp[i] ^= key[i % key.size()];
            iss << tmp;
        }

        istream_enc(const istream_enc& x) : in(x.in), key(x.key){
            iss << x.iss.rdbuf();
        }

        template<class T>
        istream_enc& operator>>(T& x) {
            iss >> x;
            return *this;
        }
    };

    template<class T>
    ostream_enc<std::basic_ostream<T> > operator<<(std::basic_ostream<T>& out, const Encrypt& enc) {
        return ostream_enc<std::basic_ostream<T> >(out, enc.k);
    }

    template<class T>
    istream_enc<std::basic_istream<T> > operator>>(std::basic_istream<T>& in, const Decrypt& dec) {
        return istream_enc<std::basic_istream<T> >(in, dec.k);
    }

    template<class T>
    T& operator<<(ostream_enc<T>& out, const NoEncrypt& dummy) {
        return out.inner();
    }

    Encrypt encrypt(const std::string& k) {
        return Encrypt(k);
    }

    NoEncrypt noencrypt() {
        return NoEncrypt();
    }

    Decrypt decrypt(const std::string& k) {
        return Decrypt(k);
    }
}

int main() {
    using namespace std;
    using namespace ch21;

    ostringstream out;
    out << encrypt("foobar") << "Hello World!" << noencrypt();
    istringstream in(out.str());
    std::string s1, s2;
    in >> decrypt("foobar") >> s1 >> s2;
    cout << s1 << ' ' << s2 << endl;
}
