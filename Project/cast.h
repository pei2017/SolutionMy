#pragma once
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

namespace cast
{
	enum Type
	{
		tint,
		tfloat,
	};

	struct MyStruct
	{
		int x;
		int y;
	};

	struct ValClass
	{
		Type	t = tint;
		int		x = 0;
		float	y = 0.0;
		MyStruct z = { 0,0 };
	};
	class myCast
	{
	private:
		ValClass m_val;
	public:
		myCast(const ValClass s) : m_val(s) {}
		operator int() const
		{
			return m_val.x;
		}
		operator float() const
		{
			return m_val.y;
		}

		operator MyStruct() const
		{
			return m_val.z;
		}
	};

	myCast my_cast_helper(const ValClass& s)
	{
		return myCast(s);
	}

	void testInner(int x, float y, MyStruct z)
	{

	}

	void test()
	{
		ValClass val;
		int x = my_cast_helper(val);
		float y = my_cast_helper(val);

		testInner(my_cast_helper(val), my_cast_helper(val), my_cast_helper(val));
	}
}