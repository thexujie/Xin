#pragma once

#include "Types.h"

namespace Xin
{
	template<typename T, typename = void>
	struct TFlagHelper
	{
		using UnderlyingT = T;
	};

	template<typename T>
	struct TFlagHelper<T, EnableIfT<IsEnumV<T>>>
	{
		using UnderlyingT = std::underlying_type_t<T>;
	};

	template<typename EnumT>
	class TEnumWrapper
	{
	public:
		using UnderlyingT = typename TFlagHelper<EnumT>::UnderlyingT;

		constexpr TEnumWrapper() = default;
		constexpr TEnumWrapper(const TEnumWrapper &) = default;
		constexpr TEnumWrapper & operator =(const TEnumWrapper &) = default;
		auto operator <=>(const TEnumWrapper &) const noexcept = default;

		constexpr TEnumWrapper(NullptrT) : Value(static_cast<EnumT>(0)) {}

		template<typename T, typename = EnableIfT<IsConvertibleToV<T, UnderlyingT> || IsSameV<T, EnumT>>>
		constexpr TEnumWrapper(T Value) : Value(static_cast<EnumT>(static_cast<UnderlyingT>(Value))) {}

		constexpr TEnumWrapper & operator =(EnumT Value)
		{
			this->Value = Value;
			return *this;
		}

		constexpr TEnumWrapper & Set(EnumT Value, bool Set)
		{
			if (Set)
				this->Value = static_cast<EnumT>(static_cast<UnderlyingT>(this->Value) | static_cast<UnderlyingT>(Value));
			else
				this->Value = static_cast<EnumT>(static_cast<UnderlyingT>(this->Value) & (~static_cast<UnderlyingT>(Value)));
			return *this;
		}

		constexpr TEnumWrapper & Clear()
		{
			Value = static_cast<EnumT>(0);
			return *this;
		}

		constexpr TEnumWrapper & Clear(EnumT Components)
		{
			Value = static_cast<EnumT>(static_cast<UnderlyingT>(Value) & (~static_cast<UnderlyingT>(Components)));
			return *this;
		}

		template<typename T, typename = EnableIfT<IsConvertibleToV<T, UnderlyingT> || IsSameV<T, EnumT>>>
		constexpr bool Any(const T & Component) const
		{
			return (static_cast<UnderlyingT>(Value) & static_cast<UnderlyingT>(Component)) != 0;
		}

		template<typename T, typename = EnableIfT<IsConvertibleToV<T, UnderlyingT> || IsSameV<T, EnumT>>>
		constexpr bool All(const T & Component) const
		{
			return (static_cast<UnderlyingT>(Value) & static_cast<UnderlyingT>(Component)) == static_cast<UnderlyingT>(Component);
		}

		constexpr TEnumWrapper operator ~() const
		{
			return static_cast<EnumT>(~static_cast<UnderlyingT>(Value));
		}

		template<typename T, typename = EnableIfT<IsConvertibleToV<T, UnderlyingT> || IsSameV<T, EnumT>>>
		constexpr TEnumWrapper operator |(const T & Component) const
		{
			return static_cast<EnumT>(static_cast<UnderlyingT>(Value) | static_cast<UnderlyingT>(Component));
		}

		template<typename T, typename = EnableIfT<IsConvertibleToV<T, UnderlyingT> || IsSameV<T, EnumT>>>
		constexpr TEnumWrapper operator &(const T & Component) const
		{
			return static_cast<EnumT>(static_cast<UnderlyingT>(Value) & static_cast<UnderlyingT>(Component));
		}

		template<typename T, typename = EnableIfT<IsConvertibleToV<T, UnderlyingT> || IsSameV<T, EnumT>>>
		constexpr TEnumWrapper & operator |=(const T & Component)
		{
			Value = static_cast<EnumT>(static_cast<UnderlyingT>(Value) | static_cast<UnderlyingT>(Component));
			return *this;
		}

		template<typename T, typename = EnableIfT<IsConvertibleToV<T, UnderlyingT> || IsSameV<T, EnumT>>>
		constexpr TEnumWrapper & operator &=(const T & Component)
		{
			Value = static_cast<EnumT>(static_cast<UnderlyingT>(Value) & static_cast<UnderlyingT>(Component));
			return *this;
		}

		template<typename T, typename = EnableIfT<IsConvertibleToV<T, UnderlyingT> || IsSameV<T, EnumT>>>
		constexpr TEnumWrapper operator ==(const T & Component) const { return static_cast<UnderlyingT>(Value) == static_cast<UnderlyingT>(Component); }

		template<typename T, typename = EnableIfT<IsConvertibleToV<T, UnderlyingT> || IsSameV<T, EnumT>>>
		constexpr TEnumWrapper operator !=(const T & Component) const { return static_cast<UnderlyingT>(Value) != static_cast<UnderlyingT>(Component); }

		constexpr operator UnderlyingT() const
		{
			return static_cast<UnderlyingT>(Value);
		}

		constexpr const EnumT & Get() const { return Value; }
		constexpr EnumT & Get() { return Value; }

		constexpr operator EnumT &() { return Value; }
		constexpr operator const EnumT &() const { return Value; }

		constexpr explicit operator bool() const { return !!static_cast<UnderlyingT>(Value); }

	public:
		EnumT Value = static_cast<EnumT>(0);
	};

	template<typename Enum>
	constexpr bool IsFlagEnabledValueV = IsCompleteV<TEnumWrapper<Enum>> && IsEnumV<Enum> && !IsConvertibleToV<Enum, int>;

	template<typename Enum>
	concept FlagEnabledEnumC = IsFlagEnabledValueV<Enum>;

