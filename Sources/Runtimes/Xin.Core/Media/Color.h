#pragma once

#include "Media.Types.h"

namespace Xin
{
#define COLORS_ITEM(Name, Value) Name = Value,
	enum class EColor : uint32
	{
#include "Colors.inl"
	};
#undef COLORS_ITEM


	template struct CORE_API TVec4<uint8>;

	class CORE_API FColor : public TVec4<uint8>
	{
	public:
		static FType & StaticType();

	public:
		FColor() = default;
		constexpr FColor(const FColor &) = default;
		FColor & operator =(const FColor &) = default;
		~FColor() = default;
		bool operator ==(const FColor &) const noexcept = default;
		auto operator <=>(const FColor &) const = default; // { return TVec4<uint8>::operator <=>(Another); }

		using TVec4::operator +;
		using TVec4::operator -;

		constexpr FColor(EColor Color) : FColor(uint32(Color)) {}
		constexpr FColor(uint8 R, uint8 G, uint8 B, uint8 A = 0xFF) : TVec4(R, G, B, A) {}
		constexpr FColor(TVec3<uint8> RGB, uint8 A = 0xFF) : TVec4(RGB.R, RGB.G, RGB.B, A) {}
		constexpr FColor(uint32 ARGB) :
			FColor(
				uint8((ARGB >> 16) & 0xFF),
				uint8((ARGB >> 8) & 0xFF),
				uint8((ARGB >> 0) & 0xFF),
				uint8((ARGB >> 24) & 0xFF)) {}

		constexpr uint32 ToARGB() const
		{
			return (W << 24) | (X << 16) | (Y << 8) | Z;
		}

		constexpr operator uint32() const { return ToARGB(); }
		constexpr explicit operator bool() const { return A > 0; }

		operator FVector3() const { return FVector3(R / 255.0f, G / 255.0f, B / 255.0f); }
		operator FVector4() const { return FVector4(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f); }
		operator Vec3F() const { return Vec3F(R / 255.0f, G / 255.0f, B / 255.0f); }
		operator Vec4F() const { return Vec4F(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f); }

		static FColor FromARGB(uint32 ARGB)
		{
			FColor Result;
			Result.A = (ARGB >> 24) & 0xFF;
			Result.R = (ARGB >> 16) & 0xFF;
			Result.G = (ARGB >> 8) & 0xFF;
			Result.B = (ARGB >> 0) & 0xFF;
			return Result;
		}

		friend uintx Hash(const FColor & Color)
		{
			return Hash(Color.ToARGB());
		}

	public:
		explicit FColor(FVariant Variant)
		{
			switch (Variant.Kind)
			{
			case EVariantKind::Integer:
			{
				uint32 ARGB = (uint32)Variant.IntegerValue;
				A = (ARGB >> 24) & 0xFF;
				R = (ARGB >> 16) & 0xFF;
				G = (ARGB >> 8) & 0xFF;
				B = (ARGB >> 0) & 0xFF;
				break;
			}
			case EVariantKind::Vec4U:
			{
				A = uint8(Variant.Vec4UValue.A);
				R = uint8(Variant.Vec4UValue.R);
				G = uint8(Variant.Vec4UValue.G);
				B = uint8(Variant.Vec4UValue.B);
				break;
			}
			case EVariantKind::Vec4I:
			{
				A = uint8(Variant.Vec4IValue.A);
				R = uint8(Variant.Vec4IValue.R);
				G = uint8(Variant.Vec4IValue.G);
				B = uint8(Variant.Vec4IValue.B);
				break;
			}
			default:
				break;
			}
		}

		operator FVariant () const { return operator uint32(); }
	};

	static_assert(sizeof(FColor) == sizeof(int));


	template struct CORE_API TVec4<float32>;

	class CORE_API FColorF : public TVec4<float32>
	{
	public:
		static FType & StaticType();

	public:
		FColorF() = default;
		constexpr FColorF(const FColorF &) = default;
		FColorF & operator =(const FColorF &) = default;
		~FColorF() = default;
		bool operator ==(const FColorF &) const noexcept = default;
		auto operator <=>(const FColorF &) const = default; // { return TVec4<float32>::operator <=>(Another); }

		using TVec4::operator +;
		using TVec4::operator -;

		constexpr FColorF(float32 R, float32 G, float32 B, float32 A = 1.0f) : TVec4(R, G, B, A) {}
		constexpr FColorF(TVec3<float32> RGB, float32 A = 1.0f) : TVec4(RGB.R, RGB.G, RGB.B, A) {}

