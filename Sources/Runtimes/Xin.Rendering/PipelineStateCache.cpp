#include "PCH.h"
#include "PipelineStateCache.h"

namespace Xin::Rendering
{
	static FSamplerDesc MakeStaticShadowComparisonSampler(EAddressMode AddressMode, EComparison Comparison, uint32 BindingIndex, uint32 BindingSpace, EBorderColor BorderColor)
	{
		FSamplerDesc SamplerDesc = { };

		SamplerDesc.ShaderRelevance = EShaderRelevance::All;
		SamplerDesc.BindingIndex = BindingIndex;
		SamplerDesc.BindingSpace = BindingSpace;

		SamplerDesc.SampleFilter = ESampleFilter::Linear;

		SamplerDesc.AddressModeU = AddressMode;
		SamplerDesc.AddressModeV = AddressMode;
		SamplerDesc.AddressModeW = AddressMode;

		SamplerDesc.MipLODBias = 0.0f;
		SamplerDesc.MaxAnisotropy = 8;
		SamplerDesc.Comparison = Comparison;

		SamplerDesc.MinLOD = 0.0f;
		SamplerDesc.MaxLOD = Float32Max;

		SamplerDesc.BorderColor = BorderColor;

		return SamplerDesc;
	}

	static FSamplerDesc MakeStaticSampler(ESampleFilter SampleFilter, EAddressMode AddressMode, uint32 BindingIndex, uint32 BindingSpace, EBorderColor BorderColor = EBorderColor::TransparentBlack)
	{
		FSamplerDesc SamplerDesc = { };

		SamplerDesc.ShaderRelevance = EShaderRelevance::All;
		SamplerDesc.BindingIndex = BindingIndex;
		SamplerDesc.BindingSpace = BindingSpace;

		SamplerDesc.SampleFilter = SampleFilter;
		SamplerDesc.AddressModeU = AddressMode;
		SamplerDesc.AddressModeV = AddressMode;
		SamplerDesc.AddressModeW = AddressMode;

		SamplerDesc.MipLODBias = 0.0f;
		SamplerDesc.MaxAnisotropy = 8;
		SamplerDesc.Comparison = EComparison::Never;

		SamplerDesc.MinLOD = 0.0f;
		SamplerDesc.MaxLOD = Float32Max;

		SamplerDesc.BorderColor = BorderColor;

		return SamplerDesc;
	}

	const uint32 StaticSamplerBindingSpace = 0;

	const FSamplerDesc StaticSamplerDescs[] =
	{
		MakeStaticSampler(ESampleFilter::Point, EAddressMode::Wrap, SSRI_PointWrap, StaticSamplerBindingSpace),
		MakeStaticSampler(ESampleFilter::Point, EAddressMode::Clamp, SSRI_PointClamp, StaticSamplerBindingSpace),
		MakeStaticSampler(ESampleFilter::Point, EAddressMode::Border, SSRI_PointBorder, StaticSamplerBindingSpace),
		MakeStaticSampler(ESampleFilter::Linear, EAddressMode::Wrap, SSRI_LinearWrap, StaticSamplerBindingSpace),
		MakeStaticSampler(ESampleFilter::Linear, EAddressMode::Clamp, SSRI_LinearClamp, StaticSamplerBindingSpace),
		MakeStaticSampler(ESampleFilter::Linear, EAddressMode::Border, SSRI_LinearBorder, StaticSamplerBindingSpace),
		MakeStaticSampler(ESampleFilter::Trilinear, EAddressMode::Wrap, SSRI_TrilinearWrap, StaticSamplerBindingSpace),
		MakeStaticSampler(ESampleFilter::Trilinear, EAddressMode::Clamp, SSRI_TrilinearClamp, StaticSamplerBindingSpace),
		MakeStaticSampler(ESampleFilter::Trilinear, EAddressMode::Border, SSRI_TrilinearBorder, StaticSamplerBindingSpace),
	};

	FPipelineStateCache::FPipelineStateCache(IRHIDevice & Device, bool ReversedZ) : Device(Device), ReversedZ(ReversedZ)
	{

	}

