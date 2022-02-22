#pragma once

#include "Vector4.h"
#include "Quat.h"

namespace Xin
{
	struct FMatrix
	{
		FMatrix() = default;
		FMatrix(const FMatrix &) = default;
		FMatrix & operator =(const FMatrix &) = default;
		~FMatrix() = default;

		FMatrix(const FVector4 & Row0In, const FVector4 & Row1In, const FVector4 & Row2In, const FVector4 & Row3In) : Row0(Row0In), Row1(Row1In), Row2(Row2In), Row3(Row3In) { }
		FMatrix(const FVector4 (& Rows)[4]) : Row0(Rows[0]), Row1(Rows[1]), Row2(Rows[2]), Row3(Rows[3]) { }

		FMatrix(const float32 (& Floats)[16])
			: Row0(Floats[0], Floats[1], Floats[2], Floats[3])
			, Row1(Floats[4], Floats[5], Floats[6], Floats[7])
			, Row2(Floats[8], Floats[9], Floats[10], Floats[11])
			, Row3(Floats[12], Floats[13], Floats[14], Floats[15]) {}

		FMatrix(const float32 (& Float4s)[4][4]) : Row0(Float4s[0]), Row1(Float4s[1]), Row2(Float4s[2]), Row3(Float4s[3]) { }

		FMatrix(float32 F00, float32 F01, float32 F02, float32 F03,
			float32 F10, float32 F11, float32 F12, float32 F13,
			float32 F20, float32 F21, float32 F22, float32 F23,
			float32 F30, float32 F31, float32 F32, float32 F33) :
			Row0(F00, F01, F02, F03), Row1(F10, F11, F12, F13), Row2(F20, F21, F22, F23), Row3(F30, F31, F32, F33) { }

		FVector4 & operator [](int32 RowIndex) { return Rows[RowIndex]; }
		const FVector4 & operator [](int32 RowIndex) const { return Rows[RowIndex]; }

	public:
		FMatrix Inverse() const { return DirectX::XMMatrixInverse(nullptr/*!!!*/, ToXMMatrix()); }

		FMatrix Inverse(FVector4 & Determinant) const
		{
			DirectX::XMVECTOR XMDeterminant;
			DirectX::XMMATRIX XMMatrix = DirectX::XMMatrixInverse(&XMDeterminant, ToXMMatrix());
			Determinant = XMDeterminant;
			return XMMatrix;
		}

		FMatrix Transpose() const { return DirectX::XMMatrixTranspose(ToXMMatrix()); }

		FMatrix Multiply(const FMatrix & Another) const { return DirectX::XMMatrixMultiply(ToXMMatrix(), Another.ToXMMatrix()); }

		FVector3 Transform(const FVector3 & Point) const { return DirectX::XMVector3Transform(Point, ToXMMatrix()); }
		FVector3 TransformNormal(const FVector3 & Normal) const { return DirectX::XMVector3TransformNormal(Normal, ToXMMatrix()); }
		FVector3 TransformNormal(const FVector4 & Normal) const { return DirectX::XMVector3TransformNormal(Normal, ToXMMatrix()); }
		FVector3 TransformCoord(const FVector3 & Point) const { return DirectX::XMVector3TransformCoord(Point, ToXMMatrix()); }
		FVector3 TransformCoord(const FVector4 & Point) const { return DirectX::XMVector3TransformCoord(Point, ToXMMatrix()); }
		FVector4 Transform(const FVector4 & Vector) const { return DirectX::XMVector4Transform(Vector, ToXMMatrix()); }

		bool Decompose(FVector3 & Scale, FQuat & Rotation, FVector3 & Translation) const
		{
			DirectX::XMVECTOR XMScale;
			DirectX::XMVECTOR XMRotate;
			DirectX::XMVECTOR XMTranslation;
			if (!DirectX::XMMatrixDecompose(&XMScale, &XMRotate, &XMTranslation, ToXMMatrix()))
				return false;
			Scale = XMScale;
			Rotation = XMRotate;
			Translation = XMTranslation;
			return true;
		}