		constexpr FColorF(EColor Color) : FColorF(uint32(Color)) {}
		constexpr FColorF(const FColor & Color) : TVec4(Color.R / 255.0f, Color.G / 255.0f, Color.B / 255.0f, Color.A / 255.0f) {}
		constexpr FColorF(uint8 R, uint8 G, uint8 B, uint8 A = 0xFF) : TVec4(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f) {}
		constexpr FColorF(TVec3<uint8> RGB, uint8 A = 0xFF) : TVec4(RGB.R / 255.0f, RGB.G / 255.0f, RGB.B / 255.0f, A / 255.0f) {}
		constexpr FColorF(uint32 ARGB) : FColorF( uint8((ARGB >> 16) & 0xFF), uint8((ARGB >> 8) & 0xFF), uint8((ARGB >> 0) & 0xFF), uint8((ARGB >> 24) & 0xFF)) {}

		constexpr explicit operator bool() const { return A > 0; }

		constexpr operator FVector3() const { return FVector3(R, G, B); }
		constexpr operator FVector4() const { return FVector4(R, G, B, A); }
		constexpr operator Vec3F() const { return Vec3F(R, G, B); }
		constexpr operator Vec4F() const { return Vec4F(R, G, B, A); }
		constexpr operator FColor() const { return FColor(uint8(R * 255.0f), uint8(G * 255.0f), uint8(B * 255.0f), uint8(A * 255.0f)); }

		static FColorF FromARGB(uint32 ARGB)
		{
			FColorF Result;
			Result.A = ((ARGB >> 24) & 0xFF) / 255.0f;
			Result.R = ((ARGB >> 16) & 0xFF) / 255.0f;
			Result.G = ((ARGB >> 8)  & 0xFF) / 255.0f;
			Result.B = ((ARGB >> 0)  & 0xFF) / 255.0f;
			return Result;
		}

		friend uintx Hash(const FColorF & Color)
		{
			return HashCombine(Color.R, Color.G, Color.B, Color.A);
		}

	public:
		explicit FColorF(FVariant Variant)
		{
			switch (Variant.Kind)
			{
			case EVariantKind::Integer:
			{
				uint32 ARGB = (uint32)Variant.IntegerValue;
				A = ((ARGB >> 24) & 0xFF) / 255.0f;
				R = ((ARGB >> 16) & 0xFF) / 255.0f;
				G = ((ARGB >> 8)  & 0xFF) / 255.0f;
				B = ((ARGB >> 0)  & 0xFF) / 255.0f;
				break;
			}
			case EVariantKind::Vec4U:
			{
				A = uint8(Variant.Vec4UValue.A) / 255.0f;
				R = uint8(Variant.Vec4UValue.R) / 255.0f;
				G = uint8(Variant.Vec4UValue.G) / 255.0f;
				B = uint8(Variant.Vec4UValue.B) / 255.0f;
				break;
			}
			case EVariantKind::Vec4I:
			{
				A = uint8(Variant.Vec4IValue.A) / 255.0f;
				R = uint8(Variant.Vec4IValue.R) / 255.0f;
				G = uint8(Variant.Vec4IValue.G) / 255.0f;
				B = uint8(Variant.Vec4IValue.B) / 255.0f;
				break;
			}
			default:
				break;
			}
		}

		operator FVariant () const { return operator Vec4F(); }
	};

	inline FColor Lerp(FColor ColorA, FColor ColorB, float32 Inter)
	{
		if (ColorA.A == 0)
		{
			return FColor
			{
				ColorB.R,
				ColorB.G,
				ColorB.B,
				Lerp(uint8(0), ColorB.A, Inter),
			};
		}
		else if (ColorB.A == 0)
		{
			return FColor
			{
				ColorA.R,
				ColorA.G,
				ColorA.B,
				Lerp(ColorA.A, uint8(0), Inter),
			};
		}
		else
			return FColor
			{
				Lerp(ColorA.R, ColorB.R, Inter),
				Lerp(ColorA.G, ColorB.G, Inter),
				Lerp(ColorA.B, ColorB.B, Inter),
				0xFF,
			};
	}

	inline FArchive & operator >>(FArchive & Serializer, FColor & Color)
	{
		Serializer >> Color.X;
		Serializer >> Color.Y;
		Serializer >> Color.Z;
		Serializer >> Color.W;
		return Serializer;
	}

	inline FArchive & operator >>(FArchive & Serializer, FColorF & Color)
	{
		Serializer >> Color.X;
		Serializer >> Color.Y;
		Serializer >> Color.Z;
		Serializer >> Color.W;
		return Serializer;
	}

	template<>
	struct CORE_API TFormatter<FColor>
	{
		FStringV Format(const FColor & Value, FStringV Formal = FStringV::None) const
		{
			if (Formal.Empty())
				Formal = u8"({}, {}, {}, {})"V;
			return Xin::Format(Formal, Value.A, Value.R, Value.G, Value.B);
		}

		FColor Parse(FStringV String) const
		{
			throw EError::NotImplemented;
		}
	};

	namespace Colors
	{
#define COLORS_ITEM(Name, Value) constexpr FColor Name { Value };
#include "Colors.inl"
#undef COLORS_ITEM
	}
	CORE_API FStringV ColorName(EColor Color);
	CORE_API EColor ColorValue(FStringV Name);
}
