#pragma once

#include "Basic.Types.h"

namespace Xin
{
	template<typename LowT, typename HighT = LowT>
	struct TDoubleInteger
	{
		static_assert (sizeof(LowT) == sizeof(HighT), "both types must have the same size");
		static_assert (!std::numeric_limits<LowT>::is_signed, "type of lower half must be unsigned");

	public:
		LowT LowValue;
		HighT HighValue;

		constexpr TDoubleInteger() : LowValue(LowT()), HighValue(HighT()) {}
		constexpr TDoubleInteger(const TDoubleInteger & That) : LowValue(That.LowValue), HighValue(That.HighValue) {}
		constexpr TDoubleInteger(const LowT & LowValue_, const HighT & HighValue_) : LowValue(LowValue_), HighValue(HighValue_) {}

#pragma warning(push)
#pragma warning(disable: 4293)

		TDoubleInteger(int8 Value) : LowValue(LowT(Value)), HighValue(HighT((Value < 0) ? -1 : 0)) {}
		TDoubleInteger(int16 Value) : LowValue(LowT(Value)), HighValue(sizeof(Value) > sizeof(LowT) ? HighT(Value >> (sizeof(LowT) * CHAR_BIT)) : HighT(Value < 0 ? -1 : 0)) {}
		TDoubleInteger(int32 Value) : LowValue(LowT(Value)), HighValue(sizeof(Value) > sizeof(LowT) ? HighT(Value >> (sizeof(LowT) * CHAR_BIT)) : HighT(Value < 0 ? -1 : 0)) {}
		TDoubleInteger(long32 Value) : LowValue(LowT(Value)), HighValue(sizeof(Value) > sizeof(LowT) ? HighT(Value >> (sizeof(LowT) * CHAR_BIT)) : HighT(Value < 0 ? -1 : 0)) {}
		TDoubleInteger(int64_t Value) : LowValue(LowT(Value)), HighValue(sizeof(Value) > sizeof(LowT) ? HighT(Value >> (sizeof(LowT) * CHAR_BIT)) : HighT(Value < 0 ? -1 : 0)) {}

		TDoubleInteger(uint8 Value) : LowValue(LowT(Value)), HighValue(HighT()) {}
		TDoubleInteger(uint16 Value) : LowValue(LowT(Value)), HighValue(sizeof(Value) > sizeof(LowT) ? (Value >> (sizeof(LowT) * CHAR_BIT)) : HighT()) {}
		TDoubleInteger(uint32 Value) : LowValue(LowT(Value)), HighValue(sizeof(Value) > sizeof(LowT) ? (Value >> (sizeof(LowT) * CHAR_BIT)) : HighT()) {}
		TDoubleInteger(ulong32 Value) : LowValue(LowT(Value)), HighValue(sizeof(Value) > sizeof(LowT) ? (Value >> (sizeof(LowT) * CHAR_BIT)) : HighT()) {}
		TDoubleInteger(uint64 Value) : LowValue(LowT(Value)), HighValue(sizeof(Value) > sizeof(LowT) ? (Value >> (sizeof(LowT) * CHAR_BIT)) : HighT()) {}

		TDoubleInteger(float Value) requires IsFloatingPointV<LowT>
			: LowValue(std::fmod(Value, static_cast<float>(LowT(~LowT())) + 1.0f)),
			HighValue((Value < 0.0f) ? (Value / (static_cast<float>(LowT(~LowT())) + 1.0f) - 1u) : (Value / (static_cast<float>(LowT(~LowT())) + 1.0f))) {}
		TDoubleInteger(double Value) requires IsFloatingPointV<LowT>
			: LowValue(std::fmod(Value, static_cast<double>(LowT(~LowT())) + 1.0)),
			HighValue((Value < 0.0) ? (Value / (static_cast<double>(LowT(~LowT())) + 1.0) - 1u) : (Value / (static_cast<double>(LowT(~LowT())) + 1.0))) {}
		TDoubleInteger(long double Value) requires IsFloatingPointV<LowT>
			: LowValue(std::fmod(Value, static_cast<long double>(LowT(~LowT())) + 1.0l)),
			HighValue((Value < 0.0l) ? (Value / (static_cast<long double>(LowT(~LowT())) + 1.0l) - 1u) : (Value / (static_cast<long double>(LowT(~LowT())) + 1.0l))) {}
#pragma warning(pop)

