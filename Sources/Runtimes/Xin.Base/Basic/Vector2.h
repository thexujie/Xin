#pragma once

#include "Basic.Types.h"
#include "XMath.h"
#include "Vec2.h"

namespace Xin
{
	using FVector1 = float32;
	using float1 = FVector1;

	struct [[Meta(SystemType)]] FVector2
	{
		FVector2() = default;
		FVector2(const FVector2 &) = default;
		FVector2(const Vec2F & Vec2) : X(Vec2.X), Y(Vec2.Y) {}
		FVector2 & operator =(const FVector2 &) = default;
		~FVector2() = default;

		bool operator ==(const FVector2 & Another) const
		{
			return !!DirectX::XMVector2Equal(ToXMVector(), Another.ToXMVector());
		}
		auto operator <=>(const FVector2 & Another) const { return Compare(X, Another.X, Y, Another.Y); }

		constexpr FVector2(float32 InX, float32 InY) : X(InX), Y(InY) { }
		constexpr FVector2(const float32 (& FloatsIn)[2]) : X(FloatsIn[0]), Y(FloatsIn[1]) { }

		FVector2 & operator =(const float32 (& FloatsIn)[2])
		{
			X = FloatsIn[0];
			Y = FloatsIn[1];
			return *this;
		}

		FVector2 & operator =(const Vec2F & Vec2) { X = Vec2.X; Y = Vec2.Y; return *this; }
		operator Vec2F() const { return { X, Y }; }

		float32 & operator [](int32 Index) { return Floats[Index]; }
		const float32 & operator [](int32 Index) const { return Floats[Index]; }

		bool HasNan() const { return DirectX::XMVector2IsNaN(ToXMVector()); }

		FVector2 & Normalize() { return Set(DirectX::XMVector3Normalize(ToXMVector())); }
		FVector2 Normalized() const { return FVector2(DirectX::XMVector3Normalize(ToXMVector())); }

		float32 Length() const { return DirectX::XMVectorGetX(DirectX::XMVector2Length(ToXMVector())); }
		float32 LengthSquare() const { return DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(ToXMVector())); }
		float32 Dot(const FVector2 & Another) const { return DirectX::XMVectorGetX(DirectX::XMVector2Dot(ToXMVector(), Another.ToXMVector())); }
		FVector2 Cross(const FVector2 & Another) const { return DirectX::XMVector2Cross(ToXMVector(), Another.ToXMVector()); }

		FVector2 Abs() const { return DirectX::XMVectorAbs(ToXMVector()); }
		FVector2 Negate() const { return DirectX::XMVectorNegate(ToXMVector()); }
		FVector2 Reciprocal() const { return DirectX::XMVectorReciprocal(ToXMVector()); }

		FVector2 Add(const FVector2 & Another) const { return DirectX::XMVectorAdd(ToXMVector(), Another.ToXMVector()); }
		FVector2 Subtract(const FVector2 & Another) const { return DirectX::XMVectorSubtract(ToXMVector(), Another.ToXMVector()); }
		FVector2 Multiply(const FVector2 & Another) const { return DirectX::XMVectorMultiply(ToXMVector(), Another.ToXMVector()); }
		FVector2 Divide(const FVector2 & Another) const { return DirectX::XMVectorDivide(ToXMVector(), Another.ToXMVector()); }
		FVector2 MultiplyAdd(const FVector2 & VectorMultiply, const FVector2 & VectorAdd) const { return DirectX::XMVectorMultiplyAdd(ToXMVector(), VectorMultiply.ToXMVector(), VectorAdd.ToXMVector()); }

		FVector2 operator +() const { return *this; }
		FVector2 operator -() const { return Negate(); }

		FVector2 operator +(const FVector2 & Another) const { return Add(Another); }
		FVector2 & operator +=(const FVector2 & Another) { return Set(Add(Another)); }

		FVector2 operator -(const FVector2 & Another) const { return Subtract(Another); }
		FVector2 & operator -=(const FVector2 & Another) { return Set(Subtract(Another)); }

