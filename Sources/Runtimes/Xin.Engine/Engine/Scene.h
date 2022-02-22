#pragma once

#include "Primitive.h"
#include "SceneView.h"

namespace Xin::Engine
{
	class FSceneView;

	enum class EVisualBuffer
	{
		SceneOutput = 0,

		SceneColor,
		BaseNormal,
		SceneDepth,

		SSAOOcclusion,
		LinearDepth,
		ShadowDepth,
		FXAA,

		Custom0,
		Custom1,
		Custom2,
		Custom3,
		Custom4,
		Custom5,
		Custom6,
		Custom7,
		Custom8,
		Custom9,
	};

	class ENGINE_API FSceneRenderer : public IRenderingObject
	{
	public:
		FSceneRenderer(FCommandExecutor & CommandExecutor);

		void Update();
		void AddPrimitive(IPrimitiveRef RenderPrimitive);

	public:
		FCommandExecutor & CommandExecutor;

		TList<IPrimitiveRef> Primitives;

		TList<IPrimitiveRef> PenddingAddedPrimitives;
		TList<IPrimitiveRef> PenddingRemovedPrimitives;
	};

	struct FSceneRenderSettings
	{
		EVisualBuffer VisualBuffer = EVisualBuffer::SceneOutput;

		EPostProcessMode PostProcessMode = EPostProcessMode::CS;
		bool WireFrameRender = false;

		EAOMode AOMode = EAOMode::HBAO;

		EHBAOMode HBAOMode = EHBAOMode::CS;
		EHBAOLevel HBAOLevel = EHBAOLevel::Level1;
		EHBAOQuality HBAOQuality = EHBAOQuality::Quality0;

		ESSAOMode SSAOMode = ESSAOMode::CS;
		ESSAOLevel SSAOLevel = ESSAOLevel::Level1;
		ESSAOQuality SSAOQuality = ESSAOQuality::Quality0;

		bool BloomCS = false;
		EBloomMode BloomMode = EBloomMode::CS;
		EBloomLevel BloomLevel = EBloomLevel::Level1;

		EFormat HDRColorFormat = EFormat::R11G11B10F;
		EGussianBlurMode GussianBlurMode = EGussianBlurMode::PixelShader;
		bool ToneMappingCS = true;

		EFXAAMode FXAAMode = EFXAAMode::CS;
		EFXAALevel FXAALevel = EFXAALevel::Level7;

		uint32 CascadeLevel = 3;
		TArray<float32, CSMCascadeMax + 1> CascadePercentages = { 0, 5, 15, 100 };

		bool ReversedZ = true;
		bool PreZ = true;

		uint32 BackBufferCount = 3;

		float BloomBlendFactor = 0.65f;
		float BloomThreshold = 1.0f;

		uint32 PCFBlurRadius = 3;

		FAxisSystem AxisSystem {};

		bool TransientResourceEnabled = true;
	};

	class ENGINE_API FScene : public IRenderingObject
	{
	public:
		FScene() = default;

		virtual void Update(FCommandExecutor & Executor) {}

	public:
		TList<IPrimitiveRef> Primitives;
		FSceneView SceneView;
		FDirectionalLight MainLight;

		FSceneRenderSettings RenderSettings;
	};
}
