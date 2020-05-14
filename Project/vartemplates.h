#pragma once
#include <variant>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <list>

namespace vartemplates
{
	//1.可变参数模板--递归(接受0-多个参数）
	namespace ns1
	{
		template<typename FuncT>
		void for_each(FuncT func)
		{

		}

		template<typename FuncT, typename First, typename... Rest>
		void for_each(FuncT func, First first, Rest... rest)
		{
			func(first);
			return for_each(func, rest...);
		}

		template<typename ...Args>
		void Test(Args ...args)
		{
			auto func = [](auto element) {
				std::cout << typeid(element).name() << ":" << element << std::endl;
			};
			for_each(func, args...);
		}
	}

	//2.可变参数模板--递归2(接受0-多个参数)(C++17)
	namespace ns2
	{
		template<typename FuncT, size_t I = 0, typename ...Tp>
		inline typename std::enable_if_t<I == sizeof ...(Tp)> for_each(std::tuple<Tp ...>& t, FuncT func)
		{

		}

		template<typename FuncT, size_t I = 0, typename ...Tp>
		inline typename std::enable_if_t < I < sizeof ...(Tp)> for_each(std::tuple<Tp ...>& t, FuncT func)
		{
			func(std::get<I>(t));
			for_each<FuncT, I + 1, Tp...>(t, func);
		}

		template<typename ...Args>
		void Test(Args ...args)
		{
#if __cplusplus > 201402L
			auto elements = std::forward_as_tuple(args...);
			for_each(elements, [](auto element) {
				std::cout << typeid(element).name() << ":" << element << std::endl;
				});
#endif
		}
	}


	//2.可变参数模板--逗号表达式 && 初始化列表
	namespace ns3
	{
		template<typename FuncT>
		void for_each(FuncT func)
		{

		}

		template<typename FuncT, typename... Args>
		inline void for_each(FuncT func, Args... args)
		{
			int arr[] = { (func(args), 0)... };
			return;
		}

		template<typename ...Args>
		void Test(Args ...args)
		{
			auto func = [](auto x) {
				std::cout << typeid(x).name() << ":" << x << std::endl;
			};
			for_each(func, args...);
		}
	}

	void test()
	{
		ns1::Test(1, 1.0);
		ns1::Test();
		ns2::Test(1, 1.0);
		ns2::Test();
		ns3::Test(1, 1.0);
		ns3::Test();
	}
}