	IRHIPipelineState * FPipelineStateCache::FetchPipelineState(TArray<IShaderInstance *, MaxShaderStages> Shaders,
		ETopology Topology, TView<FInputElement> InputElements, FOutputFormat OutputFormat,
		FRasterizeState RasterizeState, FBlendState BlendState, FDepthStencilState DepthStencilState)
	{
		FCachedPipelineState CachedPipelineState =
		{
			0,
			Shaders, Topology, InputElements, OutputFormat,
			RasterizeState, BlendState, DepthStencilState
		};
		CachedPipelineState.HashCode = HashCombine(Shaders, Topology, InputElements, OutputFormat, RasterizeState, BlendState, DepthStencilState);

		if (auto Iter = PipilineStates.Find(CachedPipelineState); Iter != PipilineStates.End())
			return Iter->second.Get();

		TArray<IRHIShaderModule *, MaxShaderStages> ShaderModules {};
		for (uint32 ShaderStageIndex = 0; ShaderStageIndex < MaxShaderStages; ++ShaderStageIndex)
		{
			IRHIShaderModule * ShaderModule = Shaders[ShaderStageIndex] ? Shaders[ShaderStageIndex]->GetShaderModule() : nullptr;
			if (ShaderModule)
				ShaderModules[ShaderModule->ShaderInstance.GetShaderStage()] = ShaderModule;
		}

		TArray<FSamplerDesc, MaxStaticSamplers> StaticSamplers;
		StaticSamplers = StaticSamplerDescs;
		StaticSamplers[ArraySize(StaticSamplerDescs)] = MakeStaticShadowComparisonSampler(EAddressMode::Border,
			ReversedZ ? EComparison::GreaterEqual : EComparison::LessEqual, SSRI_Shadow, StaticSamplerBindingSpace, 
			ReversedZ ? EBorderColor::OpaqueWhite : EBorderColor::TransparentBlack);
		
		FPipelineStateDesc PipelineStateDesc = 
		{
			Topology,
			ShaderModules,
			{ EPrimitiveTopology::None, { }, InputElements }, OutputFormat,
			RasterizeState, BlendState, DepthStencilState,
			StaticSamplers,
		};
		IRHIPipelineStateRef RHIPipelineState = Device.CreatePipelineState(PipelineStateDesc);
		PipilineStates.InsertOrAssign(CachedPipelineState, RHIPipelineState);
		return RHIPipelineState.Get();
	}

	IRHIIndirectSignature * FPipelineStateCache::FetchIndirectSignature(const FIndirectSignature & IndirectSignature, IRHIPipelineState * RHIPipelineState)
	{
		FCachedIndirectSignature CachedIndirectSignature {};
		CachedIndirectSignature.HashCode = HashCombine(RHIPipelineState, IndirectSignature);
		CachedIndirectSignature.RHIPipelineState = RHIPipelineState;
		CachedIndirectSignature.IndirectSignature = IndirectSignature;

		if (auto Iter = IndirectSignatures.Find(CachedIndirectSignature); Iter != IndirectSignatures.End())
			return Iter->second.Get();

		IRHIIndirectSignatureRef RHIIndirectSignature = Device.CreateIndirectSignature(IndirectSignature, RHIPipelineState);
		IndirectSignatures.InsertOrAssign(CachedIndirectSignature, RHIIndirectSignature);
		return RHIIndirectSignature.Get();
	}

	FMeshRenderStateIndex FPipelineStateCache::IndexMeshRenderState(const FMeshRenderState & MeshRenderState)
	{
		// We must initialize FMeshRenderState by default {} to avoid UB padding bytes.
		uintx HashCode = CityHash64(MeshRenderState);
		//uintx HashCode = Hash(MeshRenderState);
		if (auto Iter = MeshRenderStateIndices.Find(HashCode); Iter != MeshRenderStateIndices.End())
			return Iter->Second.First;

		FMeshRenderStateIndex MeshRenderStateIndex = (uint32)(MeshRenderStates.Size);
		MeshRenderStates.Add(MeshRenderState);
		MeshRenderStateIndices.Insert({ HashCode, TPair { MeshRenderStateIndex, MeshRenderState } });
		return MeshRenderStateIndex;
	}

	const FMeshRenderState & FPipelineStateCache::IndexMeshRenderState(FMeshRenderStateIndex MeshRenderStateIndex)
	{
		return MeshRenderStates[MeshRenderStateIndex.Value];
	}

	FIndirectSignatureIndex FPipelineStateCache::IndexIndirectSignature(const FIndirectSignature & IndirectSignature)
	{
		if (auto Iter = IndirectSignatureIndices.Find(IndirectSignature); Iter != IndirectSignatureIndices.End())
			return Iter->Second;

		FIndirectSignatureIndex IndirectSignatureIndex = (uint32)(IndexedIndirectSignatures.Size);
		IndexedIndirectSignatures.Add(IndirectSignature);
		IndirectSignatureIndices.Insert({ IndirectSignature, IndirectSignatureIndex });
		return IndirectSignatureIndex;
	}

	const FIndirectSignature & FPipelineStateCache::IndexIndirectSignature(FIndirectSignatureIndex IndirectSignatureIndex)
	{
		return IndexedIndirectSignatures[IndirectSignatureIndex.Value];
	}
}