		// string constructor
		//  - skips std::isspace characters at string beginning
		//  - when radix is zero, is detected by following rules:
		//     "0" -> 8, "0x" -> 16, "0b" -> 2, otherwise 10
		explicit TDoubleInteger(const char * String, uint32 Radix = 0);

		// assignment operators
		//  - only between double_integers
		//    and unsigned version of this one (if this one is signed)
		TDoubleInteger & operator =(const TDoubleInteger & That) { LowValue = That.LowValue; HighValue = That.HighValue; return *this; }

		template<typename = std::enable_if<!std::is_same_v<LowT, HighT>>>
		TDoubleInteger & operator =(const TDoubleInteger<LowT, LowT> & That) { LowValue = That.LowValue; HighValue = That.HighValue; return *this; }

		// integer operators
		constexpr bool operator !() const { return !bool(*this); }
		constexpr explicit operator bool() const { return !!HighValue || !!LowValue; }
		constexpr operator TDoubleInteger<LowT, LowT>() const { return TDoubleInteger<LowT, LowT>(LowValue, HighValue); }

		TDoubleInteger operator +() const { return *this; };
		TDoubleInteger operator -() const { TDoubleInteger<LowT, HighT> Result = ~*this; ++Result; return Result; }
		TDoubleInteger operator ~() const { return TDoubleInteger<LowT, HighT>(~LowValue, ~HighValue); }

		TDoubleInteger operator ++(int)
		{
			TDoubleInteger<LowT, HighT> Previous = *this;
			++ *this;
			return Previous;
		}

		TDoubleInteger operator --(int)
		{
			TDoubleInteger<LowT, HighT> Previous = *this;
			-- * this;
			return Previous;
		}

		TDoubleInteger & operator ++()
		{
			++LowValue;
			if (!LowValue)
			{
				++HighValue;
			};
			return *this;
		}

		TDoubleInteger & operator --()
		{
			if (!LowValue)
			{
				--HighValue;
			};
			--LowValue;
			return *this;
		}

		TDoubleInteger & operator +=(const TDoubleInteger & Addend)
		{
			LowT PreviousLowValue = LowValue;
			LowValue += Addend.LowValue;
			HighValue += Addend.HighValue;
			HighValue += (LowValue < PreviousLowValue);
			return *this;
		}

		TDoubleInteger & operator -=(const TDoubleInteger & Subtraction)
		{
			return *this += -Subtraction;
		}

		TDoubleInteger & operator *=(const TDoubleInteger & Multiplier)
		{
			if (!Multiplier)
			{
				*this = TDoubleInteger<LowT, HighT>();
				return *this;
			};

			TDoubleInteger<LowT, HighT> Previous = *this;

			LowValue = LowT();
			HighValue = HighT();

			uint32 BitIndex = 0u;
			uint32 NumBits = CHAR_BIT * (sizeof(LowT) + sizeof(HighT));

			TDoubleInteger<LowT, HighT> MultiplierRemain = Multiplier;
			while (BitIndex < NumBits && !!MultiplierRemain)
			{
				if (!!(MultiplierRemain.LowValue & LowT(1u)))
				{
					*this += Previous << BitIndex;
				};

				MultiplierRemain >>= 1u;
				++BitIndex;
			};

			return *this;
		}

		TDoubleInteger & operator /=(const TDoubleInteger & Divisor)
		{
			*this = Divide(Divisor);
			return *this;
		}

