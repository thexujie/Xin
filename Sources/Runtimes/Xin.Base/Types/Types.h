#pragma once

#include "../Xin.Base.Types.h"

#define XIN_TYPES 1

namespace Xin
{
	//#define Str(xxx) xxx
	//#define StrView(xxx) TView(xxx)
	//#define XE_ANSI 1

	using u8ios = std::basic_ios<char8_t, std::char_traits<char8_t>>;
	using u8streambuf = std::basic_streambuf<char8_t, std::char_traits<char8_t>>;
	using u8istream = std::basic_istream<char8_t, std::char_traits<char8_t>>;
	using u8ostream = std::basic_ostream<char8_t, std::char_traits<char8_t>>;
	using u8iostream = std::basic_iostream<char8_t, std::char_traits<char8_t>>;
	using u8stringbuf = std::basic_stringbuf<char8_t, std::char_traits<char8_t>, std::allocator<char8_t>>;
	using u8istringstream = std::basic_istringstream<char8_t, std::char_traits<char8_t>, std::allocator<char8_t>>;
	using u8ostringstream = std::basic_ostringstream<char8_t, std::char_traits<char8_t>, std::allocator<char8_t>>;
	using u8stringstream = std::basic_stringstream<char8_t, std::char_traits<char8_t>, std::allocator<char8_t>>;
	using u8filebuf = std::basic_filebuf<char8_t, std::char_traits<char8_t>>;
	using u8ifstream = std::basic_ifstream<char8_t, std::char_traits<char8_t>>;
	using u8ofstream = std::basic_ofstream<char8_t, std::char_traits<char8_t>>;
	using u8fstream = std::basic_fstream<char8_t, std::char_traits<char8_t>>;

	using xstring = std::basic_string<char8_t, std::char_traits<char8_t>>;
	using xstring_view = std::basic_string_view<char8_t, std::char_traits<char8_t>>;
	using xios = std::basic_ios<char8_t, std::char_traits<char8_t>>;
	using xstreambuf = std::basic_streambuf<char8_t, std::char_traits<char8_t>>;
	using xistream = std::basic_istream<char8_t, std::char_traits<char8_t>>;
	using xostream = std::basic_ostream<char8_t, std::char_traits<char8_t>>;
	using xiostream = std::basic_iostream<char8_t, std::char_traits<char8_t>>;
	using xstringbuf = std::basic_stringbuf<char8_t, std::char_traits<char8_t>, std::allocator<char8_t>>;
	using xistringstream = std::basic_istringstream<char8_t, std::char_traits<char8_t>, std::allocator<char8_t>>;
	using xostringstream = std::basic_ostringstream<char8_t, std::char_traits<char8_t>, std::allocator<char8_t>>;
	using xstringstream = std::basic_stringstream<char8_t, std::char_traits<char8_t>, std::allocator<char8_t>>;
	using xfilebuf = std::basic_filebuf<char8_t, std::char_traits<char8_t>>;
	using xifstream = std::basic_ifstream<char8_t, std::char_traits<char8_t>>;
	using xofstream = std::basic_ofstream<char8_t, std::char_traits<char8_t>>;
	using xfstream = std::basic_fstream<char8_t, std::char_traits<char8_t>>;

	struct NovalT {};
	static inline constexpr NovalT noval { };

	using long32 = long;
	using ulong32 = unsigned long;
	using float32 = float;
	using float64 = double;

#if XIN_X64
	using intx = int64_t;
	using uintx = uint64_t;
#else
	using intx = int32_t;
	using uintx = uint32_t;
#endif
	using byte = uint8_t;
	using int8 = int8_t;
	using uint8 = uint8_t;
	using int16 = int16_t;
	using uint16 = uint16_t;
	using int32 = int32_t;
	using uint32 = uint32_t;
	using int64 = int64_t;
	using uint64 = uint64_t;

	using chara = char;
	using charw = wchar_t;
	using char8 = char8_t;
	using char16 = char16_t;
	using char32 = char32_t;

	using nullp = nullptr_t;
	using sizet = size_t;
	using alignt = std::align_val_t;

	using voidp = void *;
	using bytep = byte *;

	constexpr uintx ByteBits = 8;
	constexpr uintx UnitKiB = 1024;
	constexpr uintx UnitMiB = 1024 * UnitKiB;
	constexpr uintx UnitGiB = 1024 * UnitMiB;
	constexpr uint32 InfinityU32 = 0x7f800000;
	const float32 NanF = std::nanf("0");
	const float64 NanF64 = std::nan("0");

	constexpr int16 Int16Min = std::numeric_limits<int16>::min();
	constexpr int16 Int16Max = std::numeric_limits<int16>::max();
	constexpr uint16 UInt16Min = std::numeric_limits<uint16>::min();
	constexpr uint16 UInt16Max = std::numeric_limits<uint16>::max();
	constexpr int32 Int32Min = std::numeric_limits<int32>::min();
	constexpr int32 Int32Max = std::numeric_limits<int32>::max();
	constexpr uint32 UInt32Min = std::numeric_limits<uint32>::min();
	constexpr uint32 UInt32Max = std::numeric_limits<uint32>::max();
	constexpr int64_t Int64Min = std::numeric_limits<int64_t>::min();
	constexpr int64_t Int64Max = std::numeric_limits<int64_t>::max();
	constexpr uint64 UInt64Min = std::numeric_limits<uint64>::min();
	constexpr uint64 UInt64Max = std::numeric_limits<uint64>::max();

#if XIN_X64
	constexpr intx IntXMin = Int64Min;
	constexpr intx IntXMax = Int64Max;
	constexpr uintx UIntXMin = UInt64Min;
	constexpr uintx UIntXMax = UInt64Max;
#else
	constexpr uintx IntXMin = Int32Min;
	constexpr uintx IntXMax = Int32Max;
	constexpr uintx UIntXMin = UInt32Min;
	constexpr uintx UIntXMax = UInt32Max;
#endif

	constexpr float32 Float32Min = std::numeric_limits<float32>::lowest();
	constexpr float32 Float32Max = std::numeric_limits<float32>::max();
	constexpr float32 Float32Minimum = std::numeric_limits<float32>::min();
	constexpr float32 Float32Epsilon = std::numeric_limits<float32>::epsilon();
	constexpr float32 Float32NaN = std::numeric_limits<float32>::quiet_NaN();
	constexpr float32 Float32SignalingNaN = std::numeric_limits<float32>::signaling_NaN();
	constexpr float32 Float32PI = 3.141592654f;

	constexpr float64 Float64Min = std::numeric_limits<float64>::lowest();
	constexpr float64 Float64Max = std::numeric_limits<float64>::max();
	constexpr float64 Float64Minimum = std::numeric_limits<float64>::min();
	constexpr float64 Float64Epslion = std::numeric_limits<float64>::epsilon();
	constexpr float64 Float64NaN = std::numeric_limits<float64>::quiet_NaN();
	constexpr float64 Float64SignalingNaN = std::numeric_limits<float64>::signaling_NaN();
	constexpr float64 Float64PI = 3.14159265358979323846;

	template<typename T>
	constexpr T FloatEpslion = 0;

	template<typename T>
	constexpr T Infinity = std::numeric_limits<T>::infinity();

	template<typename T>
	constexpr T Epslion = std::numeric_limits<T>::epsilon();

	template<typename T>
	constexpr T NaN = std::numeric_limits<T>::quiet_NaN();

	template<typename T>
	constexpr T QuietNaN = std::numeric_limits<T>::quiet_NaN();

	template<typename T>
	constexpr T SignalingNaN = std::numeric_limits<T>::signaling_NaN();

	template<>
	constexpr float32 FloatEpslion<float32> = Float32Epsilon;
	template<>
	constexpr float64 FloatEpslion<float64> = Float64Epslion;

	template<typename T>
	forceinline constexpr T * AddressOf(T & Value) noexcept { return __builtin_addressof(Value); }

	template<typename T>
	forceinline const T * AddressOf(const T &&) = delete;

	constexpr bool IsConstantEvaluated() noexcept { return std::is_constant_evaluated(); }

	template<uintx Index, typename... ParameterTs> requires (Index < sizeof...(ParameterTs))
	struct TPackIndexing {};

	template<uintx Index, typename HeadParameterT, typename... TailParameterTs> requires (Index <= sizeof...(TailParameterTs))
	struct TPackIndexing<Index, HeadParameterT, TailParameterTs...>
	{
		using Type = typename TPackIndexing<Index - 1, TailParameterTs...>::Type;
	};

	template<typename HeadParameterT, typename... TailParameterTs>
	struct TPackIndexing<0, HeadParameterT, TailParameterTs...>
	{
		using Type = HeadParameterT;
	};

	template<uintx Index, typename... ValueTs>
		requires (Index < sizeof...(ValueTs))
	using PackIndexingT = typename TPackIndexing<Index, ValueTs...>::Type;

	template<uintx Index, typename HeadArgumentT, typename... OtherArgumentTs, typename ReturnT = PackIndexingT<Index, HeadArgumentT, OtherArgumentTs...>> requires (Index <= sizeof...(OtherArgumentTs))
	auto PackIndexingArgument(HeadArgumentT && HeadArgument, OtherArgumentTs &&... OtherArguments) -> ReturnT &&
	{
		if constexpr (Index == 0)
			return Forward<HeadArgumentT>(HeadArgument);
		else
		{
			using NextArgumentT = PackIndexingT<Index - 1, OtherArgumentTs...>;
			return Forward<NextArgumentT>(PackIndexingArgument<Index - 1, OtherArgumentTs...>(Forward<OtherArgumentTs>(OtherArguments)...));
		}
	}

	template<intx Index, typename T, typename T0, typename ...Ts>
	constexpr intx PackFindType()
	{
		if constexpr (std::is_same_v<T, T0>)
			return Index;
		else
		{
			if constexpr (sizeof...(Ts) == 0)
				return -1;
			else
				return PackFindType<Index + 1, T, Ts...>();
		}
	}

	template<typename T, typename ...Ts>
	constexpr intx PackTypeIndex = PackFindType<0, T, Ts...>();


	template<typename T, intx Index, T Value0, T ...Values>
	constexpr intx PackFindValue(T Value)
	{
		if (Value == Value0)
			return Index;
		else
		{
			if constexpr (sizeof...(Values) == 0)
				return -1;
			else
				return PackFindValue<T, Index + 1, Values...>(Value);
		}
	}

	template<typename T, T Value, T...Values>
	constexpr intx PackValueIndex = PackFindValue<T, 0, Values...>(Value);

	template<typename T>
	struct TDummy { };

	template<typename T>
	inline constexpr bool IsDummyV = false;

	template<typename T>
	inline constexpr bool IsDummyV<TDummy<T>> = true;

	using FStrongOrdering = std::strong_ordering;

	template<typename T>
	inline constexpr bool HasUniqueObjectRepresentationsV = std::has_unique_object_representations_v<T>;

	// ---------------- NullPtrT
	using NullptrT = decltype(__nullptr);

	template<typename T>
	using NullPointerT = decltype(__nullptr);

	// ---------------- VoidT
	template<typename... Types>
	using VoidT = void;

	template<typename T>
	inline constexpr bool IsConstV = std::is_const_v<T>;
	template<typename T>
	concept ConstC = std::is_const_v<T>;

	// ---------------- RemoveReference
	template<typename T>
	struct RemoveReference
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveReference<T&>
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveReference<T&&>
	{
		using Type = T;
	};

	template<typename T>
	using RemoveRefT = typename RemoveReference<T>::Type;

	// ---------------- RemoveCV
	template<typename T>
	struct RemoveCV
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveCV<const T>
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveCV<volatile T>
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveCV<const volatile T>
	{
		using Type = T;
	};

	template<typename T>
	using RemoveCVT = typename RemoveCV<T>::Type;

	template<typename T>
	using RemoveCVRefT = RemoveCVT<RemoveRefT<T>>;


	// ---------------- RemoveExtent
	template<typename T>
	struct RemoveExtent
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveExtent<T[]>
	{
		using Type = T;
	};

	template<typename T, uintx N>
	struct RemoveExtent<T[N]>
	{
		using Type = T;
	};

	template<typename T>
	using RemoveExtentT = typename RemoveExtent<T>::Type;

	// ---------------- Add CVRef
	template<typename... TypesT>
	void Swallow(TypesT &&...) {}

	template<typename T>
	struct AddConst
	{
		using Type = const T;
	};

	template<typename T>
	using AddConstT = typename AddConst<T>::Type;

	template<typename T>
	struct AddVolatile
	{
		using Type = volatile T;
	};

	template<typename T>
	using AddVolatileT = typename AddVolatile<T>::Type;

	template<typename T>
	struct AddCV
	{
		using Type = const volatile T;
	};

	template<typename T>
	using AddCVT = typename AddCV<T>::Type;

	// ---------------- AddPointer
	template<typename T, typename = void>
	struct AddPointer
	{
		using Type = T;
	};

	template<typename T>
	struct AddPointer<T, VoidT<RemoveRefT<T> *>>
	{
		using Type = RemoveRefT<T> *;
	};

	template<typename T>
	using AddPointerT = typename AddPointer<T>::Type;

	template<typename T>
	using RemovePointerT = std::remove_pointer_t<T>;

	template<typename T>
	using PointeeT = RemovePointerT<T>;

	// ---------------- EnableIf
	template<bool Test, typename T = void>
	struct EnableIf {};

	template<typename T>
	struct EnableIf<true, T>
	{
		using Type = T;
	};

	template<bool Test, typename T = void>
	using EnableIfT = typename EnableIf<Test, T>::Type;


	//----------------- Add
	template<typename T>
	struct AddLValueReference
	{
		using Type = T&;
	};

	template<>
	struct AddLValueReference<void>
	{
		using Type = void;
	};

	template<typename T>
	using AddLValueReferenceT = typename AddLValueReference<T>::Type;

	template<typename T>
	struct AddRValueReference
	{
		using Type = T&&;
	};

	template<>
	struct AddRValueReference<void>
	{
		using Type = void;
	};

	template<typename T>
	using AddRValueReferenceT = typename AddRValueReference<T>::Type;

	template<bool Cond, typename TrueT, typename FalseT>
	struct Conditional
	{
		using Type = TrueT;
	};

	template<typename TrueT, typename FalseT>
	struct Conditional<false, TrueT, FalseT>
	{
		using Type = FalseT;
	};

	template<bool Cond, typename TrueT, typename FalseT>
	using ConditionalT = typename Conditional<Cond, TrueT, FalseT>::Type;

