#include "str.h"

namespace
{
	void test0()
	{
		Item i1(222);
		Item i2(111);
		string s = ToString(i1, i2);
		string s2 = ToString2(i1, i2);
		cout << s << endl;
		cout << s2 << endl;
	}

	void test1()
	{
		print(10, 'c', "bbb", string("123"));
	}

	void test2()
	{
		vector<int> i = { 1,2,3,4,5 };
		{
			vector<int> temp;
			temp.swap(i);
		}
		int size = i.size();
	}

	void test3()
	{
		class Node
		{
			int x;
			int y;
		};

		std::list<Node*> list;
		//auto iter = --list.end();
		list.push_back(new Node());
	}

	struct Valclass
	{
		int x = 0;
		float y = 1.0;
	};
	enum Type
	{
		tint,
		tfloat,
	};
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

	template<Type>
	auto get(const std::vector<Valclass>& vec, size_t idx)
	{
		return vec[idx];
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

	void test4Inner(int x, float y) {}

	void test4()
	{
		Valclass val;
		int x = my_cast2(val);
		float y = my_cast2(val);
		test4Inner(my_cast2(val), my_cast2(val));

		std::vector<Valclass> vals(3);
		int i = get<tint>(vals, 0);
		float j = get<tfloat>(vals, 0);
	}
}

int main()
{
	test4();
	test3();
	test2();
	test0();
	test1();
	return 0;
}