		TDoubleInteger & operator %=(const TDoubleInteger & Divisor)
		{
			TDoubleInteger Remainder;
			Divide(Divisor, &Remainder);
			*this = Remainder;
			return *this;
		}

		TDoubleInteger & operator |=(const TDoubleInteger & Another) { LowValue |= Another.LowValue; HighValue |= Another.HighValue; return *this; }
		TDoubleInteger & operator &=(const TDoubleInteger & Another) { LowValue &= Another.LowValue; HighValue &= Another.HighValue; return *this; }
		TDoubleInteger & operator ^=(const TDoubleInteger & Another) { LowValue ^= Another.LowValue; HighValue ^= Another.HighValue; return *this; }

		TDoubleInteger & operator >>=(uint32 NumBits)
		{
			// discard upper bits of the shift count, just like shift for ints does
			NumBits &= (CHAR_BIT * (sizeof(LowT) + sizeof(HighT))) - 1u;

			if (NumBits >= CHAR_BIT * sizeof(LowT))
			{
				NumBits -= CHAR_BIT * sizeof(LowT);

				LowValue = HighValue;
				LowValue >>= NumBits;

				if constexpr(!std::numeric_limits<HighT>::is_signed)
				{
					HighValue = HighT();
				}
				else if (TDoubleInteger(LowT(), HighValue) < TDoubleInteger())
				{
					HighValue = ~HighT();
					LowValue |= Mask<LowT>(NumBits) << (CHAR_BIT * sizeof(LowT) - NumBits);
				}
				else
				{
					HighValue = HighT();
				}
			}
			else
			{
				LowValue >>= NumBits;
				LowValue |= (LowT(HighValue) & this->Mask<LowT>(NumBits))
					<< (CHAR_BIT * sizeof(LowT) - NumBits);
				HighValue >>= NumBits;
			}

			return *this;
		}

		TDoubleInteger & operator <<=(uint32 NumBits)
		{
			// discard upper bits of the shift count, just like shift for ints does
			NumBits &= (CHAR_BIT * (sizeof(LowT) + sizeof(HighT))) - 1u;

			if (NumBits >= CHAR_BIT * sizeof(LowT))
			{
				NumBits -= CHAR_BIT * sizeof(LowT);

				HighValue = LowValue;
				LowValue = LowT();
				HighValue <<= NumBits;
			}
			else
			{
				const uint32 rest = CHAR_BIT * sizeof(LowT) - NumBits;

				HighValue = (LowT(HighValue) << NumBits) | ((LowValue & (this->Mask<LowT>(NumBits) << rest)) >> rest);
				LowValue <<= NumBits;
			}

			return *this;
		}

		// division
		//  - returns result, second parameters (if provided) receives remainder
		//  - throws std::logic_error on division by zero
		TDoubleInteger Divide(const TDoubleInteger &, TDoubleInteger * = 0) const;

		// rendering
		const char * print(char * buffer, std::size_t length, uint32 radix = 10u) const;
		const char * c_str(uint32 radix = 10u) const; // thread unsafe!

		// internal implementation details and helpers follow
	private:
		template<typename T>
		static T Mask(uint32 MaskBits)
		{
			T Result = T();
			++Result;
			Result <<= MaskBits;
			--Result;
			return Result;
		}
	};

