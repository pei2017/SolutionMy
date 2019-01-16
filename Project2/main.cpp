#include <variant>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <list>
#include <iterator>

using namespace std;
enum Type
{
	tint,
	tfloat,
};

struct Valclass
{
	Type t = tint;
	int x = 0;
	float y = 1.0;
};

namespace
{
	class my_cast_imp
	{
		Valclass s;
	public:
		my_cast_imp(const Valclass s) : s(s) {}
		operator int() const
		{
			return s.x;
		}
		operator float() const
		{
			return s.y;
		}
	};
	my_cast_imp my_cast2(const Valclass& s)
	{
		return my_cast_imp(s);
	}
}

namespace
{
	template<Type>
	auto get(const std::vector<Valclass>& vec, size_t idx)
	{

	}
	template<>
	auto get<tint>(const std::vector<Valclass>& vec, size_t idx)
	{
		return vec[idx].x;
	}
	template<>
	auto get<tfloat>(const std::vector<Valclass>& vec, size_t idx)
	{
		return vec[idx].y;
	}
}

namespace
{
	void testInner(int x, float y) {}

	void test()
	{
		Valclass val;
		int x = my_cast2(val);
		float y = my_cast2(val);
		testInner(my_cast2(val), my_cast2(val));

		std::vector<Valclass> vals(3);
		int i = get<tint>(vals, 0);
		float j = get<tfloat>(vals, 0);
	}
}

int main()
{
	test();
	return 0;
}