		FVector2 operator *(const FVector2 & Another) const { return Multiply(Another); }
		FVector2 & operator *=(const FVector2 & Another) { return Set(Multiply(Another)); }

		FVector2 operator /(const FVector2 & Another) const { return Divide(Another); }
		FVector2 & operator /=(const FVector2 & Another) { return Set(Divide(Another)); }

		FVector2 operator /(float32 Rate) const { return Divide(DirectX::XMVectorReplicate(Rate)); }
		FVector2 & operator /=(float32 Rate) { return Set(Divide(DirectX::XMVectorReplicate(Rate))); };
		FVector2 operator *(float32 Rate) const { return Multiply(DirectX::XMVectorReplicate(Rate)); }
		FVector2 & operator *=(float32 Rate) { return Set(Multiply(DirectX::XMVectorReplicate(Rate))); };

	public:
		union
		{
			struct
			{
				float32 X;
				float32 Y;
			};

			float32 Floats[2];
			int32 Ints[2];
			uint32 UInts[2];
		};

	public:
		FVector2(const DirectX::XMVECTOR & XMVector) { DirectX::XMStoreFloat2(reinterpret_cast<DirectX::XMFLOAT2 *>(this), XMVector); }

		FVector2 & Set(DirectX::XMVECTOR XMVector)
		{
			DirectX::XMStoreFloat2(reinterpret_cast<DirectX::XMFLOAT2 *>(this), XMVector);
			return *this;
		}

		FVector2 & operator =(DirectX::XMVECTOR XMVector) { return Set(XMVector); }
		operator DirectX::XMVECTOR() const { return DirectX::XMLoadFloat2(reinterpret_cast<const DirectX::XMFLOAT2 *>(this)); }
		DirectX::XMVECTOR ToXMVector() const { return DirectX::XMLoadFloat2(reinterpret_cast<const DirectX::XMFLOAT2 *>(this)); }

	public:
		static const FVector2 Zero;
		static const FVector2 Half;
		static const FVector2 One;
		static const FVector2 XAxis;
		static const FVector2 YAxis;