	template<typename LowT, typename HighT>
	bool operator <(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> & RValue) { return (LValue.HighValue == RValue.HighValue) ? (LValue.LowValue < RValue.LowValue) : (LValue.HighValue < RValue.HighValue); }
	template<typename LowT, typename HighT>
	bool operator >(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> & RValue) { return (RValue < LValue); }
	template<typename LowT, typename HighT>
	bool operator <=(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> &RValue) { return !(RValue < LValue); }
	template<typename LowT, typename HighT>
	bool operator >=(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> &RValue) { return !(LValue < RValue); }
	template<typename LowT, typename HighT>
	bool operator ==(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> & RValue) { return LValue.LowValue == RValue.LowValue && LValue.HighValue == RValue.HighValue; }
	template<typename LowT, typename HighT>
	bool operator !=(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> & RValue) { return LValue.LowValue != RValue.LowValue || LValue.HighValue != RValue.HighValue; }
	template<typename LowT, typename HighT>
	bool operator ||(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> & RValue) { return (LValue.HighValue || LValue.LowValue) || (RValue.HighValue || RValue.LowValue); }
	template<typename LowT, typename HighT>
	bool operator &&(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> & RValue) { return (LValue.HighValue || LValue.LowValue) && (RValue.HighValue || RValue.LowValue); }

	// Binary operator
	template<typename LowT, typename HighT>
	TDoubleInteger<LowT, HighT> operator +(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> & RValue) { return TDoubleInteger<LowT, HighT>(LValue) += RValue; }
	template<typename LowT, typename HighT>
	TDoubleInteger<LowT, HighT> operator -(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> & RValue) { return TDoubleInteger<LowT, HighT>(LValue) -= RValue; }
	template<typename LowT, typename HighT>
	TDoubleInteger<LowT, HighT> operator *(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> & RValue) { return TDoubleInteger<LowT, HighT>(LValue) *= RValue; }
	template<typename LowT, typename HighT>
	TDoubleInteger<LowT, HighT> operator /(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> & RValue) { return TDoubleInteger<LowT, HighT>(LValue) /= RValue; }
	template<typename LowT, typename HighT>
	TDoubleInteger<LowT, HighT> operator %(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> & RValue) { return TDoubleInteger<LowT, HighT>(LValue) %= RValue; }
	template<typename LowT, typename HighT>
	TDoubleInteger<LowT, HighT> operator |(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> & RValue) { return TDoubleInteger<LowT, HighT>(LValue) |= RValue; }
	template<typename LowT, typename HighT>
	TDoubleInteger<LowT, HighT> operator &(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> & RValue) { return TDoubleInteger<LowT, HighT>(LValue) &= RValue; }
	template<typename LowT, typename HighT>
	TDoubleInteger<LowT, HighT> operator ^(const TDoubleInteger<LowT, HighT> & LValue, const TDoubleInteger<LowT, HighT> & RValue) { return TDoubleInteger<LowT, HighT>(LValue) ^= RValue; }
	template<typename LowT, typename HighT>
	TDoubleInteger<LowT, HighT> operator <<(const TDoubleInteger<LowT, HighT> & Value, uint32 N) { return TDoubleInteger<LowT, HighT>(Value) <<= N; }
	template<typename LowT, typename HighT>
	TDoubleInteger<LowT, HighT> operator >>(const TDoubleInteger<LowT, HighT> & Value, uint32 N) { return TDoubleInteger<LowT, HighT>(Value) <<= N; }

