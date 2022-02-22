#pragma once

#include "Basic.Types.h"
#include "Tuple.h"

namespace Xin
{
	template<typename FirstT, typename SecondT>
	struct TPair
	{
		using first_type = FirstT;
		using second_type = SecondT;

		template<typename = void> requires DefaultConstructibleC<FirstT> && DefaultConstructibleC<SecondT>
		constexpr explicit TPair() noexcept(IsNothrowDefaultConstructibleV<FirstT> && IsNothrowDefaultConstructibleV<SecondT>) // strengthened
			: First(), Second() {}

		template<typename = void> requires (CopyConstructibleC<FirstT> && CopyConstructibleC<SecondT>)
		constexpr TPair(const FirstT & First, const SecondT & Second) noexcept(IsNothrowCopyConstructibleV<FirstT> && IsNothrowCopyConstructibleV<SecondT>) // strengthened
			: First(First), Second(Second) {}

		template<typename FirstTy, typename SecondTy> requires (ConstructibleC<FirstT, FirstTy&&> && ConstructibleC<SecondT, SecondTy&&>)
		//constexpr explicit(not (ConstructibleC<FirstTy, FirstT> && ConstructibleC<SecondTy, SecondT>))
		TPair(FirstTy && OtherFirst, SecondTy && OtherSecond) noexcept(IsNothrowConstructibleV<FirstT, FirstTy> && IsNothrowConstructibleV<SecondT, SecondTy>) // strengthened
			: First(Forward<FirstTy>(OtherFirst)), Second(Forward<SecondTy>(OtherSecond)) {}

		TPair(const TPair &) = default;
		TPair(TPair &&) = default;

		template<typename FirstTy, typename SecondTy> requires (ConstructibleC<FirstT, const FirstTy&> && ConstructibleC<SecondT, const SecondTy&>)
		//constexpr explicit(!ConjunctionV<IsConvertibleTo<const FirstTy &, FirstT>, IsConvertibleTo<const SecondTy &, SecondT>>)
		TPair(const TPair<FirstTy, SecondTy> & That) noexcept(IsNothrowConstructibleV<FirstT, const FirstTy&> && IsNothrowConstructibleV<SecondT, const SecondTy&>) // strengthened
			: First(That.First), Second(That.Second) {}

		template<typename OtherFirstT, typename OtherSecondT> requires (ConstructibleC<FirstT, OtherFirstT&&> && ConstructibleC<SecondT, OtherSecondT&&>)
		//constexpr explicit(!IsConvertibleToV<OtherFirstT, FirstT> && !IsConvertibleToV<OtherSecondT, SecondT>)
		TPair(TPair<OtherFirstT, OtherSecondT> && That) noexcept(
			IsNothrowConstructibleV<FirstT, OtherFirstT> && IsNothrowConstructibleV<SecondT, OtherSecondT>) // strengthened
			: First(Forward<OtherFirstT>(That.First)), Second(Forward<OtherSecondT>(That.Second)) {}

		//template<typename Tuple1T, typename Tuple2T, uintx... Indexes1, uintx... Indexes2>
		//constexpr TPair(Tuple1T & Tuple1, Tuple2T & Tuple2, IndexSequence<Indexes1...>, IndexSequence<Indexes2...>)
		//	: First(std::get<Indexes1>(Move(Tuple1))...), Second(std::get<Indexes2>(Move(Tuple2))...) {}
		//
		//		template<typename... Types1T, typename... Types2T>
		//		constexpr TPair(std::piecewise_construct_t, TTuple<Types1T...> Tuple1, std::tuple<Types2T...> Tuple2)
		//			: TPair(Tuple1, Tuple2, IndexSequenceFor<Types1T...> {}, IndexSequenceFor<Types2T...> {}) {}
		//
#pragma region std support
		template<typename... Types1T, typename... Types2T, uintx... Indexes1, uintx... Indexes2>
		constexpr TPair(const std::tuple<Types1T...> & Tuple1, const std::tuple<Types2T...> & Tuple2, std::index_sequence<Indexes1...>, std::index_sequence<Indexes2...>)
			: First(std::get<Indexes1>(Move(Tuple1))...), Second(std::get<Indexes2>(Move(Tuple2))...) {}

		template<typename... Types1T, typename... Types2T>
		constexpr TPair(std::piecewise_construct_t, const std::tuple<Types1T...> & Tuple1, const std::tuple<Types2T...> & Tuple2)
			: TPair(Tuple1, Tuple2, std::index_sequence_for<Types1T...> {}, std::index_sequence_for<Types2T...> {}) {}
#pragma endregion

		TPair & operator =(const volatile TPair &) = delete;

