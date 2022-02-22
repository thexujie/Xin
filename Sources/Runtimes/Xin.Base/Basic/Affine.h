#pragma once

#include "Vector2.h"

namespace Xin
{
	struct BASE_API FAffine
	{
		FAffine() = default;
		FAffine(const FAffine &) = default;
		FAffine & operator =(const FAffine &) = default;
		~FAffine() = default;

		FAffine(const FVector2 & Row0In, const FVector2 & Row1In, const FVector2 & Row2In) : Row0(Row0In), Row1(Row1In), Row2(Row2In) { }
		FAffine(const FVector2 (& Rows)[3]) : Row0(Rows[0]), Row1(Rows[1]), Row2(Rows[2]) { }

		FAffine(const float32 (& Floats)[6])
			: Row0(Floats[0], Floats[1])
			, Row1(Floats[2], Floats[3])
			, Row2(Floats[4], Floats[5]) {}

		FAffine(float32 (& Float3s)[3][2]) : Row0(Float3s[0]), Row1(Float3s[1]), Row2(Float3s[2]) { }

		FAffine(
			float32 F00, float32 F01,
			float32 F10, float32 F11,
			float32 F20, float32 F21)
			: Row0(F00, F01), Row1(F10, F11), Row2(F20, F21) { }

		FVector2 & operator [](int32 RowIndex) { return Rows[RowIndex]; }
		const FVector2 & operator [](int32 RowIndex) const { return Rows[RowIndex]; }

	public:
		union
		{
			FVector1 Floats[9];

			struct
			{
				FVector2 Rows[3];
			};

			struct
			{
				FVector2 Row0;
				FVector2 Row1;
				FVector2 Row2;
			};

			struct
			{
				float32 _11, _12;
				float32 _21, _22;
				float32 _31, _32;
			};

			struct
			{
				FVector2 Scale;
				FVector2 Skew;
				FVector2 Trans;
			};
		};

	public:
		static const FAffine Identity;
	};
}
