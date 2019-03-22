#pragma once
#include <string>
#include <iostream>
using std::string;

//CRTP
namespace crtp
{
	template <typename Derive>
	class Base
	{
	public:
		string ToString()
		{
			return static_cast<Derive&>(*this).ToStringImpl();
		}

	protected:
		string ToStringImpl()
		{
			return "";
		}
	};

	class Derive1 : public Base<Derive1>
	{
	public:
		Derive1(int val) : m_val(val)
		{

		}

		string ToStringImpl()
		{
			return std::to_string(m_val);
		}
	private:
		int m_val;
	};

	class Derive2 : public Base<Derive2>
	{
	public:
		Derive2(int val1, int val2) : m_val1(val1), m_val2(val2)
		{

		}

		string ToStringImpl()
		{
			return std::to_string(m_val1 + m_val2);
		}
	private:
		int m_val1;
		int m_val2;
	};

	void test()
	{
		Derive1 d1(99);
		std::cout << d1.ToString() << std::endl;

		Derive2 d2(1, 99);
		std::cout << d2.ToString() << std::endl;
	}
}

namespace enable
{
	enum MyEnum
	{
		enum0,
	};

	template<typename T>
	string to_str(T&& t)
	{
		std::cout << "other" << std::endl;
		return std::to_string(t);
	}

	template<typename T>
	string to_str(typename std::enable_if<std::is_enum<T>::value, T>::type t)
	{
		std::cout << "enum" << std::endl;
		return std::to_string(t);
	}

	void test()
	{
		to_str(0);

		MyEnum myEnum = enum0;
		to_str<MyEnum>(myEnum);
	}
}