		template<typename = EnableIfT<ConjunctionV<IsCopyConstructible<FirstT>, IsCopyConstructible<SecondT>>>>
		constexpr TPair & operator =(const TPair & That) noexcept(IsNothrowCopyAssignableV<FirstT> && IsNothrowCopyAssignableV<SecondT>)
		{
			First = That.First;
			Second = That.Second;
			return *this;
		}

		template<typename = EnableIfT<ConjunctionV<IsMoveConstructible<FirstT>, IsMoveConstructible<SecondT>>>>
		constexpr TPair & operator =(TPair && That) noexcept(IsNothrowMoveConstructibleV<FirstT> && IsNothrowMoveConstructibleV<SecondT>)
		{
			First = Forward<FirstT>(That.First);
			Second = Forward<SecondT>(That.Second);
			return *this;
		}

		template<typename OtherFirstT, typename OtherSecondT>
			requires (!SameC<TPair, TPair<OtherFirstT, OtherSecondT>> && AssignableC<FirstT&, const OtherFirstT&> && AssignableC<SecondT&, const OtherSecondT&>)
		constexpr TPair & operator =(const TPair<OtherFirstT, OtherSecondT> & That) noexcept(IsNoThrowAssignableV<FirstT&, const OtherFirstT&> && IsNoThrowAssignableV<SecondT&, const OtherSecondT&>) /* strengthened */
		{
			First = That.First;
			Second = That.Second;
			return *this;
		}

		template<typename OtherFirstT, typename OtherSecondT, EnableIfT<ConjunctionV<Negation<IsSame<TPair, TPair<OtherFirstT, OtherSecondT>>>, IsAssignable<FirstT&, OtherFirstT>, IsAssignable<SecondT&, OtherSecondT>>, int>  = 0>
		constexpr TPair & operator =(TPair<OtherFirstT, OtherSecondT> && That) noexcept(IsNoThrowAssignableV<FirstT&, OtherFirstT> && IsNoThrowAssignableV<SecondT&, OtherSecondT>) /* strengthened */
		{
			First = Forward<OtherFirstT>(That.First);
			Second = Forward<OtherSecondT>(That.Second);
			return *this;
		}

		constexpr void Swap(TPair & That)
		{
			if (this != AddressOf(That))
			{
				Swap(First, That.First);
				Swap(Second, That.Second);
			}
		}

		FirstT First;
		SecondT Second;
	};

	template<typename FirstT, typename SecondT>
	TPair(FirstT, SecondT) -> TPair<FirstT, SecondT>;

	template<typename FirstT, typename SecondT>
	constexpr void Swap(TPair<FirstT, SecondT> & Left, TPair<FirstT, SecondT> & Right)
	{
		Left.Swap(Right);
	}

	template<typename FirstT, typename SecondT>
	constexpr bool operator ==(const TPair<FirstT, SecondT> & Left, const TPair<FirstT, SecondT> & Right)
	{
		return Left.First == Right.First && Left.Second == Right.Second;
	}

	template<typename FirstT, typename SecondT>
	constexpr bool operator !=(const TPair<FirstT, SecondT> & Left, const TPair<FirstT, SecondT> & Right)
	{
		return !(Left == Right);
	}

	template<typename FirstT, typename SecondT>
	constexpr bool operator <(const TPair<FirstT, SecondT> & Left, const TPair<FirstT, SecondT> & Right)
	{
		return Left.First < Right.First || (!(Right.First < Left.First) && Left.Second < Right.Second);
	}

	template<typename FirstT, typename SecondT>
	constexpr bool operator >(const TPair<FirstT, SecondT> & Left, const TPair<FirstT, SecondT> & Right)
	{
		return Right < Left;
	}

	template<typename FirstT, typename SecondT>
	constexpr bool operator <=(const TPair<FirstT, SecondT> & Left, const TPair<FirstT, SecondT> & Right)
	{
		return !(Right < Left);
	}

	template<typename FirstT, typename SecondT>
	constexpr bool operator >=(const TPair<FirstT, SecondT> & Left, const TPair<FirstT, SecondT> & Right)
	{
		return !(Left < Right);
	}


	template<typename KeyT, typename ValueT>
	TPair<KeyT, ValueT> MakePair(const KeyT & First, const ValueT & Second)
	{
		return TPair<KeyT, ValueT>(First, Second);
	}

	template<typename KeyT, typename ValueT>
	struct THash<TPair<KeyT, ValueT>>
	{
		uintx operator ()(const TPair<KeyT, ValueT> & Pair) const noexcept
		{
			return Hash(Pair.First, Pair.Second);
		}
	};
}
