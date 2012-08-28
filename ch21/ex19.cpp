#include <iostream>
#include <string>
#include <termios.h>

// This isn't portable and it's also a bit crude because it only works for stdin.
// Using it with anything but cin will have no effect.
namespace ch21 {
    void set_echo(bool enable) {
        termios tty;
        tcgetattr(STDIN_FILENO, &tty);
        if(enable)
            tty.c_lflag |= ECHO;
        else
            tty.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);
    }

    std::istream& echo_on(std::istream& in) {
        if(in == std::cin)
            set_echo(true);
        return in;
    }

    std::istream& echo_off(std::istream& in) {
        if(in == std::cin)
            set_echo(false);
        return in;
    }
}

int main() {
    using namespace std;
    using namespace ch21;

    cout << "Enter hidden integer: ";
    string s;
    cin >> echo_off >> s;
    cout << "\nYou entered: " << s << endl;
    cout << "Enter shown integer: ";
    cin >> echo_on >> s;
}
