#include <iostream>
#include <X11/Xlib.h>
#include "window.h"
#include "tests.h"

using namespace std;

int main() {
    using gfxlib::Window;

    Display *display = XOpenDisplay(NULL);
    if(!display) {
        cerr << "Unable to open display.";
        return 1;
    }

    Window w(display, 640, 480, 0xfff0f0ff);
    run_tests(&w);

    XEvent event;

    Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(display, w.handle(), &wm_delete, 1);
    XSelectInput(display, w.handle(), ExposureMask | KeyPressMask | ButtonPressMask);

    while(1) {
        XNextEvent(display, &event);
        if(event.type == KeyPress)
            break;
        else if (event.type == Expose)
            w.paint();
        else if (event.xclient.data.l[0] == wm_delete)
            break;
    }

    XCloseDisplay(display);
}
