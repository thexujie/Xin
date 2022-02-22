#pragma once

#include "Basic.Types.h"

namespace Xin
{
	template<typename... ValueTs>
	class TTuple;

	template<typename T>
	inline constexpr bool IsTupleV = false;

	template<typename... ValueTs>
	inline constexpr bool IsTupleV<TTuple<ValueTs...>> = true;

	template<typename T>
	concept TupleC = IsTupleV<T>;

	template<uintx Index, typename T>
	struct TTupleValue
	{
		TTupleValue() = default;
		TTupleValue(const TTupleValue &) = default;
		TTupleValue(TTupleValue &&) = default;
		TTupleValue & operator =(TTupleValue && That) = default;
		TTupleValue & operator =(const TTupleValue & That) = default;
		auto operator <=>(const TTupleValue &) const = default;

		explicit TTupleValue(const T & Value) : Value(Value) {}

		template<typename = EnableIfT<IsMoveConstructibleV<T>>>
		explicit TTupleValue(T && Value) : Value(Forward<T>(Value)) {}

		template<typename Ty>
		explicit TTupleValue(Ty && Value) : Value(Forward<Ty>(Value)) {}

		template<typename Ty>
		TTupleValue & operator =(Ty && That)
		{
			Value = Forward<Ty>(That);
			return *this;
		}

		template<typename Ty>
		TTupleValue & operator =(const Ty & That)
		{
			Value = That;
			return *this;
		}

		T Value;
	};

	template<typename Indices, typename... ValueTs>
	struct TTupleStorage;

	template<typename T>
	inline constexpr bool IsTupleStorageV = false;

	template<uintx... Indices, typename... ValueTs>
	inline constexpr bool IsTupleStorageV<TTupleStorage<IntegerSequence<uintx, Indices...>, ValueTs...>> = true;

	template<typename T>
	concept TupleStorageC = IsTupleStorageV<T>;

	template<uintx... Indices, typename... ValueTs>
	struct TTupleStorage<IntegerSequence<uintx, Indices...>, ValueTs...> : TTupleValue<Indices, ValueTs>...
	{
	public:
		static constexpr uintx ElementCount = sizeof...(ValueTs);
		template<uintx Index>
		using ElementT = TPackIndexing<Index, ValueTs...>;

		template<uintx Index>
		using TupleValueT = TTupleValue<Index, PackIndexingT<Index, ValueTs...>>;

	public:
		TTupleStorage() noexcept = default;
		TTupleStorage(const TTupleStorage & That) noexcept = default;
		TTupleStorage(TTupleStorage && That) noexcept = default;
		TTupleStorage & operator =(const TTupleStorage & That) noexcept = default;
		TTupleStorage & operator =(TTupleStorage && That) noexcept = default;
		auto operator <=>(const TTupleStorage &) const noexcept = default;

		TTupleStorage(ValueTs &&... Values) noexcept : TTupleValue<Indices, ValueTs>(Forward<ValueTs>(Values))... { }

		template<typename... OtherValueTs> requires(sizeof...(ValueTs) == sizeof...(OtherValueTs) && (ConstructibleC<ValueTs, OtherValueTs> && ...))
		TTupleStorage(OtherValueTs &&... Values) noexcept : TTupleValue<Indices, ValueTs>(Forward<OtherValueTs>(Values))... { }

		template<typename ThatT> requires (!SameC<TTupleStorage, ThatT> && TupleStorageC<RemoveCVRefT<ThatT>>)
		TTupleStorage(ThatT && That) noexcept
			: TupleValueT<Indices>(Forward<ThatT>(That).template Get<Indices>())... { }

		template<typename ThatT> requires TupleStorageC<RemoveCVRefT<ThatT>>
		TTupleStorage & operator =(ThatT && That) noexcept
		{
			Swallow(TTupleValue<Indices, ValueTs>::operator =(Forward<ThatT>(That).template Get<Indices>())...);
			return *this;
		}

		template<uintx Index>
		PackIndexingT<Index, ValueTs...> & Get() &
		{
			return static_cast<TupleValueT<Index>&>(*this).Value;
		}


		template<uintx Index>
		const PackIndexingT<Index, ValueTs...> & Get() const &
		{
			return static_cast<const TupleValueT<Index>&>(*this).Value;
		}


		template<uintx Index>
		PackIndexingT<Index, ValueTs...> && Get() &&
		{
			return static_cast<TupleValueT<Index>&&>(*this).Value;
		}
	};

	template<typename... ValueTs>
	class TTuple
	{
	public:
		static constexpr uintx ValueCount = sizeof...(ValueTs);
		using TupleStorageT = TTupleStorage<MakeIndexSequence<sizeof...(ValueTs)>, ValueTs...>;

	public:
		TTuple() noexcept = default;
		TTuple(const TTuple &) noexcept = default;
		TTuple(TTuple &&) noexcept = default;
		TTuple & operator =(const TTuple & That) noexcept = default;
		TTuple & operator =(TTuple && That) noexcept = default;
		bool operator ==(const TTuple & That) const noexcept = default;
		auto operator <=>(const TTuple &) const noexcept = default;

		TTuple(ValueTs &&... Values) noexcept : Storage(Forward<ValueTs>(Values)...) {}

