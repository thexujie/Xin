#include "PCH.h"
#include "DepthRendering.h"

#include "RenderGraph.h"

namespace Xin::Engine
{
	class FDepthPassBaseShader : public IShaderSource
	{
	public:
		using FShaderVariance = TShaderVariance<>;

		FDepthPassBaseShader(FStringV AssetPath, EShaderStage ShaderStage, FStringV Entry) : IShaderSource(ShaderStage, AssetPath, Entry) {}
	};

	class FDepthPassVS : public FDepthPassBaseShader
	{
	public:
		DEFINE_SHADER_INSTANCE(FDepthPassVS);

		FDepthPassVS() : FDepthPassBaseShader(u8"Engine/Shaders/DepthPass.xsf"V, EShaderStage::Vertex, u8"DepthVertexShader"V) {}
	};

	class FDepthPassPS : public FDepthPassBaseShader
	{
	public:
		DEFINE_SHADER_INSTANCE(FDepthPassPS);

		FDepthPassPS() : FDepthPassBaseShader(u8"Engine/Shaders/DepthPass.xsf"V, EShaderStage::Pixel, u8"DepthPixelShader"V) {}
	};

	FDepthRenderer::FDepthRenderer(FRenderGraph & RenderGraph)
		: RenderGraph(RenderGraph)
	{
		SceneDepth = RenderGraph.CreateResource(u8"SceneDepth"V, { EFormat::D32F, RenderGraph.Scene.SceneView.ViewRect.Size, RenderGraph.Scene.RenderSettings.ReversedZ ? 0.0f : 1.0f });
	}

	void FDepthRenderer::Render(IRHIResource * ViewParameter)
	{
		const FDepthStencilState DefaultDepthStencilState = RenderGraph.Scene.RenderSettings.ReversedZ ? FDepthStencilState::DefaultReversedZ : FDepthStencilState::Default;

		MeshCommandCollector.CollectDrawCommands(RenderGraph, EMeshPass::Depth, RenderGraph.Scene, nullptr, ViewParameter, nullptr, nullptr, FRasterizeState::Default, DefaultDepthStencilState,
			FDepthPassVS::Instance(), FDepthPassVS::FShaderVariance(), FDepthPassPS::Instance(), FDepthPassPS::FShaderVariance());

		RenderGraph.AddPass(u8"PreZ"V,
			[&](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(SceneDepth, EResourceAccess::DepthWrite, EResourceLoad::Clear);
			},
			[&](FRGContext & RGContext)
			{
				RGContext.BeginPass({}, { SceneDepth, EResourceLoad::Clear }, RenderGraph.Scene.SceneView.Viewport);
				RGContext.DispatchCommands(MeshCommandCollector.DrawCommands);
				RGContext.EndPass();
			});
	}
}