	template<typename T>
	AddRValueReferenceT<T> DeclVal() noexcept;

	// ---------------- IntegralConstant
	template<typename T, T ConstantValue>
	struct IntegerConstant
	{
		using ValueType = T;
		//using Type = IntegralConstant;

		static constexpr T Value = ConstantValue;

		constexpr operator ValueType() const noexcept { return Value; }
		constexpr ValueType operator ()() const noexcept { return Value; }
	};

	// ---------------- BoolConstant
	template<bool ConstantValue>
	using BoolConstant = IntegerConstant<bool, ConstantValue>;

	using TrueType = BoolConstant<true>;
	using FalseType = BoolConstant<false>;

	template<typename T>
	constexpr bool TrueTypeV = BoolConstant<true>::Value;

	template<typename T>
	constexpr bool FalseTypeV = BoolConstant<false>::Value;

	template<typename T>
	constexpr bool DependentFalse = false;


	//template <class _Trait>
	//struct Negation : bool_constant<!static_cast<bool>(_Trait::value)> {}; // The negated result of _Trait

	template<typename T>
	struct Negation : BoolConstant<!static_cast<bool>(T::Value)> {};

	template<typename T>
	inline constexpr bool NegationV = Negation<T>::Value;

	// ---------------- DefinedType
	template<typename, typename T>
	struct TDefineHelper : FalseType {};

	template<typename T>
	struct TDefineHelper<VoidT<decltype(sizeof(T))>, T> : TrueType {};

	template<typename T>
	struct TIsDefinedType : TDefineHelper<void, T> {};

	template<typename T>
	inline constexpr bool IsDefinedTypeV = TIsDefinedType<T>::Value;

	// ---------------- Is Array
	template<typename>
	inline constexpr bool IsArrayV = false;

	template<typename T, uintx Length>
	inline constexpr bool IsArrayV<T[Length]> = true;

	template<typename T>
	inline constexpr bool IsArrayV<T[]> = true;

	template<typename T>
	struct IsArray : BoolConstant<IsArrayV<T>>
	{
		using Type = T;
	};

	template<typename T>
	concept ArrayC = IsArrayV<T>;

	template<typename T>
	concept NoneArrayC = (!IsArrayV<T>);

	// ---------------- Is Reference
	template<typename T>
	inline constexpr bool IsReferenceV = std::is_reference_v<T>;

	template<typename T>
	struct IsReference : BoolConstant<IsReferenceV<T>> {};

	template<typename T>
	concept ReferenceC = IsReferenceV<T>;

	template<typename T>
	inline constexpr bool IsNoneReferenceV = !IsReferenceV<T>;
	template<typename T>

	concept NoneReferenceC = IsNoneReferenceV<T>;

	// ---------------- IsSame
	template<typename T1, typename T2>
	inline constexpr bool IsSameV = std::is_same_v<T1, T2>;

	template<typename T1, typename T2>
	struct IsSame : BoolConstant<IsSameV<T1, T2>> {};

	template<typename T1, typename T2>
	concept SameC = IsSameV<T1, T2>;

	template<typename BaseT, typename DerivedT>
	inline constexpr bool IsBaseOfV = std::is_base_of_v<BaseT, DerivedT>;

	template<typename BaseT, typename DerivedT>
	using IsBaseOf = BoolConstant<IsBaseOfV<BaseT, DerivedT>>;

	template<typename BaseT, typename DerivedT>
	concept BaseOfC = std::is_base_of_v<BaseT, DerivedT>;

	template<typename DerivedT, typename BaseT>
	inline constexpr bool IsDerivedOfV = std::is_base_of_v<BaseT, DerivedT>;

	template<typename DerivedT, typename BaseT>
	concept DerivedOfC = IsDerivedOfV<DerivedT, BaseT>;

	template<typename T>
	inline constexpr bool IsVoidV = std::is_void_v<T>;

	template<typename T>
	struct IsVoid : BoolConstant<IsVoidV<T>> {};

	// ---------------- IsConvertible
	template<typename T, typename ToT>
	inline constexpr bool IsConvertibleToV = std::is_convertible_v<T, ToT>;
	template<typename T, typename ToT>
	concept ConvertibleToC = IsConvertibleToV<T, ToT>;

	template<typename T, typename FromT>
	inline constexpr bool IsConvertibleFromV = std::is_convertible_v<FromT, T>;
	template<typename T, typename FromT>
	concept ConvertibleFromC = IsConvertibleFromV<T, FromT>;

	template<typename FromT, typename ToT>
	concept OperatorCastableToC = requires(const FromT & From)
	{
		{ From.operator ToT() };
	};
	template<typename FromT, typename ToT>
	inline constexpr bool IsOperatorCastableToV = false;

	template<typename FromT, typename ToT> requires OperatorCastableToC<FromT, ToT>
	inline constexpr bool IsOperatorCastableToV<FromT, ToT> = true;

	// ---------------- Disjunction
	template<typename...>
	struct Disjunction : FalseType { };

	template<typename T>
	struct Disjunction<T> : T { };

	template<typename T, typename... ArgsT>
	struct Disjunction<T, ArgsT...> : ConditionalT<bool(T::Value), T, Disjunction<ArgsT...>> { };

	template<typename... ArgsT>
	inline constexpr bool DisjunctionV = Disjunction<ArgsT...>::Value;

	// ---------------- IsAnyOfV
	template<typename T, typename... ArgsT>
	inline constexpr bool IsAnyOfV = DisjunctionV<IsSame<T, ArgsT>...>;

	template<typename T, typename... ArgsT>
	concept AnyC = IsAnyOfV<T, ArgsT...>;

	// ---------------- IsEmpty
	template<typename T>
	inline constexpr bool IsEmptyV = std::is_empty_v<T>;

	template<typename T>
	struct IsEmpty : BoolConstant<IsEmptyV<T>> {};

	// ---------------- Types
	template<typename T>
	inline constexpr bool IsScalarV = std::is_scalar_v<T>; // https://en.cppreference.com/w/cpp/named_req/ScalarType
	template<typename T>
	inline constexpr bool IsUnionV = std::is_union_v<T>;
	template<typename T>
	inline constexpr bool IsEnumV = std::is_enum_v<T>;
	template<typename T>
	inline constexpr bool IsClassV = std::is_class_v<T>;
	template<typename T>
	inline constexpr bool IsEnumClassV = IsEnumV<T> && !IsConvertibleToV<T, int>;

	template<typename T>
	inline constexpr bool IsAbstractV = std::is_abstract_v<T>;

	template<typename T>
	struct IsAbstract : BoolConstant<IsAbstractV<T>> {};

	template<typename T>
	concept AbstractC = IsAbstractV<T>;

	template<typename T>
	inline constexpr bool IsFinalV = std::is_final_v<T>;

	template<typename T>
	struct IsFinal : BoolConstant<IsFinalV<T>> {};

	template<typename T>
	using UnderlyingTypeT = std::underlying_type_t<T>;

	template<typename T, bool = IsEnumV<T>>
	struct UnderlyingType
	{
		using Type = UnderlyingTypeT<T>;
	};

	template<typename T>
	concept EqualableC = requires(const T & Lhs, const T & Rhs) { { Lhs == Rhs } -> ConvertibleToC<bool>; };

	template<typename T>
	concept EqualComparableC = std::equality_comparable<T>;
	template<typename T>
	inline constexpr bool IsEqualComparableV = false;
	template<EqualComparableC T>
	inline constexpr bool IsEqualComparableV<T> = true;

	template<typename T, typename Ty>
	concept EqualityComparableWithC = std::equality_comparable_with<T, Ty>;
	template<typename T, typename Ty>
	inline constexpr bool IsEqualityComparableWithV = false;
	template<typename T, EqualityComparableWithC<T> Ty>
	inline constexpr bool IsEqualityComparableWithV<T, Ty> = true;

	template<typename CallableT, typename... ParameterTs>
	using InvokeResultT = std::invoke_result_t<CallableT, ParameterTs...>;

	template<typename CallableT, typename... ParameterTs>
	inline constexpr bool IsInvocableV = std::is_invocable_v<CallableT, ParameterTs...>;

	template<typename CallableT, typename... ParameterTs>
	concept InvocableC = IsInvocableV<CallableT, ParameterTs...>;

#pragma region Constructible
	// Constructible
	template<typename T, typename... ArgsT>
	inline constexpr bool IsConstructibleV = std::is_constructible_v<T, ArgsT...>;

	template<typename T, typename... ArgsT>
	struct IsConstructible : BoolConstant<IsConstructibleV<T, ArgsT...>> {};

	template<typename T, typename... ArgsT>
	concept ConstructibleC = IsConstructibleV<T, ArgsT...>;

	// NothrowConstructible
	template<typename T, typename... ArgsT>
	inline constexpr bool IsNothrowConstructibleV = std::is_nothrow_constructible_v<T, ArgsT...>;

	template<typename T, typename... ArgsT>
	struct IsNothrowConstructible : BoolConstant<IsNothrowConstructibleV<T, ArgsT...>> {};

	template<typename T, typename... ArgsT>
	concept NothrowConstructibleC = IsConstructibleV<T, ArgsT...>;

	// DefaultConstructible
	template<typename T>
	inline constexpr bool IsDefaultConstructibleV = std::is_constructible_v<T>;

	template<typename T>
	struct IsDefaultConstructible : BoolConstant<IsDefaultConstructibleV<T>> {};

	template<typename T>
	concept DefaultConstructibleC = IsDefaultConstructibleV<T>;

	// NothrowDefaultConstructible
	template<typename T>
	inline constexpr bool IsNothrowDefaultConstructibleV = std::is_nothrow_default_constructible_v<T>;

	template<typename T>
	struct IsNothrowDefaultConstructible : BoolConstant<IsNothrowDefaultConstructibleV<T>> {};

	template<typename T>
	concept NothrowDefaultConstructibleC = IsNothrowDefaultConstructibleV<T>;

	// CopyConstructible
	template<typename T>
	inline constexpr bool IsCopyConstructibleV = std::is_copy_constructible_v<T>;

	template<typename T>
	struct IsCopyConstructible : BoolConstant<IsCopyConstructibleV<T>> {};

	template<typename T>
	concept CopyConstructibleC = IsNothrowDefaultConstructibleV<T>;

	// NothrowCopyConstructible
	template<typename T>
	inline constexpr bool IsNothrowCopyConstructibleV = std::is_nothrow_copy_constructible_v<T>;

	template<typename T>
	struct IsNothrowCopyConstructible : BoolConstant<IsNothrowCopyConstructibleV<T>> {};

	template<typename T>
	concept NothrowCopyConstructibleC = IsNothrowDefaultConstructibleV<T>;

	// MoveConstructible
	template<typename T>
	inline constexpr bool IsMoveConstructibleV = std::is_move_constructible_v<T>;

	template<typename T>
	struct IsMoveConstructible : BoolConstant<IsMoveConstructibleV<T>> {};

	template<typename T>
	concept MoveConstructibleC = IsMoveConstructibleV<T>;

	// NothrowMoveConstructible
	template<typename T>
	inline constexpr bool IsNothrowMoveConstructibleV = std::is_nothrow_move_constructible_v<T>;

	template<typename T>
	struct IsNothrowMoveConstructible : BoolConstant<IsNothrowMoveConstructibleV<T>> {};

	template<typename T>
	concept NothrowMoveConstructibleC = IsNothrowMoveConstructibleV<T>;

	// ---------------- TriviallyConstructible
	template<typename T, typename... ArgsT>
	inline constexpr bool IsTriviallyConstructibleV = std::is_trivially_constructible_v<T, ArgsT...>;

	template<typename T, typename... ArgsT>
	struct IsTriviallyConstructible : BoolConstant<IsTriviallyConstructibleV<T, ArgsT...>> {};

	template<typename T, typename... ArgsT>
	concept TriviallyConstructibleC = IsTriviallyConstructibleV<T, ArgsT...>;

	// ---------------- TriviallyDefaultConstructible
	template<typename T>
	inline constexpr bool IsTriviallyDefaultConstructibleV = std::is_trivially_default_constructible_v<T>;

	template<typename T, typename... ArgsT>
	struct IsTriviallyDefaultConstructible : BoolConstant<IsTriviallyDefaultConstructibleV<T>> {};

	template<typename T, typename... ArgsT>
	concept TriviallyDefaultConstructibleC = IsTriviallyDefaultConstructibleV<T>;

	// ---------------- TriviallyCopyConstructible
	template<typename T>
	inline constexpr bool IsTriviallyCopyConstructibleV = std::is_trivially_copy_constructible_v<T>;

	template<typename T>
	struct IsTriviallyCopyConstructible : BoolConstant<IsTriviallyCopyConstructibleV<T>> {};

	template<typename T>
	concept TriviallyCopyConstructibleC = IsTriviallyCopyConstructibleV<T>;

	// ---------------- TriviallyMoveConstructible
	template<typename T>
	inline constexpr bool IsTriviallyMoveConstructibleV = std::is_trivially_move_constructible_v<T>;

	template<typename T>
	struct IsTriviallyMoveConstructible : BoolConstant<IsTriviallyMoveConstructibleV<T>> {};

	template<typename T>
	concept TriviallyMoveConstructibleC = IsTriviallyMoveConstructibleV<T>;


	// ---------------- ExplicitlyConvertible
	// (T)FromT
	template<typename T, typename FromT> 
	inline constexpr bool IsExplicitlyConvertibleFromV = IsConstructibleV<T, FromT> && !IsConvertibleToV<FromT, T>;

	template<typename T, typename FromT>
	concept ExplicitlyConvertibleFromC = IsExplicitlyConvertibleFromV<T, FromT>();

	// (Ty)T
	template<typename T, typename ToT>
	inline constexpr bool IsExplicitlyConvertibleToV = IsConstructibleV<ToT, T> && !IsConvertibleToV<T, ToT>;

	template<typename T, typename ToT>
	concept ExplicitlyConvertibleToC = IsExplicitlyConvertibleToV<T, ToT>();

#pragma endregion Constructible

#pragma region Destructible
	// IsDestructible
	template<typename T>
	inline constexpr bool IsDestructibleV = std::is_destructible_v<T>;

	template<typename T>
	struct IsDestructible : BoolConstant<IsDestructibleV<T>> {};

	template<typename T>
	concept DestructibleC = IsDestructibleV<T>;