		template<typename... OtherValueTs> requires(sizeof...(ValueTs) == sizeof...(OtherValueTs) && (ConstructibleC<ValueTs, OtherValueTs> && ...))
		TTuple(OtherValueTs &&... Values) noexcept : Storage(Forward<OtherValueTs>(Values)...) {}

		template<typename ThatT> requires (!SameC<TTuple, ThatT> && TupleC<RemoveCVRefT<ThatT>>)
		TTuple(ThatT && That) noexcept : Storage(Forward<ThatT>(That).Storage) { }

		template<typename ThatT> requires (!SameC<TTuple, ThatT> && TupleC<RemoveCVRefT<ThatT>>)
		TTuple & operator =(ThatT && That) noexcept
		{
			Storage.operator =(Forward<ThatT>(That).Storage);
			return *this;
		}

		TupleStorageT Storage;
	};

	static_assert(sizeof(TTuple<char, char, char, char>) == sizeof(char) * 4);
	static_assert(sizeof(TTuple<int, int, int, int>) == sizeof(int) * 4);
	static_assert(sizeof(TTuple<float, float, float, float>) == sizeof(int) * 4);
	static_assert(sizeof(TTuple<TTuple<char, char, char, char>, TTuple<char, char, char, char>>) == sizeof(char) * 8);
	static_assert(sizeof(TTuple<TTuple<int, int, int, int>, TTuple<int, int, int, int>>) == sizeof(int) * 8);
	static_assert(sizeof(TTuple<TTuple<float, float, float, float>, TTuple<float, float, float, float>>) == sizeof(int) * 8);

	//template<typename ...ValueTs>
	//constexpr bool IsTupleV = false;

	//template<typename ...ValueTs>
	//constexpr bool IsTupleV<TTuple<ValueTs...>> = true;

	template<>
	class TTuple<>
	{
	public:
		TTuple() = default;
		auto operator <=>(const TTuple &) const = default;
	};


	template<uintx Index, typename TupleT>
	struct TupleElement {};

	template<uintx Index, typename HeadT, typename... TailT>
	struct TupleElement<Index, TTuple<HeadT, TailT...>> : TupleElement<Index - 1, TTuple<TailT...>> { };

	template<typename HeadT, typename... TailT>
	struct TupleElement<0, TTuple<HeadT, TailT...>>
	{
		using Type = HeadT;
	};

	template<uintx Index, typename TupleT>
	using TupleElementT = typename TupleElement<Index, TupleT>::Type;

	template<uintx Index, typename... ValueTs>
	PackIndexingT<Index, ValueTs...> & Get(TTuple<ValueTs...> & Tuple)
	{
		return Tuple.Storage.template Get<Index>();
	}

	template<uintx Index, typename... ValueTs>
	const PackIndexingT<Index, ValueTs...> & Get(const TTuple<ValueTs...> & Tuple)
	{
		return Tuple.Storage.template Get<Index>();
	}

	template<uintx Index, typename... ValueTs>
	PackIndexingT<Index, TTuple<ValueTs...>> && Get(TTuple<ValueTs...> && Tuple)
	{
		return Forward<PackIndexingT<Index, ValueTs...>>(Tuple.Storage.template Get<Index>());
	}

	// -- support for std structure bindings.
	template<size_t Index, typename... TypesT>
	PackIndexingT<Index, TypesT...> & get(TTuple<TypesT...> & Tuple)
	{
		return Tuple.Storage.template Get<Index>();
	}

	template<size_t Index, typename... TypesT>
	const PackIndexingT<Index, TypesT...> & get(const TTuple<TypesT...> & Tuple)
	{
		return Tuple.Storage.template Get<Index>();
	}

	template<size_t Index, typename... TypesT>
	PackIndexingT<Index, TypesT...> && get(TTuple<TypesT...> && Tuple)
	{
		return Move(Tuple.Storage).template Get<Index>();
	}

	// -- support for std structure bindings.

	template<typename TupleT>
	struct TupleSize;

	template<typename... ValueTs>
	struct TupleSize<TTuple<ValueTs...>> : IntegerConstant<uintx, sizeof...(ValueTs)> { };

	template<typename TupleT>
	constexpr uintx TupleSizeV = TupleSize<TupleT>::Value;

	template<typename... ValueTs>
	forceinline TTuple<DecayT<ValueTs>...> MakeTuple(ValueTs &&... Args)
	{
		return TTuple<DecayT<ValueTs>...>(Forward<ValueTs>(Args)...);
	}

	struct FIgnore
	{
		template<typename T>
		constexpr const FIgnore & operator =(const T &) const noexcept { return *this; }
	};

	inline constexpr FIgnore Ignore {};

	template<typename... ValueTs>
	constexpr TTuple<ValueTs&...> Tie(ValueTs &... Args) noexcept
	{
		return TTuple<ValueTs&...>(Args...);
	}

	template<typename... ValueTs>
	constexpr TTuple<ValueTs&&...> ForwardAsTuple(ValueTs &&... Args) noexcept
	{
		return TTuple<ValueTs&&...>(Forward<ValueTs>(Args)...);
	}
}

namespace std
{
	template<typename... TypesT>
	struct tuple_size<Xin::TTuple<TypesT...>> : std::integral_constant<size_t, sizeof...(TypesT)> { };

	template<size_t Index, typename... TypesT>
	struct tuple_element<Index, Xin::TTuple<TypesT...>>
	{
		using type = Xin::PackIndexingT<Index, TypesT...>;
	};
}
