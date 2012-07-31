#include <iostream>

namespace Exercises
{
	using std::cout;
	using std::endl;

	class Circle;
	class Square;
	class Triangle;

	class Shape
	{
	public:
		virtual bool intersect(Shape* s) = 0;
		virtual bool intersect(Circle& s) = 0;
		virtual bool intersect(Square& s) = 0;
		virtual bool intersect(Triangle& s) = 0;
	};

	class Circle : public Shape
	{
	public:
		bool intersect(Shape* s)
		{
			return s->intersect(*this);
		}

		bool intersect(Circle& s)
		{
			cout << "Checking if Circle intersects with Circle" << endl;
			return false;
		}

		bool intersect(Square& s)
		{
			cout << "Checking if Circle intersects with Square" << endl;
			return false;
		}

		bool intersect(Triangle& t)
		{
			cout << "Checking if Circle intersects with Triangle" << endl;
			return false;
		}
	};

	class Square : public Shape
	{
	public:
		bool intersect(Shape* s)
		{
			return s->intersect(*this);
		}

		bool intersect(Circle& s)
		{
			return s.intersect(*this);
		}

		bool intersect(Square& s)
		{
			cout << "Checking if Square intersects with Square" << endl;
			return false;
		}

		bool intersect(Triangle& s)
		{
			cout << "Checking if Square intersects with Triangle" << endl;
			return false;
		}
	};

	class Triangle : public Shape
	{
	public:
		bool intersect(Shape* s)
		{
			return s->intersect(*this);
		}

		bool intersect(Circle& s)
		{
			return s.intersect(*this);
		}

		bool intersect(Square& s)
		{
			return s.intersect(*this);
		}

		bool intersect(Triangle& s)
		{
			cout << "Checking if Triangle intersects with Triangle" << endl;
			return false;
		}
	};

	bool intersect(Shape* s1, Shape* s2)
	{
		return s1->intersect(s2);
	}
}

void main()
{
	using namespace std;
	using namespace Exercises;

	Circle circle;
	Square square;
	Triangle triangle;

	intersect(&circle, &square);
	intersect(&circle, &triangle);
	intersect(&square, &circle);
	intersect(&square, &triangle);
	intersect(&triangle, &circle);
	intersect(&triangle, &square);
}