	// IsTriviallyDestructible
	template<typename T>
	inline constexpr bool IsTriviallyDestructibleV = std::is_trivially_destructible_v<T>;

	template<typename T>
	struct IsTriviallyDestructible : BoolConstant<IsTriviallyDestructibleV<T>> {};

	template<typename T>
	concept TriviallyDestructibleC = IsTriviallyDestructibleV<T>;
#pragma endregion Destructible

#pragma region Assignable
	// Assignable T = Ty
	template<typename T, typename Ty>
	inline constexpr bool IsAssignableV = std::is_assignable_v<T, Ty>;

	template<typename T, typename Ty>
	struct IsAssignable : BoolConstant<IsAssignableV<T, Ty>> {};

	template<typename T, typename Ty>
	concept AssignableC = IsAssignableV<T, Ty>;

	// NoThrowAssignable T = Ty
	template<typename T, typename Ty>
	inline constexpr bool IsNoThrowAssignableV = std::is_nothrow_assignable_v<T, Ty>;

	template<typename T, typename Ty>
	struct IsNoThrowAssignable : BoolConstant<IsNoThrowAssignableV<T, Ty>> {};

	template<typename T, typename Ty>
	concept NoThrowAssignableC = IsNoThrowAssignableV<T, Ty>;

	// TriviallyAssignable
	template<typename T, typename Ty>
	inline constexpr bool IsTriviallyAssignableV = std::is_trivially_assignable_v<T, Ty>;

	template<typename T, typename Ty>
	struct IsTriviallyAssignable : BoolConstant<IsTriviallyAssignableV<T, Ty>> {};

	template<typename T, typename Ty>
	concept TriviallyAssignableC = IsTriviallyAssignableV<T, Ty>;

	// CopyAssignable
	template<typename T>
	inline constexpr bool IsCopyAssignableV = std::is_copy_assignable_v<T>;

	template<typename T>
	struct IsCopyAssignable : BoolConstant<IsCopyAssignableV<T>> {};

	template<typename T>
	concept CopyAssignableC = IsCopyAssignableV<T>;

	// NothrowCopyAssignable
	template<typename T>
	inline constexpr bool IsNothrowCopyAssignableV = std::is_nothrow_copy_assignable_v<T>;

	template<typename T>
	struct IsNothrowCopyAssignable : BoolConstant<IsNothrowCopyAssignableV<T>> {};

	template<typename T>
	concept NothrowCopyAssignableC = IsNothrowCopyAssignableV<T>;

	// TriviallyCopyAssignable
	template<typename T>
	inline constexpr bool IsTriviallyCopyAssignableV = std::is_trivially_copy_assignable_v<T>;

	template<typename T>
	struct IsTriviallyCopyAssignable : BoolConstant<IsTriviallyCopyAssignableV<T>> {};

	template<typename T>
	concept TriviallyCopyAssignableC = IsTriviallyCopyAssignableV<T>;

	// MoveAssignable
	template<typename T>
	inline constexpr bool IsMoveAssignableV = std::is_move_assignable_v<T>;

	template<typename T>
	struct IsMoveAssignable : BoolConstant<IsMoveAssignableV<T>> {};

	template<typename T>
	concept MoveAssignableC = IsMoveAssignableV<T>;

	// NoThrowMoveAssignable
	template<typename T>
	inline constexpr bool IsNoThrowMoveAssignableV = std::is_nothrow_move_assignable_v<T>;

	template<typename T>
	struct IsNoThrowMoveAssignable : BoolConstant<IsNoThrowMoveAssignableV<T>> {};

	// TriviallyMoveAssignable
	template<typename T>
	inline constexpr bool IsTriviallyMoveAssignableV = std::is_trivially_move_assignable_v<T>;

	template<typename T>
	struct IsTriviallyMoveAssignable : BoolConstant<IsTriviallyMoveAssignableV<T>> {};

	template<typename T>
	concept TriviallyMoveAssignableC = IsTriviallyMoveAssignableV<T>;
#pragma endregion Assignable

#pragma region Trivially
	// TriviallyCopyable
	template<typename T>
	inline constexpr bool IsTriviallyCopyableV = std::is_trivially_copyable_v<T>;

	template<typename T>
	struct IsTriviallyCopyable : BoolConstant<IsTriviallyCopyableV<T>> {};

	template<typename T>
	concept TriviallyCopyableC = IsTriviallyCopyableV<T>;
	template<typename T>
	concept NoneTriviallyCopyableC = !IsTriviallyCopyableV<T>;

	template<typename T>
	inline constexpr bool IsTriviallyV = IsTriviallyConstructibleV<T> && IsTriviallyCopyableV<T>;
	template<typename T>
	concept TriviallyC = IsTriviallyV<T>;

	template<typename T>
	inline constexpr bool IsNoneTriviallyV = !IsTriviallyV<T>;
	template<typename T>
	concept NoneTriviallyC = IsNoneTriviallyV<T>;

	template<typename T>
	inline constexpr bool IsStandardLayoutV = std::is_standard_layout_v<T>;

	template<typename T>
	struct IsStandardLayout : BoolConstant<IsStandardLayoutV<T>> {};

	template<typename T>
	concept StandardLayoutC = IsStandardLayoutV<T>;
#pragma endregion Trivially

	// ---------------- IsPointer
	template<typename T>
	inline constexpr bool IsPointerV = std::is_pointer_v<T>;

	template<typename T>
	struct IsPointer : BoolConstant<IsPointerV<T>> {};

	template<typename T>
	concept PointerC = IsPointerV<T>;
	template<typename T>
	concept NonePointerC = !IsPointerV<T>;

	// ---------------- IsNullPointer
	template<typename T>
	inline constexpr bool IsNullPointerV = IsSameV<RemoveCVT<T>, NullptrT>;
	template<typename T>
	using IsNullPointer = BoolConstant<IsNullPointerV<T>>;
	template<typename T>
	concept NullPointerC = IsNullPointerV<T>;

	template<typename T>
	inline constexpr bool IsCharacterV = IsAnyOfV<RemoveCVT<T>, char, wchar_t, char8_t, char16_t, char32_t>;
	template<typename T>
	concept CharacterC = IsCharacterV<T>;
	template<typename T>
	concept NoneCharacterC = !IsCharacterV<T>;

	template<typename T>
	inline constexpr bool IsIntegerV = IsAnyOfV<RemoveCVT<T>, bool, char, signed char, unsigned char,
												wchar_t,
#ifdef __cpp_char8_t
												char8_t,
#endif // __cpp_char8_t
												char16_t, char32_t, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long>;

	template<typename T>
	inline constexpr bool IsFloatingPointV = std::is_floating_point_v<T>;

	template<typename T>
	inline constexpr bool IsArithmeticV = std::is_arithmetic_v<T>;

	template<typename T>
	inline constexpr bool IsNoneBoolIntegerV = IsIntegerV<T> && !IsSameV<T, bool>;

	template<typename T>
	inline constexpr bool IsFundamentalV = std::is_fundamental_v<T>;
	template<typename T>
	concept FundamentalC = IsFundamentalV<T>;

	template<typename T>
	inline constexpr bool IsCompoundV = std::is_compound_v<T>;
	template<typename T>
	concept CompoundC = IsCompoundV<T>;

	template<typename T>
	inline constexpr bool IsObjectV = std::is_object_v<T>;
	template<typename T>
	concept ObjectC = IsObjectV<T>;

#pragma region Arithmeticable
	template<typename Tx, typename Ty>
	concept AddableC = requires(Tx ValueX, Ty ValueY) { ValueX + ValueY; };

	template<typename Tx, typename Ty>
	inline constexpr bool IsAddableV = AddableC<Tx, Ty>;

	template<typename Tx, typename Ty>
	struct IsAddable : FalseType {};

	template<typename Tx, typename Ty> requires AddableC<Tx, Ty>
	struct IsAddable<Tx, Ty> : TrueType
	{
		using Type = decltype(DeclVal<Tx>() + DeclVal<Ty>());
	};

	template<typename Tx, typename Ty> requires AddableC<Tx, Ty>
	using IsAddableT = typename IsAddable<Tx, Ty>::Type;

	template<typename Tx, typename Ty>
	concept SubtractableC = requires(Tx ValueX, Ty ValueY) { ValueX - ValueY; };

	template<typename Tx, typename Ty>
	inline constexpr bool IsSubtractableV = SubtractableC<Tx, Ty>;

	template<typename Tx, typename Ty>
	struct IsSubtractable : FalseType {};

	template<typename Tx, typename Ty> requires SubtractableC<Tx, Ty>
	struct IsSubtractable<Tx, Ty> : TrueType
	{
		using Type = decltype(DeclVal<Tx>() - DeclVal<Ty>());
	};

	template<typename Tx, typename Ty> requires SubtractableC<Tx, Ty>
	using IsSubtractableT = typename IsSubtractable<Tx, Ty>::Type;

	template<typename Tx, typename Ty>
	concept MultiplicableC = requires(Tx ValueX, Ty ValueY) { ValueX * ValueY; };

	template<typename Tx, typename Ty>
	inline constexpr bool IsMultiplicableV = MultiplicableC<Tx, Ty>;

	template<typename Tx, typename Ty>
	struct IsMultiplicable : FalseType {};

	template<typename Tx, typename Ty> requires MultiplicableC<Tx, Ty>
	struct IsMultiplicable<Tx, Ty> : TrueType
	{
		using Type = decltype(DeclVal<Tx>() * DeclVal<Ty>());
	};

	template<typename Tx, typename Ty> requires MultiplicableC<Tx, Ty>
	using IsMultiplicableT = typename IsMultiplicable<Tx, Ty>::Type;

	template<typename Tx, typename Ty>
	concept DivisibleC = requires(Tx ValueX, Ty ValueY) { ValueX / ValueY; };

	template<typename Tx, typename Ty>
	inline constexpr bool IsDivisibleV = DivisibleC<Tx, Ty>;

	template<typename Tx, typename Ty>
	struct IsDivisible : FalseType {};

	template<typename Tx, typename Ty> requires DivisibleC<Tx, Ty>
	struct IsDivisible<Tx, Ty> : TrueType
	{
		using Type = decltype(DeclVal<Tx>() / DeclVal<Ty>());
	};

	template<typename Tx, typename Ty> requires DivisibleC<Tx, Ty>
	using IsDivisibleT = typename IsDivisible<Tx, Ty>::Type;

	template<typename Tx, typename Ty = Tx>
	concept ArithmeticableC = requires(Tx ValueX, Ty ValueY)
	{
		ValueX + ValueY;
		ValueX - ValueY;
		ValueX * ValueY;
		ValueX / ValueY;
	};

#pragma endregion

#pragma region ArithmeticLogicable
	template<typename T>
	concept ArithmeticShiftableC = requires(T Value, int32 Bit)
	{
		{ Value >> Bit } -> ConvertibleToC<T>;
		{ Value << Bit } -> ConvertibleToC<T>;
		Value >>= Bit;
		Value <<= Bit;
	};
	template<typename T>
	inline constexpr bool IsArithmeticShiftableV = false;
	template<ArithmeticShiftableC T>
	inline constexpr bool IsArithmeticShiftableV<T> = true;

	template<typename T>
	concept ArithmeticOrAbleC = requires(T ValueA, T ValueB) { (ValueA | ValueB)->T; ValueA |= ValueB; };

	template<typename T>
	inline constexpr bool IsArithmeticOrAbleV = false;
	template<ArithmeticOrAbleC T>
	inline constexpr bool IsArithmeticOrAbleV<T> = true;

	template<typename T>
	concept ArithmeticAndAbleC = requires(T ValueA, T ValueB) { (ValueA & ValueB)->T; ValueA &= ValueB; };
	template<typename T>
	inline constexpr bool IsArithmeticAndAbleV = false;
	template<ArithmeticAndAbleC T>
	inline constexpr bool IsArithmeticAndAbleV<T> = true;

	template<typename T>
	concept ArithmeticXorAbleC = requires(T ValueA, T ValueB) { (ValueA ^ ValueB)->T; ValueA ^= ValueB; };
	template<typename T>
	inline constexpr bool IsArithmeticXorAbleV = false;
	template<ArithmeticXorAbleC T>
	inline constexpr bool IsArithmeticXorAbleV<T> = true;

	template<typename T>
	concept ArithmeticNotAbleC = requires(T ValueA, T ValueB) { (~ValueA)->T; };
	template<typename T>
	inline constexpr bool IsArithmeticNotAbleV = false;
	template<ArithmeticNotAbleC T>
	inline constexpr bool IsArithmeticNotAbleV<T> = true;

	template<typename T>
	concept ArithmeticLogicableC = requires(T ValueA, T ValueB)
	{
		ValueA | ValueB;
		ValueA |= ValueB;
		ValueA & ValueB;
		ValueA &= ValueB;
		ValueA ^ ValueB;
		ValueA ^= ValueB;
		~ValueA;
	};

	template<typename T>
	inline constexpr bool IsArithmeticLogicableV = false;
	template<ArithmeticLogicableC T>
	inline constexpr bool IsArithmeticLogicableV<T> = true;
#pragma endregion ArithmeticLogicable

#pragma region Signed
	template<typename T>
	inline constexpr bool IsSignedV = std::is_signed_v<T>;

	template<typename T>
	struct IsSigned : BoolConstant<IsSignedV<T>> {};

	template<typename T>
	concept SignedC = IsSignedV<T>;

	template<typename T>
	inline constexpr bool IsUnSignedV = std::is_unsigned_v<T>;

	template<typename T>
	struct IsUnSigned : BoolConstant<IsUnSignedV<T>> {};

	template<typename T>
	concept UnSignedC = IsUnSignedV<T>;

	template<typename T>
	struct MakeSigned
	{
		static_assert(IsNoneBoolIntegerV<T> || IsEnumV<T>,
			"MakeSigned<T> requires that T shall be a (possibly cv-qualified) "
			"integral type or enumeration but not a bool type.");
		using Type = std::make_signed_t<T>;
	};

	template<typename T>
	struct MakeUnsigned
	{
		static_assert(IsNoneBoolIntegerV<T> || IsEnumV<T>,
			"MakeUnsigned<T> requires that T shall be a (possibly cv-qualified) "
			"integral type or enumeration but not a bool type.");
		using Type = std::make_unsigned_t<T>;
	};

	template<typename T>
	using MakeSignedT = typename MakeSigned<T>::Type;
	template<typename T>
	using MakeUnsignedT = typename MakeUnsigned<T>::Type;

#pragma endregion Signed

