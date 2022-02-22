#pragma once

#include "Basic.Types.h"
#include "Vector2.h"
#include "Vec3.h"
#include "XMath.h"

namespace Xin
{
	struct [[Meta(SystemType)]] FVector3
	{
		FVector3() = default;
		FVector3(const FVector3 &) = default;
		FVector3 & operator =(const FVector3 &) = default;
		~FVector3() = default;
		bool operator ==(const FVector3 & Other) const { return !!DirectX::XMVector3Equal(ToXMVector(), Other.ToXMVector()); }
		auto operator <=>(const FVector3 & Other) const { return Compare(X, Other.X, Y, Other.Y, Z, Other.Z); }

		constexpr FVector3(float32 InX, float32 InY) : X(InX), Y(InY), Z() { }
		constexpr FVector3(float32 InX, float32 InY, float32 InZ) : X(InX), Y(InY), Z(InZ) { }
		constexpr FVector3(const float32 (& FloatsIn)[3]) : X(FloatsIn[0]), Y(FloatsIn[1]), Z(FloatsIn[2]) { }
		constexpr FVector3(const FVector2 & InXY, float32 InZ) : X(InXY.X), Y(InXY.Y), Z(InZ) { }
		constexpr FVector3(const Vec3F & Vec3) : X(Vec3.X), Y(Vec3.Y), Z(Vec3.Z) { }

		FVector3 & operator =(const FVector2 & Vector2)
		{
			X = Vector2.X;
			Y = Vector2.Y;
			Z = {};
			return *this;
		}

		FVector3 & operator =(const Vec3F & Vec3)
		{
			X = Vec3.X;
			Y = Vec3.Y;
			Z = Vec3.Z;
			return *this;
		}

		FVector3 & operator =(const float32 (& FloatsIn)[3])
		{
			X = FloatsIn[0];
			Y = FloatsIn[1];
			Z = FloatsIn[2];
			return *this;
		}

		float32 & operator [](int32 Index) { return Floats[Index]; }
		const float32 & operator [](int32 Index) const { return Floats[Index]; }

		FVector3 & Set(const FVector3 & That)
		{
			DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3 *>(this), That.ToXMVector());
			return *this;
		}

		bool HasNan() const { return DirectX::XMVector3IsNaN(ToXMVector()); }

		FVector3 & Normalize() { return Set(DirectX::XMVector3Normalize(ToXMVector())); }
		FVector3 Normalized() const { return FVector3(DirectX::XMVector3Normalize(ToXMVector())); }

