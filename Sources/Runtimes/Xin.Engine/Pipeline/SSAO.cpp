#include "PCH.h"
#include "SSAO.h"

#include "RenderGraph.h"

namespace Xin::Engine
{
	DEFINE_SIMPLE_DEFAULT_SHADER(FSSAOPreparePS, u8"Engine/Shaders/SSAO_Prepare.xsf"V, EShaderStage::Pixel);

	FSSAOPreparePS SSAOPreparePS;
	DEFINE_SIMPLE_DEFAULT_SHADER(FSSAODownsamplePS, u8"Engine/Shaders/SSAO_Downsample.xsf"V, EShaderStage::Pixel);

	DEFINE_SIMPLE_DEFAULT_SHADER(FSSAOTile4x4PS, u8"Engine/Shaders/SSAO_Tile4x4.xsf"V, EShaderStage::Pixel);

	class FSSAOComputePS : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FSSAOComputePS)

		using ComputeTiled = TShaderPermutationBool<u8"XIN_SSAO_COMPUTE_TILED">;

		using FShaderVariance = TShaderVariance<ComputeTiled>;

		struct FShaderParameters
		{
			float ThicknessRcps[SSAOSampleWeightCount];
			float SampleWeights[SSAOSampleWeightCount];
			float2 InputTexelSize;
			float RejectFadeoff;
			float RcpAccentuation;
		};

		FSSAOComputePS() : IShaderSource(EShaderStage::Pixel, u8"Engine/Shaders/SSAO_Compute.xsf"V) {}
	};

	class FSSAOGuideBlurPS : public IShaderSource
	{
	public:
		using CombineWithLower = TShaderPermutationBool<u8"XIN_SSAO_UPSAMPLE_COMBINE_WITH_LOWER">;

		using FShaderVariance = TShaderVariance<CombineWithLower>;

		FSSAOGuideBlurPS(FStringV Entry) : IShaderSource(EShaderStage::Pixel, u8"Engine/Shaders/SSAO_GuideBlur.xsf"V, Entry) {}
	};

	class FSSAOGuideBlurHPS : public FSSAOGuideBlurPS
	{
	public:
		DEFINE_SHADER_INSTANCE(FSSAOGuideBlurHPS)

		FSSAOGuideBlurHPS() : FSSAOGuideBlurPS(u8"SSAO_GuideBlur_H"V) {}
	};

	class FSSAOGuideBlurVPS : public FSSAOGuideBlurPS
	{
	public:
		DEFINE_SHADER_INSTANCE(FSSAOGuideBlurVPS)

		FSSAOGuideBlurVPS() : FSSAOGuideBlurPS(u8"SSAO_GuideBlur_V"V) {}
	};


	class FSSAOUpsamplePS : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FSSAOUpsamplePS)

		using CombineWithLower = TShaderPermutationBool<u8"XIN_SSAO_UPSAMPLE_COMBINE_WITH_LOWER">;
		using BlendWithHigher = TShaderPermutationBool<u8"XIN_SSAO_UPSAMPLE_BLEND_WITH_HIGHER">;
		using FShaderVariance = TShaderVariance<CombineWithLower, BlendWithHigher>;


		FSSAOUpsamplePS() : IShaderSource(EShaderStage::Pixel, u8"Engine/Shaders/SSAO_Upsample.xsf"V) {}
	};

	class FSSAOComputeCS : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FSSAOComputeCS)

		using ComputeTiled = TShaderPermutationBool<u8"XIN_SSAO_COMPUTE_TILED">;
		using FShaderVariance = TShaderVariance<ComputeTiled>;

		struct FShaderParameters
		{
			float ThicknessRcps[SSAOSampleWeightCount];
			float SampleWeights[SSAOSampleWeightCount];
			float2 InvertSliceDimension;
			float RejectFadeoff;
			float RcpAccentuation;
		};

		FSSAOComputeCS() : IShaderSource(EShaderStage::Compute, u8"Engine/Shaders/SSAOCS_Compute.xsf"V) {}
	};

	class FSSAOUpsampleCS : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FSSAOUpsampleCS)

		using CombineWithLower = TShaderPermutationBool<u8"XIN_SSAO_UPSAMPLE_COMBINE_WITH_LOWER">;
		using BlendWithHigher = TShaderPermutationBool<u8"XIN_SSAO_UPSAMPLE_BLEND_WITH_HIGHER">;

		using FShaderVariance = TShaderVariance<CombineWithLower, BlendWithHigher>;

		FSSAOUpsampleCS() : IShaderSource(EShaderStage::Compute, u8"Engine/Shaders/SSAOCS_Upsample.xsf"V) {}
	};

	class FSSAODownsampleCS : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FSSAODownsampleCS)

		using FromSceneDepth = TShaderPermutationBool<u8"XIN_SSAO_FROM_SCENE_DEPTH">;
		using FShaderVariance = TShaderVariance<FromSceneDepth>;

		FSSAODownsampleCS() : IShaderSource(EShaderStage::Compute, u8"Engine/Shaders/SSAOCS_Downsample.xsf"V) {}
	};

	// 0.2 unit
	// 0x1 0x2 0x3 0x4
	// 1x1 1x2 1x3 1x4
	// 2x2 2x3 2x4 3x3
	static const TArray<float, SSAOSampleWeightCount> PreComputedSampleWeights =
	{
		sqrtf(1.0f - 0.2f * 0.2f), sqrtf(1.0f - 0.4f * 0.4f), sqrtf(1.0f - 0.6f * 0.6f), sqrtf(1.0f - 0.8f * 0.8f),
		sqrtf(1.0f - 0.2f * 0.2f - 0.2f * 0.2f), sqrtf(1.0f - 0.2f * 0.2f - 0.4f * 0.4f), sqrtf(1.0f - 0.2f * 0.2f - 0.6f * 0.6f), sqrtf(1.0f - 0.2f * 0.2f - 0.8f * 0.8f),
		sqrtf(1.0f - 0.4f * 0.4f - 0.4f * 0.4f), sqrtf(1.0f - 0.4f * 0.4f - 0.6f * 0.6f), sqrtf(1.0f - 0.4f * 0.4f - 0.8f * 0.8f), sqrtf(1.0f - 0.6f * 0.6f - 0.6f * 0.6f)
	};

	static const TArray<float, SSAOSampleWeightCount> SampleCounts =
	{
		4, 4, 4, 4,
		4, 8, 8, 8,
		4, 8, 8, 4
	};

	static void CalculateSSAOSampleWeights(Size4U DepthSize4U, float TanHalfFovH,
		float (& ThicknessRcps)[SSAOSampleWeightCount],
		float (& SampleWeights)[SSAOSampleWeightCount])
	{
		// The shaders are set up to sample a circular region within a 5-pixel radius.
		constexpr float ScreenspaceDiameter = 10.0f;

		// SphereDiameter = CenterDepth * ThicknessMultiplier.  This will compute the thickness of a sphere centered
		// at a specific depth.  The ellipsoid scale can stretch a sphere into an ellipsoid, which changes the
		// characteristics of the AO.
		// TanHalfFovH:  Radius of sphere in depth units if its center lies at Z = 1
		// ScreenspaceDiameter:  Diameter of sample sphere in pixel units
		// ScreenspaceDiameter / BufferWidth:  Ratio of the screen width that the sphere actually covers
		// Note about the "2.0f * ":  Diameter = 2 * Radius
		float ThicknessMultiplier = 2.0f * TanHalfFovH * ScreenspaceDiameter / DepthSize4U.Y;

		if (DepthSize4U.Z == 1)
			ThicknessMultiplier *= 2.0f;

		// This will transform a depth value from [0, thickness] to [0, 1].
		float InverseRangeFactor = 1.0f / ThicknessMultiplier;

		//SampleWeights[0] = float4::One;
		//SampleWeights[1] = float4::One;
		//SampleWeights[2] = float4::One;

		for (uintx Index = 0; Index < PreComputedSampleWeights.Size; ++Index)
		{
			ThicknessRcps[Index] = PreComputedSampleWeights[Index] * InverseRangeFactor;
			SampleWeights[Index] = PreComputedSampleWeights[Index] * SampleCounts[Index];
		}

#ifndef SAMPLE_EXHAUSTIVELY
		SampleWeights[0] = 0.0f;
		SampleWeights[2] = 0.0f;
		SampleWeights[5] = 0.0f;
		SampleWeights[7] = 0.0f;
		SampleWeights[9] = 0.0f;
#endif

		float TotalWeight = 0.0f;
		for (uintx Index = 0; Index < PreComputedSampleWeights.Size; ++Index)
			TotalWeight += SampleWeights[Index];

		for (uintx Index = 0; Index < PreComputedSampleWeights.Size; ++Index)
			SampleWeights[Index] /= TotalWeight;
	}

	FSSAO::FSSAO(FRenderGraph & RenderGraph)
		: RenderGraph(RenderGraph)
	{
		EFormat DepthFormat = EFormat::R32F;
		EFormat AOFormat = EFormat::R8UF;

		SizeU ViewSize { RenderGraph.Scene.SceneView.ViewRect.Size };

		SSAODepth1x = RenderGraph.CreateResource(u8"SSAO.Depth1x"V, { DepthFormat, ViewSize });
		SSAODepth2x = RenderGraph.CreateResource(u8"SSAO.Depth2x"V, { DepthFormat, ViewSize / 2 });
		SSAODepth4x = RenderGraph.CreateResource(u8"SSAO.Depth4x"V, { DepthFormat, ViewSize / 4 });
		SSAODepth8x = RenderGraph.CreateResource(u8"SSAO.Depth8x"V, { DepthFormat, ViewSize / 8 });
		SSAODepth16x = RenderGraph.CreateResource(u8"SSAO.Depth16x"V, { DepthFormat, ViewSize / 16 });

		SSAODepthTiled8x = RenderGraph.CreateResource(u8"SSAO.DepthTiled8x"V, { DepthFormat, { ViewSize / 8, 16 } });
		SSAODepthTiled16x = RenderGraph.CreateResource(u8"SSAO.DepthTiled16x"V, { DepthFormat, { ViewSize / 16, 16 } });
		SSAODepthTiled32x = RenderGraph.CreateResource(u8"SSAO.DepthTiled32x"V, { DepthFormat, { ViewSize / 32, 16 } });
		SSAODepthTiled64x = RenderGraph.CreateResource(u8"SSAO.DepthTiled64x"V, { DepthFormat, { ViewSize / 64, 16 } });

		SSAOOcclusionHQ2x = RenderGraph.CreateResource(u8"SSAO.OcclusionHQ2x"V, { AOFormat, ViewSize / 2, {} });
		SSAOOcclusionHQ4x = RenderGraph.CreateResource(u8"SSAO.OcclusionHQ4x"V, { AOFormat, ViewSize / 4, {} });
		SSAOOcclusionHQ8x = RenderGraph.CreateResource(u8"SSAO.OcclusionHQ8x"V, { AOFormat, ViewSize / 8, {} });
		//SSAOOcclusionHQ16x = RenderGraph.CreateTexture(u8"SSAO.OcclusionHQ16x"V, FRGTextureDesc { AOStorageFormat, SceneSize / 16 });
		SSAOOcclusionHQ16x = nullptr;

		SSAOOcclusionMerged2x = RenderGraph.CreateResource(u8"SSAO.OcclusionMerged2x"V, { AOFormat, ViewSize / 2, {} });
		SSAOOcclusionMerged4x = RenderGraph.CreateResource(u8"SSAO.OcclusionMerged4x"V, { AOFormat, ViewSize / 4, {} });
		SSAOOcclusionMerged8x = RenderGraph.CreateResource(u8"SSAO.OcclusionMerged8x"V, { AOFormat, ViewSize / 8, {} });
		//SSAOOcclusionMerged16x = RenderGraph.CreateTexture(u8"SSAO.OcclusionMerged16x"V, { AOStorageFormat, SceneSize / 16 }, {});
		SSAOOcclusionMerged16x = nullptr;

		SSAOOcclusionSmooth2x = RenderGraph.CreateResource(u8"SSAO.OcclusionSmooth2x"V, { AOFormat, ViewSize / 2, {} });
		SSAOOcclusionSmooth4x = RenderGraph.CreateResource(u8"SSAO.OcclusionSmooth4x"V, { AOFormat, ViewSize / 4, {} });
		SSAOOcclusionSmooth8x = RenderGraph.CreateResource(u8"SSAO.OcclusionSmooth8x"V, { AOFormat, ViewSize / 8, {} });

		SSAOOcclusion = RenderGraph.CreateResource(u8"SSAO.Occlusion"V, { AOFormat, ViewSize });
	}

	FSSAO::~FSSAO() {}

	void FSSAO::Render(FRGTexture * SceneDepth)
	{
		switch (RenderGraph.Scene.RenderSettings.SSAOMode)
		{
		case ESSAOMode::CS:
			RenderCS(SceneDepth);
			break;
		case ESSAOMode::PS:
			RenderPS(SceneDepth);
			break;
		default:
			break;
		}
	}

	void FSSAO::RenderPS(FRGTexture * SceneDepth)
	{
		FRenderGraph::FScopedEvent SSAORenderEvent(RenderGraph, u8"SSAO"V);

		DownsamplePS(SSAODepth2x, SSAODepthTiled8x, SSAODepth4x, SSAODepthTiled16x, SSAODepth1x, SceneDepth);
		switch (RenderGraph.Scene.RenderSettings.SSAOLevel)
		{
		default:
		case ESSAOLevel::Level0:
			ComputePS(SSAOOcclusionMerged2x, SSAODepthTiled8x);

			switch (RenderGraph.Scene.RenderSettings.SSAOQuality)
			{
			case ESSAOQuality::Quality1:
				ComputePS(SSAOOcclusionHQ2x, SSAODepth2x);
				UpsamplePS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, SSAOOcclusionHQ2x, SSAOOcclusionMerged2x, nullptr);
				break;
			default:
			case ESSAOQuality::Quality0:
				UpsamplePS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, nullptr, SSAOOcclusionMerged2x, nullptr);
				break;
			}
			break;
		case ESSAOLevel::Level1:
			ComputePS(SSAOOcclusionMerged2x, SSAODepthTiled8x);
			ComputePS(SSAOOcclusionMerged4x, SSAODepthTiled16x);
			switch (RenderGraph.Scene.RenderSettings.SSAOQuality)
			{
			case ESSAOQuality::Quality2:
				ComputePS(SSAOOcclusionHQ4x, SSAODepth4x);
				UpsamplePS(SSAOOcclusionSmooth2x, SSAODepth2x, SSAODepth4x, SSAOOcclusionHQ4x, SSAOOcclusionMerged4x, SSAOOcclusionMerged2x);
				ComputePS(SSAOOcclusionHQ2x, SSAODepth2x);
				UpsamplePS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, SSAOOcclusionHQ2x, SSAOOcclusionSmooth2x, nullptr);
				break;
			case ESSAOQuality::Quality1:
				ComputePS(SSAOOcclusionHQ4x, SSAODepth4x);
				UpsamplePS(SSAOOcclusionSmooth2x, SSAODepth2x, SSAODepth4x, SSAOOcclusionHQ4x, SSAOOcclusionMerged4x, SSAOOcclusionMerged2x);
				UpsamplePS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, nullptr, SSAOOcclusionSmooth2x, nullptr);
				break;
			default:
			case ESSAOQuality::Quality0:
				UpsamplePS(SSAOOcclusionSmooth2x, SSAODepth2x, SSAODepth4x, nullptr, SSAOOcclusionMerged4x, SSAOOcclusionMerged2x);
				UpsamplePS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, nullptr, SSAOOcclusionSmooth2x, nullptr);
				break;
			}
			break;
		case ESSAOLevel::Level2:
			DownsamplePS(SSAODepth8x, SSAODepthTiled32x, SSAODepth16x, SSAODepthTiled64x, SSAODepth4x, nullptr);

			ComputePS(SSAOOcclusionMerged2x, SSAODepthTiled8x);
			ComputePS(SSAOOcclusionMerged4x, SSAODepthTiled16x);
			ComputePS(SSAOOcclusionMerged8x, SSAODepthTiled32x);

			switch (RenderGraph.Scene.RenderSettings.SSAOQuality)
			{
			case ESSAOQuality::Quality3:
				ComputePS(SSAOOcclusionHQ8x, SSAODepth8x);
				UpsamplePS(SSAOOcclusionSmooth4x, SSAODepth4x, SSAODepth8x, SSAOOcclusionHQ8x, SSAOOcclusionMerged8x, SSAOOcclusionMerged4x);
				ComputePS(SSAOOcclusionHQ4x, SSAODepth4x);
				UpsamplePS(SSAOOcclusionSmooth2x, SSAODepth2x, SSAODepth4x, SSAOOcclusionHQ4x, SSAOOcclusionSmooth4x, SSAOOcclusionMerged2x);
				ComputePS(SSAOOcclusionHQ2x, SSAODepth2x);
				UpsamplePS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, SSAOOcclusionHQ2x, SSAOOcclusionSmooth2x, nullptr);
				break;
			case ESSAOQuality::Quality2:
				ComputePS(SSAOOcclusionHQ8x, SSAODepth8x);
				UpsamplePS(SSAOOcclusionSmooth4x, SSAODepth4x, SSAODepth8x, SSAOOcclusionHQ8x, SSAOOcclusionMerged8x, SSAOOcclusionMerged4x);
				ComputePS(SSAOOcclusionHQ4x, SSAODepth4x);
				UpsamplePS(SSAOOcclusionSmooth2x, SSAODepth2x, SSAODepth4x, SSAOOcclusionHQ4x, SSAOOcclusionSmooth4x, SSAOOcclusionMerged2x);
				UpsamplePS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, nullptr, SSAOOcclusionSmooth2x, nullptr);
				break;
			case ESSAOQuality::Quality1:
				ComputePS(SSAOOcclusionHQ8x, SSAODepth8x);
				UpsamplePS(SSAOOcclusionSmooth4x, SSAODepth4x, SSAODepth8x, SSAOOcclusionHQ8x, SSAOOcclusionMerged8x, SSAOOcclusionMerged4x);
				UpsamplePS(SSAOOcclusionSmooth2x, SSAODepth2x, SSAODepth4x, nullptr, SSAOOcclusionSmooth4x, SSAOOcclusionMerged2x);
				UpsamplePS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, nullptr, SSAOOcclusionSmooth2x, nullptr);
				break;
			default:
			case ESSAOQuality::Quality0:
				UpsamplePS(SSAOOcclusionSmooth4x, SSAODepth4x, SSAODepth8x, nullptr, SSAOOcclusionMerged8x, SSAOOcclusionMerged4x);
				UpsamplePS(SSAOOcclusionSmooth2x, SSAODepth2x, SSAODepth4x, nullptr, SSAOOcclusionSmooth4x, SSAOOcclusionMerged2x);
				UpsamplePS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, nullptr, SSAOOcclusionSmooth2x, nullptr);
				break;
			}
			break;
		}
	}

	void FSSAO::RenderCS(FRGTexture * SceneDepth)
	{
		FRenderGraph::FScopedEvent SSAORenderEvent(RenderGraph, u8"SSAOCS"V);

		DownsampleCS(SSAODepth2x, SSAODepthTiled8x, SSAODepth4x, SSAODepthTiled16x, SSAODepth1x, SceneDepth);
		switch (RenderGraph.Scene.RenderSettings.SSAOLevel)
		{
		default:
		case ESSAOLevel::Level0:
			ComputeCS(SSAOOcclusionMerged2x, SSAODepthTiled8x);

			switch (RenderGraph.Scene.RenderSettings.SSAOQuality)
			{
			case ESSAOQuality::Quality1:
				ComputeCS(SSAOOcclusionHQ2x, SSAODepth2x);
				UpsampleCS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, SSAOOcclusionHQ2x, SSAOOcclusionMerged2x, nullptr);
				break;
			default:
			case ESSAOQuality::Quality0:
				UpsampleCS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, nullptr, SSAOOcclusionMerged2x, nullptr);
				break;
			}
			break;
		case ESSAOLevel::Level1:
			ComputeCS(SSAOOcclusionMerged2x, SSAODepthTiled8x);
			ComputeCS(SSAOOcclusionMerged4x, SSAODepthTiled16x);
			switch (RenderGraph.Scene.RenderSettings.SSAOQuality)
			{
			case ESSAOQuality::Quality2:
				ComputeCS(SSAOOcclusionHQ4x, SSAODepth4x);
				UpsampleCS(SSAOOcclusionSmooth2x, SSAODepth2x, SSAODepth4x, SSAOOcclusionHQ4x, SSAOOcclusionMerged4x, SSAOOcclusionMerged2x);
				ComputeCS(SSAOOcclusionHQ2x, SSAODepth2x);
				UpsampleCS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, SSAOOcclusionHQ2x, SSAOOcclusionSmooth2x, nullptr);
				break;
			case ESSAOQuality::Quality1:
				ComputeCS(SSAOOcclusionHQ4x, SSAODepth4x);
				UpsampleCS(SSAOOcclusionSmooth2x, SSAODepth2x, SSAODepth4x, SSAOOcclusionHQ4x, SSAOOcclusionMerged4x, SSAOOcclusionMerged2x);
				UpsampleCS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, nullptr, SSAOOcclusionSmooth2x, nullptr);
				break;
			default:
			case ESSAOQuality::Quality0:
				UpsampleCS(SSAOOcclusionSmooth2x, SSAODepth2x, SSAODepth4x, nullptr, SSAOOcclusionMerged4x, SSAOOcclusionMerged2x);
				UpsampleCS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, nullptr, SSAOOcclusionSmooth2x, nullptr);
				break;
			}
			break;
		case ESSAOLevel::Level2:
			DownsampleCS(SSAODepth8x, SSAODepthTiled32x, SSAODepth16x, SSAODepthTiled64x, SSAODepth4x, nullptr);

			ComputeCS(SSAOOcclusionMerged2x, SSAODepthTiled8x);
			ComputeCS(SSAOOcclusionMerged4x, SSAODepthTiled16x);
			ComputeCS(SSAOOcclusionMerged8x, SSAODepthTiled32x);

			switch (RenderGraph.Scene.RenderSettings.SSAOQuality)
			{
			case ESSAOQuality::Quality3:
				ComputeCS(SSAOOcclusionHQ8x, SSAODepth8x);
				UpsampleCS(SSAOOcclusionSmooth4x, SSAODepth4x, SSAODepth8x, SSAOOcclusionHQ8x, SSAOOcclusionMerged8x, SSAOOcclusionMerged4x);
				ComputeCS(SSAOOcclusionHQ4x, SSAODepth4x);
				UpsampleCS(SSAOOcclusionSmooth2x, SSAODepth2x, SSAODepth4x, SSAOOcclusionHQ4x, SSAOOcclusionSmooth4x, SSAOOcclusionMerged2x);
				ComputeCS(SSAOOcclusionHQ2x, SSAODepth2x);
				UpsampleCS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, SSAOOcclusionHQ2x, SSAOOcclusionSmooth2x, nullptr);
				break;
			case ESSAOQuality::Quality2:
				ComputeCS(SSAOOcclusionHQ8x, SSAODepth8x);
				UpsampleCS(SSAOOcclusionSmooth4x, SSAODepth4x, SSAODepth8x, SSAOOcclusionHQ8x, SSAOOcclusionMerged8x, SSAOOcclusionMerged4x);
				ComputeCS(SSAOOcclusionHQ4x, SSAODepth4x);
				UpsampleCS(SSAOOcclusionSmooth2x, SSAODepth2x, SSAODepth4x, SSAOOcclusionHQ4x, SSAOOcclusionSmooth4x, SSAOOcclusionMerged2x);
				UpsampleCS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, nullptr, SSAOOcclusionSmooth2x, nullptr);
				break;
			case ESSAOQuality::Quality1:
				ComputeCS(SSAOOcclusionHQ8x, SSAODepth8x);
				UpsampleCS(SSAOOcclusionSmooth4x, SSAODepth4x, SSAODepth8x, SSAOOcclusionHQ8x, SSAOOcclusionMerged8x, SSAOOcclusionMerged4x);
				UpsampleCS(SSAOOcclusionSmooth2x, SSAODepth2x, SSAODepth4x, nullptr, SSAOOcclusionSmooth4x, SSAOOcclusionMerged2x);
				UpsampleCS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, nullptr, SSAOOcclusionSmooth2x, nullptr);
				break;
			default:
			case ESSAOQuality::Quality0:
				UpsampleCS(SSAOOcclusionSmooth4x, SSAODepth4x, SSAODepth8x, nullptr, SSAOOcclusionMerged8x, SSAOOcclusionMerged4x);
				UpsampleCS(SSAOOcclusionSmooth2x, SSAODepth2x, SSAODepth4x, nullptr, SSAOOcclusionSmooth4x, SSAOOcclusionMerged2x);
				UpsampleCS(SSAOOcclusion, SSAODepth1x, SSAODepth2x, nullptr, SSAOOcclusionSmooth2x, nullptr);
				break;
			}
			break;
		}
	}

	void FSSAO::DownsamplePS(FRGTexture * Depth2x, FRGTexture * DepthTiled8x, FRGTexture * Depth4x, FRGTexture * DepthTiled16x, FRGTexture * Depth1x, FRGTexture * SceneDepth)
	{
		FRenderGraph::FScopedEvent SSAORenderEvent(RenderGraph, u8"DownsamplePS"V);
		float FarZ = 3000.0f;
		float NearZ = 0.1f;
		float ZMagic = (FarZ - NearZ) / NearZ;
		if (SceneDepth)
			RenderGraph.BlitTextureT(Depth1x, SceneDepth, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FSSAOPreparePS>(), ZMagic);
		RenderGraph.BlitTexture(Depth2x, Depth1x, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FSSAODownsamplePS>());
		RenderGraph.BlitTexture(Depth4x, Depth2x, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FSSAODownsamplePS>());
		DownsampleTilePS(DepthTiled8x, Depth2x);
		DownsampleTilePS(DepthTiled16x, Depth4x);
	}

	void FSSAO::DownsampleTilePS(FRGTexture * DepthTiled4x, FRGTexture * Depth1x)
	{
		struct FDownsampleTiledPSInputs
		{
			FRGTexture * Depth1x;
			FRGTexture * DepthTiled4x;
		};
		FDownsampleTiledPSInputs Inputs = { Depth1x, DepthTiled4x };
		RenderGraph.AddPass(NullString,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Depth1x, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.DepthTiled4x, EResourceAccess::RenderTarget, EResourceLoad::Discard);
			},
			[Inputs = Inputs](FRGContext & RGContext)
			{
				SizeU OutputSize = Inputs.DepthTiled4x->Desc.Size;
				FViewport Viewport { 0.0f, 0.0f, (float32)OutputSize.Width, (float32)OutputSize.Height, 0.0f, 1.0f };

				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Vertex] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FScreenQuadVS>();
					MeshRenderState.Shaders[EShaderStage::Pixel] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FSSAOTile4x4PS>();
					MeshRenderState.InputElements = {};
					MeshRenderState.RasterizeState = FRasterizeState::Default;
					MeshRenderState.BlendState = FBlendState::Default;
					MeshRenderState.DepthStencilState = FDepthStencilState::Disabled;
				}

				FDrawCommand MeshCommand;
				MeshCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				MeshCommand.VertexCount = 3;
				MeshCommand.Instanced.VertexOffset = 0;
				MeshCommand.Instanced.VertexCount = 3;

				MeshCommand.Parameters.Emplace(u8"Depth1x"N, Inputs.Depth1x->Resource);
				// 0-7
				{
					IRHIResource * RHIResource = Inputs.DepthTiled4x->Resource;
					RGContext.CommandList.DiscardResource(RHIResource);

					RGContext.BeginPass(
						{
							{ RHIResource->GetResourceView(0), EResourceLoad::None },
							{ RHIResource->GetResourceView(1), EResourceLoad::None },
							{ RHIResource->GetResourceView(2), EResourceLoad::None },
							{ RHIResource->GetResourceView(3), EResourceLoad::None },
							{ RHIResource->GetResourceView(4), EResourceLoad::None },
							{ RHIResource->GetResourceView(5), EResourceLoad::None },
							{ RHIResource->GetResourceView(6), EResourceLoad::None },
							{ RHIResource->GetResourceView(7), EResourceLoad::None },
						}, nullptr, Viewport);

					IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(float4 { 1.0f / Inputs.Depth1x->Desc.Size, 1.0f / Inputs.Depth1x->Desc.Size.Y, 0.0f });
					MeshCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);
					RGContext.DispatchCommand(MeshCommand);
					RGContext.EndPass();
				}
				// 8-15
				{
					IRHIResource * RHIResource = Inputs.DepthTiled4x->Resource;
					RGContext.BeginPass(
						{
							{ RHIResource->GetResourceView(8 + 0), EResourceLoad::None },
							{ RHIResource->GetResourceView(8 + 1), EResourceLoad::None },
							{ RHIResource->GetResourceView(8 + 2), EResourceLoad::None },
							{ RHIResource->GetResourceView(8 + 3), EResourceLoad::None },
							{ RHIResource->GetResourceView(8 + 4), EResourceLoad::None },
							{ RHIResource->GetResourceView(8 + 5), EResourceLoad::None },
							{ RHIResource->GetResourceView(8 + 6), EResourceLoad::None },
							{ RHIResource->GetResourceView(8 + 7), EResourceLoad::None },
						}, nullptr, Viewport);

					IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(float4 { 1.0f / Inputs.Depth1x->Desc.Size, 3.0f / Inputs.Depth1x->Desc.Size.Y, 2.0f });
					MeshCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);
					RGContext.DispatchCommand(MeshCommand);
					RGContext.EndPass();
				}
			});
	}

	void FSSAO::ComputePS(FRGTexture * Occlusion, FRGTexture * Depth1xOrTiled4x)
	{
		float32 RejectionFalloff = 2.5f;
		float32 Accentuation = 0.1f;
		float32 TanHalfFovH = 1.0f / RenderGraph.Scene.SceneView.ProjectMatrix._11;

		FSSAOComputePS::FShaderParameters & Parameters = RenderGraph.Allocator.AllocRef<FSSAOComputePS::FShaderParameters>();
		{
			Parameters.InputTexelSize = 1.0f / Depth1xOrTiled4x->Desc.Size;
			Parameters.RejectFadeoff = 1.0f / -RejectionFalloff;
			Parameters.RcpAccentuation = 1.0f / (1.0f + Accentuation);
			CalculateSSAOSampleWeights({ Depth1xOrTiled4x->Desc.Size, 1, 1 }, TanHalfFovH, Parameters.ThicknessRcps, Parameters.SampleWeights);
		}

		struct FBloomPrepareInputs
		{
			FRGTexture * Depth1xOrTiled4x;
			FRGTexture * Occlusion;
		};
		FBloomPrepareInputs Inputs = { Depth1xOrTiled4x, Occlusion };
		RenderGraph.AddPass(u8"ComputePS"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Depth1xOrTiled4x, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.Occlusion, EResourceAccess::RenderTarget, EResourceLoad::Discard);
			},
			[Inputs = Inputs, &Parameters](FRGContext & RGContext)
			{
				SizeU OutputSize = Inputs.Occlusion->Desc.Size;
				FViewport Viewport { 0.0f, 0.0f, (float32)OutputSize.Width, (float32)OutputSize.Height, 0.0f, 1.0f };
				RGContext.BeginPass({ Inputs.Occlusion }, nullptr, Viewport);

				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(Parameters);

				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Vertex] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FScreenQuadVS>();

					FSSAOComputePS::FShaderVariance ShaderVariance;
					ShaderVariance.SetValue<FSSAOComputePS::ComputeTiled>(Inputs.Depth1xOrTiled4x->Desc.Depth > 1);

					MeshRenderState.Shaders[EShaderStage::Pixel] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FSSAOComputePS>(ShaderVariance);
					MeshRenderState.InputElements = {};
					MeshRenderState.RasterizeState = FRasterizeState::Default;
					MeshRenderState.BlendState = FBlendState::Default;
					MeshRenderState.DepthStencilState = FDepthStencilState::Disabled;
				}

				FDrawCommand MeshCommand;
				MeshCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);

				MeshCommand.VertexCount = 3;
				MeshCommand.Instanced.VertexOffset = 0;
				MeshCommand.Instanced.VertexCount = 3;

				MeshCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);
				MeshCommand.Parameters.Emplace(u8"Depth"N, Inputs.Depth1xOrTiled4x->Resource);

				RGContext.DispatchCommand(MeshCommand);
				RGContext.EndPass();
			});
	}

	void FSSAO::UpsamplePS(FRGTexture * OcclusionX2, FRGTexture * DepthX2, FRGTexture * Depth, FRGTexture * OcclusionHQ, FRGTexture * OcclusionMerged, FRGTexture * OcclusionMergedX2)
	{
		FRenderGraph::FScopedEvent SSAORenderEvent(RenderGraph, u8"UpsamplePS"V);

		struct FAOUpsampleParameter
		{
			float2 InputTexelSizeX2;
			float2 InputTexelSize;

			float StepSize;
			float BlurTolerance;
			float UpsampleTolerance;
			float NoiseFilterStrength;
		};
		FAOUpsampleParameter AOUpsampleParameter;
		{
			//NumVar g_NoiseFilterTolerance("Graphics/SSAO/Noise Filter Threshold (log10)", -3.0f, -8.0f, 0.0f, 0.25f);
			//NumVar g_BlurTolerance("Graphics/SSAO/Blur Tolerance (log10)", -5.0f, -8.0f, -1.0f, 0.25f);
			//NumVar g_UpsampleTolerance("Graphics/SSAO/Upsample Tolerance (log10)", -7.0f, -12.0f, -1.0f, 0.5f);

			static float g_BlurTolerance = -5;
			static float g_UpsampleTolerance = -7.0;
			static float g_NoiseFilterTolerance = -3.0;

			float BlurTolerance = 1.0f - powf(10.0f, g_BlurTolerance) * 1920.0f / (float)Depth->Desc.Size.X;
			BlurTolerance *= BlurTolerance;

			AOUpsampleParameter.InputTexelSizeX2 = { 1.0f / DepthX2->Desc.Size.X, 1.0f / DepthX2->Desc.Size.Y };
			AOUpsampleParameter.InputTexelSize = { 1.0f / Depth->Desc.Size.X, 1.0f / Depth->Desc.Size.Y };
			AOUpsampleParameter.StepSize = 1920.0f / Depth->Desc.Size.X;
			AOUpsampleParameter.BlurTolerance = BlurTolerance;
			AOUpsampleParameter.UpsampleTolerance = powf(10.0f, g_UpsampleTolerance);
			AOUpsampleParameter.NoiseFilterStrength = 1.0f / (powf(10.0f, g_NoiseFilterTolerance) + AOUpsampleParameter.UpsampleTolerance);;
		}

		FRGTexture * BlurStagingA = RenderGraph.CreateResource(Format(u8"SSAO.BlurStagingA [{}]"V, OcclusionMerged->Semantic), OcclusionMerged->Desc);
		FRGTexture * BlurStagingB = RenderGraph.CreateResource(Format(u8"SSAO.BlurStagingB [{}]"V, OcclusionMerged->Semantic), OcclusionMerged->Desc);

		struct FUpsamplePSInputs
		{
			FRGTexture * OcclusionX2;
			FRGTexture * BlurStagingA;
			FRGTexture * BlurStagingB;

			FRGTexture * DepthX2;
			FRGTexture * Depth;

			FRGTexture * OcclusionHQ;
			FRGTexture * OcclusionMerged;
			FRGTexture * OcclusionMergedX2;
		};

		FUpsamplePSInputs Inputs = { OcclusionX2, BlurStagingA, BlurStagingB, DepthX2, Depth, OcclusionHQ, OcclusionMerged, OcclusionMergedX2 };
		RenderGraph.AddPass(NullString,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.DepthX2, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.Depth, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.OcclusionMerged, EResourceAccess::PixelSampledTexture);
				if (Inputs.OcclusionHQ)
					CompileContext.AccessResource(Inputs.OcclusionHQ, EResourceAccess::PixelSampledTexture);
				if (Inputs.OcclusionMergedX2)
					CompileContext.AccessResource(Inputs.OcclusionMergedX2, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.BlurStagingA, EResourceAccess::RenderTarget, EResourceLoad::Discard);
			},
			[Inputs = Inputs, AOUpsampleParameter = AOUpsampleParameter](FRGContext & RGContext)
			{
				SizeU OutputSize = Inputs.BlurStagingA->Desc.Size;
				FViewport Viewport { 0.0f, 0.0f, (float32)OutputSize.Width, (float32)OutputSize.Height, 0.0f, 1.0f };
				RGContext.BeginPass({ Inputs.BlurStagingA }, nullptr, Viewport);

				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(AOUpsampleParameter);

				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Vertex] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FScreenQuadVS>();
					{
						FSSAOGuideBlurHPS::FShaderVariance ShaderVariance;
						ShaderVariance.SetValue<FSSAOGuideBlurHPS::CombineWithLower>(Inputs.OcclusionHQ);
						MeshRenderState.Shaders[EShaderStage::Pixel] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FSSAOGuideBlurHPS>(ShaderVariance);
					}

					MeshRenderState.InputElements = {};
					MeshRenderState.RasterizeState = FRasterizeState::Default;
					MeshRenderState.BlendState = FBlendState::Default;
					MeshRenderState.DepthStencilState = FDepthStencilState::Disabled;
				}
				FDrawCommand MeshCommand;

				MeshCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				MeshCommand.VertexCount = 3;
				MeshCommand.Instanced.VertexOffset = 0;
				MeshCommand.Instanced.VertexCount = 3;

				MeshCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);
				MeshCommand.Parameters.Emplace(u8"DepthX2"N, Inputs.DepthX2->Resource);
				MeshCommand.Parameters.Emplace(u8"Depth"N, Inputs.Depth->Resource);
				MeshCommand.Parameters.Emplace(u8"OcclusionMerged"N, Inputs.OcclusionMerged->Resource);
				if (Inputs.OcclusionHQ)
					MeshCommand.Parameters.Emplace(u8"OcclusionHQ"N, Inputs.OcclusionHQ->Resource);
				if (Inputs.OcclusionMergedX2)
					MeshCommand.Parameters.Emplace(u8"OcclusionMergedX2"N, Inputs.OcclusionMergedX2->Resource);

				RGContext.DispatchCommand(MeshCommand);
				RGContext.EndPass();
			});

		RenderGraph.AddPass(NullString,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.DepthX2, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.Depth, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.BlurStagingA, EResourceAccess::PixelSampledTexture);
				if (Inputs.OcclusionX2)
					CompileContext.AccessResource(Inputs.OcclusionX2, EResourceAccess::PixelSampledTexture);
				if (Inputs.OcclusionHQ)
					CompileContext.AccessResource(Inputs.OcclusionHQ, EResourceAccess::PixelSampledTexture);
				if (Inputs.OcclusionMergedX2)
					CompileContext.AccessResource(Inputs.OcclusionMergedX2, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.BlurStagingB, EResourceAccess::RenderTarget, EResourceLoad::Discard);
			},
			[Inputs = Inputs, AOUpsampleParameter = AOUpsampleParameter](FRGContext & RGContext)
			{
				SizeU OutputSize = Inputs.BlurStagingB->Desc.Size;
				FViewport Viewport { 0.0f, 0.0f, (float32)OutputSize.Width, (float32)OutputSize.Height, 0.0f, 1.0f };
				RGContext.BeginPass({ Inputs.BlurStagingB }, nullptr, Viewport);

				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(AOUpsampleParameter);

				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Vertex] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FScreenQuadVS>();
					{
						FSSAOGuideBlurVPS::FShaderVariance ShaderVariance;
						ShaderVariance.SetValue<FSSAOGuideBlurVPS::CombineWithLower>(Inputs.OcclusionHQ);
						MeshRenderState.Shaders[EShaderStage::Pixel] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FSSAOGuideBlurVPS>(ShaderVariance);
					}
					MeshRenderState.InputElements = {};
					MeshRenderState.RasterizeState = FRasterizeState::Default;
					MeshRenderState.BlendState = FBlendState::Default;
					MeshRenderState.DepthStencilState = FDepthStencilState::Disabled;
				}

				FDrawCommand MeshCommand;
				MeshCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				MeshCommand.VertexCount = 3;
				MeshCommand.Instanced.VertexOffset = 0;
				MeshCommand.Instanced.VertexCount = 3;

				MeshCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);
				MeshCommand.Parameters.Emplace(u8"DepthX2"N, Inputs.DepthX2->Resource);
				MeshCommand.Parameters.Emplace(u8"Depth"N, Inputs.Depth->Resource);
				MeshCommand.Parameters.Emplace(u8"OcclusionMerged"N, Inputs.BlurStagingA->Resource);
				if (Inputs.OcclusionHQ)
					MeshCommand.Parameters.Emplace(u8"OcclusionHQ"N, Inputs.OcclusionHQ->Resource);
				if (Inputs.OcclusionMergedX2)
					MeshCommand.Parameters.Emplace(u8"OcclusionMergedX2"N, Inputs.OcclusionMergedX2->Resource);

				RGContext.DispatchCommand(MeshCommand);
				RGContext.EndPass();
			});

		RenderGraph.AddPass(NullString,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.DepthX2, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.Depth, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.BlurStagingB, EResourceAccess::PixelSampledTexture);
				if (Inputs.OcclusionHQ)
					CompileContext.AccessResource(Inputs.OcclusionHQ, EResourceAccess::PixelSampledTexture);
				if (Inputs.OcclusionMergedX2)
					CompileContext.AccessResource(Inputs.OcclusionMergedX2, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.OcclusionX2, EResourceAccess::RenderTarget, EResourceLoad::Discard);
			},
			[Inputs = Inputs, AOUpsampleParameter = AOUpsampleParameter](FRGContext & RGContext)
			{
				SizeU OutputSize = Inputs.OcclusionX2->Desc.Size;
				FViewport Viewport { 0.0f, 0.0f, (float32)OutputSize.Width, (float32)OutputSize.Height, 0.0f, 1.0f };
				RGContext.BeginPass({ Inputs.OcclusionX2 }, nullptr, Viewport);

				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(AOUpsampleParameter);

				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Vertex] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FScreenQuadVS>();
					{
						FSSAOUpsamplePS::FShaderVariance ShaderVariance;
						ShaderVariance.SetValue<FSSAOUpsamplePS::CombineWithLower>(Inputs.OcclusionHQ);
						ShaderVariance.SetValue<FSSAOUpsamplePS::BlendWithHigher>(Inputs.OcclusionMergedX2);
						MeshRenderState.Shaders[EShaderStage::Pixel] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FSSAOUpsamplePS>(ShaderVariance);
					}

					MeshRenderState.InputElements = {};
					MeshRenderState.RasterizeState = FRasterizeState::Default;
					MeshRenderState.BlendState = FBlendState::Default;
					MeshRenderState.DepthStencilState = FDepthStencilState::Disabled;
				}
				FDrawCommand MeshCommand;
				MeshCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				MeshCommand.VertexCount = 3;
				MeshCommand.Instanced.VertexOffset = 0;
				MeshCommand.Instanced.VertexCount = 3;

				MeshCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);
				MeshCommand.Parameters.Emplace(u8"DepthX2"N, Inputs.DepthX2->Resource);
				MeshCommand.Parameters.Emplace(u8"Depth"N, Inputs.Depth->Resource);
				MeshCommand.Parameters.Emplace(u8"OcclusionMerged"N, Inputs.BlurStagingB->Resource);
				if (Inputs.OcclusionHQ)
					MeshCommand.Parameters.Emplace(u8"OcclusionHQ"N, Inputs.OcclusionHQ->Resource);
				if (Inputs.OcclusionMergedX2)
					MeshCommand.Parameters.Emplace(u8"OcclusionMergedX2"N, Inputs.OcclusionMergedX2->Resource);

				RGContext.DispatchCommand(MeshCommand);
				RGContext.EndPass();
			});
	}

	void FSSAO::DownsampleCS(FRGTexture * Depth2x, FRGTexture * DepthTiled8x, FRGTexture * Depth4x, FRGTexture * DepthTiled16x, FRGTexture * Depth1x, FRGTexture * SceneDepth)
	{
		struct FDownsampleInputs
		{
			FRGTexture * SceneDepth;

			FRGTexture * Depth1x;

			FRGTexture * Depth2x;
			FRGTexture * DepthTiled8x;
			FRGTexture * Depth4x;
			FRGTexture * DepthTiled16x;
		};
		FDownsampleInputs DownsampleInputs = { SceneDepth, Depth1x, Depth2x, DepthTiled8x, Depth4x, DepthTiled16x };

		RenderGraph.AddPass(u8"SSAO.DownsampleCS"V,
			[Inputs = DownsampleInputs](FCompileContext & CompileContext)
			{
				if (Inputs.SceneDepth)
				{
					CompileContext.AccessResource(Inputs.SceneDepth, EResourceAccess::ComputeSampledTexture);
					CompileContext.AccessResource(Inputs.Depth1x, EResourceAccess::ComputeStorageTexture);
				}
				else
					CompileContext.AccessResource(Inputs.Depth1x, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.Depth2x, EResourceAccess::ComputeStorageTexture);
				CompileContext.AccessResource(Inputs.DepthTiled8x, EResourceAccess::ComputeStorageTexture);
				CompileContext.AccessResource(Inputs.Depth4x, EResourceAccess::ComputeStorageTexture);
				CompileContext.AccessResource(Inputs.DepthTiled16x, EResourceAccess::ComputeStorageTexture);
			},
			[Inputs = DownsampleInputs](FRGContext & RGContext)
			{
				FMeshRenderState MeshRenderState;
				{
					FSSAODownsampleCS::FShaderVariance ShaderVariance;
					ShaderVariance.SetValue<FSSAODownsampleCS::FromSceneDepth>(Inputs.SceneDepth);
					MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FSSAODownsampleCS>(ShaderVariance);
				}
				FDrawCommand DispatchCommand;
				DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);

				if (Inputs.SceneDepth)
					DispatchCommand.Dispatch = { DivideCeil((uint32)Inputs.SceneDepth->Desc.Size.X / 2, 8u), DivideCeil((uint32)Inputs.SceneDepth->Desc.Size.Y / 2, 8u), 1u };
				else
					DispatchCommand.Dispatch = { DivideCeil((uint32)Inputs.Depth1x->Desc.Size.X / 2, 8u), DivideCeil((uint32)Inputs.Depth1x->Desc.Size.Y / 2, 8u), 1u };
				//DispatchCommand.ConstBuffers.Add({ EShaderType::Compute, 0, RHIParameter.Get() });

				float FarZ = 3000.0f;
				float NearZ = 0.1f;
				float ZMagic = (FarZ - NearZ) / NearZ;
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(ZMagic);
				DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);
				if (Inputs.SceneDepth)
				{
					DispatchCommand.Parameters.Emplace(u8"SceneDepth"N, Inputs.SceneDepth->Resource);
					DispatchCommand.Parameters.Emplace(u8"Depth1x"N, Inputs.Depth1x->Resource);
				}
				else
					DispatchCommand.Parameters.Emplace(u8"Depth1x"N, Inputs.Depth1x->Resource);

				DispatchCommand.Parameters.Emplace(u8"Depth2x"N, Inputs.Depth2x->Resource);
				DispatchCommand.Parameters.Emplace(u8"DepthTiled8x"N, Inputs.DepthTiled8x->Resource);
				DispatchCommand.Parameters.Emplace(u8"Depth4x"N, Inputs.Depth4x->Resource);
				DispatchCommand.Parameters.Emplace(u8"DepthTiled16x"N, Inputs.DepthTiled16x->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}

	void FSSAO::ComputeCS(FRGTexture * Occlusion, FRGTexture * DepthTile4x)
	{
		struct FComputeInputs
		{
			FRGTexture * Depth;
			FRGTexture * Occlusion;
		};

		float RejectionFalloff = 2.5f;
		float Accentuation = 0.1f;
		float TanHalfFovH = 1.0f / RenderGraph.Scene.SceneView.ProjectMatrix._11;

		FSSAOComputeCS::FShaderParameters & Parameters = RenderGraph.Allocator.AllocRef<FSSAOComputeCS::FShaderParameters>();
		{
			Parameters.InvertSliceDimension = 1.0f / DepthTile4x->Desc.Size;
			Parameters.RejectFadeoff = 1.0f / -RejectionFalloff;
			Parameters.RcpAccentuation = 1.0f / (1.0f + Accentuation);
			CalculateSSAOSampleWeights({ DepthTile4x->Desc.Size, 1, 1 }, TanHalfFovH, Parameters.ThicknessRcps, Parameters.SampleWeights);
		}

		FComputeInputs Inputs = { DepthTile4x, Occlusion };
		RenderGraph.AddPass(u8"SSAO.ComputeCS"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Depth, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.Occlusion, EResourceAccess::ComputeStorageTexture);
			},
			[Inputs = Inputs, &Parameters](FRGContext & RGContext)
			{
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(Parameters);

				FMeshRenderState MeshRenderState;
				{
					FSSAOComputeCS::FShaderVariance ShaderVariance;
					ShaderVariance.SetValue<FSSAOComputeCS::ComputeTiled>(Inputs.Depth->Desc.Depth > 1);
					MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FSSAOComputeCS>(ShaderVariance);
				}
				FDrawCommand DispatchCommand;
				DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);

				DispatchCommand.Dispatch =
				{
					DivideCeil((uint32)Inputs.Depth->Desc.Size.X + 1, 8u),
					DivideCeil((uint32)Inputs.Depth->Desc.Size.Y + 1, 8u),
					Inputs.Depth->Desc.Depth > 1 ? Inputs.Depth->Desc.Depth : 1
				};

				DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);

				DispatchCommand.Parameters.Emplace(u8"Depth"N, Inputs.Depth->Resource);
				DispatchCommand.Parameters.Emplace(u8"Occlusion"N, Inputs.Occlusion->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}

	void FSSAO::UpsampleCS(FRGTexture * OcclusionX2, FRGTexture * DepthX2, FRGTexture * Depth, FRGTexture * OcclusionHQ, FRGTexture * OcclusionMerged, FRGTexture * OcclusionMergedX2)
	{
		struct FUpsampleInputs
		{
			FRGTexture * OcclusionX2;

			FRGTexture * DepthX2;
			FRGTexture * Depth;

			FRGTexture * OcclusionHQ;
			FRGTexture * OcclusionMerged;
			FRGTexture * OcclusionMergedX2;
		};
		FUpsampleInputs UpsampleInputs = { OcclusionX2, DepthX2, Depth, OcclusionHQ, OcclusionMerged, OcclusionMergedX2 };
		RenderGraph.AddPass(u8"SSAO.UpsampleCS"V,
			[Inputs = UpsampleInputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.DepthX2, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.Depth, EResourceAccess::ComputeSampledTexture);
				if (Inputs.OcclusionHQ)
					CompileContext.AccessResource(Inputs.OcclusionHQ, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.OcclusionMerged, EResourceAccess::ComputeSampledTexture);
				if (Inputs.OcclusionMergedX2)
					CompileContext.AccessResource(Inputs.OcclusionMergedX2, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.OcclusionX2, EResourceAccess::ComputeStorageTexture);
			},
			[Inputs = UpsampleInputs](FRGContext & RGContext)
			{
				struct FAOUpsampleParameter
				{
					float2 InputTexelSizeX2;
					float2 InputTexelSize;

					float StepSize;
					float BlurTolerance;
					float UpsampleTolerance;
					float NoiseFilterStrength;
				};
				FAOUpsampleParameter AOUpsampleParameter;
				{
					//NumVar g_NoiseFilterTolerance("Graphics/SSAO/Noise Filter Threshold (log10)", -3.0f, -8.0f, 0.0f, 0.25f);
					//NumVar g_BlurTolerance("Graphics/SSAO/Blur Tolerance (log10)", -5.0f, -8.0f, -1.0f, 0.25f);
					//NumVar g_UpsampleTolerance("Graphics/SSAO/Upsample Tolerance (log10)", -7.0f, -12.0f, -1.0f, 0.5f);

					static float g_BlurTolerance = -5;
					static float g_UpsampleTolerance = -7.0;
					static float g_NoiseFilterTolerance = -3.0;

					float BlurTolerance = 1.0f - powf(10.0f, g_BlurTolerance) * 1920.0f / (float)Inputs.Depth->Desc.Size.X;
					BlurTolerance *= BlurTolerance;

					AOUpsampleParameter.InputTexelSizeX2 = { 1.0f / Inputs.DepthX2->Desc.Size.X, 1.0f / Inputs.DepthX2->Desc.Size.Y };
					AOUpsampleParameter.InputTexelSize = { 1.0f / Inputs.Depth->Desc.Size.X, 1.0f / Inputs.Depth->Desc.Size.Y };
					AOUpsampleParameter.StepSize = 1920.0f / Inputs.Depth->Desc.Size.X;
					AOUpsampleParameter.BlurTolerance = BlurTolerance;
					AOUpsampleParameter.UpsampleTolerance = powf(10.0f, g_UpsampleTolerance);
					AOUpsampleParameter.NoiseFilterStrength = 1.0f / (powf(10.0f, g_NoiseFilterTolerance) + AOUpsampleParameter.UpsampleTolerance);;
				}
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(AOUpsampleParameter);

				FMeshRenderState MeshRenderState;
				{
					FSSAOUpsampleCS::FShaderVariance ShaderVariance;
					ShaderVariance.SetValue<FSSAOUpsampleCS::CombineWithLower>(Inputs.OcclusionHQ);
					ShaderVariance.SetValue<FSSAOUpsampleCS::BlendWithHigher>(Inputs.OcclusionMergedX2);
					MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FSSAOUpsampleCS>(ShaderVariance);
				}

				FDrawCommand DispatchCommand;
				DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);

				DispatchCommand.Dispatch = { DivideCeil((uint32)Inputs.DepthX2->Desc.Size.X + 2, 16u), DivideCeil((uint32)Inputs.DepthX2->Desc.Size.Y + 2, 16u), 1u };

				DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);

				DispatchCommand.Parameters.Emplace(u8"DepthX2"N, Inputs.DepthX2->Resource);
				DispatchCommand.Parameters.Emplace(u8"Depth"N, Inputs.Depth->Resource);
				DispatchCommand.Parameters.Emplace(u8"OcclusionMerged"N, Inputs.OcclusionMerged->Resource);
				if (Inputs.OcclusionHQ)
					DispatchCommand.Parameters.Emplace(u8"OcclusionHQ"N, Inputs.OcclusionHQ->Resource);
				if (Inputs.OcclusionMergedX2)
					DispatchCommand.Parameters.Emplace(u8"OcclusionMergedX2"N, Inputs.OcclusionMergedX2->Resource);

				DispatchCommand.Parameters.Emplace(u8"OcclusionX2"N, Inputs.OcclusionX2->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}
}
