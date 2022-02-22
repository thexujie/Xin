#include "PCH.h"
#include "DefferedSceneRenderer.h"

#include "Engine.h"
#include "BasePassRendering.h"
#include "DepthRendering.h"
#include "SSAO.h"
#include "PostProcessRenderer.h"
#include "ShadowDepthRenderer.h"
#include "PostProcesses/FXAA.h"

namespace Xin::Engine
{
	FDefferedSceneRenderer::FDefferedSceneRenderer(FEngine & Engine)
		: Engine(Engine)
	{
	}

	void FDefferedSceneRenderer::Render(FRenderGraph & RenderGraph, IRHIResource * BackBuffer, IRHIResource * UIBackBuffer)
	{
		if (!SceneExposure)
		{
			//ExpVar Exposure("Graphics/HDR/Exposure", 2.0f, -8.0f, 8.0f, 0.25f);
			float InitialExposure = 2.0f;

			float LuminanceMin = 0.0001f;
			float LuminanceMax = 16.0f;
			float LuminanceMinLog = log2(LuminanceMin);
			float LuminanceMaxLog = log2(LuminanceMax);

			float InitExposure[8] =
			{
				InitialExposure, 1.0f / InitialExposure, InitialExposure, 0.0f,
				LuminanceMinLog, LuminanceMaxLog, LuminanceMaxLog - LuminanceMinLog, 1.0f / (LuminanceMaxLog - LuminanceMinLog)
			};
			SceneExposure = RenderGraph.Executor.CreateBuffer({ { 8, sizeof(float1) }, EFormat::Float1, EResourceUsage::UniformTexelBuffer | EResourceUsage::StorageTexelBuffer | EResourceUsage::CopyWrite }, ViewBytes(InitExposure));
		}

		static uint32 ShadowDepthSize = 1024;
		static int ShadowDepthBias = -200;
		static float ShadowDepthBiasClamp = 0.0f;
		static float ShadowSlopDepthBias = -1.5f;

		static float ZoomRatio = 1.0f;
		static float2 ZoomPosition = { 0.5f, 0.5f };

		static TkeybordInteractiveDebug DebugValue([&](FKey Key)
			{
				switch (Key.KeyCode)
				{
				case EKeyCode::V:
					break;
				//case EKeyCode::Up:
				//	if (Key.ControlKey * EControlKey::Shift)
				//		ShadowDepthBiasClamp = Clamp(ShadowDepthBiasClamp + 100.0f, -2000.0f, 0.0f);
				//	else if (Key.ControlKey * EControlKey::Alt)
				//		ShadowDepthBias = Clamp(ShadowDepthBias + 100, -500, 0);
				//	else
				//		ShadowSlopDepthBias = Clamp(ShadowSlopDepthBias + 0.5f, -4.0f, 0.0f);
				//	break;
				//case EKeyCode::Down:
				//	if (Key.ControlKey * EControlKey::Shift)
				//		ShadowDepthBiasClamp = Clamp(ShadowDepthBiasClamp - 100.0f, -2000.0f, 0.0f);
				//	else if (Key.ControlKey * EControlKey::Alt)
				//		ShadowDepthBias = Clamp(ShadowDepthBias - 100, -500, 0);
				//	else
				//		ShadowSlopDepthBias = Clamp(ShadowSlopDepthBias - 0.5f, -4.0f, 0.0f);
				//	break;
				//case EKeyCode::Left:
				//	ShadowDepthSize = Clamp(ShadowDepthSize / 2, 256u, 2048u);
				//	break;
				//case EKeyCode::Right:
				//	ShadowDepthSize = Clamp(ShadowDepthSize * 2, 256u, 2048u);
				//	break;

				case EKeyCode::Z:
				case EKeyCode::Left:
				case EKeyCode::Right:
				case EKeyCode::Up:
				case EKeyCode::Down:
				{
					switch(Key.KeyCode)
					{
					case EKeyCode::Z:
						if (Key.ControlKey & EControlKey::Shift)
							ZoomRatio = Clamp(ZoomRatio / 1.1f, 0.01f, 100.0f);
						else if (Key.ControlKey & EControlKey::Ctrl)
							ZoomRatio = 1.0f;
						else
							ZoomRatio = Clamp(ZoomRatio * 1.1f, 0.01f, 100.0f);
						break;
					case EKeyCode::Left:
						ZoomPosition.X = ZoomPosition.X - 0.1f / ZoomRatio;
						break;
					case EKeyCode::Right:
						ZoomPosition.X = ZoomPosition.X + 0.1f / ZoomRatio;
						break;
					case EKeyCode::Up:
						ZoomPosition.Y = ZoomPosition.Y - 0.1f / ZoomRatio;
						break;
					case EKeyCode::Down:
						ZoomPosition.Y = ZoomPosition.Y + 0.1f / ZoomRatio;
						break;
					default:
						break;
					}

					if (ZoomRatio <= 1.0f)
						ZoomPosition = {0.5f, 0.5f};
					else
					{
						float MinTexcoord = -(ZoomRatio - 1.0f);
						float MaxTexcoord = 0.0f;
						ZoomPosition.X = Clamp(ZoomPosition.X, MinTexcoord + ZoomRatio * 0.5f, MaxTexcoord + ZoomRatio * 0.5f);
						ZoomPosition.Y = Clamp(ZoomPosition.Y, MinTexcoord + ZoomRatio * 0.5f, MaxTexcoord + ZoomRatio * 0.5f);
					}
					break;
				}
				default:
					break;
				}
			});

		RectI ViewRect = RenderGraph.Scene.SceneView.ViewRect;

		FViewParameter ViewParameterData;
		ViewParameterData.ViewMatrix = RenderGraph.Scene.SceneView.ViewMatrix;
		ViewParameterData.ProjectMatrix = RenderGraph.Scene.RenderSettings.ReversedZ ? RenderGraph.Scene.SceneView.ProjectMatrix.ReversedZ() : RenderGraph.Scene.SceneView.ProjectMatrix;
		ViewParameterData.ViewProjectMatrix = ViewParameterData.ViewMatrix * ViewParameterData.ProjectMatrix;
		IRHIResource * ViewParameter = RenderGraph.CreateShaderParameterT(ViewParameterData);

		SizeU SceneSize = BackBuffer->ResourceDesc.Size;

		FShadowDepthRenderer ShadowDepthRenderer { RenderGraph };
		ShadowDepthRenderer.Render(RenderGraph.Scene.MainLight);

		FGlobalParameter GlobalParameterData;
		{
			GlobalParameterData.PCFBlurLoopBegin = -int32(RenderGraph.Scene.RenderSettings.PCFBlurRadius);
			GlobalParameterData.PCFBlurLoopEnd = RenderGraph.Scene.RenderSettings.PCFBlurRadius + 1;
			if (RenderGraph.Scene.RenderSettings.CascadeLevel > 0)
			{
				GlobalParameterData.ShadowMatrix = ShadowDepthRenderer.LightViewMatrix;
				for (uint32 CascadeIndex = 0; CascadeIndex < RenderGraph.Scene.RenderSettings.CascadeLevel; ++CascadeIndex)
				{
					FMatrix ProjectMatrix = RenderGraph.Scene.RenderSettings.ReversedZ ? ShadowDepthRenderer.ProjectMatrix[CascadeIndex].ReversedZ() : ShadowDepthRenderer.ProjectMatrix[CascadeIndex];
					FMatrix ShadowMatrix = ProjectMatrix * FMatrix::Transform({ 0.5f, 0.5f, 0.0f }, FQuat::Identity, { 0.5f, -0.5f, 1.0f });
					GlobalParameterData.CSMScale[CascadeIndex] = { ShadowMatrix._11, ShadowMatrix._22, ShadowMatrix._33 };
					GlobalParameterData.CSMOffset[CascadeIndex] = ShadowMatrix.Translation();
				}
				GlobalParameterData.CSMBorderPaddingMin = float32(RenderGraph.Scene.RenderSettings.PCFBlurRadius) / ShadowDepthRenderer.ShadowDepthTextureSize;
				GlobalParameterData.CSMBorderPaddingMax = 1.0f - GlobalParameterData.CSMBorderPaddingMin;
			}
			else
			{
				GlobalParameterData.ShadowMatrix = ShadowDepthRenderer.LightViewMatrix * ShadowDepthRenderer.ProjectMatrix[0];
				if (RenderGraph.Scene.RenderSettings.ReversedZ)
					GlobalParameterData.ShadowMatrix = GlobalParameterData.ShadowMatrix.ReversedZ();
				GlobalParameterData.ShadowMatrix *= FMatrix::Transform({ 0.5f, 0.5f, 0.0f }, FQuat::Identity, { 0.5f, -0.5f, 1.0f });
			}

			GlobalParameterData.ViewPosition = RenderGraph.Scene.SceneView.Position;
			GlobalParameterData.LightDirection = -FVector3::AxisZ * RenderGraph.Scene.MainLight.Rotation;
			GlobalParameterData.SunColor = { 8.0f, 8.0f, 8.0f, 1.0f };
			GlobalParameterData.AmbientColor = { 0.2f, 0.2f, 0.2f, 1.0f };
			GlobalParameterData.ShadowTexelSize = 1.0f / float32(ShadowDepthSize);
			GlobalParameterData.ShadowDepthBias = ShadowDepthBias * 0.01f;
		}
		IRHIResource * GlobalParameter = RenderGraph.CreateShaderParameterT(GlobalParameterData);

		FRGTexture * UIOverlap = UIBackBuffer ? RenderGraph.RegisterTexture(u8"UIOverlap"V, UIBackBuffer, EResourceAccess::None, EResourceAccess::None) : nullptr;
		FRGBuffer * Exposure = RenderGraph.RegisterBuffer(u8"Global.SceneExposure"V, SceneExposure);
		FRGTexture * ViewOutput = RenderGraph.RegisterTexture(u8"ViewOutput"V, BackBuffer, EResourceAccess::Present, EResourceAccess::Present);

		FDepthRenderer DepthRenderer { RenderGraph };
		FBasePassRenderer BasePassRenderer { RenderGraph };
		FSSAO SSAORenderer { RenderGraph };
		FPostProcessRenderer PostProcessRenderer { RenderGraph };

		DepthRenderer.Render(ViewParameter);

		SSAORenderer.Render(DepthRenderer.SceneDepth);

		BasePassRenderer.Render(GlobalParameter, ViewParameter, DepthRenderer.SceneDepth, SSAORenderer.SSAOOcclusion, ShadowDepthRenderer.ShadowDepth);


		FRGTexture * FinalOutput = BasePassRenderer.BaseColor;
		FRGResourceView * FinalOutputSRV = nullptr; // Default use FinalOutput->RHITexture->SRView
		IShaderInstance * FinalShader = Engine.ShaderBundle.GetShaderT<FHDR2SDRPS>();
		IRHIResource * ShaderParameter = nullptr;
		EBlitTextureFlag BlitTextureFlag = EBlitTextureFlag::Fill;

		if (RenderGraph.Scene.RenderSettings.VisualBuffer != EVisualBuffer::SceneColor)
		{
			if (RenderGraph.Scene.RenderSettings.PostProcessMode != EPostProcessMode::Disabled)
				PostProcessRenderer.Render(BasePassRenderer.BaseColor);

			if (RenderGraph.Scene.RenderSettings.FXAAMode != EFXAAMode::Disabled)
			{
				FRGTexture * FXAAOutput = RenderGraph.CreateResource(u8"SceneOutput"V, { EFormat::R16G16B16A16F, SceneSize, Colors::None });
				FFXAARenderer FXAARenderer(RenderGraph, (RenderGraph.Scene.RenderSettings.VisualBuffer == EVisualBuffer::FXAA) ? EFXAAFlag::Debug : EFXAAFlag::None);
				FXAARenderer.Render(BasePassRenderer.BaseColor, FXAAOutput);
				FinalOutput = FXAAOutput;
			}
		}

		switch (RenderGraph.Scene.RenderSettings.VisualBuffer)
		{
		case EVisualBuffer::SceneOutput:
			break;
		case EVisualBuffer::SceneColor:
			FinalOutput = BasePassRenderer.BaseColor;
			break;
		case EVisualBuffer::BaseNormal:
			FinalOutput = BasePassRenderer.BaseNormal;
			FinalShader = Engine.ShaderBundle.GetShaderT<FRGBAVisualizePS>();
			break;
		case EVisualBuffer::SceneDepth:
			FinalOutput = DepthRenderer.SceneDepth;
			FinalShader = Engine.ShaderBundle.GetShaderT<FZndcVisualizePS>();
			//ShaderParameter = RenderGraph.CreateShaderParameterT(EResourceUsage::Dynamic, float2 { 0.1f, 3000.0f });
			ShaderParameter = RenderGraph.CreateShaderParameterT(float2 { /*3000.0f*/400.0f, 0.1f });
			break;
		case EVisualBuffer::SSAOOcclusion:
			FinalOutput = SSAORenderer.SSAOOcclusion;
			FinalShader = Engine.ShaderBundle.GetShaderT<FRRR1VisualizePS>();
			break;
		case EVisualBuffer::LinearDepth:
			FinalOutput = SSAORenderer.SSAODepth1x;
			FinalShader = Engine.ShaderBundle.GetShaderT<FRRR1VisualizePS>();
			break;
		case EVisualBuffer::ShadowDepth:
			FinalOutput = ShadowDepthRenderer.ShadowDepth;
			FinalShader = Engine.ShaderBundle.GetShaderT<FRRR1VisualizePS>();
			BlitTextureFlag = EBlitTextureFlag::Center;
			break;
		case EVisualBuffer::FXAA:
			FinalOutput = SSAORenderer.SSAODepthTiled8x;
			FinalOutputSRV = RenderGraph.CreateResourceView(SSAORenderer.SSAODepthTiled8x, 15);
			FinalShader = Engine.ShaderBundle.GetShaderT<FRRR1VisualizePS>();
			break;
		default:
			break;
		}

		if (ZoomRatio != 1.0f)
		{
			FRGTexture * SceneZoomed = RenderGraph.CreateResource(FinalOutput->Semantic + u8"_Zoomed"V, { EFormat::R16G16B16A16F, SceneSize, Colors::AliceBlue });

			float4 ZoomParameter = {};
			float ZoomRatioRcp = 1.0f / ZoomRatio;
			ZoomParameter.XY = { ZoomPosition.X - ZoomRatioRcp * 0.5f, ZoomPosition.Y - ZoomRatioRcp * 0.5f };
			ZoomParameter.ZW = { ZoomRatioRcp, ZoomRatioRcp };
			RenderGraph.BlitTextureT(SceneZoomed, FinalOutput, Engine.ShaderBundle.GetShaderT<FZoomPS>(), ZoomParameter, EBlitTextureFlag::PassName);
			FinalOutput = SceneZoomed;
			FinalOutputSRV = RenderGraph.CreateResourceView(SceneZoomed);
		}

		RenderGraph.BlitTexture(ViewOutput, FinalOutput, FinalOutputSRV, FinalShader, ShaderParameter, BlitTextureFlag | EBlitTextureFlag::PassName);

#if 0
		int32 PreviewSize = 256;
		if (RenderGraph.Scene.RenderSettings.CascadeLevel == 0)
		{
			FRGBlitParameters ShadowDepthBlitParameters {};
			ShadowDepthBlitParameters.Input = ShadowDepthRenderer.ShadowDepth;
			ShadowDepthBlitParameters.Output = ViewOutput;
			ShadowDepthBlitParameters.Shader = Engine.ShaderBundle.GetShaderT<FRRR1VisualizePS>();
			ShadowDepthBlitParameters.ViewRect = { 0, ViewRect.Height - PreviewSize, PreviewSize, PreviewSize };
			RenderGraph.BlitTexture(ShadowDepthBlitParameters);
		}
		else
		{
			for (uint32 CascadeIndex = 0; CascadeIndex < RenderGraph.Scene.RenderSettings.CascadeLevel; ++CascadeIndex)
			{
				FRGBlitParameters BlitParameters {};
				BlitParameters.InputSRV = RenderGraph.CreateResourceView(ShadowDepthRenderer.ShadowDepth, CascadeIndex);
				BlitParameters.Output = ViewOutput;
				BlitParameters.Shader = Engine.ShaderBundle.GetShaderT<FRRR1VisualizePS>();
				BlitParameters.ViewRect = { int32(CascadeIndex) * PreviewSize, ViewRect.Height - PreviewSize, PreviewSize, PreviewSize };
				RenderGraph.BlitTexture(BlitParameters);
			}
		}
#endif

		// UI Overlap
		if (UIOverlap && false)
		{
			FRGBlitParameters BlitParameters {};
			BlitParameters.Input = UIOverlap;
			BlitParameters.Output = ViewOutput;
			BlitParameters.Shader = Engine.ShaderBundle.GetShaderT<FRGBAVisualizePS>();
			BlitParameters.BlendMode = EBlendMode::Premultiplied;
			RenderGraph.BlitTexture(BlitParameters);
		}
		RenderGraph.Execute();
	}
}
