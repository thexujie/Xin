#pragma once

#include "../Xin.Engine.Types.h"
#include "RenderMesh.h"

namespace Xin::Engine
{
	class alignas(sizeof(float4)) FPrimitiveParameter
	{
	public:
		FMatrix LocalToWorld;
	};

	class ENGINE_API IPrimitive : public IRenderingObject
	{
	public:
		IPrimitive(IRHIDevice & Devcie) : Devcie(Devcie) {}
		~IPrimitive();

		virtual FStringV GetPrimitiveName() const { return {}; }

		virtual void InitializeRHI(FCommandExecutor & Executor) {}
		virtual void UpdateRHI() {}

	public:
		virtual FMatrix GetWorldMatrix() const = 0;

	public:
		IRHIDevice & Devcie;
		TList<FRenderMesh> RenderMeshs;
	};
	using IPrimitiveRef = TReferPtr<IPrimitive>;

	class FPrimitiveCollector : public IEngineObject
	{
	public:
		FPrimitiveCollector(FEngine & Engine) : Engine(Engine) {}

	public:
		FEngine & Engine;
		TList<IPrimitiveRef> Primitives;
	};
}
