#pragma once

#include "Basic.Types.h"
#include "Pair.h"
#include "Map.h"

namespace Xin
{
	template<typename KeyT, typename MappedT, typename PredicateT = std::less<KeyT>, typename AllocatorT = std::allocator<TPair<const KeyT, MappedT>>>
	class TMultiMap : protected std::_Tree<TMapTraits<KeyT, MappedT, PredicateT, AllocatorT, true>>
	{
		// ordered red-black tree of {key, mapped} values, non-unique keys
	public:
		using ParentType = std::_Tree<TMapTraits<KeyT, MappedT, PredicateT, AllocatorT, true>>;
		using KeyType = KeyT;
		using mapped_type = MappedT;
		using KeyCompareType = PredicateT;
		using ValueCompareType = typename ParentType::value_compare;
		using PairType = TPair<const KeyT, MappedT>;
		using AllocatorType = typename ParentType::allocator_type;
		using SizeType = typename ParentType::size_type;
		using DifferenceType = typename ParentType::difference_type;
		using PointerType = typename ParentType::pointer;
		using ConstPointerType = typename ParentType::const_pointer;
		using PairReferenceType = PairType&;
		using ConstPairReferenceType = const PairType&;
		using Iterator = typename ParentType::iterator;
		using ConstIterator = typename ParentType::const_iterator;
		using ReverseIterator = typename ParentType::reverse_iterator;
		using ConstReverseIterator = typename ParentType::const_reverse_iterator;

		using _Alnode = typename ParentType::_Alnode;
		using _AlnodeTraits = typename ParentType::_Alnode_traits;

	public:
		TMultiMap() : ParentType(KeyCompareType()) {}

		explicit TMultiMap(const AllocatorType & Allcator) : ParentType(KeyCompareType(), Allcator) {}

		TMultiMap(const TMultiMap & That)
			: ParentType(That, _AlnodeTraits::select_on_container_copy_construction(That._Getal())) {}

		TMultiMap(const TMultiMap & That, const AllocatorType & Allcator) : ParentType(That, Allcator) {}

		explicit TMultiMap(const KeyCompareType & Predictor) : ParentType(Predictor) {}

		TMultiMap(const KeyCompareType & Predictor, const AllocatorType & Allcator) : ParentType(Predictor, Allcator) {}

		template<class _Iter>
		TMultiMap(_Iter First, _Iter Last) : ParentType(KeyCompareType())
		{
			insert(First, Last);
		}

		template<class _Iter>
		TMultiMap(_Iter First, _Iter Last, const KeyCompareType & Predictor) : ParentType(Predictor)
		{
			insert(First, Last);
		}

		template<class _Iter>
		TMultiMap(_Iter First, _Iter Last, const AllocatorType & Allcator) : ParentType(KeyCompareType(), Allcator)
		{
			insert(First, Last);
		}

		template<class _Iter>
		TMultiMap(_Iter First, _Iter Last, const KeyCompareType & Predictor, const AllocatorType & Allcator) : ParentType(Predictor, Allcator)
		{
			insert(First, Last);
		}

		TMultiMap & operator =(const TMultiMap & That)
		{
			ParentType::operator =(That);
			return *this;
		}

		TMultiMap(TMultiMap && That) noexcept : ParentType(Move(That)) {}

		TMultiMap(TMultiMap && That, const AllocatorType & Allcator) : ParentType(Move(That), Allcator) {}

		TMultiMap & operator =(TMultiMap && That) noexcept(_AlnodeTraits::is_always_equal::value && IsNoThrowMoveAssignableV<PredicateT>)
		{
			ParentType::operator =(Move(That));
			return *this;
		}

	public: // Emplace
		template<typename... ValueTs>
		Iterator Emplace(ValueTs &&... Values)
		{
			return ParentType::emplace(Forward<ValueTs>(Values)...).first;
		}

		void Swap(TMultiMap & That) noexcept(noexcept(ParentType::swap(That)))
		{
			ParentType::swap(That);
		}

	public: // Insert
		template<typename IteratorT>
		void Insert(IteratorT First, IteratorT Last)
		{
			ParentType::template insert<IteratorT>(First, Last);
		}