		float32 Length() const { return DirectX::XMVectorGetX(DirectX::XMVector3Length(ToXMVector())); }
		float32 LengthSquare() const { return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(ToXMVector())); }
		float32 Dot(const FVector3 & Other) const { return DirectX::XMVectorGetX(DirectX::XMVector3Dot(ToXMVector(), Other.ToXMVector())); }
		FVector3 Cross(const FVector3 & Other) const { return DirectX::XMVector3Cross(ToXMVector(), Other.ToXMVector()); }

		FVector3 Abs() const { return DirectX::XMVectorAbs(ToXMVector()); }
		FVector3 Negate() const { return DirectX::XMVectorNegate(ToXMVector()); }
		FVector3 Reciprocal() const { return DirectX::XMVectorReciprocal(ToXMVector()); }

		FVector3 Add(const FVector3 & Other) const { return DirectX::XMVectorAdd(ToXMVector(), Other.ToXMVector()); }
		FVector3 Subtract(const FVector3 & Other) const { return DirectX::XMVectorSubtract(ToXMVector(), Other.ToXMVector()); }
		FVector3 Multiply(const FVector3 & Other) const { return DirectX::XMVectorMultiply(ToXMVector(), Other.ToXMVector()); }
		FVector3 Divide(const FVector3 & Other) const { return DirectX::XMVectorDivide(ToXMVector(), Other.ToXMVector()); }
		FVector3 MultiplyAdd(const FVector3 & VectorMultiply, const FVector3 & VectorAdd) const { return DirectX::XMVectorMultiplyAdd(ToXMVector(), VectorMultiply.ToXMVector(), VectorAdd.ToXMVector()); }

		FVector3 operator +() const { return *this; }
		FVector3 operator -() const { return Negate(); }

		FVector3 operator +(const FVector3 & Other) const { return Add(Other); }
		FVector3 & operator +=(const FVector3 & Other) { return Set(Add(Other)); }

		FVector3 operator -(const FVector3 & Other) const { return Subtract(Other); }
		FVector3 & operator -=(const FVector3 & Other) { return Set(Subtract(Other)); }

		FVector3 operator *(const FVector3 & Other) const { return Multiply(Other); }
		FVector3 & operator *=(const FVector3 & Other) { return Set(Multiply(Other)); }

		FVector3 operator /(const FVector3 & Other) const { return Divide(Other); }
		FVector3 & operator /=(const FVector3 & Other) { return Set(Divide(Other)); }

		FVector3 operator /(float32 Rate) const { return Divide(DirectX::XMVectorReplicate(Rate)); }
		FVector3 & operator /=(float32 Rate) { return Set(Divide(DirectX::XMVectorReplicate(Rate))); };
		FVector3 operator *(float32 Rate) const { return Multiply(DirectX::XMVectorReplicate(Rate)); }
		FVector3 & operator *=(float32 Rate) { return Set(Multiply(DirectX::XMVectorReplicate(Rate))); };

		float32 operator %(const FVector3 & Other) const { return Dot(Other); }
		FVector3 operator ^(const FVector3 & Other) const { return Cross(Other); }

	public:
		union
		{
			struct
			{
				float32 X;
				float32 Y;
				float32 Z;
			};

			struct
			{
				float32 Pitch;
				float32 Yaw;
				float32 Roll;
			};

			struct
			{
				FVector2 XY;
				float32 __Z;
			};

			float32 Floats[3];
			int32 Ints[3];
			uint32 UInts[3];
		};

	public:
		FVector3(const DirectX::XMVECTOR & XMVector) { DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3 *>(this), XMVector); }

		FVector3 & Set(DirectX::XMVECTOR XMVector)
		{
			DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3 *>(this), XMVector);
			return *this;
		}

		FVector3 & operator =(DirectX::XMVECTOR XMVector) { return Set(XMVector); }
		operator DirectX::XMVECTOR() const { return DirectX::XMLoadFloat3(reinterpret_cast<const DirectX::XMFLOAT3 *>(this)); }
		DirectX::XMVECTOR ToXMVector() const { return DirectX::XMLoadFloat3(reinterpret_cast<const DirectX::XMFLOAT3 *>(this)); }

	public:
		static const FVector3 Zero;
		static const FVector3 Half;
		static const FVector3 One;
		static const FVector3 AxisX;
		static const FVector3 AxisY;
		static const FVector3 AxisZ;
		static const FVector3 Min;
		static const FVector3 Max;

	public:
		static FVector3 Replicate(float32 Value) { return DirectX::XMVectorReplicate(Value); }
	};

	static_assert(sizeof(FVector3) == sizeof(float) * 3);
	using float3 = FVector3;

	inline const FVector3 FVector3::Zero = {};
	inline const FVector3 FVector3::Half = { 0.5f, 0.5f, 0.5f };
	inline const FVector3 FVector3::One = { 1.0f, 1.0f, 1.0f };
	inline const FVector3 FVector3::AxisX = { 1.0f, 0.0f, 0.0f };
	inline const FVector3 FVector3::AxisY = { 0.0f, 1.0f, 0.0f };
	inline const FVector3 FVector3::AxisZ = { 0.0f, 0.0f, 1.0f };
	inline const FVector3 FVector3::Min = { Float32Min, Float32Min, Float32Min };
	inline const FVector3 FVector3::Max = { Float32Max, Float32Max, Float32Max };

	inline FVector3 operator *(float32 Rate, FVector3 Vector)
	{
		return Vector * Rate;
	}

	inline FVector3 operator /(float32 Rate, FVector3 Vector)
	{
		return DirectX::XMVectorSubtract(DirectX::XMVectorReplicate(Rate), Vector.ToXMVector());
	}

	inline FVector3 operator <(FVector3 VectorA, FVector3 VectorB)
	{
		return DirectX::XMVectorLess(VectorA, VectorB);
	}

	inline FVector3 operator <=(FVector3 VectorA, FVector3 VectorB)
	{
		return DirectX::XMVectorLessOrEqual(VectorA, VectorB);
	}

	inline FVector3 operator >(FVector3 VectorA, FVector3 VectorB)
	{
		return DirectX::XMVectorGreater(VectorA, VectorB);
	}

	inline FVector3 operator >=(FVector3 VectorA, FVector3 VectorB)
	{
		return DirectX::XMVectorGreaterOrEqual(VectorA, VectorB);
	}

	inline bool IsNaN(FVector3 Vector) { return DirectX::XMVector3IsNaN(Vector); }
	inline bool IsInfinite(FVector3 Vector) { return DirectX::XMVector3IsInfinite(Vector); }

	inline bool Equal(FVector3 VectorA, FVector3 VectorB) { return DirectX::XMVector3Equal(VectorA, VectorB); }
	inline bool NearEqual(FVector3 VectorA, FVector3 VectorB, float32 Epsilon) { return DirectX::XMVector3NearEqual(VectorA, VectorB, FVector3::Replicate(Epsilon)); }

	inline FVector3 Lerp(FVector3 VectorA, FVector3 VectorB, float32 T) { return DirectX::XMVectorLerp(VectorA, VectorB, T); }
	inline FVector3 Lerp(FVector3 VectorA, FVector3 VectorB, FVector3 T) { return DirectX::XMVectorLerpV(VectorA, VectorB, T); }

	inline FVector3 Abs(FVector3 Vector) { return DirectX::XMVectorAbs(Vector); }
	inline FVector3 Negate(FVector3 Vector) { return DirectX::XMVectorNegate(Vector); }
	inline FVector3 Reciprocal(FVector3 Vector) { return DirectX::XMVectorReciprocal(Vector); }

	inline FVector3 NegativeMultiplySubtract(FVector3 VectorA, FVector3 VectorB, FVector3 VectorC)
	{
		/* C - A * B  */
		return DirectX::XMVectorNegativeMultiplySubtract(VectorA, VectorB, VectorC);
	}

	inline FVector3 Sqrt(FVector3 Vector) { return DirectX::XMVectorSqrt(Vector); }
	inline FVector3 ReciprocalSqrt(FVector3 Vector) { return DirectX::XMVectorReciprocalSqrt(Vector); }

	inline FVector3 Exp2(FVector3 Vector) { return DirectX::XMVectorExp2(Vector); }
	inline FVector3 ExpE(FVector3 Vector) { return DirectX::XMVectorExpE(Vector); }
	inline FVector3 Exp10(FVector3 Vector) { return DirectX::XMVectorExp10(Vector); }

	inline FVector3 Log2(FVector3 Vector) { return DirectX::XMVectorLog2(Vector); }
	inline FVector3 LogE(FVector3 Vector) { return DirectX::XMVectorLog10(Vector); }
	inline FVector3 Log10(FVector3 Vector) { return DirectX::XMVectorLogE(Vector); }
	inline FVector3 Power(FVector3 Vector, FVector3 Exponent) { return DirectX::XMVectorPow(Vector, Exponent); }

	inline FVector3 Sin(FVector3 Vector) { return DirectX::XMVectorSin(Vector); }
	inline FVector3 Cos(FVector3 Vector) { return DirectX::XMVectorCos(Vector); }

	inline void SinCos(FVector3 Vector, FVector3 & Sin, FVector3 & Cos)
	{
		DirectX::XMVECTOR XMSin, XMCos;
		DirectX::XMVectorSinCos(&XMSin, &XMCos, Vector);
		Sin = XMSin;
		Cos = XMCos;
	}

	inline FVector3 SinH(FVector3 Vector) { return DirectX::XMVectorSinH(Vector); }
	inline FVector3 CosH(FVector3 Vector) { return DirectX::XMVectorCosH(Vector); }
	inline FVector3 Tan(FVector3 Vector) { return DirectX::XMVectorTan(Vector); }
	inline FVector3 ASin(FVector3 Vector) { return DirectX::XMVectorASin(Vector); }
	inline FVector3 ACos(FVector3 Vector) { return DirectX::XMVectorACos(Vector); }
	inline FVector3 ATan(FVector3 Vector) { return DirectX::XMVectorATan(Vector); }
	inline FVector3 ATan2(FVector3 VectorA, FVector3 VectorB) { return DirectX::XMVectorATan2(VectorA, VectorB); }

	inline FVector3 Min(FVector3 VectorA, FVector3 VectorB) { return DirectX::XMVectorMin(VectorA, VectorB); }
	inline FVector3 Max(FVector3 VectorA, FVector3 VectorB) { return DirectX::XMVectorMax(VectorA, VectorB); }
	inline FVector3 Clamp(FVector3 Vector, FVector3 Min, FVector3 Max) { return DirectX::XMVectorClamp(Vector, Min, Max); }
	inline FVector3 Saturate(FVector3 Vector) { return DirectX::XMVectorSaturate(Vector); }

	inline FVector3 Truncate(FVector3 Vector) { return DirectX::XMVectorTruncate(Vector); }
	inline FVector3 Mod(FVector3 VectorA, FVector3 VectorB) { return DirectX::XMVectorMod(VectorA, VectorB); }
	inline FVector3 Mod(FVector3 VectorA, float32 Value) { return DirectX::XMVectorMod(VectorA, DirectX::XMVectorReplicate(Value)); }
	inline FVector3 Floor(FVector3 Vector) { return DirectX::XMVectorFloor(Vector); }
	inline FVector3 Ceil(FVector3 Vector) { return DirectX::XMVectorCeiling(Vector); }
	inline FVector3 Round(FVector3 Vector) { return DirectX::XMVectorRound(Vector); }
}
