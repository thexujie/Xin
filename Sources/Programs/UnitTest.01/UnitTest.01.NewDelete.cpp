#include "pch.h"
#include "CppUnitTest.h"
#include <iostream>

#include "../../Runtimes/Xin.Core/Xin.Core.h"

using namespace Xin;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

class __Test_NewDelete_Allocator
{

};

void * operator new(sizet Size, __Test_NewDelete_Allocator & UnkownBase)
{
	return Malloc(Size/*, static_cast<sizet>(Alignment)*/);
}

void * operator new(sizet Size, std::align_val_t Alignment, __Test_NewDelete_Allocator & UnkownBase)
{
	return Malloc(Size, static_cast<sizet>(Alignment));
}


template<typename ...ParametersT>
void * operator new(sizet Size, ParametersT && ...Parameters)
{
	return Malloc(Size/*, static_cast<sizet>(Alignment)*/);
}

template<typename ...ParametersT>
void * operator new(sizet Size, std::align_val_t alignment, ParametersT && ...Parameters)
{
	return Malloc(Size/*, static_cast<sizet>(Alignment)*/);
}

template<typename ...ParametersT>
void operator delete(void *, ParametersT && ...Parameters)
{
	std::cout << "void operator delete(void *, ParametersT && ...Parameters)" << std::endl;
}

template<typename ...ParametersT>
void operator delete(void *, std::align_val_t alignment, ParametersT && ...Parameters)
{
	std::cout << " void operator(void *, std::align_val_t alignment, ParametersT && ...Parameters)" << std::endl;
}

void operator delete(void *, const std::nothrow_t & Tag)
{
	std::cout << "void operator delete(void *, const std::nothrow_t & Tag)" << std::endl;
}

void operator delete(void *, std::align_val_t alignment, const std::nothrow_t & Tag) throw()
{
	std::cout << "void operator delete(void *, std::align_val_t alignment, const std::nothrow_t & Tag) throw()" << std::endl;
}

void operator delete(void *, std::align_val_t alignment, __Test_NewDelete_Allocator & UnkownBase)
{
	std::cout << "void operator delete(void *, std::align_val_t alignment, __Test_NewDelete_Allocator & UnkownBase)) throw()" << std::endl;
}
namespace UnitTest01
{

	TEST_CLASS(UnitTest01_NewDelete)
	{
		struct alignas(256) FAlignedClass : public IObject
		{
			FAlignedClass() { throw std::runtime_error(""); }
			int fff[12];
		};

		struct FClass : public IObject
		{
			FClass()
			{
				//throw std::runtime_error("");
			}

			FClass(FObjectEntry * Entry)
			{
				
			}

			void* operator new ( std::size_t count, void* ptr )
			{
				return ::operator new(count, ptr);
			}

			template<typename ...ParametersT>
			void * operator new(std::size_t Size, ParametersT && ...Parameters)
			{
				FObjectEntry * ObjectEntry = nullptr;
				std::cout << " void operator(void *, std::align_val_t alignment, ParametersT && ...Parameters)" << std::endl;

				return ::operator new(count, ObjectEntry, std::forward<ParametersT>(Parameters)...);
			}
		};

	public:
		TEST_METHOD(Test_NewDelete)
		{
			try
			{
				FClass * p0 = new FClass;
				FClass * p1 = new(true) FClass;
				FClass * p2 = new(12) FClass;
			}
			catch (const std::exception &) { }

			__Test_NewDelete_Allocator UnkownBase;
			char Memory[__STDCPP_DEFAULT_NEW_ALIGNMENT__];
			try
			{
				auto * object = new(std::nothrow, UnkownBase) FAlignedClass();
			}
			catch (const std::exception &) { }
		}
	};
}