	template<typename T>
	concept OppositableC = requires(T Value) { -Value; };
	template<typename T>
	inline constexpr bool IsOppositableV = false;
	template<OppositableC T>
	inline constexpr bool IsOppositableV<T> = true;

	template<typename T>
	struct IsOppositable
	{
		using Type = decltype(-(DeclVal<T>()));
	};

	template<OppositableC T>
	using IsOppositableT = typename IsOppositable<T>::Type;

	// ---------------- IntegerSequence
	template<typename T, T... Values>
	struct IntegerSequence
	{
		static_assert(IsIntegerV<T>, "IntegerSequence<T, I...> requires T to be an integral type.");
		using ValueType = T;

		static constexpr uintx Size = sizeof...(Values);
	};

	template<typename T, T Size>
	using MakeIntegerSequence = __make_integer_seq<IntegerSequence, T, Size>;

	template<uintx... Values>
	using IndexSequence = IntegerSequence<uintx, Values...>;

	template<uintx Size>
	using MakeIndexSequence = MakeIntegerSequence<uintx, Size>;

	template<typename... TypesT>
	using IndexSequenceFor = MakeIndexSequence<sizeof...(TypesT)>;

	// ---------------- IsFunction
	template<typename>
	struct IsFunction : FalseType {};

	template<typename ReturnType, typename... ArgsType>
	struct IsFunction<ReturnType(ArgsType...)> : public TrueType {};

	template<typename ReturnType, typename... ArgsType>
	struct IsFunction<ReturnType(ArgsType..., ...)> : public TrueType {};

	template<typename T>
	constexpr bool IsFunctionV = IsFunction<T>::Value;

	template<typename T>
	concept FunctionC = IsFunctionV<T>;

	// ---------------- is_signed
	//template<typename T, typename = void>
	//struct IsSigned : FalseType { };

	//template<typename T, typename = EnableIfT<IsIntegralV<T>>>
	//struct IsSigned : FalseType { };

	// ---------------- IsMemberObjectPointer
	template<typename>
	struct IsMemberFunctionPointer : FalseType { };

	template<typename ReturnT, typename InstanceT, typename... ParameterTs>
	struct IsMemberFunctionPointer<ReturnT(InstanceT::*)(ParameterTs...)> : TrueType
	{
		static inline constexpr sizet ParameterCount = sizeof...(ParameterTs);

		using InstanceType = InstanceT;
		using ReturnType = ReturnT;
		using ValueType = ReturnT;
		template<sizet Index>
		using ParameterType = PackIndexingT<Index, ParameterTs...>;

		using FunctorType = ReturnT(ParameterTs...);
	};

	template<typename ReturnT, typename InstanceT, typename... ParameterTs>
	struct IsMemberFunctionPointer<ReturnT(InstanceT::*)(ParameterTs...) const> : TrueType
	{
		static inline constexpr sizet ParameterCount = sizeof...(ParameterTs);

		using InstanceType = InstanceT;
		using ReturnType = ReturnT;
		using ValueType = ReturnT;
		template<sizet Index>
		using ParameterType = PackIndexingT<Index, ParameterTs...>;
		using FunctorType = ReturnT(ParameterTs...);
	};

	template<typename ReturnT, typename InstanceT, typename... ParameterTs>
	struct IsMemberFunctionPointer<ReturnT(InstanceT::*)(ParameterTs...) noexcept> : TrueType
	{
		static inline constexpr sizet ParameterCount = sizeof...(ParameterTs);

		using InstanceType = InstanceT;
		using ReturnType = ReturnT;
		using ValueType = ReturnT;
		template<sizet Index>
		using ParameterType = PackIndexingT<Index, ParameterTs...>;
		using FunctorType = ReturnT(ParameterTs...);
	};

	template<typename ReturnT, typename InstanceT, typename... ParameterTs>
	struct IsMemberFunctionPointer<ReturnT(InstanceT::*)(ParameterTs...) const noexcept> : TrueType
	{
		static inline constexpr sizet ParameterCount = sizeof...(ParameterTs);

		using InstanceType = InstanceT;
		using ReturnType = ReturnT;
		using ValueType = ReturnT;
		template<sizet Index>
		using ParameterType = PackIndexingT<Index, ParameterTs...>;
		using FunctorType = ReturnT(ParameterTs...);
	};

	template<typename T>
	inline constexpr bool IsMemberFunctionPointerV = IsMemberFunctionPointer<RemoveCVT<T>>::Value;
	template<typename T>
	concept MemberFunctionPointerC = IsMemberFunctionPointerV<T>;

	template<typename>
	struct IsMemberFieldPointer : FalseType {};

	template<typename FieldT, typename InstanceT>
	struct IsMemberFieldPointer<FieldT InstanceT::*> : TrueType
	{
		using InstanceType = InstanceT;
		using FieldType = FieldT;
		using ValueType = FieldT;
	};

	template<typename T>
	inline constexpr bool IsMemberFieldPointerV = IsMemberFieldPointer<RemoveCVT<T>>::Value;
	template<typename T>
	concept MemberFieldPointerC = IsMemberFieldPointerV<T>;

	template<typename T>
	inline constexpr bool IsMemberPointerV = IsMemberFunctionPointerV<T> || IsMemberFieldPointerV<T>;
	template<typename T>
	concept MemberPointerC = IsMemberPointerV<T>;

	template<MemberPointerC T>
	using IsMemberPointer = ConditionalT<IsMemberFunctionPointerV<T>, IsMemberFunctionPointer<T>, IsMemberFieldPointer<T>>;

	// ---------------- Decay
	template<typename T>
	struct Decay
	{
		using NoneReferenceType = RemoveRefT<T>;
		using TypeInner = ConditionalT<IsFunctionV<NoneReferenceType>, AddPointerT<NoneReferenceType>, RemoveCVT<NoneReferenceType>>;
		using Type = ConditionalT<IsArrayV<NoneReferenceType>, AddPointerT<RemoveExtentT<NoneReferenceType>>, TypeInner>;
	};

	template<typename T>
	using DecayT = typename Decay<T>::Type;

	template<typename T>
	constexpr bool IsLValueReferenceV = std::is_lvalue_reference_v<T>;

	template<typename T>
	concept LValueReferenceC = IsLValueReferenceV<T>;

	template<typename T>
	concept ConstLValueReferenceC = std::is_const_v<std::remove_reference_t<T>> && std::is_lvalue_reference_v<T>;

	template<typename T>
	constexpr bool IsRValueReferenceV = std::is_rvalue_reference_v<T>;

	template<typename T>
	concept RValueReferenceC = IsRValueReferenceV<T>;

	template<typename T>
	constexpr T && Forward(RemoveRefT<T> & Arg) noexcept
	{
		return static_cast<T&&>(Arg);
	}

	template<typename T>
	constexpr T && Forward(RemoveRefT<T> && Arg) noexcept
	{
		static_assert(!IsLValueReferenceV<T>, "bad forward call");
		return static_cast<T&&>(Arg);
	}

	// ---------------- Conjunction
	template<bool FirstValue, typename FirstType, typename... RestTypes>
	struct ConjunctionHelper
	{
		using Type = FirstType;
	};

	template<typename TrueT, typename NextT, typename... RestsT>
	struct ConjunctionHelper<true, TrueT, NextT, RestsT...>
	{
		using Type = typename ConjunctionHelper<NextT::Value, NextT, RestsT...>::Type;
	};

	template<typename... Traits>
	struct Conjunction : TrueType {};

	template<typename FirstType, typename... RestTypes>
	struct Conjunction<FirstType, RestTypes...> : ConjunctionHelper<FirstType::Value, FirstType, RestTypes...>::Type { };

	template<typename... Traits>
	constexpr bool ConjunctionV = Conjunction<Traits...>::Value;


	// ---------------- IsComplete
	inline namespace Private
	{
		template<typename T, std::size_t = sizeof(T)>
		TrueType IsCompletePrivate(T *);

		FalseType IsCompletePrivate(...);
	}

	template<typename T>
	using IsComplete = decltype(IsCompletePrivate(std::declval<T *>()));

	template<typename T>
	constexpr bool IsCompleteV = IsComplete<T>::Value;

	template<typename T>
	concept IntegerC = IsIntegerV<T>;

	template<typename T>
	concept NoneBoolIntegerC = IsNoneBoolIntegerV<T>;

	template<typename T>
	concept FloatingPointC = IsFloatingPointV<T>;

	template<typename T>
	concept ArithmeticC = IsArithmeticV<T>;

	template<typename T>
	concept EnumC = IsEnumV<T>;

	template<typename T>
	concept ClassC = IsClassV<T>;

	template<typename T>
	concept EnumClassC = IsEnumClassV<T>;

	template<typename T>
	concept ScalarC = IsScalarV<T>;

	template<typename ContainerT>
	concept IterableC = requires(ContainerT Container)
	{
		Container.Begin();
		Container.End();
	};


	template<typename OutT, typename InT>
	forceinline OutT * AddressAs(InT & Source) noexcept
	{
		static_assert(sizeof(InT) == sizeof(OutT), "Input and output must have same size");
		static_assert(IsIntegerV<OutT>, "Tried to reinterpret memory as non-integral");
		return &reinterpret_cast<OutT&>(Source);
	}

	template<typename OutT, typename InT>
	forceinline OutT ValueAs(const InT & Source) noexcept
	{
		static_assert(sizeof(InT) == sizeof(OutT), "Input and output must have same size");
		static_assert(IsIntegerV<OutT> || IsFloatingPointV<OutT> || IsPointerV<OutT>, "Tried to reinterpret memory as non-integral");
		return *reinterpret_cast<const OutT *>(&Source);
	}


	template<IntegerC T>
	forceinline constexpr uint32 MakeFourCC(const T (& fourcc)[5])
	{
		static_assert(sizeof(T) == 1);
		return (static_cast<uint32>(fourcc[0])) |
			(static_cast<uint32>(fourcc[1]) << 8) |
			(static_cast<uint32>(fourcc[2]) << 16) |
			(static_cast<uint32>(fourcc[3]) << 24);
	}

	template<IntegerC T>
	forceinline constexpr uint32 MakeFourCC(T a, T b, T c, T d)
	{
		return (static_cast<uint32>(a)) |
			(static_cast<uint32>(b) << 8) |
			(static_cast<uint32>(c) << 16) |
			(static_cast<uint32>(d) << 24);
	}

	template<typename T>
	constexpr T BytesReverse(T Value)
	{
		//return std::byteswap(Value);

		static_assert(std::has_unique_object_representations_v<T>, "T may not have padding bits");
		auto ValueRepresentation = std::bit_cast<std::array<std::byte, sizeof(T)>>(Value);
		std::ranges::reverse(ValueRepresentation);
		return std::bit_cast<T>(ValueRepresentation);
	}

	template<typename T> requires IsFloatingPointV<T> || IsIntegerV<T>
	forceinline constexpr auto High(T Value)
	{
		if constexpr (sizeof(T) == 8)
		{
			uint64 UInt64Value = ValueAs<uint64, T>(Value);
			return uint32((UInt64Value >> 32) & 0xFFFFFFFFui64);
		}
		else if constexpr (sizeof(T) == 4)
		{
			uint32 UInt32Value = ValueAs<uint32, T>(Value);
			return uint16((UInt32Value >> 16) & 0xFFFFu);
		}
		else if constexpr (sizeof(T) == 2)
		{
			uint16 UInt16Value = ValueAs<uint16, T>(Value);
			return uint8((UInt16Value >> 8) & 0xFF);
		}
		else if constexpr (sizeof(T) == 1)
		{
			uint8 UInt8Value = ValueAs<uint8, T>(Value);
			return uint8((UInt8Value >> 4) & 0xF);
		}
		else
		{
			static_assert(FalseTypeV<T>, "Unsupported intergal value.");
			return 0;
		}
	}

	template<typename T> requires IsFloatingPointV<T> || IsIntegerV<T>
	forceinline constexpr auto Low(T Value)
	{
		if constexpr (sizeof(T) == 8)
		{
			uint64 UInt64Value = ValueAs<uint64, T>(Value);
			return uint32(UInt64Value & 0xFFFFFFFFui64);
		}
		else if constexpr (sizeof(T) == 4)
		{
			uint32 UInt32Value = ValueAs<uint32, T>(Value);
			return uint16(UInt32Value & 0xFFFFu);
		}
		else if constexpr (sizeof(T) == 2)
		{
			uint16 UInt16Value = ValueAs<uint16, T>(Value);
			return uint8(UInt16Value & 0xFF);
		}
		else if constexpr (sizeof(T) == 1)
		{
			uint8 UInt8Value = ValueAs<uint8, T>(Value);
			return uint8(UInt8Value & 0xF);
		}
		else
		{
			static_assert(FalseTypeV<T>, "Unsupported intergal value.");
			return 0;
		}
	}

	template<typename OutT, typename T>
	forceinline constexpr OutT HighAs(T Value)
	{
		return ValueAs<OutT>(High(Value));
	}

	template<typename OutT, typename T>
	forceinline constexpr OutT LowAs(T Value)
	{
		return ValueAs<OutT>(Low(Value));
	}

	template<typename ElementT, uintx ContainerSize>
	forceinline consteval uintx ArraySize(const ElementT (& )[ContainerSize]) noexcept
	{
		return ContainerSize;
	}

	template<typename ValueT>
	forceinline constexpr ValueT Min(const ValueT & Value) noexcept
	{
		return Value;
	}

	template<typename ValueT, typename ValueT2, typename... ArgsT>
	forceinline constexpr ValueT Min(const ValueT & Left, const ValueT2 & Right, ArgsT &&... Args) noexcept
	{
		return Min((Left < static_cast<ValueT>(Right) ? Left : static_cast<ValueT>(Right)), static_cast<ValueT>(Forward<ArgsT>(Args))...);
	}

	template<typename ValueT>
	forceinline constexpr ValueT Max(const ValueT & Value) noexcept
	{
		return Value;
	}

	template<typename ValueT, typename ValueT2, typename... ArgsT>
	forceinline constexpr ValueT Max(const ValueT & Left, const ValueT2 & Right, ArgsT &&... Args) noexcept
	{
		return Max((Left < static_cast<ValueT>(Right) ? static_cast<ValueT>(Right) : Left), static_cast<ValueT>(Forward<ArgsT>(Args))...);
	}

