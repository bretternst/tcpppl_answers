#include <iostream>
#include <string>
#include <cmath>

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

	class Mat4
	{
		static const int MAT_SIZE = 4;
		Vec4* items;

	public:
		class RangeError {};

		Mat4();
		Mat4(const Mat4& other);
		~Mat4();

		void Check(int idx) const;
		Vec4& operator[](int idx);
		const Vec4& operator[](int idx) const;
		Mat4& operator=(const Mat4& x);
		Mat4& operator+=(const Mat4& x);
		Mat4 operator+(const Mat4& x) const;
		Mat4& operator*=(const float x);
		Mat4 operator*(const float x) const;
		Mat4& operator*=(const Mat4& x);
		Mat4 operator*(const Mat4& x) const;
		friend Mat4 operator*(const float x, const Mat4& y);

		friend std::ostream& operator<<(std::ostream& output, const Mat4& x);

		void GaussianElimination(Vec4& answers);
	};

	Mat4::Mat4()
	{
		items = new Vec4[MAT_SIZE];
		for(int i = 0; i < MAT_SIZE; i++)
			items[i] = Vec4();
	}

	Mat4::Mat4(const Mat4& other)
	{
		items = new Vec4[MAT_SIZE];
		for(int i = 0; i < MAT_SIZE; i++)
			items[i] = other.items[i];
	}

	Mat4::~Mat4()
	{
		delete[] items;
	}
	
	void Mat4::Check(int idx) const
	{
		if(idx < 0 || idx >= MAT_SIZE)
			throw RangeError();
	}

	Vec4& Mat4::operator[](int idx)
	{
		Check(idx);
		return items[idx];
	}

	const Vec4& Mat4::operator[](int idx) const
	{
		Check(idx);
		return items[idx];
	}

	Mat4& Mat4::operator=(const Mat4& x)
	{
		for(int i = 0; i < Mat4::MAT_SIZE; i++)
			items[i] = x.items[i];
		return *this;
	}

	Mat4& Mat4::operator+=(const Mat4& x)
	{
		for(int i = 0; i < Mat4::MAT_SIZE; i++)
			items[i] += x.items[i];
		return *this;
	}

	Mat4 Mat4::operator+(const Mat4& x) const
	{
		Mat4 result = *this;
		result += x;
		return result;
	}

	Mat4& Mat4::operator*=(const float x)
	{
		for(int i = 0; i < Mat4::MAT_SIZE; i++)
			items[i] *= x;
		return *this;
	}

	Mat4 Mat4::operator*(const float x) const
	{
		Mat4 result = *this;
		result *= x;
		return result;
	}

	Mat4 operator*(const float x, const Mat4& y)
	{
		return y * x;
	}

	Mat4& Mat4::operator*=(const Mat4& x)
	{
		*this = *this * x;
		return *this;
	}

	Mat4 Mat4::operator*(const Mat4& x) const
	{
		Mat4 result;
		for(int i = 0; i < MAT_SIZE; i++)
		{
			for(int j = 0; j < MAT_SIZE; j++)
			{
				float a = 0;
				for(int k = 0; k < MAT_SIZE; k++)
				{
					a += items[i][k] * x.items[k][j];
				}
				result[i][j] = a;
			}
		}
		return result;
	}

	std::ostream& operator<<(std::ostream& output, const Mat4& x)
	{
		output << " _" << std::string(8*Mat4::MAT_SIZE, ' ') << '_' << std::endl;
		for(int i = 0; i < Mat4::MAT_SIZE; i++)
		{
			output << "|";
			for(int j = 0; j < Mat4::MAT_SIZE; j++)
			{
				output.width(7);
				output << x[i][j];
				if(j < Mat4::MAT_SIZE-1) output << ",";
			}
			output << "   |" << std::endl;
		}
		output << " -" << std::string(8*Mat4::MAT_SIZE, ' ') << '-' << std::endl;

		return output;
	}

	void Mat4::GaussianElimination(Vec4& answers)
	{
		int i = 0, j = 0;
		while(i < MAT_SIZE && j < MAT_SIZE)
		{
			int maxi = i;
			for(int k = i+1; k < MAT_SIZE; k++)
			{
				if(abs(items[k][j]) > abs(items[maxi][j]))
					maxi = k;
			}
			if(items[maxi][j] != 0)
			{
				Vec4 tmp = items[maxi];
				items[maxi] = items[i];
				items[i] = tmp;
				float atmp = answers[maxi];
				answers[maxi] = answers[i];
				answers[i] = atmp;
				for(int k = 0; k < MAT_SIZE; k++)
				{
					items[i][k] /= items[i][j];
				}
				answers[i] /= items[i][j];
				for(int u = i+1; u < MAT_SIZE; u++)
				{
					for(int k = 0; k < MAT_SIZE; k++)
					{
						items[u][k] -= items[u][i] * items[i][k];
					}
					answers[u] -= items[u][i] * answers[i];
				}
				i++;
			}
			j++;
		}
	}
}

int main()
{
	using namespace std;
	using namespace Exercises;

	Vec4 v1;
	v1[0] = 1.2f;
	v1[1] = 4.3f;
	v1[2] = 7.0f;
	v1[3] = 1.8f;

	Vec4 v2;
	v2[0] = 1.0f;
	v2[1] = 2.0f;
	v2[2] = 2.4f;
	v2[3] = 3.3f;

	Vec4 v3;
	v3[0] = 0.8f;
	v3[1] = 1.0f;
	v3[2] = 1.16f;
	v3[3] = 2.9f;

	Vec4 v4;
	v4[0] = 1.1f;
	v4[1] = 6.8f;
	v4[2] = 9.1f;
	v4[3] = 10.0f;

	Mat4 m1;
	m1[0] = v1;
	m1[1] = v2;
	m1[2] = v3;
	m1[3] = v4;

	Mat4 m2;
	m2[0] = v2;
	m2[1] = v3;
	m2[2] = v1;
	m2[3] = v4;

	cout << "m1:" << endl << m1;
	cout << "m2:" << endl << m2;
	cout << "m1 + m2:" << endl << m1 + m2;
	cout << "m1 * 3:" << endl << m1 * 3;
	cout << "m1 * m2:" << endl << m1 * m2;

	m1.GaussianElimination(v3);
	cout << "After Gaussian elimination:" << endl;
	cout << m1;
	cout << v3;

	return 0;
}
