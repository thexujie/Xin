#pragma once

#include "Basic.Types.h"
#include "Vector3.h"
#include "Vec4.h"

namespace Xin
{
	struct [[Meta(SystemType)]] FVector4
	{
		FVector4() = default;
		FVector4(const FVector4 &) = default;
		FVector4 & operator =(const FVector4 &) = default;
		~FVector4() = default;
		auto operator <=>(const FVector4 & Another) const { return Compare(X, Another.X, Y, Another.Y, Z, Another.Z, W, Another.W); }
		bool operator ==(const FVector4 & Another) const { return DirectX::XMVector4Equal(ToXMVector(), Another.ToXMVector()); }

		constexpr FVector4(float32 X, float32 Y) : X(X), Y(Y), Z(), W() { }
		constexpr FVector4(float32 X, float32 Y, float32 Z) : X(X), Y(Y), Z(Z), W() { }
		constexpr FVector4(float32 X, float32 Y, float32 Z, float32 W) : X(X), Y(Y), Z(Z), W(W) { }
		constexpr FVector4(const FVector2 & Vector2) : X(Vector2.X), Y(Vector2.Y), Z(), W() { }
		constexpr FVector4(const FVector2 & Vector2, float32 Z) : X(Vector2.X), Y(Vector2.Y), Z(Z), W() { }
		constexpr FVector4(const FVector2 & Vector2, float32 Z, float32 W) : X(Vector2.X), Y(Vector2.Y), Z(Z), W(W) { }
		constexpr FVector4(const FVector2 & Vector2XY, const FVector2 & Vector2ZW) : X(Vector2XY.X), Y(Vector2XY.Y), Z(Vector2ZW.X), W(Vector2ZW.X) { }
		constexpr FVector4(const FVector3 & Vector3) : X(Vector3.X), Y(Vector3.Y), Z(Vector3.Z), W() { }
		constexpr FVector4(const FVector3 & Vector3, float32 W) : X(Vector3.X), Y(Vector3.Y), Z(Vector3.Z), W(W) { }
		constexpr FVector4(const Vec2F & Vec2) : X(Vec2.X), Y(Vec2.Y), Z(), W() { }
		constexpr FVector4(const Vec2F & Vec2, float32 Z) : X(Vec2.X), Y(Vec2.Y), Z(Z), W() { }
		constexpr FVector4(const Vec2F & Vec2, float32 Z, float32 W) : X(Vec2.X), Y(Vec2.Y), Z(Z), W(W) { }
		constexpr FVector4(const Vec2F & Vec2XY, const Vec2F & Vec2XW) : X(Vec2XY.X), Y(Vec2XY.Y), Z(Vec2XW.X), W(Vec2XW.Y) { }
		constexpr FVector4(const Vec3F & Vec3) : X(Vec3.X), Y(Vec3.Y), Z(Vec3.Z), W() { }
		constexpr FVector4(const Vec3F & Vec3, float32 W) : X(Vec3.X), Y(Vec3.Y), Z(Vec3.Z), W(W) { }
		constexpr FVector4(const Vec4F & Vec4) : X(Vec4.X), Y(Vec4.Y), Z(Vec4.Z), W(Vec4.W) { }
		constexpr FVector4(const float32 (& Floats)[4]) : X(Floats[0]), Y(Floats[1]), Z(Floats[2]), W(Floats[3]) { }

		FVector4 & operator =(const FVector3 & Vector3)
		{
			X = Vector3[0];
			Y = Vector3[1];
			Z = Vector3[2];
			W = {};
			return *this;
		}

		FVector4 & operator =(const float32 (& Floats)[4])
		{
			X = Floats[0];
			Y = Floats[1];
			Z = Floats[2];
			W = Floats[3];
			return *this;
		}

		inline float32 & operator [](int32 Index) { return Floats[Index]; }
		inline const float32 & operator [](int32 Index) const { return Floats[Index]; }

		FVector4 & Set(const FVector4 & That)
		{
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(this), That.ToXMVector());
			return *this;
		}

		bool HasNan() const { return DirectX::XMVector4IsNaN(ToXMVector()); }

