#pragma once
//-----------------------------------------------------------------------------
template<typename T>
struct MyParamSizeTraits
{
	enum { arg_size = sizeof(T) };
};
template<typename T>
struct MyParamSizeTraits<const T&>
{
	enum { arg_size = sizeof(int) };
};
template<>
struct MyParamSizeTraits<bool>
{
	enum { arg_size = sizeof(int) };
};
template<>
struct MyParamSizeTraits<char>
{
	enum { arg_size = sizeof(int) };
};
template<>
struct MyParamSizeTraits<unsigned char>
{
	enum { arg_size = sizeof(int) };
};
template<>
struct MyParamSizeTraits<short>
{
	enum { arg_size = sizeof(int) };
};
template<>
struct MyParamSizeTraits<unsigned short>
{
	enum { arg_size = sizeof(int) };
};

template<typename T>
int GetFuncArgSize(T t)
{
	return MyParamSizeTraits<T>::arg_size;
}

//-----------------------------------------------------------------------------
template<typename T>
struct MyParamTraits
{
};
template<typename _typename, typename _Ret>
struct MyParamTraits<_Ret(_typename::*)()>
{
	enum { arg_count = 0 };
};
template<typename _typename, typename _Ret, typename _Arg1>
struct MyParamTraits<_Ret(_typename::*)(_Arg1)>
{
	enum { arg_count = 1 };
};
template<typename _typename, typename _Ret, typename _Arg1, typename _Arg2>
struct MyParamTraits<_Ret(_typename::*)(_Arg1, _Arg2)>
{
	enum { arg_count = 2 };
};
template<typename _typename, typename _Ret, typename _Arg1, typename _Arg2, typename _Arg3>
struct MyParamTraits<_Ret(_typename::*)(_Arg1, _Arg2, _Arg3)>
{
	enum { arg_count = 3 };
};
template<typename _typename, typename _Ret, typename _Arg1, typename _Arg2, typename _Arg3, typename _Arg4>
struct MyParamTraits<_Ret(_typename::*)(_Arg1, _Arg2, _Arg3, _Arg4)>
{
	enum { arg_count = 4 };
};
template<typename _typename, typename _Ret, typename _Arg1, typename _Arg2, typename _Arg3, typename _Arg4, typename _Arg5>
struct MyParamTraits<_Ret(_typename::*)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5)>
{
	enum { arg_count = 5 };
};
template<typename _typename, typename _Ret, typename _Arg1, typename _Arg2, typename _Arg3, typename _Arg4, typename _Arg5, typename _Arg6>
struct MyParamTraits<_Ret(_typename::*)(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6)>
{
	enum { arg_count = 6 };
};

template<typename T>
int GetFuncArgCount(T t)
{
	return MyParamTraits<T>::arg_count;
}

namespace funcargs
{
	class ClassTest
	{
	public:
		void test0()
		{

		}

// 		void test0(int i)
// 		{
// 
// 		}

		int test1(int i)
		{
			return 0;
		}
	};

	void Test()
	{
		int num = GetFuncArgCount(&ClassTest::test0);
		int num2 = GetFuncArgCount(&ClassTest::test1);

		int size1 = GetFuncArgSize(bool());
		int size2 = GetFuncArgSize(int());
		int size3 = GetFuncArgSize(double());
		int size4 = GetFuncArgSize(char());
		int size5 = GetFuncArgSize(unsigned short());
		int* pInt = nullptr;
		const int* pCInt = nullptr;
		double d = 0.0;
		const double& dRef = d;
		int size10 = GetFuncArgSize(pInt);
		int size11 = GetFuncArgSize(pCInt);
		int size12 = GetFuncArgSize<const double&>(dRef);
	}
}