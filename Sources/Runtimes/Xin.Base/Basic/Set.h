#pragma once

#include "Basic.Types.h"
#include "Pair.h"

namespace Xin
{
	template<typename KeyT, typename PredictT = std::less<KeyT>, typename AllocatorT = std::allocator<KeyT>>
	class TSet : protected std::set<KeyT, PredictT, AllocatorT>
	{
	public:
		using ParentType = std::set<KeyT, PredictT, AllocatorT>;

		using AllocatorType = typename ParentType::allocator_type;
		using SizeType = typename ParentType::size_type;
		using DifferenceType = typename ParentType::difference_type;
		using PointerType = typename ParentType::pointer;
		using ConstPointerType = typename ParentType::const_pointer;

		using Iterator = typename ParentType::iterator;
		using ConstIterator = typename ParentType::const_iterator;
		using ReverseIterator = typename ParentType::reverse_iterator;
		using ConstReverseIterator = typename ParentType::const_reverse_iterator;

	public:
		TSet() = default;
		TSet(const TSet &) = default;
		TSet(TSet &&) noexcept = default;
		TSet & operator =(const TSet &) = default;
		TSet & operator =(TSet &&) noexcept = default;

		TPair<Iterator, bool> Insert(const KeyT & Key)
		{
			auto Pair = ParentType::insert(Key);
			return { Pair.first, Pair.second };
		}

		TPair<Iterator, bool> Insert(KeyT && Key)
		{
			auto Pair = ParentType::insert(Key);
			return { Pair.first, Pair.second };
		}

		Iterator Add(const KeyT & Key) { return Insert(Key).First; }
		Iterator Add(KeyT && Key) { return Insert(Key).First; }

		Iterator Erase(ConstIterator Iter)
		{
			return ParentType::erase(Iter);
		}

		void Erase(const KeyT & Key)
		{
			ParentType::erase(Key);
		}

		bool Contains(const KeyT & Key) const
		{
			return ParentType::contains(Key);
		}

		Iterator Find(const KeyT & Key)
		{
			return ParentType::find(Key);
		}

		ConstIterator Find(const KeyT & Key) const
		{
			return ParentType::find(Key);
		}

		uintx Size() const { return ParentType::size(); }
		bool IsEmpty() const { return ParentType::empty(); }

	public:
		Iterator Begin() noexcept { return ParentType::begin(); }
		Iterator End() noexcept { return ParentType::end(); }
		ConstIterator Begin() const noexcept { return ParentType::begin(); }
		ConstIterator End() const noexcept { return ParentType::end(); }

		Iterator begin() noexcept { return ParentType::begin(); }
		Iterator end() noexcept { return ParentType::end(); }
		ConstIterator begin() const noexcept { return ParentType::begin(); }
		ConstIterator end() const noexcept { return ParentType::end(); }
	};
}

namespace std
{
	template<typename KeyT, typename PredictT = std::less<KeyT>, typename AllocatorT = std::allocator<KeyT>>
	auto begin(Xin::TSet<KeyT, PredictT, AllocatorT> & Set) noexcept
	{
		return Set.Begin();
	}

	template<typename KeyT, typename PredictT = std::less<KeyT>, typename AllocatorT = std::allocator<KeyT>>
	auto end(Xin::TSet<KeyT, PredictT, AllocatorT> & Set) noexcept
	{
		return Set.End();
	}

	template<typename KeyT, typename PredictT = std::less<KeyT>, typename AllocatorT = std::allocator<KeyT>>
	auto begin(const Xin::TSet<KeyT, PredictT, AllocatorT> & Set) noexcept
	{
		return Set.Begin();
	}

	template<typename KeyT, typename PredictT = std::less<KeyT>, typename AllocatorT = std::allocator<KeyT>>
	auto end(const Xin::TSet<KeyT, PredictT, AllocatorT> & Set) noexcept
	{
		return Set.End();
	}
}
