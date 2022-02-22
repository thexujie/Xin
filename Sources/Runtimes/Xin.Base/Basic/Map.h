#pragma once

#include "Basic.Types.h"
#include "Pair.h"

namespace Xin
{
	template<typename KeyT, typename T, typename HashT = THash<KeyT>, typename KeyEqualT = std::equal_to<KeyT>, typename AllocatorT = std::allocator<std::pair<const KeyT, T>>>
	using TUnorderedMap = std::unordered_map<KeyT, T, HashT, KeyEqualT, AllocatorT>;

	template<typename KeyT, typename HashT = std::hash<KeyT>, typename KeyEqualT = std::equal_to<KeyT>, typename AllocatorT = std::allocator<KeyT>>
	using TUnorderedSet = std::unordered_set<KeyT, HashT, KeyEqualT, AllocatorT>;

	template<typename KeyT, typename MappedT, typename PredicateT, typename AllocatorT, bool Multi>
	class TMapTraits
	{
	public:
		using key_type = KeyT;
		using value_type = TPair<const KeyT, MappedT>;
		using key_compare = PredicateT;
		using allocator_type = AllocatorT;
		using node_type = std::_Node_handle<std::_Tree_node<value_type, typename std::allocator_traits<AllocatorT>::void_pointer>, AllocatorT, std::_Node_handle_map_base, KeyT, MappedT>;

		static constexpr bool _Multi = Multi;

		template<typename... _Args>
		using _In_place_key_extractor = std::_In_place_key_extract_map<KeyT, _Args...>;

		class value_compare
		{
		public:
			bool operator ()(const value_type & _Left, const value_type & _Right) const
			{
				return Compare(_Left.First, _Right.First);
			}

		protected:
			friend std::_Tree<TMapTraits>;

			value_compare(key_compare _Pred) : Compare(_Pred) {}

			key_compare Compare;
		};

		template<typename _Ty1, typename _Ty2>
		static const KeyT & _Kfn(const TPair<_Ty1, _Ty2> & _Val)
		{
			// extract key from element value
			return _Val.First;
		}
	};

	template<typename KeyT, typename MappedT, typename PredictT = std::less<KeyT>, typename AllocatorT = std::allocator<TPair<const KeyT, MappedT>>>
	class TMap : protected std::_Tree<TMapTraits<KeyT, MappedT, PredictT, AllocatorT, false>>
	{
	public:
		using ParentType = std::_Tree<TMapTraits<KeyT, MappedT, PredictT, AllocatorT, false>>;
		using _Nodeptr = typename ParentType::_Nodeptr;

		using KeyType = KeyT;
		using MappedType = MappedT;
		using KeyCompare = PredictT;
		using ValueCompare = typename ParentType::value_compare;
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

		using insert_return_type = std::_Insert_return_type<Iterator, typename ParentType::node_type>;

	public:
		TMap() : ParentType(KeyCompare()) {}

		explicit TMap(const AllocatorType & _Al) : ParentType(KeyCompare(), _Al) {}

		TMap(const TMap & _Right) : ParentType(_Right, _AlnodeTraits::select_on_container_copy_construction(_Right._Getal())) {}

		TMap(const TMap & _Right, const AllocatorType & _Al) : ParentType(_Right, _Al) {}

		explicit TMap(const KeyCompare & _Pred) : ParentType(_Pred) {}

		TMap(const KeyCompare & _Pred, const AllocatorType & _Al) : ParentType(_Pred, _Al) {}

		template<typename IteratorT>
		TMap(IteratorT _First, IteratorT _Last) : ParentType(KeyCompare())
		{
			insert(_First, _Last);
		}

		template<typename IteratorT>
		TMap(IteratorT _First, IteratorT _Last, const KeyCompare & _Pred) : ParentType(_Pred)
		{
			insert(_First, _Last);
		}

		template<typename IteratorT>
		TMap(IteratorT _First, IteratorT _Last, const AllocatorType & _Al) : ParentType(KeyCompare(), _Al)
		{
			insert(_First, _Last);
		}

		template<typename IteratorT>
		TMap(IteratorT _First, IteratorT _Last, const KeyCompare & _Pred, const AllocatorType & _Al) : ParentType(_Pred, _Al)
		{
			insert(_First, _Last);
		}

