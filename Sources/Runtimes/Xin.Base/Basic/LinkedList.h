#pragma once

#include "Basic.Types.h"

namespace Xin
{
	template<typename T, typename AllocatorT = std::allocator<T>>
	class TLinkedList : protected std::list<T, AllocatorT>
	{
		using AllocatorType = TDefaultAllocator<T>;
		using ParentType = std::list<T, AllocatorT>;
		using Reference = typename ParentType::reference;
		using ConstReference = typename ParentType::const_reference;
		using Iterator = typename ParentType::iterator;
		using ConstIterator = typename ParentType::const_iterator;
		
	public: // Add
		Reference Back() { return ParentType::back(); }
		ConstReference Back() const { return ParentType::back(); }
		Reference Front() { return ParentType::front(); }
		ConstReference Front() const { return ParentType::front(); }
		void PopFront() { ParentType::pop_front(); }
		void PopBack() { ParentType::pop_back(); }
		
		void AddFront(T && Value)
		{
			ParentType::push_front(Forward<T>(Value));
		}

		void AddFront(const T & Value)
		{
			ParentType::push_front(Value);
		}
		
		void AddBack(T && Value)
		{
			ParentType::push_back(Forward<T>(Value));
		}

		void AddBack(const T & Value)
		{
			ParentType::push_back(Value);
		}

		template<typename ...ArgsT>
		Reference EmplaceFront(ArgsT && ...Args)
		{
			return ParentType::emplace_front(Forward<ArgsT>(Args)...);
		}

		template<typename ...ArgsT>
		Reference EmplaceBack(ArgsT && ...Args)
		{
			return ParentType::emplace_back(Forward<ArgsT>(Args)...);
		}

		template<typename ...ArgsT>
		Reference Emplace(ConstIterator Where, ArgsT && ...Args)
		{
			return ParentType::emplace(Where, Forward<ArgsT>(Args)...);
		}

		Iterator Erase(ConstIterator Where)
		{
			return ParentType::erase(Where);
		}

		Iterator Erase(ConstIterator First, ConstIterator Last)
		{
			return ParentType::erase(First, Last);
		}
	
	public: // Insert
		Iterator Insert(ConstIterator Where, T && Value)
		{
			return ParentType::insert(Where, Forward<T>(Value));
		}
		Iterator Insert(ConstIterator Where, const T & Value)
		{
			return ParentType::insert(Where, Forward<T>(Value));
		}
		Iterator Insert(ConstIterator Where, uintx Count, const T & Value)
		{
			return ParentType::insert(Where, Count, Forward<T>(Value));
		}

		Iterator Insert(ConstIterator Where, TInitializerList<T> InitializerList)
		{
			return ParentType::insert(Where, InitializerList);
		}

		auto Swap(TLinkedList<T> & Another) noexcept
		{
			ParentType::swap(Another);
		}

	public:
		void Clear() { ParentType::clear(); }

	
	public:
		uintx Size() const { return ParentType::size(); }
		Iterator Begin() { return ParentType::begin(); }
		ConstIterator Begin() const { return ParentType::begin(); }
		Iterator End() { return ParentType::end(); }
		ConstIterator End() const { return ParentType::end(); }
	};



	template<typename T>
	void Swap(TLinkedList<T> & Left, TLinkedList<T> & Right) noexcept
	{
		Left.Swap(Right);
	}
}

namespace std
{
	template<typename T>
	auto begin(Xin::TLinkedList<T> & Vector)
	{
		return Vector.Begin();
	}

	template<typename T>
	auto end(Xin::TLinkedList<T> & Vector)
	{
		return Vector.End();
	}
	
	template<typename T>
	auto begin(const Xin::TLinkedList<T> & Vector)
	{
		return Vector.Begin();
	}

	template<typename T>
	auto end(const Xin::TLinkedList<T> & Vector)
	{
		return Vector.End();
	}
}