		Iterator Insert(const PairType & Pair)
		{
			return Iterator(ParentType::_Emplace(Pair).first, ParentType::_Get_scary());
		}

		Iterator Insert(PairType && Pair)
		{
			return Iterator(ParentType::_Emplace(Move(Pair)).first, ParentType::_Get_scary());
		}

		template<typename Value2T, EnableIfT<IsConstructibleV<PairType, Value2T>>>
		Iterator Insert(Value2T && Value)
		{
			return this->emplace(Forward<Value2T>(Value));
		}

		template<typename Value2T, EnableIfT<IsConstructibleV<PairType, Value2T>>>
		Iterator Insert(ConstIterator Where, Value2T && Value)
		{
			return this->emplace_hint(Where, Forward<Value2T>(Value));
		}

	public: // EqualRange
		TPair<Iterator, Iterator> EqualRange(const KeyType & KeyValue)
		{
			const auto _Result = ParentType::_Eqrange(KeyValue);
			const auto _Scary = ParentType::_Get_scary();
			return { Iterator(_Result.first, _Scary), Iterator(_Result.second, _Scary) };
		}

		TPair<ConstIterator, ConstIterator> EqualRange(const KeyType & KeyValue) const
		{
			const auto _Result = ParentType::_Eqrange(KeyValue);
			const auto _Scary = ParentType::_Get_scary();
			return { ConstIterator(_Result.first, _Scary), ConstIterator(_Result.second, _Scary) };
		}

		template<class _Other, class _Mycomp = KeyCompareType, class = typename _Mycomp::is_transparent>
		TPair<Iterator, Iterator> EqualRange(const _Other & KeyValue)
		{
			const auto _Result = ParentType::_Eqrange(KeyValue);
			const auto _Scary = ParentType::_Get_scary();
			return { Iterator(_Result.first, _Scary), Iterator(_Result.second, _Scary) };
		}

		template<class OtherKeyT, class _Mycomp = KeyCompareType, class = typename _Mycomp::is_transparent>
		TPair<ConstIterator, ConstIterator> EqualRange(const OtherKeyT & KeyValue) const
		{
			const auto _Result = ParentType::_Eqrange(KeyValue);
			const auto _Scary = ParentType::_Get_scary();
			return { ConstIterator(_Result.first, _Scary), ConstIterator(_Result.second, _Scary) };
		}

	public:
		TMultiMap(TInitializerList<PairType> InitializerList) : ParentType(KeyCompareType())
		{
			insert(InitializerList);
		}

		TMultiMap(TInitializerList<PairType> InitializerList, const KeyCompareType & Predictor) : ParentType(Predictor)
		{
			insert(InitializerList);
		}

		TMultiMap(TInitializerList<PairType> InitializerList, const AllocatorType & Allcator) : ParentType(KeyCompareType(), Allcator)
		{
			insert(InitializerList);
		}

		TMultiMap(TInitializerList<PairType> InitializerList, const KeyCompareType & Predictor, const AllocatorType & Allcator)
			: ParentType(Predictor, Allcator)
		{
			insert(InitializerList);
		}

		TMultiMap & operator =(TInitializerList<PairType> InitializerList)
		{
			ParentType::clear();
			insert(InitializerList);
			return *this;
		}

	public: // Find
		Iterator Find(const KeyT & Key) { return ParentType::find(Key); }
		ConstIterator Find(const KeyT & Key) const { return ParentType::find(Key); }

	public:
		Iterator Erase(ConstIterator Where) { return ParentType::erase(Where); }
		Iterator Erase(ConstIterator First, ConstIterator Last) { return ParentType::erase(First, Last); }
		Iterator Erase(const KeyT & Key) { return ParentType::erase(Key); }

	public: // Range Iterators
		Iterator Begin() { return ParentType::begin(); }
		Iterator End() { return ParentType::end(); }
		ConstIterator Begin() const { return ParentType::begin(); }
		ConstIterator End() const { return ParentType::end(); }