		TMap & operator =(const TMap & _Right)
		{
			ParentType::operator =(_Right);
			return *this;
		}

		TMap(TMap && _Right) noexcept : ParentType(Move(_Right)) {}

		TMap(TMap && _Right, const AllocatorType & _Al) : ParentType(Move(_Right), _Al) {}

		TMap & operator =(TMap && _Right) noexcept(_AlnodeTraits::is_always_equal::value && IsNoThrowMoveAssignableV<PredictT>)
		{
			ParentType::operator =(Move(_Right));
			return *this;
		}

		MappedType & operator [](KeyType && _Keyval)
		{
			// find element matching _Keyval or insert value-initialized value
			return _Try_emplace(Move(_Keyval)).First->_Myval.Second;
		}

		void Swap(TMap & _Right) noexcept(noexcept(ParentType::swap(_Right)))
		{
			ParentType::swap(_Right);
		}

	public: // Insert
		TPair<Iterator, bool> Insert(const PairType & Pair)
		{
			const auto _Result = ParentType::_Emplace(Pair);
			return { Iterator(_Result.first, ParentType::_Get_scary()), _Result.second };
		}

		TPair<Iterator, bool> Insert(PairType && Pair)
		{
			const auto _Result = ParentType::_Emplace(Move(Pair));
			return { Iterator(_Result.first, ParentType::_Get_scary()), _Result.second };
		}

		template<typename ValueTy> requires ConstructibleC<PairType, ValueTy&&>
		TPair<Iterator, bool> Insert(ValueTy && Value)
		{
			const auto _Result = ParentType::_Emplace(Forward<ValueTy>(Value));
			return { Iterator(_Result.first, ParentType::_Get_scary()), _Result.second };
		}

		template<typename ValueTy> requires ConstructibleC<PairType, ValueTy&&>
		Iterator Insert(ConstIterator _Where, ValueTy && Value)
		{
			return ParentType::emplace_hint(_Where, Forward<ValueTy>(Value));
		}

	private:
		template<typename _Keyty, typename... _Mappedty>
		TPair<_Nodeptr, bool> _Try_emplace(_Keyty && _Keyval, _Mappedty &&... _Mapval)
		{
			const auto _Loc = ParentType::_Find_lower_bound(_Keyval);
			if (ParentType::_Lower_bound_duplicate(_Loc._Bound, _Keyval))
			{
				return { _Loc._Bound, false };
			}

			ParentType::_Check_grow_by_1();

			const auto _Scary = ParentType::_Get_scary();
			const auto _Inserted = _STD _Tree_temp_node<_Alnode>(ParentType::_Getal(), _Scary->_Myhead, std::piecewise_construct,
					_STD forward_as_tuple(Forward<_Keyty>(_Keyval)),
					_STD forward_as_tuple(Forward<_Mappedty>(_Mapval)...))
				._Release();

			// nothrow hereafter
			return { _Scary->_Insert_node(_Loc._Location, _Inserted), true };
		}

		template<typename _Keyty, typename... _Mappedty>
		_Nodeptr _Try_emplace_hint(const _Nodeptr _Hint, _Keyty && _Keyval, _Mappedty &&... _Mapval)
		{
			const auto _Loc = ParentType::_Find_hint(_Hint, _Keyval);
			if (_Loc._Duplicate)
			{
				return _Loc._Location._Parent;
			}

			ParentType::_Check_grow_by_1();

			const auto _Scary = ParentType::_Get_scary();
			const auto _Inserted = _STD _Tree_temp_node<_Alnode>(ParentType::_Getal(), _Scary->_Myhead, std::piecewise_construct,
					_STD forward_as_tuple(Forward<_Keyty>(_Keyval)),
					_STD forward_as_tuple(Forward<_Mappedty>(_Mapval)...))
				._Release();

			// nothrow hereafter
			return _Scary->_Insert_node(_Loc._Location, _Inserted);
		}

