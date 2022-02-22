#include "PCH.h"
#include "BasePassRendering.h"

#include "RenderGraph.h"

namespace Xin::Engine
{
	class FBasePassBaseShader : public IShaderSource
	{
	public:
		FBasePassBaseShader(FStringV AssetPath, EShaderStage ShaderStage, FStringV EntryName) : IShaderSource(ShaderStage, AssetPath, EntryName)
		{
		}
	};

	class FBasePassVS : public FBasePassBaseShader
	{
	public:
		using CSMCascadeCount = TShaderPermutationIntRange<u8"XIN_CSM_CASCADE_COUNT", 0, 4>;
		using CSMCascadeMax = TShaderPermutationIntRange<u8"XIN_CSM_CASCADE_MAX", 0, 4>;

		using FShaderVariance = TShaderVariance<>;

		DEFINE_SHADER_INSTANCE(FBasePassVS);

		FBasePassVS() : FBasePassBaseShader(u8"Engine/Shaders/BasePass.xsf"V, EShaderStage::Vertex, u8"BaseVertexShader"V)
		{
		}
	};

	class FBasePassPS : public FBasePassBaseShader
	{
	public:
		DEFINE_SHADER_INSTANCE(FBasePassPS);

		using SSAOTexture = TShaderPermutationBool<u8"XIN_SSAO_TEXTURE">;

		using CSMCascadeCount = TShaderPermutationIntRange<u8"XIN_CSM_CASCADE_COUNT", 0, 4>;
		using CSMCascadeMax = TShaderPermutationIntRange<u8"XIN_CSM_CASCADE_MAX", 0, 4>;

		using FShaderVariance = TShaderVariance<
			SSAOTexture,
			CSMCascadeCount, CSMCascadeMax>;

		FBasePassPS() : FBasePassBaseShader(u8"Engine/Shaders/BasePass.xsf"V, EShaderStage::Pixel, u8"BasePixelShader"V)
		{
		}
	};

	FBasePassRenderer::FBasePassRenderer(FRenderGraph & RenderGraph) : RenderGraph(RenderGraph)
	{
		SizeI SceneSize = RenderGraph.Scene.SceneView.ViewRect.Size;
		BaseColor = RenderGraph.CreateResource(u8"BaseColor"V, { EFormat::R16G16B16A16F, SceneSize, Colors::None });
		BaseNormal = RenderGraph.CreateResource(u8"BaseNormal"V, { EFormat::R11G11B10F, SceneSize, Colors::None });
	}

	void FBasePassRenderer::Render(IRHIResource * GlobalParameter, IRHIResource * ViewParameter, FRGTexture * SceneDepth, FRGTexture * SSAOOcclusion, FRGTexture * ShadowDepth)
	{
		FBasePassVS::FShaderVariance VertexShaderVariance;

		FBasePassPS::FShaderVariance PixelShaderVariance;
		PixelShaderVariance.SetValue<FBasePassPS::SSAOTexture>(!!SSAOOcclusion && RenderGraph.Scene.RenderSettings.SSAOMode != ESSAOMode::Disabled);
		PixelShaderVariance.SetValue<FBasePassPS::CSMCascadeCount>(RenderGraph.Scene.RenderSettings.CascadeLevel);
		PixelShaderVariance.SetValue<FBasePassPS::CSMCascadeMax>(CSMCascadeMax);

		MeshCommandCollector.CollectDrawCommands(RenderGraph, EMeshPass::Base, RenderGraph.Scene, GlobalParameter, ViewParameter, SSAOOcclusion, ShadowDepth,
			RenderGraph.Scene.RenderSettings.WireFrameRender ? FRasterizeState::DefaultWireframe : FRasterizeState::Default, FDepthStencilState(EComparison::Equal, false),
			FBasePassVS::Instance(), VertexShaderVariance, FBasePassPS::Instance(), PixelShaderVariance);

		RenderGraph.AddPass(u8"Base Pass"V,
			[&, ShadowDepth, SSAOOcclusion, SceneDepth](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(BaseColor, EResourceAccess::RenderTarget, EResourceLoad::Clear);
				CompileContext.AccessResource(BaseNormal, EResourceAccess::RenderTarget, EResourceLoad::Clear); // Discard for SRV is invalid.
				CompileContext.AccessResource(ShadowDepth, EResourceAccess::PixelSampledTexture, EResourceLoad::None);
				CompileContext.AccessResource(SceneDepth, EResourceAccess::DepthRead, EResourceLoad::None);
				if (RenderGraph.Scene.RenderSettings.SSAOMode != ESSAOMode::Disabled)
					CompileContext.AccessResource(SSAOOcclusion, EResourceAccess::PixelSampledTexture);
			},
			[&, ShadowDepth, SSAOOcclusion, SceneDepth](FRGContext & RGContext)
			{
				RGContext.BeginPass({ { BaseColor, EResourceLoad::Clear }, { BaseNormal, EResourceLoad::Clear } }, { SceneDepth, EResourceLoad::Load }, RenderGraph.Scene.SceneView.Viewport);
				RGContext.DispatchCommands(MeshCommandCollector.DrawCommands);
				RGContext.EndPass();
			});
	}
}