		static FVector2 Replicate(float32 Value) { return DirectX::XMVectorReplicate(Value); }
	};

	static_assert(sizeof(FVector2) == sizeof(float) * 2);
	using float2 = FVector2;

	inline const FVector2 FVector2::Zero(0.0f, 0.0f);
	inline const FVector2 FVector2::Half(0.5f, 0.5f);
	inline const FVector2 FVector2::One(1.0f, 1.0f);
	inline const FVector2 FVector2::XAxis(1.0f, 0.0f);
	inline const FVector2 FVector2::YAxis(0.0f, 1.0f);

	inline FVector2 operator *(float32 Rate, FVector2 Vector)
	{
		return Vector * Rate;
	}

	inline FVector2 operator /(float32 Rate, FVector2 Vector)
	{
		return DirectX::XMVectorSubtract(DirectX::XMVectorReplicate(Rate), Vector.ToXMVector());
	}

	inline bool IsNaN(FVector2 Vector) { return DirectX::XMVector2IsNaN(Vector); }
	inline bool IsInfinite(FVector2 Vector) { return DirectX::XMVector2IsInfinite(Vector); }

	inline bool Equal(FVector2 VectorA, FVector2 VectorB) { return DirectX::XMVector2Equal(VectorA, VectorB); }
	inline bool NearEqual(FVector2 VectorA, FVector2 VectorB, float32 Epsilon) { return DirectX::XMVector2NearEqual(VectorA, VectorB, FVector2::Replicate(Epsilon)); }

	inline FVector2 Lerp(FVector2 VectorA, FVector2 VectorB, float32 T) { return DirectX::XMVectorLerp(VectorA, VectorB, T); }
	inline FVector2 Lerp(FVector2 VectorA, FVector2 VectorB, FVector2 T) { return DirectX::XMVectorLerpV(VectorA, VectorB, T); }

	inline FVector2 Abs(FVector2 Vector) { return DirectX::XMVectorAbs(Vector); }
	inline FVector2 Negate(FVector2 Vector) { return DirectX::XMVectorNegate(Vector); }
	inline FVector2 Reciprocal(FVector2 Vector) { return DirectX::XMVectorReciprocal(Vector); }

	inline FVector2 NegativeMultiplySubtract(FVector2 VectorA, FVector2 VectorB, FVector2 VectorC)
	{
		/* C - A * B  */
		return DirectX::XMVectorNegativeMultiplySubtract(VectorA, VectorB, VectorC);
	}

	inline FVector2 Sqrt(FVector2 Vector) { return DirectX::XMVectorSqrt(Vector); }
	inline FVector2 ReciprocalSqrt(FVector2 Vector) { return DirectX::XMVectorReciprocalSqrt(Vector); }

	inline FVector2 Exp2(FVector2 Vector) { return DirectX::XMVectorExp2(Vector); }
	inline FVector2 ExpE(FVector2 Vector) { return DirectX::XMVectorExpE(Vector); }
	inline FVector2 Exp10(FVector2 Vector) { return DirectX::XMVectorExp10(Vector); }

	inline FVector2 Log2(FVector2 Vector) { return DirectX::XMVectorLog2(Vector); }
	inline FVector2 LogE(FVector2 Vector) { return DirectX::XMVectorLog10(Vector); }
	inline FVector2 Log10(FVector2 Vector) { return DirectX::XMVectorLogE(Vector); }
	inline FVector2 Power(FVector2 Vector, FVector2 Exponent) { return DirectX::XMVectorPow(Vector, Exponent); }

	inline FVector2 Sin(FVector2 Vector) { return DirectX::XMVectorSin(Vector); }
	inline FVector2 Cos(FVector2 Vector) { return DirectX::XMVectorCos(Vector); }

	inline void SinCos(FVector2 Vector, FVector2 & Sin, FVector2 & Cos)
	{
		DirectX::XMVECTOR XMSin, XMCos;
		DirectX::XMVectorSinCos(&XMSin, &XMCos, Vector);
		Sin = XMSin;
		Cos = XMCos;
	}

	inline FVector2 SinH(FVector2 Vector) { return DirectX::XMVectorSinH(Vector); }
	inline FVector2 CosH(FVector2 Vector) { return DirectX::XMVectorCosH(Vector); }
	inline FVector2 Tan(FVector2 Vector) { return DirectX::XMVectorTan(Vector); }
	inline FVector2 ASin(FVector2 Vector) { return DirectX::XMVectorASin(Vector); }
	inline FVector2 ACos(FVector2 Vector) { return DirectX::XMVectorACos(Vector); }
	inline FVector2 ATan(FVector2 Vector) { return DirectX::XMVectorATan(Vector); }
	inline FVector2 ATan2(FVector2 VectorA, FVector2 VectorB) { return DirectX::XMVectorATan2(VectorA, VectorB); }

	inline FVector2 Min(FVector2 VectorA, FVector2 VectorB) { return DirectX::XMVectorMin(VectorA, VectorB); }
	inline FVector2 Max(FVector2 VectorA, FVector2 VectorB) { return DirectX::XMVectorMax(VectorA, VectorB); }
	inline FVector2 Clamp(FVector2 Vector, FVector2 Min, FVector2 Max) { return DirectX::XMVectorClamp(Vector, Min, Max); }
	inline FVector2 Saturate(FVector2 Vector) { return DirectX::XMVectorSaturate(Vector); }

	inline FVector2 Truncate(FVector2 Vector) { return DirectX::XMVectorTruncate(Vector); }
	inline FVector2 Mod(FVector2 VectorA, FVector2 VectorB) { return DirectX::XMVectorMod(VectorA, VectorB); }
	inline FVector2 Mod(FVector2 VectorA, float32 Value) { return DirectX::XMVectorMod(VectorA, DirectX::XMVectorReplicate(Value)); }
	inline FVector2 Floor(FVector2 Vector) { return DirectX::XMVectorFloor(Vector); }
	inline FVector2 Ceil(FVector2 Vector) { return DirectX::XMVectorCeiling(Vector); }
	inline FVector2 Round(FVector2 Vector) { return DirectX::XMVectorRound(Vector); }
}