	public:
		template<class... _Mappedty>
		TPair<Iterator, bool> TryEmplace(const KeyType & _Keyval, _Mappedty &&... _Mapval)
		{
			const auto _Result = _Try_emplace(_Keyval, Forward<_Mappedty>(_Mapval)...);
			return { Iterator(_Result.first, ParentType::_Get_scary()), _Result.second };
		}

		template<class... _Mappedty>
		Iterator TryEmplace(const ConstIterator _Hint, const KeyType & _Keyval, _Mappedty &&... _Mapval)
		{
			return Iterator(
				_Try_emplace_hint(_Hint._Ptr, _Keyval, Forward<_Mappedty>(_Mapval)...), ParentType::_Get_scary());
		}

		template<class... _Mappedty>
		TPair<Iterator, bool> TryEmplace(KeyType && _Keyval, _Mappedty &&... _Mapval)
		{
			const auto _Result = _Try_emplace(Move(_Keyval), Forward<_Mappedty>(_Mapval)...);
			return { Iterator(_Result.first, ParentType::_Get_scary()), _Result.Second };
		}

		template<class... _Mappedty>
		Iterator TryEmplace(const ConstIterator _Hint, KeyType && _Keyval, _Mappedty &&... _Mapval)
		{
			return Iterator(_Try_emplace_hint(_Hint._Ptr, Move(_Keyval), Forward<_Mappedty>(_Mapval)...),
				ParentType::_Get_scary());
		}

	private:
		template<class _Keyty, class _Mappedty>
		TPair<_Nodeptr, bool> _Insert_or_assign(_Keyty && _Keyval, _Mappedty && _Mapval)
		{
			const auto _Loc = ParentType::_Find_lower_bound(_Keyval);
			if (ParentType::_Lower_bound_duplicate(_Loc._Bound, _Keyval))
			{
				_Loc._Bound->_Myval.Second = Forward<_Mappedty>(_Mapval);
				return { _Loc._Bound, false };
			}

			ParentType::_Check_grow_by_1();

			const auto _Scary = ParentType::_Get_scary();
			const auto _Inserted = _STD _Tree_temp_node<_Alnode>(
					ParentType::_Getal(), _Scary->_Myhead, Forward<_Keyty>(_Keyval), Forward<_Mappedty>(_Mapval))
				._Release();

			// nothrow hereafter
			return { _Scary->_Insert_node(_Loc._Location, _Inserted), true };
		}

		template<class _Keyty, class _Mappedty>
		_Nodeptr _Insert_or_assign_hint(const _Nodeptr _Hint, _Keyty && _Keyval, _Mappedty && _Mapval)
		{
			const auto _Loc = ParentType::_Find_hint(_Hint, _Keyval);
			if (_Loc._Duplicate)
			{
				_Loc._Location._Parent->_Myval.second = Forward<_Mappedty>(_Mapval);
				return _Loc._Location._Parent;
			}

			ParentType::_Check_grow_by_1();

			const auto _Scary = ParentType::_Get_scary();
			const auto _Inserted = _Tree_temp_node<_Alnode>(
					ParentType::_Getal(), _Scary->_Myhead, Forward<_Keyty>(_Keyval), Forward<_Mappedty>(_Mapval))
				._Release();

			// nothrow hereafter
			return _Scary->_Insert_node(_Loc._Location, _Inserted);
		}

	public:
		template<typename MappedTy>
		TPair<Iterator, bool> InsertOrAssign(const KeyType & Keyval, MappedTy && Mapval)
		{
			const auto _Result = _Insert_or_assign(Keyval, Forward<MappedTy>(Mapval));
			return { Iterator(_Result.First, ParentType::_Get_scary()), _Result.Second };
		}

		template<typename MappedTy>
		Iterator InsertOrAssign(const ConstIterator Where, const KeyType & Keyval, MappedTy && Mapval)
		{
			return Iterator(
				_Insert_or_assign_hint(Where._Ptr, Keyval, Forward<MappedTy>(Mapval)), ParentType::_Get_scary());
		}

		template<typename MappedTy>
		TPair<Iterator, bool> InsertOrAssign(KeyType && Keyval, MappedTy && _Mapval)
		{
			const auto _Result = _Insert_or_assign(Move(Keyval), Forward<MappedTy>(_Mapval));
			return { Iterator(_Result.First, ParentType::_Get_scary()), _Result.Second };
		}

