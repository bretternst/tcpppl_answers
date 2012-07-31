namespace Exercises
{
	class Monster;
	class Object;

	class Warrior
	{
		virtual void Move(int,int) = 0;
		virtual void PickUp(Object*) = 0;
		virtual void Drop(Object*) = 0;
		virtual void Equip(Object*) = 0; // e.g. equip sword, shield, etc.
		virtual void Use(Object*) = 0; // e.g. scroll, potion
		virtual void Use(Object*,Object*) = 0; // e.g. use key on door
		virtual void Use(Object*,Monster*) = 0; // e.g. use scroll's spell on monster
		virtual void Attack(Monster*) = 0;
		virtual void TakeDamage(int) = 0;
	};

	class Monster
	{
		virtual void Move(int,int) = 0;
		virtual void Attack(Warrior*) = 0;
		virtual void TakeDamage(int) = 0;
	};

	class Object
	{
		virtual void Use(Object*) = 0; // e.g. use key on door (called by Warrior::Use(Object*,Object*))
		virtual void Use(Warrior*) = 0; // e.g. use on warrior (might decrease number of "charges" left, etc., called by Warrior::Use(Object*)0)
		virtual void Use(Monster*) = 0; // e.g. use on monster (called by Warrior::Use(Object*,Monster*))
		virtual void Degrade(int) = 0; // e.g. degrade a sword by 1 for each attack
	};
}

int main()
{
	return 0;
}