	public: // Misc
		bool Contains(const KeyT & Key) const { return ParentType::contains(Key); }
		uintx GetSize() const { return ParentType::_Get_scary()->_Mysize; }
		void Clear() noexcept { ParentType::clear(); }

	public:
		using ParentType::_Unchecked_begin;
		using ParentType::_Unchecked_end;
	};

	////template<class _Iter, class _Pr = less<_Guide_key_t<_Iter>>, class _Alloc = allocator<_Guide_pair_t<_Iter>>,
	////		EnableIfT<conjunction_v<_Is_iterator<_Iter>, negation<_Is_allocator<_Pr>>, _Is_allocator<_Alloc>>, int>  = 0>
	////TMultiMap(_Iter, _Iter, _Pr = _Pr(), _Alloc = _Alloc())
	////-> TMultiMap<_Guide_key_t<_Iter>, _Guide_val_t<_Iter>, _Pr, _Alloc>;

	////template<class _Kty, class _Ty, class _Pr = less<_Kty>, class _Alloc = allocator<pair<const _Kty, _Ty>>,
	////		EnableIfT<conjunction_v<negation<_Is_allocator<_Pr>>, _Is_allocator<_Alloc>>, int>  = 0>
	////TMultiMap(TInitializerList<pair<_Kty, _Ty>>, _Pr = _Pr(), _Alloc = _Alloc())
	////->
	////TMultiMap<_Kty, _Ty, _Pr, _Alloc>;

	////template<class _Iter, class _Alloc, EnableIfT<conjunction_v<_Is_iterator<_Iter>, _Is_allocator<_Alloc>>, int>  = 0>
	////TMultiMap(_Iter, _Iter, _Alloc) -> TMultiMap<_Guide_key_t<_Iter>, _Guide_val_t<_Iter>, less<_Guide_key_t<_Iter>>, _Alloc>;

	////template<class _Kty, class _Ty, class _Alloc, EnableIfT<_Is_allocator<_Alloc>::value, int>  = 0>
	////TMultiMap(TInitializerList<pair<_Kty, _Ty>>, _Alloc)
	////->
	////TMultiMap<_Kty, _Ty, less<_Kty>, _Alloc>;

	template<class KeyT, class MappedT, class PredicateT, class AllocatorT>
	bool operator ==(const TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Left, const TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Right)
	{
		return Left.GetSize() == Right.GetSize() && _STD equal(Left._Unchecked_begin(), Left._Unchecked_end_iter(), Right._Unchecked_begin());
	}

	template<class KeyT, class MappedT, class PredicateT, class AllocatorT>
	bool operator !=(const TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Left, const TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Right)
	{
		return !(Left == Right);
	}

	template<class KeyT, class MappedT, class PredicateT, class AllocatorT>
	bool operator <(const TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Left, const TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Right)
	{
		return _STD lexicographical_compare(
			Left._Unchecked_begin(), Left._Unchecked_end_iter(), Right._Unchecked_begin(), Right._Unchecked_end_iter());
	}

	template<class KeyT, class MappedT, class PredicateT, class AllocatorT>
	bool operator >(const TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Left, const TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Right)
	{
		return Right < Left;
	}

	template<class KeyT, class MappedT, class PredicateT, class AllocatorT>
	bool operator <=(const TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Left, const TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Right)
	{
		return !(Right < Left);
	}

	template<class KeyT, class MappedT, class PredicateT, class AllocatorT>
	bool operator >=(const TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Left, const TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Right)
	{
		return !(Left < Right);
	}

	template<class KeyT, class MappedT, class PredicateT, class AllocatorT>
	void Swap(TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Left, TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Right) noexcept(noexcept(Left.Swap(Right)))
	{
		Left.Swap(Right);
	}

	template<class KeyT, class MappedT, class PredicateT, class AllocatorT, class ErasePredicateT>
	typename TMultiMap<KeyT, MappedT, PredicateT, AllocatorT>::SizeType EraseIf(TMultiMap<KeyT, MappedT, PredicateT, AllocatorT> & Container, ErasePredicateT Predictor)
	{
		return std::_Erase_nodes_if(Container, std::_Pass_fn(Predictor));
	}
}
