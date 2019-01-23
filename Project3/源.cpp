#include <vector>
typedef unsigned long DWORD;

//stdcall
//win32

int test(int i, int j)
{
	i++;
	j++;
	return i + j;
}

class Base
{
public:
	int test(int i, int j)
	{
		i++;
		j++;
		return i + j;
	}
	int m = 1;
};

typedef int(Base::*pMember)(int, int);
union MyUnion
{
	void* pVoid;
	pMember pFunc;
};

void test1()
{
	Base base;
	DWORD bufferLen = 8;
	int arr[2] = { 1,2 };

	DWORD hr = 0;
	void* pBuffer = arr;
	void* pObj = &base;
	void* pFunc = 0;
	{
		MyUnion my; my.pFunc = (&Base::test);
		pFunc = my.pVoid;
	}


	DWORD nsp = 0;
	__asm
	{
		push ecx;
		push esi;
		push edi;
		mov nsp, esp;
		sub esp, bufferLen;

		mov esi, pBuffer;
		mov edi, esp;
		mov ecx, bufferLen;
		rep movsb;

		mov ecx, pObj;
		call[pFunc];
		mov hr, eax;

		mov esp, nsp;
		pop edi;
		pop esi;
		pop ecx;
	}
}

void test2()
{

	DWORD bufferLen = 8;
	int arr[2] = { 1,2 };

	DWORD hr = 0;
	void* pBuffer = arr;
	void* pFunc = test;
	void* pObj = nullptr;

	DWORD nsp = 0;
	__asm
	{
		push ecx;
		push esi;
		push edi;
		mov nsp, esp;
		sub esp, bufferLen;

		mov esi, pBuffer;
		mov edi, esp;
		mov ecx, bufferLen;
		rep movsb;

		mov ecx, pObj;
		call[pFunc];
		mov hr, eax;

		mov esp, nsp;
		pop edi;
		pop esi;
		pop ecx;
	}
}

int main()
{
	test1();
	test2();
	return 0;
}