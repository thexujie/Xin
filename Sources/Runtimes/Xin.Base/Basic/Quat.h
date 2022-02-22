#pragma once

#include "Basic.Types.h"
#include "Vector3.h"

namespace Xin
{
	struct FMatrix;

	struct FQuat
	{
	public:
		FQuat() = default;
		FQuat(const FQuat &) = default;
		FQuat & operator =(const FQuat &) = default;
		~FQuat() = default;
		auto operator <=>(const FQuat & Another) const { return Compare(X, Another.X, Y, Another.Y, Z, Another.Z, W, Another.W); }

		constexpr FQuat(float32 InX, float32 InY, float32 InZ, float32 InW = 1.0f) : X(InX), Y(InY), Z(InZ), W(InW) { }
		constexpr FQuat(const FVector3 & Vector3) : X(Vector3.X), Y(Vector3.Y), Z(Vector3.Z), W(0) { }
		constexpr FQuat(const FVector3 & Vector3, float32 InW) : X(Vector3.X), Y(Vector3.Y), Z(Vector3.Z), W(InW) { }
		constexpr FQuat(const float32 (& FloatsIn)[4]) : X(FloatsIn[0]), Y(FloatsIn[1]), Z(FloatsIn[2]), W(FloatsIn[3]) { }

		FQuat & operator =(const float32 (& FloatsIn)[4])
		{
			X = FloatsIn[0];
			Y = FloatsIn[1];
			Z = FloatsIn[2];
			W = FloatsIn[3];
			return *this;
		}

		inline float32 & operator [](int32 Index) { return Floats[Index]; }
		inline const float32 & operator [](int32 Index) const { return Floats[Index]; }

		FQuat & Set(const FQuat & That)
		{
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(this), That.ToXMVector());
			return *this;
		}

		bool HasNan() const { return DirectX::XMVector4IsNaN(ToXMVector()); }

		FQuat & Normalize() { return Set(DirectX::XMQuaternionNormalize(ToXMVector())); }

		float32 Length3() const { return DirectX::XMVectorGetX(DirectX::XMVector3Length(ToXMVector())); }
		float32 Length3Square() const { return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(ToXMVector())); }
		float32 Length() const { return DirectX::XMVectorGetX(DirectX::XMQuaternionLength(ToXMVector())); }
		float32 LengthSquare() const { return DirectX::XMVectorGetX(DirectX::XMQuaternionLengthSq(ToXMVector())); }
		float32 Dot(const FQuat & Another) const { return DirectX::XMVectorGetX(DirectX::XMQuaternionDot(ToXMVector(), Another.ToXMVector())); }

		FQuat Abs() const { return DirectX::XMVectorAbs(ToXMVector()); }
		FQuat Negate() const { return DirectX::XMVectorNegate(ToXMVector()); }
		FQuat Reciprocal() const { return DirectX::XMVectorReciprocal(ToXMVector()); }
		FQuat Inverse() const { return DirectX::XMQuaternionInverse(ToXMVector()); }

		FQuat Multiply(const FQuat & Another) const { return DirectX::XMQuaternionMultiply(ToXMVector(), Another.ToXMVector()); }

		FQuat operator +() const { return *this; }
		FQuat operator -() const { return Negate(); }

		FQuat operator *(const FQuat & Another) const { return Multiply(Another); }
		FQuat & operator *=(const FQuat & Another) { return Set(Multiply(Another)); }

		FVector3 Angles() const
		{
			// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
			float SINGULARITY_THRESHOLD = 0.4999995f;
			float Heading, Attitude, Bank;
			{
				float SingularityTest = X * Y + Z * W;
				Attitude = Asin(2.f * X * Y + 2.f * Z * W);
				if (SingularityTest > SINGULARITY_THRESHOLD)
				{
					Heading = 2.f * Atan2(X, W);
					Bank = 0;
				}
				else if (SingularityTest < -SINGULARITY_THRESHOLD)
				{
					Heading = -2.f * Atan2(X, W);
					Bank = 0;
				}
				else
				{
					Heading = Atan2(2.f * Y * W - 2 * X * Z, 1 - 2.f * Y * Y - 2 * Z * Z);
					Bank = Atan2(2.f * X * W - 2 * Y * Z, 1 - 2.f * X * X - 2.f * Z * Z);
				}
			}
			return FVector3 { Bank, Heading, Attitude };
		}

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
		FQuat(DirectX::FXMVECTOR FXMVector) { DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(this), FXMVector); }

		FQuat & Set(DirectX::FXMVECTOR FXMVector)
		{
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(this), FXMVector);
			return *this;
		}

		FQuat & operator =(DirectX::FXMVECTOR FXMVector) { return Set(FXMVector); }
		operator DirectX::XMVECTOR() const { return DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(this)); }
		DirectX::XMVECTOR ToXMVector() const { return DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4 *>(this)); }
		DirectX::XMVECTOR & AsXMVector() { return *reinterpret_cast<DirectX::XMVECTOR *>(this); }
		const DirectX::XMVECTOR & AsXMVector() const { return *reinterpret_cast<const DirectX::XMVECTOR *>(this); }

		FVector3 Transform(const FVector3 & Vector) const { return DirectX::XMVector3Rotate(Vector, ToXMVector()); }
		FVector3 InverseTransform(const FVector3 & Vector) const { return DirectX::XMVector3Rotate(Vector, ToXMVector()); }

	public:
		static const FQuat Zero;
		static const FQuat Identity;

	public:
		static FQuat Slerp(const FQuat & VectorMin, const FQuat & VectorMax, float32 Inter)
		{
			return DirectX::XMQuaternionSlerp(VectorMin, VectorMax, Inter);
		}

		static FQuat Rotate(float32 Pitch, float32 Yaw, float32 Roll)
		{
			return DirectX::XMQuaternionRotationRollPitchYaw(Pitch, Yaw, Roll);
		}

		static FQuat Rotate(const FVector3 & Angles)
		{
			return DirectX::XMQuaternionRotationRollPitchYawFromVector(Angles);
		}

		static FQuat RotateAxis(const FVector3 & Axis, float Angle)
		{
			return DirectX::XMQuaternionRotationAxis(Axis, Angle);
		}
	};

	static_assert(sizeof(FQuat) == sizeof(float) * 4);
	using quat4 = FQuat;

	inline const FQuat FQuat::Zero(0.0f, 0.0f, 0.0f, 0.0f);
	inline const FQuat FQuat::Identity(0.0f, 0.0f, 0.0f, 1.0f);

	inline FVector3 operator *(const FVector3 & Vector, const FQuat & Quat)
	{
		return DirectX::XMVector3Rotate(Vector, Quat);
	}
}
