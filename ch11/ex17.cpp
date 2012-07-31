#include <iostream>

namespace Exercises
{
	class Vector
	{
		int sz;
		float* items;
		
	public:
		class RangeError {};
		class SizeError {};

		Vector();
		Vector(int size);
		Vector(const Vector& other);
		~Vector();

		void Check(int idx) const;
		void CheckSizeEquals(const Vector& other) const;
		float& operator[](const int idx);
		float operator[](const int idx) const;

		Vector& operator+=(const Vector& x);
		Vector& operator-=(const Vector& x);
		Vector& operator*=(const Vector& x);
		Vector& operator/=(const Vector& x);
		Vector operator+(const Vector& x) const;
		Vector operator-(const Vector& x) const;
		Vector operator*(const Vector& x) const;
		Vector operator/(const Vector& x) const;

		Vector& Vector::operator=(const Vector& x);
		Vector& operator+=(const float x);
		Vector& operator-=(const float x);
		Vector& operator*=(const float x);
		Vector& operator/=(const float x);
		Vector operator+(const float x) const;
		Vector operator-(const float x) const;
		Vector operator*(const float x) const;
		Vector operator/(const float x) const;
		friend Vector operator+(const float x, const Vector& y);
		friend Vector operator-(const float x, const Vector& y);
		friend Vector operator*(const float x, const Vector& y);
		friend Vector operator/(const float x, const Vector& y);

		friend std::ostream& operator<<(std::ostream& output, const Vector& x);
	};

	Vector::Vector() : sz(0)
	{
	}

	Vector::Vector(int size) : sz(size)
	{
		items = new float[sz];
		for(int i = 0; i < sz; i++)
			items[i] = 0.0;
	}

	Vector::Vector(const Vector& other)
	{
		sz = other.sz;
		items = new float[sz];
		for(int i = 0; i < sz; i++)
			items[i] = other.items[i];
	}

	Vector::~Vector()
	{
		delete[] items;
	}

	void Vector::Check(const int idx) const
	{
		if(idx < 0 || idx >= sz)
			throw RangeError();
	}

	void Vector::CheckSizeEquals(const Vector& other) const
	{
		if(sz != other.sz)
			throw SizeError();
	}

	float& Vector::operator[](const int idx)
	{
		Check(idx);
		return items[idx];
	}

	float Vector::operator[](const int idx) const
	{
		Check(idx);
		return items[idx];
	}

	Vector& Vector::operator=(const Vector& x)
	{
		if(sz != x.sz)
		{
			delete[] items;
			items = new float[x.sz];
			sz = x.sz;
		}
		for(int i = 0; i < sz; i++)
			items[i] = x.items[i];
		return *this;
	}

	Vector& Vector::operator+=(const Vector& x)
	{
		CheckSizeEquals(x);
		for(int i = 0; i < sz; i++)
			items[i] += x.items[i];
		return *this;
	}

	Vector& Vector::operator-=(const Vector& x)
	{
		CheckSizeEquals(x);
		for(int i = 0; i < sz; i++)
			items[i] -= x.items[i];
		return *this;
	}

	Vector& Vector::operator*=(const Vector& x)
	{
		CheckSizeEquals(x);
		for(int i = 0; i < sz; i++)
			items[i] *= x.items[i];
		return *this;
	}

	Vector& Vector::operator/=(const Vector& x)
	{
		CheckSizeEquals(x);
		for(int i = 0; i < sz; i++)
			items[i] /= x.items[i];
		return *this;
	}

	Vector Vector::operator+(const Vector& x) const
	{
		Vector result = *this;
		result += x;
		return result;
	}

	Vector Vector::operator-(const Vector& x) const
	{
		Vector result = *this;
		result -= x;
		return result;
	}

	Vector Vector::operator*(const Vector& x) const
	{
		Vector result = *this;
		result *= x;
		return result;
	}

	Vector Vector::operator/(const Vector& x) const
	{
		Vector result = *this;
		result /= x;
		return result;
	}

	Vector& Vector::operator+=(const float x)
	{
		for(int i = 0; i < sz; i++)
			items[i] += x;
		return *this;
	}

	Vector& Vector::operator-=(const float x)
	{
		for(int i = 0; i < sz; i++)
			items[i] -= x;
		return *this;
	}

	Vector& Vector::operator*=(const float x)
	{
		for(int i = 0; i < sz; i++)
			items[i] *= x;
		return *this;
	}

	Vector& Vector::operator/=(const float x)
	{
		for(int i = 0; i < sz; i++)
			items[i] /= x;
		return *this;
	}

	Vector Vector::operator+(const float x) const
	{
		Vector result = *this;
		result += x;
		return result;
	}

	Vector Vector::operator-(const float x) const
	{
		Vector result = *this;
		result -= x;
		return result;
	}

	Vector Vector::operator*(const float x) const
	{
		Vector result = *this;
		result *= x;
		return result;
	}

	Vector Vector::operator/(const float x) const
	{
		Vector result = *this;
		result /= x;
		return result;
	}

	Vector operator+(const float x, const Vector& y)
	{
		return y + x;
	}

	Vector operator-(const float x, const Vector& y)
	{
		Vector result(y.sz);
		for(int i = 0; i < y.sz; i++)
			result[i] = x - y.items[i];
		return result;
	}

	Vector operator*(const float x, const Vector& y)
	{
		return y * x;
	}

	Vector operator/(const float x, const Vector& y)
	{
		Vector result(y.sz);
		for(int i = 0; i < y.sz; i++)
			result[i] = x / y.items[i];
		return result;
	}

	std::ostream& operator<<(std::ostream& output, const Vector& x)
	{
		output << '[';
		for(int i = 0; i < x.sz; i++)
		{
			output << x.items[i];
			if(i < x.sz-1) output << ',';
		}
		output << ']';
		return output;
	}
}

int main()
{
	using namespace std;
	using namespace Exercises;

	Vector x(5);
	x[0] = 1.2f;
	x[1] = 4.3f;
	x[2] = 7.0f;
	x[3] = 1.8f;
	x[4] = 1.0f;

	Vector y(5);
	y[0] = 1.0f;
	y[1] = 2.0f;
	y[2] = 2.4f;
	y[3] = 3.3f;
	y[4] = 0.1f;

	cout << x << " + " << y << " = " << x + y << endl;
	cout << x << " - " << y << " = " << x - y << endl;
	cout << x << " * " << y << " = " << x * y << endl;
	cout << x << " / " << y << " = " << x / y << endl;

	float z = 2.1f;

	cout << x << " + " << z << " = " << x + z << endl;
	cout << x << " - " << z << " = " << x - z << endl;
	cout << x << " * " << z << " = " << x * z << endl;
	cout << x << " / " << z << " = " << x / z << endl;

	cout << z << " + " << x << " = " << z + x << endl;
	cout << z << " - " << x << " = " << z - x << endl;
	cout << z << " * " << x << " = " << z * x << endl;
	cout << z << " / " << x << " = " << z / x << endl;

	return 0;
}
