namespace Exercises
{
    class Monster;
    class Object;

    class Warrior
    {
        virtual void move(int,int) = 0;
        virtual void pick_up(Object*) = 0;
        virtual void drop(Object*) = 0;
        virtual void equip(Object*) = 0; // e.g. equip sword, shield, etc.
        virtual void use(Object*) = 0; // e.g. scroll, potion
        virtual void use(Object*,Object*) = 0; // e.g. use key on door
        virtual void use(Object*,Monster*) = 0; // e.g. use scroll's spell on monster
        virtual void attack(Monster*) = 0;
        virtual void take_damage(int) = 0;
    };

    class Monster
    {
        virtual void move(int,int) = 0;
        virtual void attack(Warrior*) = 0;
        virtual void take_damage(int) = 0;
    };

    class Object
    {
        virtual void use(Object*) = 0; // e.g. use key on door (called by Warrior::Use(Object*,Object*))
        virtual void use(Warrior*) = 0; // e.g. use on warrior (might decrease number of "charges" left, etc., called by Warrior::Use(Object*)0)
        virtual void use(Monster*) = 0; // e.g. use on monster (called by Warrior::Use(Object*,Monster*))
        virtual void degrade(int) = 0; // e.g. degrade a sword by 1 for each attack
    };
}

int main()
{
    return 0;
}
