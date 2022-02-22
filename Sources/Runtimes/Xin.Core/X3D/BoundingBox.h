#pragma once

#include "X3D.Types.h"

namespace Xin
{
	class CORE_API FBoundingBox
	{
	public:
		FBoundingBox() = default;
		constexpr FBoundingBox(const FBoundingBox &) = default;
		FBoundingBox & operator =(const FBoundingBox &) = default;
		~FBoundingBox() = default;
		bool operator ==(const FBoundingBox &) const noexcept = default;
		auto operator <=>(const FBoundingBox & Another) const = default; // { return TVec4<uint8>::operator <=>(Another); }

		constexpr FBoundingBox(FVector3 InitPoint) : MinPoint(InitPoint), MaxPoint(InitPoint) {}
		constexpr FBoundingBox(FVector3 MinPoint, FVector3 MaxPoint) : MinPoint(MinPoint), MaxPoint(MaxPoint) {}
		explicit operator bool() const { return (MaxPoint - MinPoint).LengthSquare() > 0; }

		//friend uintx Hash(const FBoundingBox & BoundingBox)
		//{
		//	return Hash(BoundingBox.Min, BoundingBox.Max);
		//}

		FBoundingBox & AddPoint(FVector3 Point)
		{
			MinPoint = Min(MinPoint, Point);
			MaxPoint = Max(MinPoint, Point);
			return *this;
		}

		FBoundingBox & AddPoints(TView<FVector3> Points)
		{
			for (const FVector3 & Point : Points)
			{
				MinPoint = Min(MinPoint, Point);
				MaxPoint = Max(MinPoint, Point);
			}
			return *this;
		}

	public:
		FVector3 MinPoint;
		FVector3 MaxPoint;
	};
}
