#include <variant>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct Item
{
public:
	int x;
	Item(int x): x(x)
	{
	}
public:
	string toString()
	{
		return to_string(x);
	}
};

class Object
{
public:
	int a = 1;
};

static const string BEGIN = "begin:";
static const string END = " end!";

namespace
{
	string ToString()
	{
		return "";
	}

	string ToString(Object obj)
	{
		return "obj-"+to_string(obj.a)+ " ";
	}

	template<typename ArgHead, typename... Args>
	string ToString(ArgHead head, Args... args)
	{
		return head.toString().append(ToString(args...));
	}

	template<typename... Args>
	string ToString(Object obj, Args... args)
	{
		return BEGIN + ToString(obj) + ToString(args...) + END;
	}
}

namespace
{
	template<size_t I = 0, typename FuncT, typename ...Tp>
	inline typename std::enable_if_t<I == sizeof ...(Tp)> for_each(std::tuple<Tp ...>&, FuncT)
	{
	}

	template<size_t I = 0, typename FuncT, typename ...Tp>
	inline typename std::enable_if_t < I < sizeof ...(Tp)> for_each(std::tuple<Tp ...>& t, FuncT f)
	{
		f(std::get<I>(t));
		for_each<I + 1, FuncT, Tp...>(t, f);
	}

	template<typename ...Args>
	auto print(Args ...args)
	{
		auto a = std::forward_as_tuple(args...);
		for_each(a, [](auto x)
		{
			std::cout << typeid(x).name() << ":" << x << std::endl;
		});
	}
}

class Test
{
public:

public:
	vector<Item> items;
};

void test0()
{
	Object obj;
	Item i1(222);
	Item i2(111);
	string s = ToString(obj, i1, i2);
	cout << s << endl;
}

void test1()
{
	print(10, 'c', "bbb", string("123"));
}

int main()
{
	test0();
	test1();
	return 0;
}