		FVector4 & Normalize() { return Set(DirectX::XMVector4Normalize(ToXMVector())); }

		float32 Length3() const { return DirectX::XMVectorGetX(DirectX::XMVector3Length(ToXMVector())); }
		float32 Length3Square() const { return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(ToXMVector())); }
		float32 Length() const { return DirectX::XMVectorGetX(DirectX::XMVector4Length(ToXMVector())); }
		float32 LengthSquare() const { return DirectX::XMVectorGetX(DirectX::XMVector4LengthSq(ToXMVector())); }
		float32 Dot(const FVector4 & Another) const { return DirectX::XMVectorGetX(DirectX::XMVector4Dot(ToXMVector(), Another.ToXMVector())); }
		FVector4 Cross(const FVector4 & Another1, const FVector4 & Another2) const { return DirectX::XMVector4Cross(ToXMVector(), Another1.ToXMVector(), Another2.ToXMVector()); }

		FVector4 Abs() const { return DirectX::XMVectorAbs(ToXMVector()); }
		FVector4 Negate() const { return DirectX::XMVectorNegate(ToXMVector()); }
		FVector4 Reciprocal() const { return DirectX::XMVectorReciprocal(ToXMVector()); }

		FVector4 Add(const FVector4 & Another) const { return DirectX::XMVectorAdd(ToXMVector(), Another.ToXMVector()); }
		FVector4 Subtract(const FVector4 & Another) const { return DirectX::XMVectorSubtract(ToXMVector(), Another.ToXMVector()); }
		FVector4 Multiply(const FVector4 & Another) const { return DirectX::XMVectorMultiply(ToXMVector(), Another.ToXMVector()); }
		FVector4 Divide(const FVector4 & Another) const { return DirectX::XMVectorDivide(ToXMVector(), Another.ToXMVector()); }
		FVector4 MultiplyAdd(const FVector4 & VectorMultiply, const FVector4 & VectorAdd) const { return DirectX::XMVectorMultiplyAdd(ToXMVector(), VectorMultiply.ToXMVector(), VectorAdd.ToXMVector()); }

		FVector4 operator +() const { return *this; }
		FVector4 operator -() const { return Negate(); }

		FVector4 operator +(const FVector4 & Another) const { return Add(Another); }
		FVector4 & operator +=(const FVector4 & Another) { return Set(Add(Another)); }

		FVector4 operator -(const FVector4 & Another) const { return Subtract(Another); }
		FVector4 & operator -=(const FVector4 & Another) { return Set(Subtract(Another)); }

		FVector4 operator *(const FVector4 & Another) const { return Multiply(Another); }
		FVector4 & operator *=(const FVector4 & Another) { return Set(Multiply(Another)); }

		FVector4 operator /(const FVector4 & Another) const { return Divide(Another); }
		FVector4 & operator /=(const FVector4 & Another) { return Set(Divide(Another)); }

		FVector4 operator /(float32 Rate) const { return Divide(DirectX::XMVectorReplicate(Rate)); }
		FVector4 & operator /=(float32 Rate) { return Set(Divide(DirectX::XMVectorReplicate(Rate))); };
		FVector4 operator *(float32 Rate) const { return Multiply(DirectX::XMVectorReplicate(Rate)); }
		FVector4 & operator *=(float32 Rate) { return Set(Multiply(DirectX::XMVectorReplicate(Rate))); };

	public:
		union
		{
			struct
			{
				float32 X;
				float32 Y;
				float32 Z;
				float32 W;
			};

			struct
			{
				FVector3 XYZ;
				float32 __W;
			};

			struct
			{
				FVector2 XY;
				FVector2 ZW;
			};

			float32 Floats[4];
			int32 Ints[4];
			uint32 UInts[4];
		};

	public:
		FVector4(const DirectX::XMVECTOR & XMVector) { DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(this), XMVector); }

		FVector4 & Set(const DirectX::XMVECTOR & XMVector)
		{
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(this), XMVector);
			return *this;
		}

		FVector4 & operator =(const DirectX::XMVECTOR & XMVector) { return Set(XMVector); }
		operator DirectX::XMVECTOR() const { return DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(this)); }
		DirectX::XMVECTOR ToXMVector() const { return DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(this)); }
		DirectX::XMVECTOR & AsXMVector() { return *reinterpret_cast<DirectX::XMVECTOR *>(this); }
		const DirectX::XMVECTOR & AsXMVector() const { return *reinterpret_cast<const DirectX::XMVECTOR *>(this); }

	public:
		static const FVector4 Zero;
		static const FVector4 Half;
		static const FVector4 One;
		static const FVector4 Min;
		static const FVector4 Max;

		static const FVector4 AxisX;
		static const FVector4 AxisY;
		static const FVector4 AxisZ;
		static const FVector4 AxisW;

	public:
		static FVector4 Replicate(float32 Value) { return DirectX::XMVectorReplicate(Value); }
	};

	static_assert(sizeof(FVector4) == sizeof(float) * 4, "Size of Vector4 is not rational.");
	using float4 = FVector4;

	inline const FVector4 FVector4::Zero(0.0f, 0.0f, 0.0f, 0.0f);
	inline const FVector4 FVector4::Half(0.5f, 0.5f, 0.5f, 0.5f);
	inline const FVector4 FVector4::One(1.0f, 1.0f, 1.0f, 1.0f);
	inline const FVector4 FVector4::Min(Float32Min, Float32Min, Float32Min, Float32Min);
	inline const FVector4 FVector4::Max(Float32Max, Float32Max, Float32Max, Float32Max);

	inline const FVector4 FVector4::AxisX(1.0f, 0.0f, 0.0f, 0.0f);
	inline const FVector4 FVector4::AxisY(0.0f, 1.0f, 0.0f, 0.0f);
	inline const FVector4 FVector4::AxisZ(0.0f, 0.0f, 1.0f, 0.0f);
	inline const FVector4 FVector4::AxisW(0.0f, 0.0f, 0.0f, 1.0f);

	inline FVector4 operator *(float32 Rate, FVector4 Vector)
	{
		return Vector * Rate;
	}

	inline FVector4 operator /(float32 Rate, FVector4 Vector)
	{
		return DirectX::XMVectorSubtract(DirectX::XMVectorReplicate(Rate), Vector.ToXMVector());
	}

	inline FVector4 operator <(FVector4 VectorA, FVector4 VectorB)
	{
		return DirectX::XMVectorLess(VectorA, VectorB);
	}

	inline FVector4 operator <=(FVector4 VectorA, FVector4 VectorB)
	{
		return DirectX::XMVectorLessOrEqual(VectorA, VectorB);
	}

	inline FVector4 operator >(FVector4 VectorA, FVector4 VectorB)
	{
		return DirectX::XMVectorGreater(VectorA, VectorB);
	}

	inline FVector4 operator >=(FVector4 VectorA, FVector4 VectorB)
	{
		return DirectX::XMVectorGreaterOrEqual(VectorA, VectorB);
	}

	inline bool IsNaN(FVector4 Vector) { return DirectX::XMVector4IsNaN(Vector); }
	inline bool IsInfinite(FVector4 Vector) { return DirectX::XMVector4IsInfinite(Vector); }

	inline bool Equal(FVector4 VectorA, FVector4 VectorB) { return DirectX::XMVector4Equal(VectorA, VectorB); }
	inline bool NearEqual(FVector4 VectorA, FVector4 VectorB, float32 Epsilon) { return DirectX::XMVector4NearEqual(VectorA, VectorB, FVector4::Replicate(Epsilon)); }

	inline FVector4 Lerp(FVector4 VectorA, FVector4 VectorB, float32 T) { return DirectX::XMVectorLerp(VectorA, VectorB, T); }
	inline FVector4 Lerp(FVector4 VectorA, FVector4 VectorB, FVector4 T) { return DirectX::XMVectorLerpV(VectorA, VectorB, T); }

	inline FVector4 Abs(FVector4 Vector) { return DirectX::XMVectorAbs(Vector); }
	inline FVector4 Negate(FVector4 Vector) { return DirectX::XMVectorNegate(Vector); }
	inline FVector4 Reciprocal(FVector4 Vector) { return DirectX::XMVectorReciprocal(Vector); }

	inline FVector4 NegativeMultiplySubtract(FVector4 VectorA, FVector4 VectorB, FVector4 VectorC)
	{
		/* C - A * B  */
		return DirectX::XMVectorNegativeMultiplySubtract(VectorA, VectorB, VectorC);
	}

	inline FVector4 Sqrt(FVector4 Vector) { return DirectX::XMVectorSqrt(Vector); }
	inline FVector4 ReciprocalSqrt(FVector4 Vector) { return DirectX::XMVectorReciprocalSqrt(Vector); }

	inline FVector4 Exp2(FVector4 Vector) { return DirectX::XMVectorExp2(Vector); }
	inline FVector4 ExpE(FVector4 Vector) { return DirectX::XMVectorExpE(Vector); }
	inline FVector4 Exp10(FVector4 Vector) { return DirectX::XMVectorExp10(Vector); }

	inline FVector4 Log2(FVector4 Vector) { return DirectX::XMVectorLog2(Vector); }
	inline FVector4 LogE(FVector4 Vector) { return DirectX::XMVectorLog10(Vector); }
	inline FVector4 Log10(FVector4 Vector) { return DirectX::XMVectorLogE(Vector); }
	inline FVector4 Power(FVector4 Vector, FVector4 Exponent) { return DirectX::XMVectorPow(Vector, Exponent); }

	inline FVector4 Sin(FVector4 Vector) { return DirectX::XMVectorSin(Vector); }
	inline FVector4 Cos(FVector4 Vector) { return DirectX::XMVectorCos(Vector); }

	inline void SinCos(FVector4 Vector, FVector4 & Sin, FVector4 & Cos)
	{
		DirectX::XMVECTOR XMSin, XMCos;
		DirectX::XMVectorSinCos(&XMSin, &XMCos, Vector);
		Sin = XMSin;
		Cos = XMCos;
	}

	inline FVector4 SinH(FVector4 Vector) { return DirectX::XMVectorSinH(Vector); }
	inline FVector4 CosH(FVector4 Vector) { return DirectX::XMVectorCosH(Vector); }
	inline FVector4 Tan(FVector4 Vector) { return DirectX::XMVectorTan(Vector); }
	inline FVector4 ASin(FVector4 Vector) { return DirectX::XMVectorASin(Vector); }
	inline FVector4 ACos(FVector4 Vector) { return DirectX::XMVectorACos(Vector); }
	inline FVector4 ATan(FVector4 Vector) { return DirectX::XMVectorATan(Vector); }
	inline FVector4 ATan2(FVector4 VectorA, FVector4 VectorB) { return DirectX::XMVectorATan2(VectorA, VectorB); }

	inline FVector4 Min(FVector4 VectorA, FVector4 VectorB) { return DirectX::XMVectorMin(VectorA, VectorB); }
	inline FVector4 Max(FVector4 VectorA, FVector4 VectorB) { return DirectX::XMVectorMax(VectorA, VectorB); }
	inline FVector4 Clamp(FVector4 Vector, FVector4 Min, FVector4 Max) { return DirectX::XMVectorClamp(Vector, Min, Max); }
	inline FVector4 Saturate(FVector4 Vector) { return DirectX::XMVectorSaturate(Vector); }

	inline FVector4 Truncate(FVector4 Vector) { return DirectX::XMVectorTruncate(Vector); }
	inline FVector4 Mod(FVector4 VectorA, FVector4 VectorB) { return DirectX::XMVectorMod(VectorA, VectorB); }
	inline FVector4 Mod(FVector4 VectorA, float32 Value) { return DirectX::XMVectorMod(VectorA, DirectX::XMVectorReplicate(Value)); }
	inline FVector4 Floor(FVector4 Vector) { return DirectX::XMVectorFloor(Vector); }
	inline FVector4 Ceil(FVector4 Vector) { return DirectX::XMVectorCeiling(Vector); }
	inline FVector4 Round(FVector4 Vector) { return DirectX::XMVectorRound(Vector); }
}
