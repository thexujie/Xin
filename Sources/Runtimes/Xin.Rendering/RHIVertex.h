#pragma once

#include "Xin.Rendering.Types.h"

namespace Xin::Rendering
{
	struct FRHIVertex2D
	{
		FRHIVertex2D() = default;
		FRHIVertex2D(const PointF & Position, const PointF & Texcoord = {}) : Position(Position.X, Position.Y, 0.0f, 1.0f), Texcoord(Texcoord) {}
		FRHIVertex2D(const FVector2 & Position, const FVector2 & Texcoord = {}) : Position(Position.X, Position.Y, 0.0f, 1.0f), Texcoord(Texcoord) {}
		FRHIVertex2D(const FVector4 & Position, const FVector2 & Texcoord = {}) : Position(Position), Texcoord(Texcoord) {}

		FVector4 Position;
		FVector2 Texcoord;
	};
	static_assert(sizeof(FRHIVertex2D) == sizeof(float) * 6);
}
