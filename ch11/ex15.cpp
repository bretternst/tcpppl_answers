#include <iostream>

namespace Exercises
{
	class Vec4
	{
		static const int VEC_SIZE = 4;
		float* items;
		
	public:
		class RangeError {};

		Vec4();
		Vec4(const Vec4& other);
		~Vec4();

		void Check(int idx) const;
		float& operator[](const int idx);
		float operator[](const int idx) const;

		Vec4& operator+=(const Vec4& x);
		Vec4& operator-=(const Vec4& x);
		Vec4& operator*=(const Vec4& x);
		Vec4& operator/=(const Vec4& x);
		Vec4 operator+(const Vec4& x) const;
		Vec4 operator-(const Vec4& x) const;
		Vec4 operator*(const Vec4& x) const;
		Vec4 operator/(const Vec4& x) const;

		Vec4& Vec4::operator=(const Vec4& x);
		Vec4& operator+=(const float x);
		Vec4& operator-=(const float x);
		Vec4& operator*=(const float x);
		Vec4& operator/=(const float x);
		Vec4 operator+(const float x) const;
		Vec4 operator-(const float x) const;
		Vec4 operator*(const float x) const;
		Vec4 operator/(const float x) const;
		friend Vec4 operator+(const float x, const Vec4& y);
		friend Vec4 operator-(const float x, const Vec4& y);
		friend Vec4 operator*(const float x, const Vec4& y);
		friend Vec4 operator/(const float x, const Vec4& y);

		friend std::ostream& operator<<(std::ostream& output, const Vec4& x);
	};

	Vec4::Vec4()
	{
		items = new float[VEC_SIZE];
		for(int i = 0; i < VEC_SIZE; i++)
			items[i] = 0.0;
	}

	Vec4::Vec4(const Vec4& other)
	{
		items = new float[VEC_SIZE];
		for(int i = 0; i < VEC_SIZE; i++)
			items[i] = other.items[i];
	}

	Vec4::~Vec4()
	{
		delete[] items;
	}

	void Vec4::Check(const int idx) const
	{
		if(idx < 0 || idx >= VEC_SIZE)
			throw RangeError();
	}

	float& Vec4::operator[](const int idx)
	{
		Check(idx);
		return items[idx];
	}

	float Vec4::operator[](const int idx) const
	{
		Check(idx);
		return items[idx];
	}

	Vec4& Vec4::operator=(const Vec4& x)
	{
		for(int i = 0; i < Vec4::VEC_SIZE; i++)
			items[i] = x.items[i];
		return *this;
	}

	Vec4& Vec4::operator+=(const Vec4& x)
	{
		for(int i = 0; i < Vec4::VEC_SIZE; i++)
			items[i] += x.items[i];
		return *this;
	}

	Vec4& Vec4::operator-=(const Vec4& x)
	{
		for(int i = 0; i < Vec4::VEC_SIZE; i++)
			items[i] -= x.items[i];
		return *this;
	}

	Vec4& Vec4::operator*=(const Vec4& x)
	{
		for(int i = 0; i < Vec4::VEC_SIZE; i++)
			items[i] *= x.items[i];
		return *this;
	}

	Vec4& Vec4::operator/=(const Vec4& x)
	{
		for(int i = 0; i < Vec4::VEC_SIZE; i++)
			items[i] /= x.items[i];
		return *this;
	}

	Vec4 Vec4::operator+(const Vec4& x) const
	{
		Vec4 result = *this;
		result += x;
		return result;
	}

	Vec4 Vec4::operator-(const Vec4& x) const
	{
		Vec4 result = *this;
		result -= x;
		return result;
	}

	Vec4 Vec4::operator*(const Vec4& x) const
	{
		Vec4 result = *this;
		result *= x;
		return result;
	}

	Vec4 Vec4::operator/(const Vec4& x) const
	{
		Vec4 result = *this;
		result /= x;
		return result;
	}

	Vec4& Vec4::operator+=(const float x)
	{
		for(int i = 0; i < Vec4::VEC_SIZE; i++)
			items[i] += x;
		return *this;
	}

	Vec4& Vec4::operator-=(const float x)
	{
		for(int i = 0; i < Vec4::VEC_SIZE; i++)
			items[i] -= x;
		return *this;
	}

	Vec4& Vec4::operator*=(const float x)
	{
		for(int i = 0; i < Vec4::VEC_SIZE; i++)
			items[i] *= x;
		return *this;
	}

	Vec4& Vec4::operator/=(const float x)
	{
		for(int i = 0; i < Vec4::VEC_SIZE; i++)
			items[i] /= x;
		return *this;
	}

	Vec4 Vec4::operator+(const float x) const
	{
		Vec4 result = *this;
		result += x;
		return result;
	}

	Vec4 Vec4::operator-(const float x) const
	{
		Vec4 result = *this;
		result -= x;
		return result;
	}

	Vec4 Vec4::operator*(const float x) const
	{
		Vec4 result = *this;
		result *= x;
		return result;
	}

	Vec4 Vec4::operator/(const float x) const
	{
		Vec4 result = *this;
		result /= x;
		return result;
	}

	Vec4 operator+(const float x, const Vec4& y)
	{
		return y + x;
	}

	Vec4 operator-(const float x, const Vec4& y)
	{
		Vec4 result;
		for(int i = 0; i < Vec4::VEC_SIZE; i++)
			result[i] = x - y.items[i];
		return result;
	}

	Vec4 operator*(const float x, const Vec4& y)
	{
		return y * x;
	}

	Vec4 operator/(const float x, const Vec4& y)
	{
		Vec4 result;
		for(int i = 0; i < Vec4::VEC_SIZE; i++)
			result[i] = x / y.items[i];
		return result;
	}

	std::ostream& operator<<(std::ostream& output, const Vec4& x)
	{
		output << '[';
		for(int i = 0; i < Vec4::VEC_SIZE; i++)
		{
			output << x.items[i];
			if(i < Vec4::VEC_SIZE-1) output << ',';
		}
		output << ']';
		return output;
	}
}

int main()
{
	using namespace std;
	using namespace Exercises;

	Vec4 x;
	x[0] = 1.2f;
	x[1] = 4.3f;
	x[2] = 7.0f;
	x[3] = 1.8f;

	Vec4 y;
	y[0] = 1.0f;
	y[1] = 2.0f;
	y[2] = 2.4f;
	y[3] = 3.3f;

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
