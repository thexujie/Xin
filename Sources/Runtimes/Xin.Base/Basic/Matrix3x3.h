#pragma once

#include "Vector3.h"

namespace Xin
{
	struct FMatrix3x3
	{
		FMatrix3x3() = default;
		FMatrix3x3(const FMatrix3x3 &) = default;
		FMatrix3x3 & operator =(const FMatrix3x3 &) = default;
		~FMatrix3x3() = default;

		FMatrix3x3(const FVector3 & Row0In, const FVector3 & Row1In, const FVector3 & Row2In) : Row0(Row0In), Row1(Row1In), Row2(Row2In) { }
		FMatrix3x3(const FVector3 (& Rows)[3]) : Row0(Rows[0]), Row1(Rows[1]), Row2(Rows[2]) { }

		FMatrix3x3(const float32 (& Floats)[9])
			: Row0(Floats[0], Floats[1], Floats[2])
			, Row1(Floats[3], Floats[4], Floats[5])
			, Row2(Floats[6], Floats[7], Floats[8]) {}

		FMatrix3x3(float32 (& Float3s)[3][3]) : Row0(Float3s[0]), Row1(Float3s[1]), Row2(Float3s[2]) { }

		FMatrix3x3(float32 F00, float32 F01, float32 F02,
			float32 F10, float32 F11, float32 F12,
			float32 F20, float32 F21, float32 F22)
			: Row0(F00, F01, F02), Row1(F10, F11, F12), Row2(F20, F21, F22) { }

		FVector3 & operator [](int32 RowIndex) { return Rows[RowIndex]; }
		const FVector3 & operator [](int32 RowIndex) const { return Rows[RowIndex]; }

	public:
		union
		{
			FVector1 Floats[9];

			struct
			{
				FVector3 Rows[3];
			};

			struct
			{
				FVector3 Row0;
				FVector3 Row1;
				FVector3 Row2;
			};

			struct
			{
				float32 _11, _12, _13;
				float32 _21, _22, _23;
				float32 _31, _32, _33;
			};
		};

	public:
		static const FMatrix3x3 Identity;
	};

	static_assert(sizeof(FMatrix3x3) == sizeof(float) * 9);
	using float3x3 = FMatrix3x3;

	inline const FMatrix3x3 FMatrix3x3::Identity = { FVector3::AxisX, FVector3::AxisY, FVector3::AxisZ };
}
