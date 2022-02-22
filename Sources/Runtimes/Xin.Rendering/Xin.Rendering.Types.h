#pragma once

#include "Xin.RHI/Xin.RHI.h"

#if XIN_WINDOWS
#ifdef _Xin_Rendering_Module_
#define RENDERING_API __declspec(dllexport)
#else
#define RENDERING_API
#endif
#endif

namespace Xin::Rendering
{
	using namespace Xin::RHI;

	class FEngine;
	class FCommandList;
	class FCommandExecutor;


	class RENDERING_API IRenderingObject : public ISharedRefer
	{
	public:
		virtual void SetDebugName(FStringV Name) { DebugName = Name ; }

	public:
		FString DebugName;
	};
	using IRenderingObjectRef = TReferPtr<IRenderingObject>;

	struct FMeshRenderState
	{
		ETopology Topology = ETopology::TriangleList;
		TArray<IShaderInstance *, MaxShaderStages> Shaders {};
		TArray<FInputElement, MaxInputElements> InputElements {};
		FRasterizeState RasterizeState;
		FBlendState BlendState;
		FDepthStencilState DepthStencilState;

		auto operator<=>(const FMeshRenderState &) const noexcept = default;

		friend uintx Hash(const FMeshRenderState & MeshRenderState)
		{
			return HashCombine(
				MeshRenderState.Topology,
				MeshRenderState.Shaders,
				MeshRenderState.InputElements,
				MeshRenderState.RasterizeState,
				MeshRenderState.BlendState,
				MeshRenderState.DepthStencilState
			);
		}
	};
	using FMeshRenderStateIndex = TNamedValue<uint32, "MeshRenderState">;

	class RENDERING_API IAssetStream : public IDataStream
	{
	public:
		IAssetStream(FStringV AssetPath, FStringV FilePath) : AssetPath(AssetPath), FilePath(FilePath) {}

	public:
		FString AssetPath;
		FString FilePath;
	};
	using IAssetStreamRef = TReferPtr<IAssetStream>;
}
