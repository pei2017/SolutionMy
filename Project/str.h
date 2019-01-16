#pragma once

#include <variant>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <list>

using namespace std;
static const string BEGIN = "begin:";
static const string END = " end!";

struct Item
{
public:
	int x;
	Item(int x) : x(x) {}
public:
	string toString() {
		return to_string(x);
	}
};

//递归
namespace
{
	string ToString()
	{
		return "";
	}

	template<typename Last>
	string ToString(Last last)
	{
		return last.toString();
	}

	template<typename First, typename... Rest>
	string ToString(First first, Rest... rest)
	{
		return first.toString().append(ToString(rest...));
	}
}

//逗号表达式 && 初始化列表
namespace
{
	template<typename... Args>
	string ToString2(Args... args)
	{
		string s = BEGIN;
		int arr[] = { (s.append(args.toString()), 0)... };
		s.append(END);
		return s;
	}
}

//递归2
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

//CRTP
template <typename Derive>
class KObjectDescriptorBase
{
public:
	string ToString()
	{
		return static_cast<Derive&>(*this).ToStringImpl();
	}

private:
	string ToStringImpl()
	{
		return "";
	}
};

//VIRTURE
//序列化 && 反序列化
class IHost
{
public:
	virtual string tostr() = 0;
};

struct Arg
{
	int i;
	string s;
};
typedef std::vector<Arg> ARGS;

class Characters : public IHost
{
public:
	string tostr()
	{
		return m_host->tostr() + "char ";
	}

	void func()
	{
	}
	IHost* m_host;
};

class Font
{
public:
	Characters* m_host;

	string tostr()
	{
		return m_host->tostr() + "font ";
	}

	void func(int i)
	{
	}
	void func2(string s)
	{
	}

	void process(string funName, ARGS args)
	{
		//1FuncMap 2字符串hash-compare
		if (funName == "func2")
			func2(args[0].s);
		else if (funName == "func")
			func(args[0].i);
	}
};

void process(string ObjectName, string funName, ARGS args)
{
	if (funName == "KEtFont")
	{
		Font font;
		font.process(funName, args);
		/*		KEtFont::process(&font, funName, args);*/
	}
}