	template<IterableC ContainerT>
	forceinline constexpr auto Max(const ContainerT & Container)
	{
		auto IterBegin = Container.Begin();
		auto IterEnd = Container.Begin();
		if (IterBegin == IterEnd)
			*IterBegin;

		auto Iter = IterBegin;
		++IterBegin;
		while (IterBegin != IterEnd)
		{
			if (*IterBegin > *Iter)
				Iter = IterBegin;
			++IterBegin;
		}
		return *Iter;
	}

	template<IterableC ContainerT>
	forceinline constexpr auto Min(const ContainerT & Container)
	{
		auto IterBegin = Container.Begin();
		auto IterEnd = Container.Begin();
		if (IterBegin == IterEnd)
			*IterBegin;

		auto Iter = IterBegin;
		++IterBegin;
		while (IterBegin != IterEnd)
		{
			if (*IterBegin < *Iter)
				Iter = IterBegin;
			++IterBegin;
		}
		return *Iter;
	}

	template<FundamentalC ValueT>
	forceinline constexpr ValueT Clamp(const ValueT & Value, const ValueT & ValueMin, const ValueT & ValueMax) noexcept
	{
		if (Value < ValueMin)
			return ValueMin;
		if (ValueMax < Value)
			return ValueMax;
		return Value;
	}

	template<typename ValueT>
	forceinline constexpr bool IsBetween(const ValueT & Value, const ValueT & ValueMin, const ValueT & ValueMax) noexcept
	{
		return ValueMin <= Value && Value <= ValueMax;
	}

	template<typename ValueT>
	forceinline constexpr ValueT Saturate(const ValueT & Value)
	{
		return Value < ValueT(0) ? ValueT(0) : (ValueT(1) < Value ? ValueT(1) : Value);
	}

	template<typename ValueT>
	forceinline constexpr ValueT Lerp(const ValueT & ValueMin, const ValueT & ValueMax, float32 Inter)
	{
		return ValueMin + static_cast<ValueT>((ValueMax - ValueMin) * Inter);
	}

	template<typename T> requires (!IsConstV<T>)
	forceinline constexpr RemoveRefT<T> && Move(T && Arg) noexcept
	{
		return static_cast<RemoveRefT<T>&&>(Arg);
	}

	template<typename T>
	forceinline void Swap(T & Left, T & Right) noexcept
	{
		static_assert(IsNothrowMoveConstructibleV<T> && IsNoThrowMoveAssignableV<T>);

		T Temp = Move(Left);
		Left = Move(Right);
		Right = Move(Temp);
	}

	template<typename T, typename OtherT = T>
	forceinline T Exchange(T & Value, OtherT && NewValue) noexcept
	{
		static_assert(IsNothrowMoveConstructibleV<T> && IsNoThrowAssignableV<T&, OtherT>);
		T OldValue = static_cast<T&&>(Value);
		Value = static_cast<OtherT&&>(NewValue);
		return OldValue;
	}

	template<typename IterT>
	intx Distance(IterT First, IterT Last)
	{
		return std::distance(First, Last);
	}

	template<typename CharT>
	forceinline constexpr uintx Length(const CharT * String) noexcept
	{
		if (!String)
			return 0;

		uintx Result = 0;
		while (*String++)
			++Result;
		return Result;
	}

	template<typename CharT, typename IntergerT>
	IntergerT ToInterger(const CharT * String, uintx Size, int32 Radis = 0)
	{
		if (Size == UIntXMax)
			Size = Length(String);

		while (Size && *String == ' ')
		{
			++String;
			--Size;
		}

		IntergerT sign = 1;
		if (String[0] == L'-')
		{
			sign = (IntergerT)-1;
			++String;
		}

		if (!Size || !String)
			return 0;

		const CharT * Curr = String;
		if (Radis <= 0)
		{
			if (Curr[0] == '0' && Size >= 2)
			{
				if (Radis > 1)
				{
					Curr += 1;
				}
				else if (Curr[1] == 'b' || Curr[1] == 'B' || Curr[1] == '0')
				{
					Radis = 2;
					Curr += 2;
				}
				else if (Curr[1] == 'x' || Curr[1] == 'X')
				{
					Radis = 16;
					Curr += 2;
				}
				else if (Curr[1] == 'h' || Curr[1] == 'H')
				{
					Radis = 8;
					Curr += 2;
				}
				else if (Curr[1] == 'd' || Curr[1] == 'D')
				{
					Radis = 10;
					Curr += 2;
				}
				else
					Radis = 10;
			}
			else
				Radis = 10;
		}

		IntergerT integer = 0;
		IntergerT number = 0;
		CharT ch = 0;

		while (Size--)
		{
			ch = *Curr++;
			if (!ch)
				break;

			if (ch >= '0' && ch <= '9')
				number = ch - '0';
			else if (ch >= 'A' && ch <= 'Z')
				number = ch - 'A' + 10;
			else if (ch >= 'a' && ch <= 'z')
				number = ch - 'a' + 10;
			else
				break;

			if (number >= (IntergerT)Radis)
				break;

			integer = integer * (IntergerT)Radis + number;
		}
		return integer * sign;
	}

	template<typename CharT>
	bool TStringToBoolean(const CharT * String, uintx Length)
	{
		return false;
	}

	template<typename CharT, typename FloatingPointT>
	FloatingPointT ToFloatingPoint(const CharT * String, uintx Size = -1)
	{
		if (Size == UIntXMax)
			Size = Length(String);

		if (!Size)
			return FloatingPointT(0);

		const CharT * end = String + Size;
		bool sign_radix = true;
		bool sign_exp = true;
		// reading integer part.
		bool read_integer = true;
		// reading exp part.
		bool read_exp = false;

		// radix = integer + decimal.
		// integer part.
		FloatingPointT integer = 0;
		// decimal part.
		FloatingPointT decimal = 0;
		// exp part.
		int32 exp = 0;
		// use to caculate decimal.
		FloatingPointT decimal_index = FloatingPointT(1) / FloatingPointT(10);

		if (String[0] == '-')
		{
			sign_radix = false;
			++String;
		}
		else {}

		while (String < end)
		{
			CharT ch = *String++;
			if (ch == '-')
			{
				if (read_exp)
					sign_exp = false;
				else
					break;
			}
			else if (ch == '.')
			{
				read_integer = false;
			}
			else if (ch == 'E' || ch == 'e')
			{
				read_integer = false;
				read_exp = true;
			}
			else if ('0' <= ch && ch <= '9')
			{
				int32 number = (int32)(ch - '0');
				if (read_exp)
				{
					exp *= 10;
					exp += number;
				}
				else if (read_integer)
				{
					integer *= 10;
					integer += number;
				}
				else
				{
					decimal += number * decimal_index;
					decimal_index /= FloatingPointT(10);
				}
			}
			else
				break;
		}

		FloatingPointT result = (FloatingPointT)(integer + decimal);
		if (!sign_radix)
			result = -result;

		if (sign_exp)
		{
			for (int32 cnt = 0; cnt < exp; ++cnt)
				result *= 10;
		}
		else
		{
			for (int32 cnt = 0; cnt < exp; ++cnt)
				result /= FloatingPointT(10);
		}

		return result;
	}

	enum class EError : int
	{
		Generic = std::numeric_limits<int>::min(),
		Inner,
		Eof,
		Failed,
		Nullptr,
		Unreachable,
		OutOfMemory,
		OutOfBound,
		Args,
		Exists,
		Access,
		State,
		IO,
		Broken,

		Timeout,
		Cancel,

		BadData,
		BadFormat,

		NotNow,
		NotSupported,
		NotImplemented,
		NotFound,

		InvalidOperation,

		OK = 0,
		True,
		False,
		Pendding,
	};

	BASE_API const char8 * ErrorStr(EError err);

	template<FloatingPointC T>
	T MakeFloat(auto Sign, auto Exponent, auto Fraction);

	template<> inline float32 MakeFloat<float32>(uint32 Sign, uint32 Exponent, uint32 Fraction)
	{
		union
		{
			float32 Float32Value;
			struct
			{
#if XIN_WINDOWS
				uint32 Fraction : 23;
				uint32 Exponent : 8;
				uint32 Sign : 1;
#else
				uint32 Sign : 1;
				uint32 Exponent : 8;
				uint32 Fraction : 23;
#endif
			};
		}FloatBits;
		FloatBits.Sign = Sign;
		FloatBits.Exponent = Exponent;
		FloatBits.Fraction = Fraction;
		return FloatBits.Float32Value;
	}

	forceinline float32 MakeFloat32(uint32 Sign, uint32 Exponent, uint32 Fraction)
	{
		return MakeFloat<float32>(Sign, Exponent, Fraction);
	}

	forceinline bool IsNaN(float32 Value) { return std::isnan(Value); }
	forceinline bool IsNaN(float64 Value) { return std::isnan(Value); }

	template<typename T>
	forceinline const T & DefaultIfNaN(const T & Value, const T & DefaultValue)
	{
		return IsNaN(Value) ? DefaultValue : Value;
	}

	template<typename T>
	concept Iteraterable = requires(T Instance)
	{
		Instance.Begin();
		Instance.End();
	};

	template<typename T>
	struct TDeleterDefault
	{
		TDeleterDefault() = default;
		TDeleterDefault(const TDeleterDefault &) = default;
		TDeleterDefault & operator =(const TDeleterDefault &) = default;
		~TDeleterDefault() = default;

		forceinline void operator ()(T * Ptr)
		{
			delete Ptr;
		}
	};

	template<typename T>
	struct TDeleterDefault<T[]>
	{
		TDeleterDefault() = default;
		TDeleterDefault(const TDeleterDefault &) = default;
		TDeleterDefault & operator =(const TDeleterDefault &) = default;
		~TDeleterDefault() = default;

		forceinline void operator ()(T * Ptr)
		{
			delete[] Ptr;
		}
	};

	template<typename T>
	struct TEqual
	{
		forceinline constexpr bool operator ()(const T & Left, const T & Right) const { return Left == Right; }
	};

	template<typename T>
	struct TLess
	{
		forceinline constexpr bool operator ()(const T & Left, const T & Right) const { return Left < Right; }
	};

	template<typename T>
	struct TGreater
	{
		forceinline constexpr bool operator ()(const T & Left, const T & Right) const { return Left > Right; }
	};

	template<typename T>
	forceinline T Abs(const T & Value)
	{
		return Value < (T)0 ? -Value : Value;
	}

	struct FNullAddress
	{
		template<typename T>
		constexpr bool operator==(T Value) const
		{
			return Value == T(0);
		}

		template<typename T>
		constexpr operator T() const
		{
			return T(0);
		}
	};
	constexpr FNullAddress NullAddress;

#pragma warning(push)
#pragma warning(disable: 4245)
	struct FNullIndex
	{
		template<typename T>
		constexpr bool operator==(T Value) const
		{
			if constexpr (IsAnyOfV<T, int8, uint8, int16, uint16, int32, uint32, int64, uint64>)
				return Value == T(-1);
			return false;
		}

		template<typename T>
		constexpr operator T() const
		{
			return T(-1);
		}
	};
#pragma warning(pop)
	constexpr FNullIndex NullIndex;

	namespace Array
	{
		template<typename T, EqualityComparableWithC<T> Ty>
		forceinline constexpr uintx FindFirst(const T * Elements, uintx Count, Ty && Item)
		{
			for (uintx Index = 0; Index < Count; ++Index)
			{
				if (Elements[Index] == Item)
					return Index;
			}
			return NullIndex;
		}

		template<typename T>
		forceinline constexpr uintx FindFirst(const T * Elements, uintx Count, const T * Pattern, uintx PatternCount)
		{
			if (PatternCount > Count)
				return NullIndex;

			for (uintx Index = 0; Index <= Count - PatternCount; ++Index)
			{
				bool Match = true;
				for (uintx PatternIndex = 0; PatternIndex < PatternCount; ++PatternIndex)
				{
					if (Elements[Index + PatternIndex] != Pattern[PatternIndex])
					{
						Match = false;
						break;
					}
				}

				if (Match)
					return Index;
			}

			return NullIndex;
		}

		template<typename T>
		forceinline constexpr uintx FindFirstOf(const T * Elements, uintx Count, const T * Pattern, uintx PatternCount)
		{
			for (uintx Index = 0; Index < Count; ++Index)
			{
				for (uintx PatternIndex = 0; PatternIndex < PatternCount; ++PatternIndex)
				{
					if (Elements[Index] == Pattern[PatternIndex])
						return Index;
				}
			}
			return NullIndex;
		}

		template<typename T>
		forceinline constexpr uintx FindLast(const T * Elements, uintx Count, const T & Item)
		{
			for (uintx Index = Count; Index > 0; --Index)
			{
				if (Item == Elements[Index - 1])
					return Index;
			}
			return NullIndex;
		}

		template<typename T>
		forceinline constexpr uintx FindLast(const T * Elements, uintx Count, const T * Pattern, uintx PatternCount)
		{
			if (PatternCount > Count)
				return NullIndex;

			for (uintx Index = Count - PatternCount; Index > 0; --Index)
			{
				bool Match = true;
				for (uintx PatternIndex = 0; PatternIndex < PatternCount; ++PatternIndex)
				{
					if (Elements[Index + PatternIndex] != Pattern[PatternIndex])
					{
						Match = false;
						break;
					}
				}

				if (Match)
					return Index;
			}
			return NullIndex;
		}

		template<typename T>
		forceinline constexpr uintx FindLastOf(const T * Elements, uintx Count, const T * Pattern, uintx PatternCount)
		{
			for (uintx Index = Count; Index > 0; --Index)
			{
				for (uintx PatternIndex = 0; PatternIndex < PatternCount; ++PatternIndex)
				{
					if (Elements[Index - 1] == Pattern[PatternIndex])
						return Index - 1;
				}
			}
			return NullIndex;
		}

		template<typename T>
		forceinline constexpr bool StartsWith(const T * Elements, uintx Count, const T * Pattern, uintx PatternCount)
		{
			if (PatternCount > Count)
				return false;

			for (uintx Index = 0; Index < PatternCount; ++Index)
			{
				if (Elements[Index] != Pattern[Index])
					return false;
			}
			return true;
		}

		template<typename T>
		forceinline constexpr bool StartsWith(const T * Elements, uintx Count, const T & Item)
		{
			if (Count == 0)
				return false;

			return Elements[0] == Item;
		}

		template<typename T>
		forceinline constexpr bool EndsWith(const T * Elements, uintx Count, const T * Pattern, uintx PatternCount)
		{
			if (PatternCount > Count)
				return false;

			for (uintx Index = 0; Index < PatternCount; ++Index)
			{
				if (Elements[Count - PatternCount + Index] != Pattern[Index])
					return false;
			}
			return true;
		}

