#pragma once
#include <vector>
#include <assert.h>

namespace invoke
{
	typedef unsigned long DWORD;
	typedef unsigned char byte;

	//edi esi ebx ebp esp
#define Get_Func_Addr(FUNCTION, OUTPTR)  \
		__asm{ mov eax, FUNCTION } \
		__asm{ mov OUTPTR, eax} \

	class Buffer
	{
	private:
		byte* m_buffer;
		size_t m_pos;

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
		size_t len() { return m_pos; }

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

	void MyInvoke(const void* pThis, const void* pFunc, const void* pArgs, size_t len)
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


	//----------------------------------------------------------------------------
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

		void testMember1()
		{
			Base base;
			Buffer buffer;
			buffer.writeInt32(1);
			buffer.writeDouble64(0.5);


			//���̳� ��Ա����
			//mov	eax, offset Base::memberFunction1 (0E38AC0h)  
			//mov	dword ptr[pFunc], eax
			const void* pFunc1;
			Get_Func_Addr(Base::memberFunction1, pFunc1);
			assert(4 == sizeof(&Base::memberFunction1));

			{
				using PtrMemberBase = int(Base::*)(int, double);
				union MyUnion
				{
					void* pVoid;
					PtrMemberBase pMember1;
				};

				const void* pFunc2 = nullptr;
				MyUnion my; my.pMember1 = (&Base::memberFunction1);
				pFunc2 = my.pVoid;
				assert(pFunc1 == pFunc2);
			}

			MyInvoke((void*)(&base), pFunc1, buffer.get(), buffer.len());
		}

	}

	//----------------------------------------------------------------------------
	namespace
	{
		class BaseA
		{
		public:
			virtual void func1() = 0;
			int mA = 1;
			int mA2 = 2;
		};

		class BaseA2
		{
		public:
			virtual void func2() = 0;
			virtual void func22() = 0;
			int mA = 3;
			int mA2 = 4;
		};

		class DeriveB : public BaseA
		{
		public:
			void func1() override
			{
				assert(mB == 5);
				printf("class DeriveB func1\n");
			}
			int mB = 5;
			int mB2 = 6;
		};

		class DeriveC : public DeriveB,
			public BaseA2
		{
		public:
			void func1() override
			{
				assert(mC == 7);
				printf("class DeriveC func1\n");
			}
			void func2() override
			{
				assert(mC == 7);
				printf("class DeriveC func2\n");
			}
			void func22() override
			{
				assert(mC == 7);
				printf("class DeriveC func22\n");
			}
			int mC = 7;
			int mC2 = 8;
		};

		using PtrMemberC = void(DeriveC::*)();
		void innerTestNothing(PtrMemberC ptr)
		{
			//nothing
		}

		void testMemberPtr()
		{
			//��̳г�Ա����ָ��8�ֽڣ�����������ʵ�֡�����������̳У�����ı䣬�����ݲ����ۡ�
			//https://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible
			//4�ֽ�ptr��4�ֽ���������thisƫ����������ƫ����Ϊ0
			assert(8 == sizeof(&DeriveC::func1));
			innerTestNothing(&DeriveC::func1);
			// 			mov         dword ptr[ebp - 0CCh], 0D76A11h
			// 			mov         dword ptr[ebp - 0C8h], 0
			// 			mov         eax, dword ptr[ebp - 0C8h]
			// 			push        eax
			// 			mov         ecx, dword ptr[ebp - 0CCh]
			// 			push        ecx
			// 			call        invoke::`anonymous namespace'::innerTestNothing (0D786E0h)  


			//4�ֽ�ptr��4�ֽ���������thisƫ����������ƫ����Ϊ14h,��20���ֽ�
			assert(20 == sizeof(DeriveB)); //vptr+4*int = 4+4*4
			innerTestNothing(&DeriveC::func2);
			// 			mov         dword ptr[ebp - 0DCh], 26A11h
			// 			mov         dword ptr[ebp - 0D8h], 14h
			// 			mov         eax, dword ptr[ebp - 0D8h]
			// 			push        eax
			// 			mov         ecx, dword ptr[ebp - 0DCh]
			// 			push        ecx
			// 			call        invoke::`anonymous namespace'::innerTestNothing (0286E0h)  
		}



		template<typename T1, typename T2>
		constexpr size_t GetOffset()
		{
			T1* ptrSrc = (T1*)0x00000001;
			T2* ptrDest = static_cast<T1*>(ptrSrc);
			return reinterpret_cast<uintptr_t>(ptrDest) - reinterpret_cast<uintptr_t>(ptrSrc);
		}

		void testMember2()
		{
			//����testMemberPtr����ʾһ�¶�̳�ʱ����֪��������׵�ַ�ͼ̳нṹʱ����������thisƫ���������ó�Ա������
			std::unique_ptr<BaseA> spBaseA(new DeriveC());
			const void* pFunc1;
			Get_Func_Addr(DeriveC::func2, pFunc1);

			//MyInvoke(spBaseA.get(), pFunc1, nullptr, 0); //wrong

			size_t thisOffset = GetOffset<DeriveC, BaseA2>();
			assert(thisOffset == 20 && sizeof(DeriveB) == 20);
			const void* pThisAdjusted = (char*)(spBaseA.get()) + thisOffset;
			MyInvoke(pThisAdjusted, pFunc1, nullptr, 0); //right
		}

		void testMember3()
		{
			//����testMember2�����ȡ�����麯����ַ�ɲ������أ��ǿ��Եġ�ֻҪ�ṩ��ȷ��thisָ�룬Ȼ����Զ����麯����
			std::unique_ptr<BaseA> spBaseA(new DeriveC());

			const void* pFunc2 = nullptr;
			const void* pFunc22 = nullptr;
			{
				using PtrMemberBase = void(BaseA2::*)();
				union MyUnion
				{
					void* pVoid;
					PtrMemberBase pMember1;
				};
				MyUnion my;
				my.pMember1 = (&BaseA2::func2);
				pFunc2 = my.pVoid;
				my.pMember1 = (&BaseA2::func22);
				pFunc22 = my.pVoid;
			}

			size_t thisOffset = GetOffset<DeriveC, BaseA2>();
			assert(thisOffset == 20 && sizeof(DeriveB) == 20);
			const void* pThisAdjusted = (char*)(spBaseA.get()) + thisOffset;

			//�����ͨ�����Ǽ����ƫ�Ƶ�thisָ��(ecx��ʱΪthisָ��)(Ҳ��BaseA2*ָ��)�����麯������һ��ָ��(BaseA2::func2)��ƫ��0
			MyInvoke(pThisAdjusted, pFunc2, nullptr, 0);
// 			mov         eax, dword ptr[ecx]	//��ʱeaxΪ�麯�����׵�ַ��pVTable[0]
// 			jmp         dword ptr[eax]		//ȡ*(pVTable[0])


			//���麯�����ڶ���ָ��(BaseA2::func22)��ƫ��4���ֽ�
			MyInvoke(pThisAdjusted, pFunc22, nullptr, 0);
// 			mov         eax, dword ptr[ecx]	//��ʱeaxΪ�麯�����׵�ַ��pVTable[0]
// 			jmp         dword ptr[eax + 4]	//ȡ*(pVTable[1])
		}
	}


	//----------------------------------------------------------------------------
	void test()
	{
		testMember1();
		testMemberPtr();
		testMember2();
		testMember3();
	}
}