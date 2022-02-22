#pragma once

#include "Core.Types.h"

namespace Xin
{
	template<typename T, TFixedString Name>
	struct TNamedValue
	{
		constexpr TNamedValue() = default;
		constexpr TNamedValue(const TNamedValue &) = default;
		TNamedValue & operator =(const TNamedValue &) = default;
		bool operator ==(const TNamedValue &) const requires EqualableC<T> = default;
		auto operator <=>(const TNamedValue &) const noexcept requires ComparableC<T> = default;

		constexpr TNamedValue(T Value) : Value(Value) {}

		constexpr TNamedValue & operator =(T Value_)
		{
			Value = Value_;
			return *this;
		}

		constexpr TNamedValue operator +(TNamedValue Another) const requires ArithmeticC<T> { return Value + Another.Value; }
		constexpr TNamedValue operator -(TNamedValue Another) const requires ArithmeticC<T> { return Value - Another.Value; }
		constexpr TNamedValue operator *(TNamedValue Another) const requires ArithmeticC<T> { return Value * Another.Value; }
		constexpr TNamedValue operator /(TNamedValue Another) const requires ArithmeticC<T> { return Value / Another.Value; }

		constexpr void operator +=(TNamedValue Another) requires ArithmeticC<T> { Value += Another.Value; }
		constexpr void operator -=(TNamedValue Another) requires ArithmeticC<T> { Value -= Another.Value; }
		constexpr void operator *=(TNamedValue Another) requires ArithmeticC<T> { Value *= Another.Value; }
		constexpr void operator /=(TNamedValue Another) requires ArithmeticC<T> { Value /= Another.Value; }

		constexpr TNamedValue operator +(T Another) const requires ArithmeticC<T> { return Value + Another; }
		constexpr TNamedValue operator -(T Another) const requires ArithmeticC<T> { return Value - Another; }
		constexpr TNamedValue operator *(T Another) const requires ArithmeticC<T> { return Value * Another; }
		constexpr TNamedValue operator /(T Another) const requires ArithmeticC<T> { return Value / Another; }

		constexpr void operator +=(T Another) requires ArithmeticC<T> { Value += Another; }
		constexpr void operator -=(T Another) requires ArithmeticC<T> { Value -= Another; }
		constexpr void operator *=(T Another) requires ArithmeticC<T> { Value *= Another; }
		constexpr void operator /=(T Another) requires ArithmeticC<T> { Value /= Another; }

		constexpr TNamedValue operator ~() const requires IntegerC<T> { return ~Value; }
		constexpr TNamedValue operator |(TNamedValue Another) const requires IntegerC<T> { return Value | Another.Value; }
		constexpr TNamedValue operator &(TNamedValue Another) const requires IntegerC<T> { return Value & Another.Value; }
		constexpr void operator |=(TNamedValue Another) requires IntegerC<T> { Value |= Another.Value; }
		constexpr void operator &=(TNamedValue Another) requires IntegerC<T> { Value &= Another.Value; }

	public:
		T Value;
	};

	template<ArithmeticC IndexT, typename StructT>
	struct TIndexedStructure
	{
		constexpr TIndexedStructure() = default;
		constexpr TIndexedStructure(const TIndexedStructure &) = default;
		TIndexedStructure & operator =(const TIndexedStructure &) = default;
		bool operator ==(const TIndexedStructure & Another) const { return Value == Another.Value; }
		auto operator <=>(const TIndexedStructure & Another) const noexcept { return Value <=> Another.Value; }

		constexpr TIndexedStructure(const StructT & Struct, IndexT Value) : Struct(Struct), Value(Value) {}

	public:
		StructT Struct;
		IndexT Value;
	};

	template<typename WrapValueT, typename T>
	struct TWrappedValue
	{
	public:
		TWrappedValue(T & Value) : Value(Value) {}

		T & Value;
	};

	template<typename WrapValueT, typename T>
	IDataStream & operator >>(IDataStream & Stream, TWrappedValue<WrapValueT, T> && Wrapper)
	{
		WrapValueT WrapValue;
		Stream >> WrapValue;
		Wrapper.Value = static_cast<T>(WrapValue);
		return Stream;
	}

	template<typename WrapValueT, typename T>
	IDataStream & operator >>(IDataStream & Stream, TWrappedValue<WrapValueT, T> & Wrapper)
	{
		WrapValueT WrapValue;
		Stream >> WrapValue;
		Wrapper.Value = static_cast<T>(WrapValue);
		return Stream;
	}

	template<typename WrapValueT, typename T>
	IDataStream & operator <<(IDataStream & Stream, const TWrappedValue<WrapValueT, T> & Wrapper)
	{
		WrapValueT WrapValue = static_cast<WrapValueT>(Wrapper.Value);
		Stream << WrapValue;
		return Stream;
	}
}