		template<typename T>
		forceinline constexpr bool EndsWith(const T * Elements, uintx Length, const T & Item)
		{
			if (Length == 0)
				return false;

			return Elements[Length - 1] == Item;
		}
	}

	template<typename T>
	concept ComparableC = requires(const T & a, const T & b)
	{
		a <=> b;
	};

	template<typename T>
	inline constexpr bool IsComparableV = false;

	template<ComparableC T>
	inline constexpr bool IsComparableV<T> = true;

	template<ComparableC T, ComparableC Ty = T>
	using CompareResultT = decltype(DeclVal<const RemoveRefT<T> &>() <=> DeclVal<const RemoveRefT<Ty> &>());

	template<ComparableC ...Ts>
	using CompareCategoryT = std::common_comparison_category_t<CompareResultT<Ts>...>;

	using CompareOrderingT = std::partial_ordering;

	template<typename T>
	forceinline CompareOrderingT Compare(const T & Lhs, const T & Rhs)
	{
		return Lhs <=> Rhs;
	}

	template<EnumC EnumT>
	forceinline CompareOrderingT Compare(const EnumT & Lhs, const EnumT & Rhs)
	{
		return UnderlyingTypeT<EnumT>(Lhs) <=> UnderlyingTypeT<EnumT>(Rhs);
	}

	template<FloatingPointC T>
	forceinline CompareOrderingT Compare(const T & Lhs, const T & Rhs)
	{
		if (Abs(Lhs - Rhs) < Epslion<T>)
			return CompareOrderingT::equivalent;
		else if (Lhs < Rhs)
			return CompareOrderingT::less;
		else
			return CompareOrderingT::greater;
	}

	//template<FloatingPointC T>
	//forceinline CompareOrderingT Compare(const T & Lhs, const T & Rhs)
	//{
	//	return Lhs <=> Rhs;
	//}

	template<ComparableC T0, ComparableC T1, ComparableC... Ts>
	forceinline CompareOrderingT Compare(const T0 & Lhs, const T1 & Rhs, Ts &&... Tails)
	{
		static_assert(sizeof...(Ts) % 2 == 0, "Ts must be even");
		if (auto Comp = Compare(Lhs, Rhs); Comp != 0)
			return Comp;
		return Compare(Forward<Ts>(Tails)...);
	}


	template<typename T>
	forceinline bool Equal(const T & Lhs, const T & Rhs)
	{
		return Lhs == Rhs;
	}

	template<FloatingPointC T>
	forceinline bool ApproximatelyEqual(const T & Lhs, const T & Rhs, const T & EPL = Epslion<T>)
	{
		return Abs(Lhs - Rhs) <= Max(Abs(Lhs), Abs(Rhs)) * EPL;
	}

	template<FloatingPointC T>
	forceinline bool EssentiallyEqual(const T & Lhs, const T & Rhs, const T & EPL = Epslion<T>)
	{
		return Abs(Lhs - Rhs) <= Min(Abs(Lhs), Abs(Rhs)) * EPL;
	}

	template<FloatingPointC T>
	forceinline bool Equal(const T & Lhs, const T & Rhs)
	{
		return !IsNaN(Lhs) && !IsNaN(Rhs) && EssentiallyEqual(Lhs, Rhs);
	}

	template<ComparableC T0, ComparableC T1, ComparableC... Ts>
	forceinline bool Equal(const T0 & Lhs, const T1 & Rhs, Ts &&... Tails)
	{
		static_assert(sizeof...(Ts) % 2 == 0, "Ts must be even");
		if (not Equal(Lhs, Rhs))
			return false;
		return Equal(Forward<Ts>(Tails)...);
	}

	template<typename T>
	struct TReferenceWrapper
	{
		using PointerT = T const *;

		T & Inner = *static_cast<T *>(nullptr);

		constexpr TReferenceWrapper() = default;
		constexpr TReferenceWrapper(nullptr_t) { }
		constexpr TReferenceWrapper(T & Inner) : Inner(Inner) {}
		constexpr TReferenceWrapper(const TReferenceWrapper &) noexcept = default;

		constexpr TReferenceWrapper & operator =(const TReferenceWrapper & That) noexcept
		{
			// R U OK !!!
			Pointer() = That.Pointer();
			return *this;
		}

		constexpr bool operator ==(const TReferenceWrapper & Other) const { return Pointer() == Other.Pointer(); }
		constexpr bool operator ==(const T & Other) const { return &Inner == &Other; }
		constexpr auto operator <=>(const TReferenceWrapper & Other) const { return Pointer() <=> Other.Pointer(); }
		constexpr auto operator <=>(const T & Other) const { return &Inner <=> &Other; }
		constexpr operator const T & () const { return Inner; }
		constexpr operator T & () { return Inner; }
		PointerT Pointer() const { return *static_cast<const PointerT *>(static_cast<const void *>(this)); }
		PointerT & Pointer() { return *static_cast<PointerT *>(static_cast<void *>(this)); }
		explicit operator bool() const noexcept { return Pointer() != nullptr; }
	};


	// DO not define special template of TAdapter out of namespace Xin!!!
	template<typename T, typename Ty>
	struct TAdapter {};

	// T -> Ty
	template<typename T, typename Ty>
	concept AdaptableC = requires(const T & Value)
	{
		{ TAdapter<T, Ty>()(Value) } -> ConvertibleToC<Ty>;
	};

	template<typename T, typename Ty>
	inline constexpr bool IsAdapterAbleV = false;

	template<typename T, typename Ty> requires AdaptableC<T, Ty>
	inline constexpr bool IsAdapterAbleV<T, Ty> = true;


	template<typename T, bool Const>
	struct TPointerIterator
	{
		using ElementT = ConditionalT<IsLValueReferenceV<T>, TReferenceWrapper<RemoveRefT<T>>, T>;
		using IteratorItemT = ConditionalT<Const, T const, T>;
		using IteratorElementT = ConditionalT<Const, ElementT const, ElementT>;

		IteratorElementT * Pointer;

		constexpr TPointerIterator(IteratorElementT * Pointer) noexcept : Pointer(Pointer) {}

		constexpr bool operator ==(TPointerIterator const &) const noexcept = default;
		constexpr auto operator <=>(TPointerIterator const & Other) const noexcept { return Pointer <=> Other.Pointer; }

		constexpr bool operator ==(TPointerIterator<T, !Const> const & Other) const noexcept { return Pointer == Other.Pointer; }
		constexpr auto operator <=>(TPointerIterator<T, !Const> const & Other) const noexcept { return Pointer <=> Other.Pointer; }

		constexpr TPointerIterator & operator ++() noexcept { ++Pointer; return *this; }
		constexpr TPointerIterator operator ++(int) noexcept { return { Pointer + 1 }; }

		constexpr TPointerIterator & operator --() noexcept { --Pointer; return *this; }
		constexpr TPointerIterator operator --(int)noexcept { return { Pointer - 1 }; }

		constexpr TPointerIterator & operator +=(intx Offset) noexcept { Pointer += Offset; return *this; }
		constexpr TPointerIterator & operator -=(intx Offset) noexcept { Pointer -= Offset; return *this; }

		constexpr IteratorItemT & operator *() const noexcept { return *Pointer; }
		//constexpr IteratorItemT const & operator *() const { return *Pointer; }
		constexpr IteratorElementT * operator ->() noexcept { return Pointer; }
		constexpr IteratorElementT const * operator ->() const noexcept { return Pointer; }
		constexpr intx operator -(TPointerIterator Other) const noexcept { return Pointer - Other.Pointer; }
		constexpr TPointerIterator operator +(intx Offset) const noexcept { return Pointer + Offset; }
		constexpr TPointerIterator operator -(intx Offset) const noexcept { return Pointer - Offset; }
	};


	template<typename T>
	struct TView
	{
	public:
		using ElementT = ConditionalT<IsLValueReferenceV<T>, TReferenceWrapper<RemoveRefT<T>>, T>;

		struct TReferenceIterator
		{
			const ElementT * Pointer = nullptr;

			constexpr TReferenceIterator(const ElementT * Pointer) : Pointer(Pointer) {}
			constexpr bool operator ==(const TReferenceIterator &) const = default;

			constexpr TReferenceIterator & operator ++()
			{
				++Pointer;
				return *this;
			}

			constexpr TReferenceIterator operator ++(int) { return { Pointer + 1 }; }
			constexpr T operator *() const { return Pointer->Inner; }
			constexpr T operator *() { return Pointer->Inner; }
		};
		using IteratorT = ConditionalT<IsLValueReferenceV<T>, TReferenceIterator, const ElementT *>;

	public:
		ElementT const * Data = nullptr;
		uintx Size = 0;

	public:
		constexpr TView() noexcept = default;
		constexpr TView(NullptrT) noexcept {}
		constexpr TView(const TView &) noexcept = default;
		constexpr TView(TView &&) noexcept = default;

		constexpr TView & operator =(const TView &) noexcept = default;
		constexpr TView & operator =(TView &&) noexcept = default;

		constexpr operator ElementT const * () const { return Data; }

		constexpr TView(TInitializerList<ElementT> InitializerList) noexcept : Data(InitializerList.begin()), Size(InitializerList.size()) {}

		constexpr TView(ElementT const * Data, uintx Size) noexcept requires (!CharacterC<T>) : Data(Data), Size(Size) {}

		constexpr TView(ElementT const * String, uintx Size) noexcept requires CharacterC<T> : Data(String), Size(Size == NullIndex ? Length(String) : Size) {}

		template<uintx ArraySize>
		constexpr TView(const ElementT (& String)[ArraySize]) noexcept requires (!CharacterC<T>) : Data(String), Size(ArraySize) {}

		template<uintx ArraySize>
		constexpr TView(const ElementT (& String)[ArraySize]) noexcept requires (CharacterC<T>) : Data(String), Size(ArraySize > 0 ? Length(Data) : ArraySize) {}


		template<CharacterC Ty> requires SameC<T, Ty>
		constexpr TView(Ty const * const & String) noexcept requires CharacterC<T> : Data(String), Size(Length(String)) {}

		explicit constexpr TView(const chara * String) noexcept requires SameC<T, char8>: Data(reinterpret_cast<const char8 *>(String)), Size(Length(String)) {}

		template<CharacterC Ty> requires SameC<T, Ty>
		constexpr TView(std::basic_string_view<Ty> String) noexcept : Data(String.data()), Size(String.size()) {}

		constexpr uintx GetSize() const noexcept { return Size; }
		constexpr ElementT const * GetData() const noexcept { return Data; }
		constexpr bool Empty() const noexcept { return Size == 0; }
		constexpr bool Valid() const noexcept { return Size != 0; }

		constexpr ElementT const & operator [](uintx Index) const noexcept { return Data[Index]; }

		constexpr void Reset() noexcept
		{
			Data = nullptr;
			Size = 0;
		}

		constexpr uintx FindFirst(const T & Ch) const noexcept
		{
			return Array::FindFirst<T>(Data, Size, Ch);
		}

		constexpr uintx FindFirst(TView Pattern) const noexcept
		{
			return Array::FindFirst<T>(Data, Size, Pattern.Data, Pattern.Size);
		}

		constexpr uintx FindFirstOf(TView Chs) const noexcept
		{
			return Array::FindFirstOf<ElementT>(Data, Size, Chs.Data, Chs.Size);
		}

		constexpr uintx FindLast(const ElementT & Ch) const noexcept
		{
			return Array::FindLast<T>(Data, Size, Ch);
		}

		constexpr uintx FindLast(TView Pattern) const noexcept
		{
			return Array::FindLast<ElementT>(Data, Size, Pattern.Data, Pattern.Size);
		}

		constexpr uintx FindLastOf(TView Chs) const noexcept
		{
			return Array::FindLastOf<ElementT>(Data, Size, Chs.Data, Chs.Size);
		}

		constexpr bool Contains(TView Pattern) const noexcept
		{
			return FindFirst(Pattern) != NullIndex;
		}

		constexpr bool Contains(const ElementT & Element) const noexcept
		{
			return FindFirst(Element) != NullIndex;
		}

		bool StartsWith(TView Pattern) const noexcept
		{
			return Array::StartsWith(Data, Size, Pattern.Data, Pattern.Size);
		}

		bool StartsWith(const T & Element) const noexcept
		{
			return Array::StartsWith(Data, Size, Element);
		}

		bool EndsWith(TView Pattern) const noexcept
		{
			return Array::EndsWith(Data, Size, Pattern.Data, Pattern.Size);
		}

		bool EndsWith(const ElementT & Element) const noexcept
		{
			return Array::EndsWith(Data, Size, Element);
		}

		constexpr TView SubView(uintx Index, uintx Count = NullIndex) const noexcept
		{
			if (Count == NullIndex)
			{
				if (Index >= Size)
					return TView();
				Count = Size - Index;
			}
			else if (Index + Count > Size)
				return TView();
			else {}

			return TView(Data + Index, Count);
		}

		explicit constexpr operator bool() const noexcept { return !!Size; }
		constexpr IteratorT Begin() const noexcept { return Data; }
		constexpr IteratorT End() const noexcept { return Data + Size; }

		constexpr const T & First() const noexcept { return Data[0]; }
		constexpr const T & Last() const noexcept { return Data[Size - 1]; }

		template<typename Ty> requires (CharacterC<T> && SameC<T, Ty>)
		constexpr operator std::basic_string_view<Ty>() const noexcept
		{
			return std::basic_string_view<Ty>(Data, Size);
		}

		constexpr TView TrimStart(TView TrimmedChars = WhiteSpaces) noexcept requires CharacterC<T>
		{
			uintx Index = 0;
			for (; Index < Size; ++Index)
			{
				if (TrimmedChars.FindFirst(Data[Index]) == NullIndex)
					break;
			}

			return SubView(Index);
		}

		constexpr TView TrimEnd(TView TrimmedChars = WhiteSpaces) noexcept requires CharacterC<T>
		{
			uintx Index = Size;
			for (; Index > 0; --Index)
			{
				if (TrimmedChars.FindFirst(Data[Index - 1]) == NullIndex)
					break;
			}

			return SubView(0, Index);
		}

