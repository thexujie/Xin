#pragma once

#include "RHITypes.h"
#include "RHIAdapter.h"
#include "RHICommandList.h"
#include "RHIFence.h"
#include "RHIHeap.h"
#include "RHIShaderCompiler.h"
#include "RHIViewport.h"

namespace Xin::RHI
{
	class IRHIShaderModule;

	class RHI_API IShaderInstance : public IRHIObject
	{
	public:
		virtual EShaderStage GetShaderStage() const = 0;
		virtual FStringV GetFilePath() const = 0;
		virtual FStringV GetEntryName() const = 0;

		virtual TView<byte> GetShaderCode() const = 0;
		virtual const FReflectParameters & GetReflectParameters() const = 0;

		virtual IRHIShaderModule * GetShaderModule() = 0;
	};
	using IShaderInstanceRef = TReferPtr<IShaderInstance>;


	class RHI_API IRHIShaderModule : public IRHIObject
	{
	public:
		IRHIShaderModule(IShaderInstance & ShaderInstance) : ShaderInstance(ShaderInstance) {}

	public:
		IShaderInstance & ShaderInstance;
	};

	using IRHIShaderModuleRef = TReferPtr<IRHIShaderModule>;


	constexpr uint32 MaxStaticSamplers = 16;

	struct FPipelineStateDesc
	{
		ETopology Topology = ETopology::TriangleList;
		TArray<IRHIShaderModule *, MaxShaderStages> Shaders {};

		FInputFormat InputFormat;
		FOutputFormat OutputFormat;

		FRasterizeState RasterizeState;
		FBlendState BlendState;
		FDepthStencilState DepthStencilState;

		TArray<FSamplerDesc, MaxStaticSamplers> StaticSamplers;

		bool IsStaticSampler(uint32 BindingIndex, uint32 BindingSpace) const
		{
			for (const FSamplerDesc & StaticSampler : StaticSamplers)
			{
				if (StaticSampler.BindingIndex == BindingIndex && StaticSampler.BindingSpace == BindingSpace)
					return true;
			}
			return false;
		}
	};

	class RHI_API IRHIDevice : public IRHIObject
	{
	public:
		IRHIDevice() = default;

	public:
		virtual EDeviceRHI GetDeviceRHI() const = 0;
		virtual ETextureCompression GetTextureCompression() const = 0;

		virtual IRHIShaderCompiler & GetShaderCompiler() = 0;

		virtual IRHIFenceRef CreateFence() = 0;
		virtual IRHIShaderModuleRef CreateShaderModule(IShaderInstance & ShaderInstance) = 0;
		virtual IRHIPipelineStateRef CreatePipelineState(const FPipelineStateDesc & PipelineStateDesc) = 0;

		virtual IRHICommandListRef CreateCommandList(ECommandType CommandType) = 0;
		virtual IRHICommandQueueRef CreateCommandQueue(const FCommandQueueDesc & ExecutorDesc) = 0;

		virtual IRHIViewportRef CreateViewport(IRHICommandQueue * CommandExecutor, voidp WindowHandle, SizeU Size, uint32 NumBuffers,
			EFormat Format = EFormat::RGBA, EPresentMode SwapEffect = EPresentMode::FlipDiscard, FMSAA MSAA = {}) = 0;

		virtual IRHIHeapRef CreateHeap(const FHeapDesc & HeapDesc) = 0;
		virtual IRHIResourceRef CreateCommitedResource(const FResourceDesc & ResourceDes) = 0;
		virtual IRHIResourceRef CreateReservedResource(const FResourceDesc & ResourceDesc) = 0;
		virtual IRHIResourceRef CreatePlacedResource(const FResourceDesc & ResourceDesc, const FRHIMemory & Memory) = 0;
		virtual IRHIResourceRef CreatePlacedResource(const FResourceDesc & ResourceDesc, IHeapAllocator & HeapAllocator) = 0;
		virtual IRHIResourceViewRef CreateResourceView(IRHIResource * RHIResource, const FResourceViewDesc & ResourceViewDesc) = 0;

		virtual IRHIIndirectSignatureRef CreateIndirectSignature(const FIndirectSignature & IndirectSignature, IRHIPipelineState * RHIPipelineState = nullptr) = 0;

	public:
		virtual EResourceAccess GetForamtAccess(EFormat Format) const = 0;
		virtual FAllocateInfo GetAllocateInfo(const FResourceDesc & ResourceDesc, bool LocatedResource = false) const = 0;
		virtual void UpdateContext() = 0;
		virtual void ClearContext() = 0;
		virtual uint64 GetMemoryUsage() const = 0;
	};

	using IRHIDeviceRef = TReferPtr<IRHIDevice>;
}