		template<typename MappedTy>
		Iterator InsertOrAssign(const ConstIterator Where, KeyType && Keyval, MappedTy && Mapval)
		{
			return Iterator(_Insert_or_assign_hint(Where._Ptr, Move(Keyval), Forward<MappedTy>(Mapval)), ParentType::_Get_scary());
		}

	public:
		TMap(TInitializerList<PairType> _Ilist) : ParentType(KeyCompare())
		{
			insert(_Ilist);
		}

		TMap(TInitializerList<PairType> _Ilist, const KeyCompare & _Pred) : ParentType(_Pred)
		{
			insert(_Ilist);
		}

		TMap(TInitializerList<PairType> _Ilist, const AllocatorType & _Al) : ParentType(KeyCompare(), _Al)
		{
			insert(_Ilist);
		}

		TMap(TInitializerList<PairType> _Ilist, const KeyCompare & _Pred, const AllocatorType & _Al)
			: ParentType(_Pred, _Al)
		{
			insert(_Ilist);
		}

		TMap & operator =(TInitializerList<PairType> _Ilist)
		{
			ParentType::clear();
			insert(_Ilist);
			return *this;
		}

	public:
		MappedType & operator [](const KeyType & _Keyval)
		{
			return _Try_emplace(_Keyval).First->_Myval.Second;
		}

		_NODISCARD MappedType & At(const KeyType & _Keyval)
		{
			const auto _Loc = ParentType::_Find_lower_bound(_Keyval);
			if (!ParentType::_Lower_bound_duplicate(_Loc._Bound, _Keyval))
			{
				std::_Xout_of_range("invalid TMap<K, T> key");
			}

			return _Loc._Bound->_Myval.Second;
		}

		_NODISCARD const MappedType & At(const KeyType & _Keyval) const
		{
			const auto _Loc = ParentType::_Find_lower_bound(_Keyval);
			if (!ParentType::_Lower_bound_duplicate(_Loc._Bound, _Keyval))
			{
				std::_Xout_of_range("invalid TMap<K, T> key");
			}

			return _Loc._Bound->_Myval.Second;
		}

	public: // Remove
		template<typename _Iter = Iterator, EnableIfT<!IsSameV<_Iter, ConstIterator>>>
		Iterator Erase(Iterator _Where) noexcept /* strengthened */
		{
			return ParentType::erase(_Where);
		}

		Iterator Erase(ConstIterator _Where) noexcept /* strengthened */
		{
			return ParentType::erase(_Where);
		}

		Iterator Erase(ConstIterator _First, ConstIterator _Last) noexcept /* strengthened */
		{
			return ParentType::erase(_First, _Last);
		}

		SizeType Remove(const KeyType & _Keyval) noexcept(noexcept(ParentType::erase(_Keyval))) /* strengthened */
		{
			return ParentType::erase(_Keyval);
		}

	public: // Clear
		void Clear() noexcept
		{
			const auto _Scary = ParentType::_Get_scary();
			_Scary->_Orphan_ptr(nullptr);
			auto _Head = _Scary->_Myhead;
			_Scary->_Erase_tree(ParentType::_Getal(), _Head->_Parent);
			_Head->_Parent = _Head;
			_Head->_Left = _Head;
			_Head->_Right = _Head;
			_Scary->_Mysize = 0;
		}

		void Reset() noexcept { Clear(); }

	public: // Contains
		bool Contains(const KeyT & Key) const { return ParentType::contains(Key); }
		uintx Size() const { return ParentType::_Get_scary()->_Mysize; }

	public: // Find
		Iterator Find(const KeyT & Key) { return ParentType::find(Key); }
		ConstIterator Find(const KeyT & Key) const { return ParentType::find(Key); }

	public: // Range Iterators
		Iterator Begin() { return ParentType::begin(); }
		Iterator End() { return ParentType::end(); }
		ConstIterator Begin() const { return ParentType::begin(); }
		ConstIterator End() const { return ParentType::end(); }

	public: // _Unchecke
		using ParentType::_Unchecked_begin;
		using ParentType::_Unchecked_end;
	};
}
