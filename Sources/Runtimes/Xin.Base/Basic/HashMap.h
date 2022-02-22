#pragma once

#include "Basic.Types.h"
#include "Map.h"

namespace Xin
{
	template <typename KeyT, typename ValueT, typename HasherT = THasher<KeyT>, typename KeyEqualT = TEqual<KeyT>>
	class THashMap : protected std::unordered_map< KeyT, ValueT, HasherT, KeyEqualT>
	{
	public:
		using ParentType = std::unordered_map< KeyT, ValueT, HasherT, KeyEqualT>;
		using Iterator = ParentType::iterator;
		using ConstIterator = ParentType::const_iterator;
	public:
		THashMap() = default;

		Iterator Find(const KeyT & Key)
		{
			return ParentType::find(Key);
		}

		ConstIterator Find(const KeyT & Key) const
		{
			return ParentType::find(Key);
		}

		Iterator Begin() { return ParentType::begin(); }
		ConstIterator Begin() const { return ParentType::begin(); }

		Iterator End() { return ParentType::end(); }
		ConstIterator End() const { return ParentType::end(); }

		template<typename MappedValueT>
		TPair<Iterator, bool> InsertOrAssign(const KeyT & Key, MappedValueT && Value)
		{
			auto Pair = ParentType::insert_or_assign(Key, Forward<MappedValueT>(Value));
			return { Pair.first, Pair.second };
		}
	};
}