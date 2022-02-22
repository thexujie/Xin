#include "pch.h"
#include "CppUnitTest.h"
#include <iostream>

#include "../../Runtimes/Xin.Core/Xin.Core.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest01
{
	TEST_CLASS(UnitTest01_Initialize)
	{
		struct A
		{
			int i;
			A()
			{

			} // 用户提供的默认构造函数，不初始化 i
		};

		struct B
		{
			A a;
		}; // 隐式定义的默认构造函数

		struct Ccc
		{
			int Ccc_c = 1;
			//Ccc()
			//{
			//	Ccc_c = 12;
			//}

		};

		struct C
		{
			int a;
			float b;
			double c;
			char d;
			short e;
			std::string f;
			Ccc ccc;
		};

		C f() {
			return C();
		}

	public:
		TEST_METHOD(Test_Initialize)
		{
			int * pia = new int[10];
			int * pib = new int[10]();
			C * pc = new C();
			C c1;
			C c2 = C();
			C c3 = C(f());

			std::cout << "c1.a=" << c1.a << std::endl; //C().a=???
			std::cout << "C().a=" << C().a << std::endl; //C().a=0
			std::cout << "pc->a=" << pc->a << std::endl; //C().a=0

			static A sa;
			static B sb;
			static C sc;
			B b = B();
			B * pb = new B();
			char buffer[32];
			B * pbb = new (buffer)B();

			std::cout << "sa.i=" << sa.i << std::endl; //sa.i=0
			std::cout << "sb.a.i=" << sb.a.i << std::endl; //sb.i=0
			std::cout << "A().i=" << A().i << std::endl; //A().i=???
			std::cout << "B().a.i=" << B().a.i << std::endl; //B().a.i=0
			std::cout << "b.a.i=" << b.a.i << std::endl; // b.a.i = 0
			std::cout << "pb->a.i=" << pb->a.i << std::endl; //pb->a.i=0
			std::cout << "pbb->a.i=" << pbb->a.i << std::endl; //pbb->a.i=0
		}
	};
}
