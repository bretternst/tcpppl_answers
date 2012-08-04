namespace ch12
{
    // Dummy classes defined simply so that this compiles.
    //
    class BBslider {};
    class BBdial {};
    //
    //

    // In the examples in 12.4, I noticed that Ival_slider and Ival_dial
    // didn't actually provide any functionality beyond the base Ival_box
    // class. They only differed in implementation. Therefore, I suggest
    // that there is no need to have an interface hierarchy beyond one
    // level of inheritance.
    //
    // I am also going to assume that whatever implementation system is
    // used does not have a set of abstract interfaces that can be used.
    // Some may, but this is a generalized solution that could be used
    // with a user interface toolkit of any kind--poorly designed or not.

    // Ival_box represents the interface that will be presented throughout
    // the user's code.
    class Ival_box
    {
    public:
        virtual int get_value() = 0;
        virtual void set_value(int i) = 0;
        virtual void reset_value(int i) = 0;
        virtual void prompt() = 0;
        virtual bool was_changed() = 0;
        virtual ~Ival_box() {}
    };

    // Concrete class that creates a slider from the fictional "BB" library.
    // Per the exercise requirements, a single pointer is used for the
    // implementation.
    class BBslider_val_box : public Ival_box
    {
        int value;
        int last_value; // for was_changed()

        class BBslider_impl : public BBslider
        {
            int get_value();
            void set_value();
            void prompt();
        };

        BBslider_impl* slider; // Used by public functions in this class.

    public:
        BBslider_val_box(int, int); // Constructs new BBslider
        ~BBslider_val_box(); // Destructs BBslider
        int get_value();
        void set_value(int i);
        void reset_value(int i);
        void prompt();
        bool was_changed();
    };

    class BBdial_val_box : public Ival_box
    {
        int value;
        int last_value; // for was_changed()

        class BBdial_impl : public BBdial
        {
            int get_value();
            void set_value();
            void prompt();
        };

        BBdial_impl* dial;

    public:
        BBdial_val_box(int, int);
        ~BBdial_val_box() {};
        int get_value();
        void set_value(int i);
        void reset_value(int i);
        void prompt();
        bool was_changed();
    };
}

using namespace ch12;

Ival_box* makeSlider()
{
    // commented out because the ctor doesn't exist
    //Ival_box* i = new BBdial_val_box(2,3);
    //return i;
    return 0;
}

int useSlider()
{
    Ival_box* i = makeSlider();
    i->set_value(5);
    i->prompt();
    int x = i->get_value();
    return x;
}


// This approach is:
// - Very easy to use (instantiate specific class, use interface everywhere else; the
//   user can choose whichever factory technique he/she wants to use.)
// - Relatively easy to program. In the absence of specific requirements, I tend to favor
//   simplicity over other factors like performance or compilation time.
//   This technique is straightforward. Having two hierarchies is, in my opinion, over-
//   engineered and will only make your co-workers hate you.
// - New concepts cannot be added to the hierarchy because there is no hierarchy. If a
//   new capability is added to Ival_box, inheriting classes must implement it in some way.
// - Making changes to implementations is easy. Making changes to interfaces (such as the
//   Ival_box interface) can be much trickier because it will require all implementers to
//   support it. Changing Ival_box at all requires a full re-build.
// - Recompilation is not required for the bulk of the program if changes are made to the
//   implementation because most of the program refers only to Ival_box. However, the parts
//   of the program that directly use the descendants of Ival_box may need to be recompiled
//   if the implementation changes.
int main()
{
    return 0;
}
