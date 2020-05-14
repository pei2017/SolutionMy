#pragma once
#include <algorithm>
#include <vector>
#include <map>

namespace tttemplate
{
	using std::vector;
	using std::map;
	using std::string;

	template<typename T>
	struct Traits { using type = T; };

	template<typename T>
	struct Container
	{
		// ����������Ƕ�����ͣ��޷������Զ��Ƶ�
		Container(int, typename Traits<T>::type v) {}
	};

#if __cplusplus > 201402L
	// �Զ��ƶ���
	template<typename T>
	Container(int ,T)->Container<T>;
#endif

	template<typename T1, typename T2>
	struct Container2
	{
		Container2(T1, T2, int) {}
	};

	void ttest()
	{
#if __cplusplus > 201402L
		Container c(0, 0); //  �������Զ��Ƶ��Ľ���� Container<int>
		Container2 c2(5,5,0);
#endif
	}
}