	template<typename T>
	constexpr bool IsTFlagTypeV = false;
	template<typename T>
	constexpr bool IsTFlagTypeV<TEnumWrapper<T>> = true;
	template<typename T>
	concept TFlagTypeC = IsTFlagTypeV<T>;

	template<typename T>
	struct IsTFlagType {};

	template<typename T>
	struct IsTFlagType<TEnumWrapper<T>>
	{
		using Type = T;
	};

	//template<typename T>
	//struct THash<TFlag<T>>
	//{
	//	uintx operator ()(const TFlag<T> & Flag) const noexcept
	//	{
	//		return THash<T>()(Flag.Get());
	//	}
	//};


	template<EnumClassC EnumT>
	constexpr TEnumWrapper<EnumT> operator |(EnumT Flags, EnumT TestFlags)
	{
		using UnderlyingType = UnderlyingTypeT<EnumT>;
		return static_cast<EnumT>(static_cast<UnderlyingType>(Flags) | static_cast<UnderlyingType>(TestFlags));
	}
	template<EnumClassC EnumT>
	constexpr TEnumWrapper<EnumT> operator |(EnumT Flags, TEnumWrapper<EnumT> TestFlags)
	{
		using UnderlyingType = UnderlyingTypeT<EnumT>;
		return static_cast<EnumT>(static_cast<UnderlyingType>(Flags) | static_cast<UnderlyingType>(TestFlags.Value));
	}

	template<EnumClassC EnumT>
	constexpr TEnumWrapper<EnumT> operator &(EnumT Flags, EnumT TestFlags)
	{
		using UnderlyingType = UnderlyingTypeT<EnumT>;
		return static_cast<EnumT>(static_cast<UnderlyingType>(Flags) & static_cast<UnderlyingType>(TestFlags));
	}

	template<EnumClassC EnumT>
	constexpr TEnumWrapper<EnumT> operator &(EnumT Flags, TEnumWrapper<EnumT> TestFlags)
	{
		using UnderlyingType = UnderlyingTypeT<EnumT>;
		return static_cast<EnumT>(static_cast<UnderlyingType>(Flags) & static_cast<UnderlyingType>(TestFlags.Value));
	}

	template<EnumClassC EnumT>
	constexpr TEnumWrapper<EnumT> operator ^(EnumT Flags, EnumT TestFlags)
	{
		using UnderlyingType = UnderlyingTypeT<EnumT>;
		return static_cast<EnumT>(static_cast<UnderlyingType>(Flags) ^ static_cast<UnderlyingType>(TestFlags));
	}

	template<EnumClassC EnumT>
	constexpr TEnumWrapper<EnumT> operator ^(EnumT Flags, TEnumWrapper<EnumT> TestFlags)
	{
		using UnderlyingType = UnderlyingTypeT<EnumT>;
		return static_cast<EnumT>(static_cast<UnderlyingType>(Flags) ^ static_cast<UnderlyingType>(TestFlags.Value));
	}

	template<EnumClassC EnumT>
	constexpr EnumT & operator |=(EnumT & Flags, EnumT TestFlags)
	{
		using UnderlyingType = UnderlyingTypeT<EnumT>;
		return Flags = static_cast<EnumT>(static_cast<UnderlyingType>(Flags) | static_cast<UnderlyingType>(TestFlags));
	}

	template<EnumClassC EnumT>
	constexpr EnumT & operator &=(EnumT & Flags, EnumT TestFlags)
	{
		using UnderlyingType = UnderlyingTypeT<EnumT>;
		return Flags = static_cast<EnumT>(static_cast<UnderlyingType>(Flags) & static_cast<UnderlyingType>(TestFlags));
	}

	template<EnumClassC EnumT>
	constexpr EnumT & operator ^=(EnumT & Flags, EnumT TestFlags)
	{
		using UnderlyingType = UnderlyingTypeT<EnumT>;
		return Flags = static_cast<EnumT>(static_cast<UnderlyingType>(Flags) ^ static_cast<UnderlyingType>(TestFlags));
	}

	template<EnumClassC EnumT>
	constexpr TEnumWrapper<EnumT> operator ~(EnumT Flags)
	{
		using UnderlyingType = UnderlyingTypeT<EnumT>;
		return static_cast<EnumT>(~static_cast<UnderlyingType>(Flags));
	}

	template<EnumClassC EnumT, typename FlagT> requires (ConvertibleToC<FlagT, UnderlyingTypeT<EnumT>> || SameC<EnumT, FlagT>)
	constexpr bool AnyFlags(EnumT Flags, FlagT TestFlags)
	{
		using UnderlyingType = UnderlyingTypeT<EnumT>;
		return (static_cast<UnderlyingType>(Flags) & static_cast<UnderlyingType>(TestFlags)) != 0;
	}

	template<EnumClassC EnumT, typename FlagT> requires (ConvertibleToC<FlagT, UnderlyingTypeT<EnumT>> || SameC<EnumT, FlagT>)
	constexpr bool AllFlags(EnumT Flags, FlagT TestFlags)
	{
		using UnderlyingType = UnderlyingTypeT<EnumT>;
		return (static_cast<UnderlyingType>(Flags) & static_cast<UnderlyingType>(TestFlags)) == static_cast<UnderlyingType>(TestFlags);
	}

	constexpr bool AnyBits(uint32 Flags, uint32 Bits)
	{
		return (Flags & Bits) != 0;
	}

	constexpr bool AllBits(uint32 Flags, uint32 Bits)
	{
		return (Flags & Bits) == Bits;
	}
}
