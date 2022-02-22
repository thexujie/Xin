#pragma once

#include "Basic.Types.h"

// ---
#include "Locale.h"
#include "KeyCode.h"
#include "Enumerable.h"
#include "System.h"
#include "Random.h"
#include "Typeid.h"

#include "Pointer.h"
#include "UniquePtr.h"
#include "SharedPtr.h"

// --- Containers
#include "DoubleInterger.h"
#include "Array.h"
#include "Bits.h"
#include "Tuple.h"
#include "Pair.h"
#include "Set.h"
#include "List.h"
#include "LinkedList.h"
#include "Map.h"
#include "MultiMap.h"
#include "HashMap.h"

// --- 
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Thickness.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quat.h"
#include "Matrix.h"
#include "Matrix3x3.h"
#include "Transform.h"
#include "Affine.h"
#include "Fraction.h"

#include "Strings.h"
#include "Name.h"
#include "Text.h"
#include "Guid.h"

#include "Format.h"
#include "Formatter.h"
#include "Log.h"
#include "Encoding.h"

#include "Stream.h"
#include "Archive.h"

#include "Callable.h"
#include "Function.h"
#include "Event.h"
#include "Variant.h"
#include "Object.h"

namespace Xin
{
	template<typename CharT>
	TList<TView<CharT>> Split(TView<CharT> String, const CharT & Delimiter)
	{
		TList<TView<CharT>> Result;
		uintx Offset = 0;
		while (true)
		{
			TView<CharT> Token = GetLine<CharT>(String, Offset, Delimiter);
			if (!Token.Size)
				break;
			Result.Add(Token);
		}
		return Result;
	}

	template<typename CharT>
	TList<TView<CharT>> Split(TView<CharT> String, TView<CharT> Delimiters)
	{
		TList<TView<CharT>> Result;
		uintx Offset = 0;
		while (true)
		{
			TView<CharT> Token = GetLine<CharT>(String, Offset, Delimiters);
			if (!Token.Size)
				break;
			Result.Add(Token);
		}
		return Result;
	}

	template<typename T>
	struct TFormatter<TVec2<T>>
	{
		FStringV Format(const TVec2<T> & Value, FStringV Formal = FStringV::None) const
		{
			if (Formal.Empty())
				Formal = u8"{({}, {})}"V;
			return Xin::Format(Formal, Value.X, Value.Y);
		}

		TVec2<T> Parse(FStringV String) const
		{
			TList<FStringV> ComponentStrings = Split(String, char8(','));
			if (ComponentStrings.Size != 2)
				return {};

			TVec2<T> Value;
			Value.X = TConverter<T>().Convert(ComponentStrings[0]);
			Value.Y = TConverter<T>().Convert(ComponentStrings[1]);
			return Value;
		}
	};

	template<typename T>
	struct TFormatter<TVec3<T>>
	{
		FStringV Format(const TVec3<T> & Value, FStringV Formal = FStringV::None) const
		{
			if (Formal.Empty())
				Formal = u8"({}, {}, {})"V;
			return Xin::Format(Formal, Value.X, Value.Y, Value.Z);
		}

		TVec3<T> Parse(FStringV String) const
		{
			TList<FStringV> ComponentStrings = Split(String, char8(','));
			if (ComponentStrings.Size == 3)
			{
				TVec3<T> Value;
				Value.X = TConverter<T>().Convert(ComponentStrings[0]);
				Value.Y = TConverter<T>().Convert(ComponentStrings[1]);
				Value.Z = TConverter<T>().Convert(ComponentStrings[2]);
				return Value;
			}
			if (ComponentStrings.Size == 1)
			{
				TVec3<T> Value;
				Value.X = Value.Y = Value.Z = TConverter<T>().Convert(ComponentStrings[0]);
				return Value;
			}
			else
				return { };

		}
	};

	template<typename T>
	struct TFormatter<TVec4<T>>
	{
		FStringV Format(const TVec4<T> & Value, FStringV Formal = FStringV::None) const
		{
			if (Formal.Empty())
				Formal = u8"({}, {}, {}, {})"V;
			return Xin::Format(Formal, Value.X, Value.Y, Value.Z, Value.W);
		}

		TVec4<T> Parse(FStringV String) const
		{
			TList<FStringV> ComponentStrings = Split(String, char8(','));
			if (ComponentStrings.Size == 4)
			{
				TVec4<T> Value;
				Value.X = TConverter<T>().Convert(ComponentStrings[0]);
				Value.Y = TConverter<T>().Convert(ComponentStrings[1]);
				Value.Z = TConverter<T>().Convert(ComponentStrings[2]);
				Value.W = TConverter<T>().Convert(ComponentStrings[3]);
				return Value;
			}
			if (ComponentStrings.Size == 1)
			{
				TVec4<T> Value;
				Value.X = Value.Y = Value.Z = Value.W = TConverter<T>().Convert(ComponentStrings[0]);
				return Value;
			}
			else
				return {};

		}
	};
	template<typename T>
	struct TFormatter<TThickness<T>>
	{
		FStringV Format(const TThickness<T> & Value, FStringV Formal = FStringV::None) const
		{
			if (Formal.Empty())
				Formal = u8"({}, {}, {}, {})"V;
			return Xin::Format(Formal, Value.LeftTop, Value.Top, Value.Right, Value.Bottom);
		}

		TThickness<T> Parse(FStringV String) const
		{
			TList<FStringV> ComponentStrings = Split(String, char8(','));
			if (ComponentStrings.Size == 4)
			{
				TThickness<T> Value;
				Value.Left = TConverter<T>().Convert(ComponentStrings[0]);
				Value.Top = TConverter<T>().Convert(ComponentStrings[1]);
				Value.Right = TConverter<T>().Convert(ComponentStrings[2]);
				Value.Bottom = TConverter<T>().Convert(ComponentStrings[3]);
				return Value;
			}
			if (ComponentStrings.Size == 1)
			{
				TThickness<T> Value;
				Value.Left = Value.Top = Value.Right = Value.Bottom = TConverter<T>().Convert(ComponentStrings[0]);
				return Value;
			}
			else
				return { };
		}
	};

	template<typename T>
	struct TFormatter<TOptional<T>>
	{
		FStringV Format(const TOptional<T> & Value, FStringV Formal = FStringV::None) const
		{
			if (Formal.Empty())
				Formal = u8"{}"V;

			if (Value.HasValue)
				return Xin::Format(Formal, Value.Value);
			else
				return u8"'nullptr'"V;
		}

		TOptional<T> Parse(FStringV String) const
		{
			throw EError::NotImplemented;
		}
	};

	template<>
	struct THash<FVariant>
	{
	public:
		uint64 operator()(const FVariant & Value) const noexcept
		{
			uint64 Result = Xin::Hash((uint32)Value.VariantKind);
			switch (Value.Kind)
			{
			case EVariantKind::Integer: return HashAppend(Result, Value.IntegerValue);
			case EVariantKind::Float32: return HashAppend(Result, Value.Float32Value);
			case EVariantKind::Float64: return HashAppend(Result, Value.Float64Value);

			case EVariantKind::Name: return HashAppend(Result, Value.NameValue.Nameid.Identifier);
			case EVariantKind::Text: return HashAppend(Result, Value.TextValue.Nameid.Identifier);
			case EVariantKind::String: return HashAppend(Result, Value.StringValue);

			case EVariantKind::ReferPointer: return HashAppend(Result, Value.ReferPointerValue);
			default:
				return Result;
			}
		}
	};
}