	template<typename LowT, typename HighT>
	TDoubleInteger<LowT, HighT> TDoubleInteger<LowT, HighT>::Divide(const TDoubleInteger<LowT, HighT> & Divisor, TDoubleInteger<LowT, HighT> * Remainder) const
	{
		if (!Divisor)
			throw std::logic_error("div/0");

		// reset sign bits
		TDoubleInteger<LowT, HighT> Zero;
		TDoubleInteger<LowT, HighT> DivisorAbs = Divisor;
		TDoubleInteger<LowT, HighT> DividendAbs = *this;

		if (DivisorAbs < Zero)
			DivisorAbs = -DivisorAbs;

		if (DividendAbs < Zero)
			DividendAbs = -DividendAbs;

		// remainder only
		//  - shortcut, return zero
		if (DividendAbs < DivisorAbs)
		{
			if (Remainder)
				*Remainder = *this;
			return Zero;
		}

		TDoubleInteger<LowT, HighT> Reminder_;
		TDoubleInteger<LowT, HighT> Quotient;
		TDoubleInteger<LowT, HighT> One;
		++One;

		uint32 NumBits = CHAR_BIT * (sizeof(LowT) + sizeof(HighT));

		while (Reminder_ < DivisorAbs)
		{
			Reminder_ <<= 1u;
			if (!!(DividendAbs & (One << --NumBits)))
			{
				Reminder_.LowValue |= One.LowValue;
			}
		}

		while (true)
		{
			if (Reminder_ < DivisorAbs)
			{
				if (!NumBits) break;

				--NumBits;
				Reminder_ <<= 1u;

				if (!!(DividendAbs & (One << NumBits)))
				{
					Reminder_.LowValue |= One.LowValue;
				}
			}
			else
			{
				Reminder_ -= DivisorAbs;
				Quotient |= One << NumBits;
			}
		}

		// correct sign bits
		if ((Divisor < Zero) ^ (*this < Zero))
			Quotient = -Quotient;

		// results
		if (Remainder)
		{
			if (*this < Zero)
			{
				Reminder_ = -Reminder_;
			}
			*Remainder = Reminder_;
		}
		return Quotient;
	}

	template<typename LowT, typename HighT>
	TDoubleInteger<LowT, HighT>::TDoubleInteger(const char * String, uint32 Radix) : LowValue(LowT()), HighValue(HighT())
	{
		if (!String)
			return;

		TDoubleInteger<LowT, HighT> digit;
		TDoubleInteger<LowT, HighT> radix;
		bool minus = false;

		// skip blanks to first character
		while (std::isspace(*String)) ++String;

		if (*String == '-')
		{
			++String;
			minus = true;

			// skip optional spaces after minus
			while (std::isspace(*String)) ++String;
		};

		// if no radix was provided, autodetect
		if (Radix)
		{
			radix.LowValue = Radix;
		}
		else
		{
			if (*String == '0')
			{
				++String;
				radix.LowValue = 8u;

				if (*String == 'x' || *String == 'X')
				{
					++String;
					radix.LowValue = 16u;
				};
				if (*String == 'b' || *String == 'B')
				{
					++String;
					radix.LowValue = 2u;
				};
			}
			else
			{
				radix.LowValue = 10u;
			};
		};

		while (std::isxdigit(*String))
		{
			if (std::isdigit(*String))
			{
				digit.LowValue = static_cast<uint32>(*String - '0');
			}
			else if (std::islower(*String))
			{
				digit.LowValue = static_cast<uint32>(*String - 'a' + 10u);
			}
			else
			{
				digit.LowValue = static_cast<uint32>(*String - 'A' + 10u);
			};

			*this *= radix;
			*this += digit;
			++String;
		};

		if (minus)
		{
			*this = -*this;
		};
		return;
	};

	template<typename LowT, typename HighT>
	const char * TDoubleInteger<LowT, HighT>::print(char * buffer, std::size_t length, uint32 radix) const
	{
		if (!*this)
			return "0";

		if (radix < 2 || radix > 37)
			return "(inv radix)";

		if (length)
		{
			std::size_t i = length;
			buffer[--i] = '\0';

			TDoubleInteger<LowT, HighT> zero;
			TDoubleInteger<LowT, HighT> r;
			TDoubleInteger<LowT, HighT> x = *this;

			if (x < zero)
			{
				x = -x;
				if (x < zero)
				{
					buffer[0] = '-';
					TDoubleInteger<LowT, LowT>
						(LowValue, HighValue).print(buffer + 1, length - 1, radix);

					return buffer;
				};
			};

			while (i && !!x)
			{
				x = x.Divide(TDoubleInteger<LowT, HighT>(radix), &r);

				uint8 c = *reinterpret_cast<uint8 *>(&r);
				if (c < 10u)
					buffer[--i] = c + '0';
				else
					buffer[--i] = c + 'A' - 10u;
			};

			if (*this < zero)
			{
				if (i)
				{
					buffer[--i] = '-';
				}
				else
					return NULL;
			};

			if (!x)
			{
				std::memmove(buffer, buffer + i, length - i);
				return buffer;
			};
		};

		return NULL;
	};