		FQuat Rotation() const { return DirectX::XMQuaternionRotationMatrix(ToXMMatrix()); }

		FVector3 Scale() const
		{
			DirectX::XMVECTOR XMScale, XMRotate, XMTranslation;
			if (!DirectX::XMMatrixDecompose(&XMScale, &XMRotate, &XMTranslation, ToXMMatrix()))
				return FVector3::One;
			return XMScale;
		}

		FVector3 Translation() const { return Row3.XYZ; }

		FMatrix operator *(const FMatrix & Another) const { return Multiply(Another); }
		FMatrix & operator *=(const FMatrix & Another) { return Set(DirectX::XMMatrixMultiply(ToXMMatrix(), Another.ToXMMatrix())); }

	public:
		union
		{
			FVector1 Floats[16];

			struct
			{
				FVector4 Rows[4];
			};

			struct
			{
				FVector4 Row0;
				FVector4 Row1;
				FVector4 Row2;
				FVector4 Row3;
			};

			struct
			{
				float32 _11, _12, _13, _14;
				float32 _21, _22, _23, _24;
				float32 _31, _32, _33, _34;
				float32 _41, _42, _43, _44;
			};
		};

	public:
		FMatrix(const DirectX::XMMATRIX & XMMatrix) : Row0(XMMatrix.r[0]), Row1(XMMatrix.r[1]), Row2(XMMatrix.r[2]), Row3(XMMatrix.r[3]) {}

		FMatrix & Set(const DirectX::XMMATRIX & XMMatrix)
		{
			Row0 = XMMatrix.r[0];
			Row1 = XMMatrix.r[1];
			Row2 = XMMatrix.r[2];
			Row3 = XMMatrix.r[3];
			return *this;
		}

		FMatrix & operator =(const DirectX::XMMATRIX & XMMatrix) { return Set(XMMatrix); }
		operator DirectX::XMMATRIX() const { return DirectX::XMMATRIX(Floats); }
		DirectX::XMMATRIX ToXMMatrix() const { return DirectX::XMMATRIX(Floats); }

	public:
		static const FMatrix Identity;
		static const FMatrix ReverseZ;
		FMatrix ReversedZ() const { return Multiply(ReverseZ); }

	public:
		static FMatrix Transform(const FVector3 & Translation, const FVector3 & Rotation, const FVector3 & Scaling)
		{
			return DirectX::XMMatrixAffineTransformation(Scaling, {}, FQuat::Rotate(Rotation), Translation);
		}

		static FMatrix Transform(const FVector3 & Translation, const FQuat & Rotation, const FVector3 & Scaling)
		{
			return DirectX::XMMatrixAffineTransformation(Scaling, {}, Rotation, Translation);
		}

		static FMatrix Translate(const FVector3 & Translation)
		{
			return DirectX::XMMatrixTranslationFromVector(Translation);
		}

		static FMatrix Translate(float32 OffsetX, float32 OffsetY, float32 OffsetZ)
		{
			return DirectX::XMMatrixTranslation(OffsetX, OffsetY, OffsetZ);
		}

		static FMatrix RotatePitchYawRoll(const FVector3 & PitchYawRoll)
		{
			return DirectX::XMMatrixRotationRollPitchYaw(PitchYawRoll.X, PitchYawRoll.Y, PitchYawRoll.Z);
		}

		static FMatrix RotatePitchYawRoll(float32 Pitch, float32 Yaw, float32 Roll)
		{
			return DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll);
		}

		static FMatrix Rotate(const FQuat & Quatation)
		{
			return DirectX::XMMatrixRotationQuaternion(Quatation);
		}

		static FMatrix RotateAxis(const FVector4 & Axis, float32 Angle)
		{
			return DirectX::XMMatrixRotationAxis(Axis, Angle);
		}

