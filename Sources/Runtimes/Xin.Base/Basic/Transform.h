#pragma once

#include "Matrix.h"
#include "Quat.h"
#include "Stream.h"

namespace Xin
{
	/**
	 * RTS
	 */
	struct BASE_API FTransform
	{
		FQuat Rotation;
		FVector3 Translation;
		FVector3 Scaling;

		FTransform()
		{
			Rotation = FQuat::Identity;
			Translation = FVector3::Zero;
			Scaling = FVector3::One;
		}

		FTransform(const FQuat & Rotation_, const FVector3 & Translation_, const FVector3 & Scaling_)
		{
			Rotation = Rotation_;
			Translation = Translation_;
			Scaling = Scaling_;
		}

		FTransform(const FTransform &) = default;
		FTransform(FTransform &&) = default;
		FTransform & operator =(const FTransform & That) = default;

		FMatrix ToMatrix() const { return FMatrix::Transform(Translation, Rotation, Scaling); }
		FTransform Inverse() const;

		FTransform operator *(const FTransform & Another) const;

	public:
		static const FTransform Identity;

		static FTransform Rotate(FQuat Rotation)
		{
			FTransform Transform = {};
			Transform.Rotation = Rotation;
			Transform.Scaling = FVector3::One;
			return Transform;
		}

		static FTransform Scale(FVector3 Scaling)
		{
			FTransform Transform = {};
			Transform.Rotation = FQuat::Identity;
			Transform.Scaling = Scaling;
			return Transform;
		}

		static FTransform Translate(FVector3 Translation)
		{
			FTransform Transform = {};
			Transform.Rotation = FQuat::Identity;
			Transform.Translation = Translation;
			Transform.Scaling = FVector3::One;
			return Transform;
		}
	};

	inline IDataStream & operator <<(IDataStream & Stream, const FTransform & Transform)
	{
		Stream << Transform.Rotation;
		Stream << Transform.Translation;
		Stream << Transform.Scaling;
		return Stream;
	}

	inline IDataStream & operator >>(IDataStream & Stream, FTransform & Transform)
	{
		Stream >> Transform.Rotation;
		Stream >> Transform.Translation;
		Stream >> Transform.Scaling;
		return Stream;
	}
}
