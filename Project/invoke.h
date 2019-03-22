#pragma once
#include <vector>

namespace invoke
{
	typedef unsigned long DWORD;
	typedef unsigned char byte;

#define Get_Func_Addr(FUNCTION, OUTPTR)  \
		__asm{ mov eax, FUNCTION } \
		__asm{ mov OUTPTR, eax} \

	class Buffer
	{
	private:
		byte* m_buffer;
		unsigned int m_pos;

	public:
		Buffer() : m_pos(0)
		{
			m_buffer = new byte[1024];
		}
		~Buffer()
		{
			delete m_buffer;
		}
		void* get() { return m_buffer; }
		unsigned int len() { return m_pos; }

		void writeInt32(int i) { innerWrite(i); }
		void writeDouble64(double d) { innerWrite(d); }

	private:
		template<typename T>
		void innerWrite(const T& val)
		{
			memcpy(m_buffer + m_pos, &val, sizeof(T));
			m_pos += sizeof(T);
		}
	};

	void MyInvoke(const void* pThis, const void* pFunc, const void* pArgs, unsigned int len)
	{
		int retVal = 0;
		DWORD nsp = 0;
		__asm
		{
			//push ecx;
			push esi;
			push edi;
			mov nsp, esp;
			sub esp, len;

			mov ecx, len;
			test ecx, ecx;
			jz CALLFUNC;

			mov esi, pArgs;
			mov edi, esp;
			rep movsb;

		CALLFUNC:
			mov ecx, pThis;
			call[pFunc];
			mov retVal, eax;

			mov esp, nsp;
			pop edi;
			pop esi;
			//pop ecx;
		}
	}


	namespace
	{
		int normalFuncion(int i, short j)
		{
			std::cout << "normalFuncion" << std::endl;
			return i + j;
		}

		class Base
		{
		public:
			int memberFunction1(int i, double j)
			{
				std::cout << "Base::memberFunction1" << std::endl;
				return i + j + m;
			}
		private:
			int m = 1;
		};

		typedef int(Base::*PtrMember1)(int, double);
		union MyUnion
		{
			void*		pVoid;
			PtrMember1	pMember1;
		};
	}

	void testMember10()
	{
		Base base;
		Buffer buffer;
		buffer.writeInt32(1);
		buffer.writeDouble64(0.5);

		unsigned int pFunc;
		Get_Func_Addr(Base::memberFunction1, pFunc);
		MyInvoke((void*)(&base), reinterpret_cast<void*>(pFunc), buffer.get(), buffer.len());
	}

	void testMember11()
	{
		Base base;
		Buffer buffer;
		buffer.writeInt32(1);
		buffer.writeDouble64(1.0);

		const void* pFunc = nullptr;
		{
			MyUnion my; my.pMember1 = (&Base::memberFunction1);
			pFunc = my.pVoid;
		}

		MyInvoke(&base, pFunc, buffer.get(), buffer.len());
	}

	void test()
	{
		testMember10();
		testMember11();
	}
}