		static FMatrix Scale(const FVector3 & Scale)
		{
			return DirectX::XMMatrixScalingFromVector(Scale);
		}

		static FMatrix Scale(float32 ScaleX, float32 ScaleY, float32 ScaleZ)
		{
			return DirectX::XMMatrixScaling(ScaleX, ScaleY, ScaleZ);
		}

		static FMatrix PerspectiveLH(float32 FovY, float32 AspectRatio, float32 NearZ, float32 FarZ)
		{
			return DirectX::XMMatrixPerspectiveFovLH(FovY, AspectRatio, NearZ, FarZ);
		}

		static FMatrix PerspectiveRH(float32 FovY, float32 AspectRatio, float32 NearZ, float32 FarZ)
		{
			return DirectX::XMMatrixPerspectiveFovRH(FovY, AspectRatio, NearZ, FarZ);
		}

		static FMatrix OrthographicLH(float32 ViewWidth, float32 ViewHeight, float32 NearZ, float32 FarZ)
		{
			return DirectX::XMMatrixOrthographicLH(ViewWidth, ViewHeight, NearZ, FarZ);
		}

		static FMatrix OrthographicRH(float32 ViewWidth, float32 ViewHeight, float32 NearZ, float32 FarZ)
		{
			return DirectX::XMMatrixOrthographicRH(ViewWidth, ViewHeight, NearZ, FarZ);
		}

		static FMatrix OrthographicOffCenterLH(float32 Left, float32 Right, float32 Bottom, float32 Top, float32 NearZ, float32 FarZ)
		{
			return DirectX::XMMatrixOrthographicOffCenterLH(Left, Right, Bottom, Top, NearZ, FarZ);
		}

		static FMatrix OrthographicOffCenterRH(float32 Left, float32 Right, float32 Bottom, float32 Top, float32 NearZ, float32 FarZ)
		{
			return DirectX::XMMatrixOrthographicOffCenterRH(Left, Right, Bottom, Top, NearZ, FarZ);
		}

		static FMatrix LookAtLH(const FVector3 & Position, const FVector3 & Center, const FVector3 & Upward)
		{
			return DirectX::XMMatrixLookAtLH(Position, Center, Upward);
		}

		static FMatrix LookAtRH(const FVector3 & Position, const FVector3 & Center, const FVector3 & Upward)
		{
			return DirectX::XMMatrixLookAtRH(Position, Center, Upward);
		}

		static FMatrix LookToLH(const FVector3 & Position, const FVector3 & Forward, const FVector3 & Upward)
		{
			return DirectX::XMMatrixLookToLH(Position, Forward, Upward);
		}

		static FMatrix LookToRH(const FVector3 & Position, const FVector3 & Forward, const FVector3 & Upward)
		{
			return DirectX::XMMatrixLookToRH(Position, Forward, Upward);
		}
	};

	static_assert(sizeof(FMatrix) == sizeof(float) * 16, "Size of Matrix is not rational.");
	using float4x4 = FMatrix;

	inline const FMatrix FMatrix::Identity = { FVector4::AxisX, FVector4::AxisY, FVector4::AxisZ, FVector4::AxisW };
	inline const FMatrix FMatrix::ReverseZ = { FVector4::AxisX, FVector4::AxisY, { 0, 0, -1, 0 }, { 0, 0, 1, 1 } };

	inline FVector3 operator *(const FVector3 & Point, const FMatrix & Matrix)
	{
		return Matrix.TransformCoord(Point);
	}

	inline FVector3 operator *(const FMatrix & Matrix, const FVector3 & Point)
	{
		return Matrix.TransformCoord(Point);
	}

	inline FVector4 operator *(const FVector4 & Point, const FMatrix & Matrix)
	{
		return Matrix.Transform(Point);
	}

	inline FVector4 operator *(const FMatrix & Matrix, const FVector4 & Point)
	{
		return Matrix.Transform(Point);
	}
}