		constexpr TView Trim(TView TrimmedChars = WhiteSpaces) noexcept requires CharacterC<T>
		{
			uintx IndexFrom = 0;
			for (; IndexFrom < Size; ++IndexFrom)
			{
				if (TrimmedChars.FindFirst(Data[IndexFrom]) == NullIndex)
					break;
			}

			uintx IndeTo = Size;
			for (; IndeTo > IndexFrom; --IndeTo)
			{
				if (TrimmedChars.FindFirst(Data[IndeTo - 1]) == NullIndex)
					break;
			}

			return SubView(IndexFrom, IndeTo - IndexFrom);
		}

	public:
		template<typename... ArgsT>
		TView Format(const ArgsT &... Args) requires CharacterC<T>;

	public: // Adapter
		template<AdaptableC<TView> Ty>
		TView & operator =(const Ty & Value)
		{
			*this = TAdapter<Ty, TView>()(Value);
			return *this;
		}

		template<AdaptableC<TView> Ty>
		TView(const Ty & Value)
		{
			*this = TAdapter<Ty, TView>()(Value);
		}

	public:
		static TView None;
		static TView WhiteSpaces;
	};

	template<typename T>
	TView<T> TView<T>::None = { };

	template<CharacterC T>
	TView(const T *) -> TView<T>;

	template<typename T>
	constexpr auto begin(TView<T> & View) { return View.Begin(); }

	template<typename T>
	constexpr auto end(TView<T> & View) { return View.End(); }

	template<typename T>
	constexpr auto begin(const TView<T> & View) { return View.Begin(); }

	template<typename T>
	constexpr auto end(const TView<T> & View) { return View.End(); }

	template<>
	TView<char> TView<char>::WhiteSpaces = " \t\n\r\0\x0b";

	template<>
	TView<wchar_t> TView<wchar_t>::WhiteSpaces = L" \t\n\r\0\x0b";

	template<>
	TView<char8_t> TView<char8_t>::WhiteSpaces = u8" \t\n\r\0\x0b";

	template<typename T>
	forceinline auto operator <=>(const TView<T> & Lhs, const TView<T> & Rhs)
	{
		return Compare(Lhs.Data, Rhs.Data, Lhs.Length, Rhs.Length) <=> 0;
	}

	template<typename T>
	forceinline bool operator ==(const TView<T> & Lhs, const TView<T> & Rhs)
	{
		return Compare(Lhs.Data, Rhs.Data, Lhs.Length, Rhs.Length) == 0;
	}

	template<typename CharT, uintx FixedSize>
	forceinline uintx Hash(TView<CharT> String)
	{
		return HashArray(String.Data, String.Length);
	}

	template<typename T>
	forceinline TView<byte> ViewBytes(const T & Data)
	{
		return TView<byte> { reinterpret_cast<const byte *>(&Data), sizeof(T) };
	}

	template<typename OutT, typename InT>
	forceinline TView<OutT> ReinterpretView(TView<InT> View)
	{
		return TView<OutT> { reinterpret_cast<const OutT *>(View.Data), View.Size * sizeof(InT) / sizeof(OutT) };
	}

	template<typename T>
	inline constexpr bool IsViewV = false;

	template<typename T>
	inline constexpr bool IsViewV<TView<T>> = true;

	template<typename T>
	concept ViewC = IsViewV<T>;

	template<ViewC T>
	using ViewElementT = typename T::ElementT;


	using FAStringV = TView<char>;
	using FWStringV = TView<wchar_t>;
	using FUStringV = TView<char8_t>;

	using FU16StringV = TView<char16_t>;
	using FU32StringV = TView<char32_t>;
	using FStringV = TView<char8>;

	inline std::string_view StdStringV(FAStringV String)
	{
		return { String.Data, String.Size };
	}

	inline std::string_view StdStringV(FUStringV String)
	{
		return { (const char *)String.Data, String.Size };
	}

	inline std::wstring_view StdStringV(FWStringV String)
	{
		return { String.Data, String.Size };
	}

	inline std::string StdString(FAStringV String)
	{
		return { String.Data, String.Size };
	}

	inline std::string StdString(FUStringV String)
	{
		return { (const char *)String.Data, String.Size };
	}

	inline std::wstring StdString(FWStringV String)
	{
		return { String.Data, String.Size };
	}

	constexpr TView<chara> operator ""V(const chara * Literal, uintx Length)
	{
		return Xin::TView<chara>(Literal, Length);
	}

	constexpr TView<charw> operator ""V(const charw * Literal, uintx Length)
	{
		return Xin::TView<charw>(Literal, Length);
	}

	constexpr TView<char8> operator ""V(const char8 * Literal, uintx Length)
	{
		return Xin::TView<char8>(Literal, Length);
	}

	constexpr TView<char16> operator ""V(const char16 * Literal, uintx Length)
	{
		return Xin::TView<char16>(Literal, Length);
	}

	constexpr TView<char32> operator ""V(const char32 * Literal, uintx Length)
	{
		return Xin::TView<char32>(Literal, Length);
	}

	template<CharacterC CharT, uintx FixedSize>
	struct TFixedString
	{
	public:
		using CharType = CharT;
		static constexpr uintx Size = FixedSize;

		constexpr TFixedString(const CharT * String)
		{
			for (uintx Index = 0; Index < FixedSize; ++Index)
				Data[Index] = String[Index];
			Data[FixedSize] = 0;
		}

		CharT Data[FixedSize + 1];

		constexpr operator FStringV() const { return { Data, FixedSize }; }
	};

	template<CharacterC CharT, uintx FixedSize>
	TFixedString(const CharT (& String)[FixedSize]) -> TFixedString<CharT, FixedSize - 1>;

	template<typename CharT = char8>
	TView<CharT> WhiteSpaces;

	template<>
	inline TView<chara> WhiteSpaces<chara> = " \t\n\r\0\x0b";
	template<>
	inline TView<charw> WhiteSpaces<charw> = L" \t\n\r\0\x0b";
	template<>
	inline TView<char8> WhiteSpaces<char8> = u8" \t\n\r\0\x0b";
	template<>
	inline TView<char16> WhiteSpaces<char16> = u" \t\n\r\0\x0b";
	template<>
	inline TView<char32> WhiteSpaces<char32> = U" \t\n\r\0\x0b";

	template<typename CharT>
	constexpr bool IsWhiteSpace(CharT Ch)
	{
		return Contains(WhiteSpaces<CharT>, Ch);
	}

	template<typename CharT>
	constexpr CharT ToUpper(CharT Ch)
	{
		return ('a' <= Ch && Ch <= 'z') ? (Ch - ('a' - 'A')) : Ch;
	}

	template<typename CharT>
	constexpr CharT ToLower(CharT Ch)
	{
		return ('A' <= Ch && Ch <= 'A') ? Ch + 32 : Ch;
	}

	template<typename CharT>
	TView<CharT> Trim(TView<CharT> String)
	{
		for (uintx Index = 0; Index < String.Length; ++Index)
		{
			if (!IsWhiteSpace(String.Data[Index]))
			{
				String.Length -= Index;
				String.Data += Index;
				break;
			}
		}

		for (uintx Index = 0; Index < String.Size; ++Index)
		{
			if (!IsWhiteSpace(String.Data[String.Size - 1 - Index]))
			{
				String.Size -= Index;
				break;
			}
		}

		return String;
	}

	template<typename CharT>
	constexpr int32 StringCompare(const CharT * StringLeft, uintx LengthLeft, const CharT * StringRight, uintx LengthRight)
	{
		if (LengthLeft == NullIndex) LengthLeft = Length<CharT>(StringLeft);
		if (LengthRight == NullIndex) LengthRight = Length<CharT>(StringRight);

		if (!LengthLeft && !LengthRight)
			return 0;

		if (!LengthLeft)
			return -1;

		if (!LengthRight)
			return 1;

		if constexpr (IsAnyOfV<CharT, char, char8_t>)
			return strncmp((const char *)StringLeft, (const char *)StringRight, Max(LengthLeft, LengthRight));
		else if constexpr (IsAnyOfV<CharT, wchar_t, char16_t>)
			return wcsncmp((const wchar_t *)StringLeft, (const wchar_t *)StringRight, Max(LengthLeft, LengthRight));
		else
			throw EError::NotImplemented;
	}

	template<typename CharT = char8>
	int32 StringCompareIC(const CharT * StringLeft, uintx LengthLeft, const CharT * StringRight, uintx LengthRight)
	{
		if (LengthLeft == NullIndex) LengthLeft = Length<CharT>(StringLeft);
		if (LengthRight == NullIndex) LengthRight = Length<CharT>(StringRight);

		if (!LengthLeft && !LengthRight)
			return 0;

		if (!LengthLeft)
			return -1;

		if (!LengthRight)
			return 1;

		if constexpr (IsAnyOfV<CharT, char, char8_t>)
			return _strnicmp((const char *)StringLeft, (const char *)StringRight, Max(LengthLeft, LengthRight));
		else if constexpr (IsAnyOfV<CharT, wchar_t, char16_t>)
			return _wcsnicmp((const wchar_t *)StringLeft, (const wchar_t *)StringRight, Max(LengthLeft, LengthRight));
		else
			throw EError::NotImplemented;
	}

	template<CharacterC CharT>
	int32 StringCompare(TView<CharT> Left, TView<CharT> Right)
	{
		return StringCompare<CharT>(Left.Data, Left.Size, Right.Data, Right.Size);
	}

	template<CharacterC CharT = char8>
	int32 StringCompareIC(TView<CharT> Left, TView<CharT> Right)
	{
		return StringCompareIC<CharT>(Left.Data, Left.Size, Right.Data, Right.Size);
	}

	template<CharacterC CharT>
	bool StringEqual(TView<CharT> Left, TView<CharT> Right)
	{
		return StringCompare<CharT>(Left.Data, Left.Size, Right.Data, Right.Size) == 0;
	}

	template<CharacterC CharT = char8>
	bool StringEqualIC(TView<CharT> Left, TView<CharT> Right)
	{
		return StringCompareIC<CharT>(Left.Data, Left.Size, Right.Data, Right.Size) == 0;
	}
	

	template<CharacterC T>
	auto operator <=>(const TView<T> & Left, const TView<T> & Right)
	{
		return StringCompare<T>(Left.Data, Left.Size, Right.Data, Right.Size) <=> 0;
	}

	template<CharacterC T>
	bool operator ==(const TView<T> & Left, const TView<T> & Right)
	{
		return StringEqual<T>(Left, Right);
	}

	template<CharacterC T, uintx Length>
	bool operator ==(const TView<T> & Left, const T (& Right)[Length])
	{
		return StringEqual<T>(Left, TView<T> { Right, Length > 1 ? Length - 1 : 0 });
	}

	template<CharacterC T, uintx Length>
	bool operator ==(const T (& Left)[Length], const TView<T> & Right)
	{
		return StringEqual<T>(TView<T> { Left, Length > 1 ? Length - 1 : 0 }, Right) == 0;
	}

	template<typename T>
	struct TSlice
	{
	public:
		using ElementT = ConditionalT<LValueReferenceC<T>, TReferenceWrapper<RemoveRefT<T>>, T>;

	public:
		ElementT * Data = nullptr;
		uintx Size = 0;

		using Iterator = TPointerIterator<T, false>;
		using ConstIterator = TPointerIterator<T, true>;

	public:
		constexpr TSlice() = default;
		constexpr TSlice(NullptrT) {}
		constexpr TSlice(const TSlice &) = default;

		constexpr TSlice(ElementT * Data, uintx Size) requires (!CharacterC<T>) : Data(Data), Size(Size) {}

		constexpr TSlice(ElementT * String, uintx Length) requires CharacterC<T> : Data(String), Size(Length == NullIndex ? Xin::Length(String) : Length) {}

		constexpr TSlice(ElementT * String) requires CharacterC<T> : Data(String), Size(Length(String)) {}

		template<uintx Size>
		constexpr TSlice(ElementT(& Data)[Size]) : Data(Data), Size(Size) {}

		TSlice & operator =(const TSlice & That)
		{
			uintx Count = Size > That.Size ? That.Size : Size;
			for (uintx Index = 0; Index < Count; ++Index)
				Data[Index] = That.Data[Index];
			return *this;
		}

		TSlice & operator =(TView<T> View)
		{
			uintx Count = Size > View.Length ? View.Length : Size;
			for (uintx Index = 0; Index < Count; ++Index)
				Data[Index] = View.Data[Index];
			return *this;
		}

		uintx GetSize() const { return Size; }
		ElementT * GetData() const { return Data; }
		bool IsEmpty() const { return Size == 0; }
		const T & operator [](uintx Index) const { return Data[Index]; }
		T & operator [](uintx Index) { return Data[Index]; }

		uintx FindFirst(const T & Ch) const
		{
			return Array::FindFirst<T>(Data, Size, Ch);
		}

		uintx FindFirst(TView<T> Pattern) const
		{
			return Array::FindFirst<T>(Data, Size, Pattern.Data, Pattern.Length);
		}

		uintx FindFirstOf(TView<T> Chs) const
		{
			return Array::FindFirstOf<T>(Data, Size, Chs.Data, Chs.Length);
		}

		uintx FindLast(const T & Ch) const
		{
			return Array::FindLast<T>(Data, Size, Ch);
		}

		uintx FindLast(TView<T> Pattern) const
		{
			return Array::FindLast<T>(Data, Size, Pattern.Data, Pattern.Length);
		}

		uintx FindLastOf(TView<T> Chs) const
		{
			return Array::FindLastOf<T>(Data, Size, Chs.Data, Chs.Length);
		}

		bool StartsWith(TView<T> Pattern) const
		{
			return Array::StartsWith(Data, Size, Pattern.Data, Pattern.Length);
		}

		bool StartsWith(const T & Ch) const
		{
			return Array::StartsWith(Data, Size, Ch);
		}

		bool EndsWith(TView<T> Pattern) const
		{
			return Array::EndsWith(Data, Size, Pattern.Data, Pattern.Length);
		}

		bool EndsWith(const T & Ch) const
		{
			return Array::EndsWith(Data, Size, Ch);
		}

		TSlice SubSlice(uintx Index, uintx Count = NullIndex) const
		{
			if (Count == NullIndex)
			{
				if (Index >= Size)
					return TSlice();
				Count = Size - Index;
			}
			else if (Index + Count > Size)
				return TSlice();
			else {}

			return TSlice(Data + Index, Count);
		}

		constexpr ConstIterator Begin() const { return Data; }
		constexpr ConstIterator End() const { return Data + Size; }
		constexpr Iterator Begin() { return Data; }
		constexpr Iterator End() { return Data + Size; }