	template<typename LowT, typename HighT>
	const char * TDoubleInteger<LowT, HighT>::c_str(uint32 radix) const
	{
		static char sz[CHAR_BIT * (sizeof(LowT) + sizeof(HighT)) + 2u];
		return this->print(sz, sizeof sz, radix);
	};
}

namespace std
{
	template<typename LowT, typename HighT>
	class numeric_limits<Xin::TDoubleInteger<LowT, HighT>>
	{
	public:
		static constexpr bool is_specialized = true;

		static Xin::TDoubleInteger<LowT, HighT> min() throw()
		{
			return Xin::TDoubleInteger<LowT, HighT>(std::numeric_limits<LowT>::min(),
				std::numeric_limits<HighT>::min());
		};

		static Xin::TDoubleInteger<LowT, HighT> max() throw()
		{
			return Xin::TDoubleInteger<LowT, HighT>(std::numeric_limits<LowT>::max(),
				std::numeric_limits<HighT>::max());
		};

		static constexpr int digits = std::numeric_limits<HighT>::digits + std::numeric_limits<LowT>::digits;
		static constexpr int digits10 = digits * 643 / 2136;

		static constexpr bool is_signed = std::numeric_limits<HighT>::is_signed;
		static constexpr bool is_integer = true;
		static constexpr bool is_exact = true;
		static constexpr int radix = 2;

		static Xin::TDoubleInteger<LowT, HighT> epsilon() throw()
		{
			return Xin::TDoubleInteger<LowT, HighT>();
		}

		static Xin::TDoubleInteger<LowT, HighT> round_error() throw()
		{
			return Xin::TDoubleInteger<LowT, HighT>();
		}

		static constexpr int min_exponent = 0;
		static constexpr int min_exponent10 = 0;
		static constexpr int max_exponent = 0;
		static constexpr int max_exponent10 = 0;

		static constexpr bool has_infinity = false;
		static constexpr bool has_quiet_NaN = false;
		static constexpr bool has_signaling_NaN = false;
		static constexpr bool has_denorm_loss = false;

		static Xin::TDoubleInteger<LowT, HighT> infinity() throw()
		{
			return Xin::TDoubleInteger<LowT, HighT>();
		}

		static Xin::TDoubleInteger<LowT, HighT> quiet_NaN() throw()
		{
			return Xin::TDoubleInteger<LowT, HighT>();
		}

		static Xin::TDoubleInteger<LowT, HighT> signaling_NaN() throw()
		{
			return Xin::TDoubleInteger<LowT, HighT>();
		}

		static Xin::TDoubleInteger<LowT, HighT> denorm_min() throw()
		{
			return Xin::TDoubleInteger<LowT, HighT>();
		}

		static constexpr bool is_iec559 = false;
		static constexpr bool is_bounded = true;
		static constexpr bool is_modulo = true;

		static constexpr bool traps = std::numeric_limits<HighT>::traps;
		static constexpr bool tinyness_before = false;
		static constexpr float_round_style round_style = round_indeterminate;
	};

	template<typename LowT, typename HighT>
	struct make_unsigned<Xin::TDoubleInteger<LowT, HighT>>
	{
		typedef Xin::TDoubleInteger<LowT, LowT> type;
	};

	template<typename LowT, typename HighT>
	struct is_integral<Xin::TDoubleInteger<LowT, HighT>> : std::integral_constant<bool, true> {};
}

namespace Xin
{
	typedef TDoubleInteger<uint64, int64_t> Int128;
	typedef TDoubleInteger<uint64, uint64> UInt128;

	typedef TDoubleInteger <UInt128, Int128>  Int256;
	typedef TDoubleInteger <UInt128, UInt128> UInt256;
}
