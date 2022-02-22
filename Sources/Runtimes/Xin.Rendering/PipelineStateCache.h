#pragma once

#include "Xin.Rendering.Types.h"

namespace Xin::Rendering
{
	enum EStaticSamplerRegisterIndex
	{
		SSRI_PointWrap = 5,
		SSRI_PointClamp,
		SSRI_PointBorder,
		SSRI_LinearWrap,
		SSRI_LinearClamp,
		SSRI_LinearBorder,
		SSRI_TrilinearWrap,
		SSRI_TrilinearClamp,
		SSRI_TrilinearBorder,
		SSRI_Shadow,
	};

	using FIndirectSignatureIndex = TNamedValue<uint32, "IndirectSignature">;

	class FCachedPipelineState
	{
	public:
		uintx HashCode;

		TArray<IShaderInstance *, MaxShaderStages> Shaders {};
		ETopology Topology;
		TArray<FInputElement, MaxInputElements> InputElements {};
		FOutputFormat OutputFormat;
		FRasterizeState RasterizeState;
		FBlendState BlendState;
		FDepthStencilState DepthStencilState;

		bool operator ==(const FCachedPipelineState & Another) const { return HashCode == Another.HashCode; }
		friend uintx Hash(const FCachedPipelineState & This) { return This.HashCode; }
	};


	class FCachedIndirectSignature
	{
	public:
		uintx HashCode;

		IRHIPipelineState * RHIPipelineState;
		FIndirectSignature IndirectSignature;

		bool operator ==(const FCachedIndirectSignature & Another) const { return HashCode == Another.HashCode; }
		friend uintx Hash(const FCachedIndirectSignature & Index) { return Index.HashCode; }
	};

	class RENDERING_API FPipelineStateCache : public IRenderingObject
	{
	public:
		FPipelineStateCache(IRHIDevice & Device, bool ReversedZ);

	public:
		IRHIPipelineState * FetchPipelineState(TArray<IShaderInstance *, MaxShaderStages> Shaders,
			ETopology Topology, TView<FInputElement> InputElements, FOutputFormat OutputFormat,
			FRasterizeState RasterizeState, FBlendState BlendState, FDepthStencilState DepthStencilState);

		IRHIIndirectSignature * FetchIndirectSignature(const FIndirectSignature & IndirectSignature, IRHIPipelineState * RHIPipelineState = nullptr);

	private:
		THashMap<FCachedPipelineState, IRHIPipelineStateRef> PipilineStates;
		THashMap<FCachedIndirectSignature, IRHIIndirectSignatureRef> IndirectSignatures;

	public:
		FMeshRenderStateIndex IndexMeshRenderState(const FMeshRenderState & MeshRenderState);
		const FMeshRenderState & IndexMeshRenderState(FMeshRenderStateIndex MeshRenderStateIndex);

		FIndirectSignatureIndex IndexIndirectSignature(const FIndirectSignature & IndirectSignature);
		const FIndirectSignature & IndexIndirectSignature(FIndirectSignatureIndex IndirectSignatureIndex);

	public:
		TList<FMeshRenderState> MeshRenderStates;
		TMap<uintx, TPair<FMeshRenderStateIndex, FMeshRenderState>> MeshRenderStateIndices;

		TList<FIndirectSignature> IndexedIndirectSignatures;
		TMap<FIndirectSignature, FIndirectSignatureIndex> IndirectSignatureIndices;

	public:
		IRHIDevice & Device;
		bool ReversedZ = false;
	};
}