		constexpr const T & First() const { return Data[0]; }
		constexpr const T & Last() const { return Data[Size - 1]; }
		constexpr T & First() { return Data[0]; }
		constexpr T & Last() { return Data[Size - 1]; }

		operator TView<T>() const { return TView<T>(Data, Size); }
		//operator TInitializerList<T>() const { return TInitializerList<T>(Data, Data + Size); }

		template<typename = void> requires CharacterC<T>
		operator std::basic_string_view<T>() const
		{
			return std::basic_string_view<T>(Data, Size);
		}

		TSlice & Replace(T Pattern, T C)
		{
			for (uintx Index = 0; Index < Size; ++Index)
			{
				if (Data[Index] == Pattern)
					Data[Index] = C;
			}
			return *this;
		}

		TSlice TrimStart(TView<T> TrimmedChars = TView<T>::WhiteSpaces) requires IsCharacterV<T>
		{
			uintx Index = 0;
			for (; Index < Size; ++Index)
			{
				if (TrimmedChars.FindFirst(Data[Index]) == NullIndex)
					break;
			}

			return SubSlice(Index);
		}

		TSlice TrimEnd(TView<T> TrimmedChars = TView<T>::WhiteSpaces) requires IsCharacterV<T>
		{
			uintx Index = Size;
			for (; Index > 0; --Index)
			{
				if (TrimmedChars.FindFirst(Data[Index - 1]) == NullIndex)
					break;
			}

			return SubSlice(0, Index);
		}

		TSlice Trim(TView<T> TrimmedChars = TView<T>::WhiteSpaces) requires IsCharacterV<T>
		{
			uintx IndexFrom = 0;
			for (; IndexFrom < Size; ++IndexFrom)
			{
				if (TrimmedChars.FindFirst(Data[IndexFrom]) == NullIndex)
					break;
			}

			uintx IndeTo = Size;
			for (; IndeTo > IndexFrom; --IndeTo)
			{
				if (TrimmedChars.FindFirst(Data[IndeTo - 1]) == NullIndex)
					break;
			}

			return SubSlice(IndexFrom, IndeTo - IndexFrom);
		}

		template<typename... ArgsT>
		TView<T> Format(const ArgsT &... Args) requires CharacterC<T>;

	public:
		static TSlice None;
	};

	template<typename T>
	TSlice<T> TSlice<T>::None = {};

	template<CharacterC T>
	TSlice(const T *) -> TSlice<T>;

	template<typename T>
	forceinline auto operator <=>(const TSlice<T> & Lhs, const TSlice<T> & Rhs)
	{
		return StringCompare(Lhs.Data, Rhs.Data, Lhs.Length, Rhs.Length) <=> 0;
	}

	template<typename T>
	forceinline bool operator ==(const TSlice<T> & Lhs, const TSlice<T> & Rhs)
	{
		return StringCompare(Lhs.Data, Rhs.Data, Lhs.Length, Rhs.Length) == 0;
	}


	template<typename T>
	auto begin(TSlice<T> & Slice)
	{
		return Slice.Begin();
	}

	template<typename T>
	auto end(TSlice<T> & Slice)
	{
		return Slice.End();
	}

	template<typename T>
	auto begin(const TSlice<T> & Slice)
	{
		return Slice.Begin();
	}

	template<typename T>
	auto end(const TSlice<T> & Slice)
	{
		return Slice.End();
	}
}

namespace Xin
{
	template<typename T>
	inline T AlignWith(T Value, uintx Alignment)
	{
		static_assert(IsIntegerV<T>, "AlignWith must align with an intergral value.");
		return (T)(((uintx)Value + Alignment - 1) / Alignment * Alignment);
	}

	template<typename ToT, typename FromT>
	forceinline ToT Round(const FromT & Value)
	{
		return static_cast<ToT>(Value + FromT(0.5));
	}

	template<typename T>
	forceinline int8 RoundI8(const T & Value)
	{
		return Round<T, int8>(Value);
	}

	template<typename T>
	forceinline uint8 RoundU8(const T & Value)
	{
		return Round<uint8, T>(Value);
	}

	template<typename T>
	forceinline int16 RoundI16(const T & Value)
	{
		return Round<int16, T>(Value);
	}

	template<typename T>
	forceinline uint16 RoundU16(const T & Value)
	{
		return Round<uint16, T>(Value);
	}

	template<typename T>
	forceinline int32 RoundI32(const T & Value)
	{
		return Round<int32, T>(Value);
	}

	template<typename T>
	forceinline uint32 RoundU32(const T & Value)
	{
		return Round<uint32, T>(Value);
	}

	template<typename T>
	forceinline int64_t RoundI64(const T & Value)
	{
		return Round<int64_t, T>(Value);
	}

	template<typename T>
	forceinline uint64 RoundU64(const T & Value)
	{
		return Round<uint64, T>(Value);
	}

	template<typename T>
	forceinline float32 RoundF32(const T & Value)
	{
		return Round<float32, T>(Value);
	}

	template<typename T>
	forceinline float64 RoundF64(const T & Value)
	{
		return Round<float64, T>(Value);
	}

	template<IntegerC T>
	forceinline T Round(const T & Lhs, const T & Rhs)
	{
		return Lhs / Rhs;
	}

	template<IntegerC T>
	forceinline T RoundUp(const T & Lhs, const T & Rhs)
	{
		return (Lhs + Rhs - 1) / Rhs;
	}

	template<IntegerC T>
	forceinline void SetBitIndex(T & Value, uintx Index, bool Bit)
	{
		if (Bit)
			Value |= (T(1) << Index);
		else
			Value &= ~(T(1) << Index);
	}

	template<IntegerC T>
	forceinline bool GetBitAt(T Value, uintx Index)
	{
		return !!(Value & (T(1) << Index));
	}

	template<IntegerC T>
	forceinline constexpr T & SetBits(T & Value, T Bits, bool Set)
	{
		if (Set)
			Value |= Bits;
		else
			Value &= ~Bits;
		return Value;
	}

	template<EnumC T, typename FlagT> requires (ConvertibleToC<FlagT, UnderlyingTypeT<T>> || SameC<T, FlagT>)
	forceinline constexpr T & SetFlags(T & Value, FlagT Flag, bool Set)
	{
		using UnderlyingType = UnderlyingTypeT<T>;
		if (Set)
			Value = static_cast<T>(static_cast<UnderlyingType>(Value) | static_cast<UnderlyingType>(Flag));
		else
			Value = static_cast<T>(static_cast<UnderlyingType>(Value) & ~static_cast<UnderlyingType>(Flag));
		return Value;
	}

	template<IntegerC T>
	forceinline constexpr bool HasFlags(T Value, T Flag)
	{
		return (Value & Flag) == Flag;
	}

	template<EnumC T>
	forceinline constexpr bool HasFlags(T Value, T Flag)
	{
		using UnderlyingType = UnderlyingTypeT<T>;
		return (static_cast<UnderlyingType>(Value) & static_cast<UnderlyingType>(Flag)) != 0;
	}

	template<EnumC T>
	forceinline constexpr bool HasFlagAll(T Value, T Flag)
	{
		using UnderlyingType = UnderlyingTypeT<T>;
		return (static_cast<UnderlyingType>(Value) & static_cast<UnderlyingType>(Flag)) == static_cast<UnderlyingType>(Flag);
	}

	forceinline bool FloatEqual(float32 FloatA, float32 FloatB)
	{
		return Abs(FloatA - FloatB) < Float32Epsilon;
	}

	template<typename T, typename Ty>
	concept ImplicitPointerConvertibleC = SameC<T, Ty> || ConvertibleToC<T&, Ty&>;

	template<typename T, typename Ty>
	inline constexpr bool IsImplicitPointerConvertibleV = false;

	template<typename T, ImplicitPointerConvertibleC<T> Ty>
	inline constexpr bool IsImplicitPointerConvertibleV<T, Ty> = true;

	template<typename T, typename Ty>
	concept StaticPointerConvertibleC = SameC<T, Ty> || ConvertibleToC<T&, Ty&> || ConvertibleToC<Ty&, T&>;

	template<typename T, typename Ty>
	inline constexpr bool IsStaticPointerConvertibleV = false;

	template<typename T, StaticPointerConvertibleC<T> Ty>
	inline constexpr bool IsStaticPointerConvertibleV<T, Ty> = true;

	template<typename Ty, StaticPointerConvertibleC<Ty> T>
	Ty * StaticCast(T * Pointer)
	{
		return static_cast<Ty *>(Pointer);
	}

	template<typename Ty, StaticPointerConvertibleC<Ty> T>
	Ty & StaticCast(T & Pointer)
	{
		return static_cast<Ty &>(Pointer);
	}

	template<typename Ty, StaticPointerConvertibleC<Ty> T>
	const Ty * StaticCast(const T * Pointer)
	{
		return static_cast<const Ty *>(Pointer);
	}

	template<typename Ty, StaticPointerConvertibleC<Ty> T>
	const Ty & StaticCast(const T & Pointer)
	{
		return static_cast<const Ty &>(Pointer);
	}

	template<typename Ty, StaticPointerConvertibleC<Ty> T>
	Ty & StaticCastRef(T * Pointer)
	{
		return *static_cast<Ty *>(Pointer);
	}

	template<typename Ty, StaticPointerConvertibleC<Ty> T>
	const Ty & StaticCastRef(const T * Pointer)
	{
		return *static_cast<const Ty *>(Pointer);
	}

	template<typename Ty, typename T>
	Ty * InterpretCast(T * Pointer)
	{
		return reinterpret_cast<Ty *>(Pointer);
	}

	template<typename Ty, typename T>
	const Ty * InterpretCast(const T * Pointer)
	{
		return reinterpret_cast<const Ty *>(Pointer);
	}

	template<typename Ty, typename T>
	Ty & InterpretCastRef(T * Pointer)
	{
		return *reinterpret_cast<Ty *>(Pointer);
	}

	template<typename Ty, typename T>
	const Ty & InterpretCastRef(const T * Pointer)
	{
		return *reinterpret_cast<const Ty *>(Pointer);
	}

	template<typename T>
	T * ConstCast(const T * Pointer)
	{
		return const_cast<T *>(Pointer);
	}

	template<typename T>
	T & ConstCast(const T & Pointer)
	{
		return const_cast<T &>(Pointer);
	}

	template<typename T = int32, T Step = T { 1 }>
	struct IRange
	{
	public:
		struct Iterator
		{
		public:
			Iterator() = default;
			Iterator(const Iterator &) = default;
			Iterator(T Value_) : Value(Value_) {}
			Iterator & operator =(const Iterator &) = default;
			bool operator ==(const Iterator &) const = default;

			Iterator & operator ++()
			{
				Value += Step;
				return *this;
			}

			Iterator & operator --()
			{
				Value -= Step;
				return *this;
			}

			T & operator *() { return Value; }
			const T & operator *() const { return Value; }

			T Value;
		};

		IRange(T MaxValue_) : MinValue(0), MaxValue(MaxValue_) {}

		Iterator Begin() const { return MinValue; }
		Iterator End() const { return MaxValue; }

		Iterator begin() const { return MinValue; }
		Iterator end() const { return MaxValue; }

		T MinValue;
		T MaxValue;
	};

	template<typename T>
	struct TFormatter;

	template<typename T>
	struct TConverter;

	struct FTypeid;
	struct FType;

	template<typename T>
	FType & Typeof();

	template<typename T>
	struct TStaticType;

	static_assert(IsSameV<PackIndexingT<0, int, float, bool>, int>);
	static_assert(IsSameV<PackIndexingT<1, int, float, bool>, float>);
	static_assert(IsSameV<PackIndexingT<2, int, float, bool>, bool>);

	struct FSourceLocation
	{
		constexpr FSourceLocation() noexcept = default;

		const char * File = "";
		const char * Function = "";
		uint32 Line = 0;
		uint32 Column = 0;

		static consteval FSourceLocation Current(
			const char * File = __builtin_FILE(),
			const char * Function = __builtin_FUNCTION(),
			uint32 Line = __builtin_LINE(),
			uint32 Column = __builtin_COLUMN()) noexcept
		{
			FSourceLocation Result;
			Result.Line = Line;
			Result.Column = Column;
			Result.File = File;
			Result.Function = Function;
			return Result;
		}

		FStringV Format(FStringV Formal = u8"{}"V) const;
	};

	BASE_API void AssertFailed(TView<char8> Expresion, TView<char8> Message, FSourceLocation SourceLocation = FSourceLocation::Current());

	inline void AssertFailed(TView<char8> Message, FSourceLocation SourceLocation = FSourceLocation::Current())
	{
		AssertFailed(u8""V, Message, SourceLocation);
	}

	inline void Assert(bool Success, TView<char8> Expresion, TView<char8> Message, const FSourceLocation & SourceLocation = FSourceLocation::Current())
	{
		if (Success)
			return;

		AssertFailed(Expresion, Message, SourceLocation);
	}

	inline void Assert(bool Success, TView<char8> Message, const FSourceLocation & SourceLocation = FSourceLocation::Current())
	{
		if (Success)
			return;

		AssertFailed(u8""V, Message, SourceLocation);
	}
}


#ifdef XIN_DEBUG
#define AssertExpr(Exp) Assert((!!(Exp)), u8## #Exp ##V, u8""V, FSourceLocation::Current())
#define AssertFalse(Exp) Assert(false, u8## #Exp ##V, u8""V, FSourceLocation::Current())
#define AssertForaml(Exp, Formal, ...) Assert(!!(Exp), u8## #Exp ##V, Format(Formal, __VA_ARGS__), FSourceLocation::Current())
#define AssertReturn(Exp, ReturnValue) AssertExpr(Exp); if (!(Exp)) return ReturnValue
#else
#define AssertExpr(Exp) ((void)0)
#define AssertFalse(Exp) ((void)0)
#define AssertForaml(Exp, Formal, ...) ((void)0)
#define AssertReturn(Exp, ReturnValue) if (!(Exp)) return ReturnValue
#endif

#define EnsureExpr(Exp) Assert(!!(Exp), u8## #Exp ##V, u8""V, FSourceLocation::Current())
#define EnsureForaml(Exp, Formal, ...) Assert(!!(Exp), u8## #Exp ##V, Format(Formal, __VA_ARGS__), FSourceLocation::Current())

#include "Types_Math.inl"
#include "Types_Algorithm.inl"
#include "Types_Flag.inl"
#include "Types_Hash.inl"
#include "Types_Allocator.inl"
#include "Types_Atomic.inl"
