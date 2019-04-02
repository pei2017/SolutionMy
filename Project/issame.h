#pragma once
#include <algorithm>

namespace myisame
{
	template<int N>
	int testN()
	{
		return N;
	}

	struct MyStruct
	{
		int x;
		int y;
	};

	constexpr int for_each1()
	{
		return 0;
	}

	template<typename First, typename... Rest>
	constexpr int for_each1(First first, Rest... rest)
	{
		return (std::is_same<int, First>::value) + for_each1(rest...);
	}

#define ISSAME1(T) (std::is_same<T, T1>::value)
#define ISSAME2(T) (std::is_same<T, T2>::value) + ISSAME1(T)
#define ISSAME3(T) (std::is_same<T, T3>::value) + ISSAME2(T)
#define GET_ARGTYPE_COUNT(Type, totalCount) ISSAME##totalCount(Type)

	template<typename T1, typename T2, typename T3>
	int GetParamCount()
	{
		const int v = (std::is_same<MyStruct, T1>::value) + (std::is_same<MyStruct, T2>::value) + (std::is_same<MyStruct, T3>::value);
		const int v2 = GET_ARGTYPE_COUNT(MyStruct, 3);
		return v + v2;
	}

	void Test()
	{
		testN<for_each1(1, 1.0, 2.0, 3)>();
		int i = GetParamCount<MyStruct, int, double>();
	}
}