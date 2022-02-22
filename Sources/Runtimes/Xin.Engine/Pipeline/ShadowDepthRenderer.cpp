#include "PCH.h"
#include "ShadowDepthRenderer.h"

#include "RenderGraph.h"

namespace Xin::Engine
{
	class FShadowDepthPassBaseShader : public IShaderSource
	{
	public:
		using FShaderVariance = TShaderVariance<>;

		FShadowDepthPassBaseShader(FStringV AssetPath, EShaderStage ShaderStage, FStringV Entry) : IShaderSource(ShaderStage, AssetPath, Entry)
		{
		}
	};

	class FShadowDepthPassVS : public FShadowDepthPassBaseShader
	{
	public:
		DEFINE_SHADER_INSTANCE(FShadowDepthPassVS);

		FShadowDepthPassVS() : FShadowDepthPassBaseShader(u8"Engine/Shaders/ShadowDepthPass.xsf"V, EShaderStage::Vertex, u8"DepthVertexShader"V)
		{
		}
	};

	class FShadowDepthPassPS : public FShadowDepthPassBaseShader
	{
	public:
		DEFINE_SHADER_INSTANCE(FShadowDepthPassPS);

		FShadowDepthPassPS() : FShadowDepthPassBaseShader(u8"Engine/Shaders/ShadowDepthPass.xsf"V, EShaderStage::Pixel, u8"DepthPixelShader"V)
		{
		}
	};

	FShadowDepthRenderer::FShadowDepthRenderer(FRenderGraph & RenderGraph)
		: RenderGraph(RenderGraph)
	{
		if (RenderGraph.Scene.RenderSettings.CascadeLevel == 0)
			ShadowDepth = RenderGraph.CreateResource(u8"ShadowDepth"V, { EFormat::D16UF, { ShadowDepthTextureSize, ShadowDepthTextureSize }, RenderGraph.Scene.RenderSettings.ReversedZ ? 0.0f : 1.0f });
		else
			ShadowDepth = RenderGraph.CreateResource(u8"ShadowDepth"V, { EFormat::D16UF, { ShadowDepthTextureSize, ShadowDepthTextureSize, RenderGraph.Scene.RenderSettings.CascadeLevel }, RenderGraph.Scene.RenderSettings.ReversedZ ? 0.0f : 1.0f });
	}

	FFrustum GenerateFrustum(const FMatrix & ProjectMatrix)
	{
		FFrustum Frustum = {};
		Frustum.Position = FVector3::Zero;
		Frustum.Rotation = FQuat::Identity;

		FVector4 AxisPoints[6] =
		{
			{ 1.0f, 0.0f, 1.0f, 1.0f },   // right (at far plane)
			{ -1.0f, 0.0f, 1.0f, 1.0f },   // left
			{ 0.0f, 1.0f, 1.0f, 1.0f },   // top
			{ 0.0f, -1.0f, 1.0f, 1.0f },   // bottom

			{ 0.0f, 0.0f, 0.0f, 1.0f },     // near
			{ 0.0f, 0.0f, 1.0f, 1.0f }      // far
		};

		FMatrix ProjectMatrixInv = ProjectMatrix.Inverse();
		for (uintx Index = 0; Index < 6; ++Index)
			AxisPoints[Index] = ProjectMatrixInv.Transform(AxisPoints[Index]);

		Frustum.RightSlope = AxisPoints[0].X / AxisPoints[0].Z;
		Frustum.LeftSlope = AxisPoints[1].X / AxisPoints[1].Z;
		Frustum.TopSlope = AxisPoints[2].Y / AxisPoints[2].Z;
		Frustum.BottomSlope = AxisPoints[3].Y / AxisPoints[3].Z;

		Frustum.NearZ = AxisPoints[4].Z / AxisPoints[4].W;
		Frustum.FarZ = AxisPoints[5].Z / AxisPoints[5].W;

		return Frustum;
	}

	TArray<FVector4, 8> GenerateFrustumCornerPoints(const FFrustum & Frustum)
	{
		TArray<FVector4, 8> CornerPoints;
		FVector4 Slopes = { Frustum.LeftSlope, Frustum.TopSlope, Frustum.RightSlope, Frustum.BottomSlope };

		FVector4 NearSlopes = Slopes * Frustum.NearZ;
		CornerPoints[0] = { NearSlopes[0], NearSlopes[1], Frustum.NearZ, 1.0f };
		CornerPoints[1] = { NearSlopes[2], NearSlopes[1], Frustum.NearZ, 1.0f };
		CornerPoints[2] = { NearSlopes[2], NearSlopes[3], Frustum.NearZ, 1.0f };
		CornerPoints[3] = { NearSlopes[0], NearSlopes[3], Frustum.NearZ, 1.0f };

		FVector4 FarSlopes = Slopes * Frustum.FarZ;
		CornerPoints[4] = { FarSlopes[0], FarSlopes[1], Frustum.FarZ, 1.0f };
		CornerPoints[5] = { FarSlopes[2], FarSlopes[1], Frustum.FarZ, 1.0f };
		CornerPoints[6] = { FarSlopes[2], FarSlopes[3], Frustum.FarZ, 1.0f };
		CornerPoints[7] = { FarSlopes[0], FarSlopes[3], Frustum.FarZ, 1.0f };


		return CornerPoints;
	}

	TArray<FVector4, 8> GenerateFrustumCornerPointsCSM(const FFrustum & Frustum, float32 NearPercentage, float32 FarPercentage)
	{
		float32 NearZ = Lerp(Frustum.NearZ, Frustum.FarZ, NearPercentage);
		float32 FarZ = Lerp(Frustum.NearZ, Frustum.FarZ, FarPercentage);
		TArray<FVector4, 8> CornerPoints;
		FVector4 Slopes = { Frustum.LeftSlope, Frustum.TopSlope, Frustum.RightSlope, Frustum.BottomSlope };

		FVector4 NearSlopes = Slopes * NearZ;
		CornerPoints[0] = { NearSlopes[0], NearSlopes[1], NearZ, 1.0f };
		CornerPoints[1] = { NearSlopes[2], NearSlopes[1], NearZ, 1.0f };
		CornerPoints[2] = { NearSlopes[2], NearSlopes[3], NearZ, 1.0f };
		CornerPoints[3] = { NearSlopes[0], NearSlopes[3], NearZ, 1.0f };

		FVector4 FarSlopes = Slopes * FarZ;
		CornerPoints[4] = { FarSlopes[0], FarSlopes[1], FarZ, 1.0f };
		CornerPoints[5] = { FarSlopes[2], FarSlopes[1], FarZ, 1.0f };
		CornerPoints[6] = { FarSlopes[2], FarSlopes[3], FarZ, 1.0f };
		CornerPoints[7] = { FarSlopes[0], FarSlopes[3], FarZ, 1.0f };


		return CornerPoints;
	}

	void FShadowDepthRenderer::Render(const FDirectionalLight & DirectionalLight)
	{
		FFrustum CameraViewFrustum = GenerateFrustum(RenderGraph.Scene.SceneView.ProjectMatrix);
		CameraViewFrustum.FarZ = Min(DirectionalLight.FrustumFarZ, CameraViewFrustum.FarZ);
		LightViewMatrix = FMatrix::LookToLH(DirectionalLight.Position, RenderGraph.Scene.RenderSettings.AxisSystem.Forward * DirectionalLight.Rotation, RenderGraph.Scene.RenderSettings.AxisSystem.Upward * DirectionalLight.Rotation);
		FMatrix CameraViewMatrixInv = RenderGraph.Scene.SceneView.ViewMatrix.Inverse();

		auto DispatchRender = [&](FMatrix & ProjectMatrix, FDrawCommandCollector & MeshCommandCollector, float32 NearPercentage, float32 FarPercentage, FRGResourceView * ShadowDepthRTV)
		{
			TArray<FVector4, 8> CornerPoints = GenerateFrustumCornerPointsCSM(CameraViewFrustum, NearPercentage * 0.01f, FarPercentage * 0.01f);

			FVector3 AABBMinLight = FVector3::Max;
			FVector3 AABBMaxLight = FVector3::Min;

			for (uint32 PointIndex = 0; PointIndex < CornerPoints.Size; ++PointIndex)
			{
				// Camera -> World
				FVector4 WorldSpacePoint = CameraViewMatrixInv.Transform(CornerPoints[PointIndex]);
				// World -> Light
				FVector4 LightSpacePoint = LightViewMatrix.Transform(WorldSpacePoint);
				AABBMinLight = Min(AABBMinLight, LightSpacePoint.XYZ);
				AABBMaxLight = Max(AABBMaxLight, LightSpacePoint.XYZ);
			}

			bool AlignToViewFrustum = false;
			if (AlignToViewFrustum)
			{
				FVector2 DiagonalLength = FVector2::Replicate((CornerPoints[0].XYZ - CornerPoints[6].XYZ).Length());
				FVector2 WorldUnitsPerTexel = DiagonalLength / float32(ShadowDepthTextureSize);

				FVector2 BorderOffset = (DiagonalLength - (AABBMaxLight.XY - AABBMinLight.XY)) * FVector2::Half;
				AABBMinLight.XY -= BorderOffset;
				AABBMaxLight.XY += BorderOffset;
				AABBMinLight.XY = Floor(AABBMinLight.XY / WorldUnitsPerTexel) * WorldUnitsPerTexel;
				AABBMaxLight.XY = Floor(AABBMaxLight.XY / WorldUnitsPerTexel) * WorldUnitsPerTexel;
			}
			else
			{
				uint32 ExtendSize = RenderGraph.Scene.RenderSettings.PCFBlurRadius * 2 + 1;

				FVector2 BorderExtend = (AABBMaxLight.XY - AABBMinLight.XY) * float32(ExtendSize) * 0.5f / float32(ShadowDepthTextureSize - ExtendSize);
				AABBMinLight.XY -= BorderExtend;
				AABBMaxLight.XY += BorderExtend;

				FVector2 WorldUnitsPerTexel = (AABBMaxLight.XY - AABBMinLight.XY) / float32(ShadowDepthTextureSize);
				AABBMinLight.XY = Floor(AABBMinLight.XY / WorldUnitsPerTexel) * WorldUnitsPerTexel;
				AABBMaxLight.XY = Floor(AABBMaxLight.XY / WorldUnitsPerTexel) * WorldUnitsPerTexel;
			}

			//float32 NearZ = Min(AABBMinLight.Z, 0);
			//float32 FarZ = Max(AABBMaxLight.Z, AABBCenter.Z + Light.FarZ * 0.5f);
			//ProjectMatrix = FMatrix::OrthographicOffCenterLH(AABBMinLight.X, AABBMaxLight.X, AABBMinLight.Y, AABBMaxLight.Y, NearZ, AABBMaxLight.Z);
			ProjectMatrix = FMatrix::OrthographicOffCenterLH(AABBMinLight.X, AABBMaxLight.X, AABBMinLight.Y, AABBMaxLight.Y, DirectionalLight.NearZ, DirectionalLight.FarZ);
			RenderSM(ProjectMatrix, ShadowDepthRTV, MeshCommandCollector);
		};

		if (RenderGraph.Scene.RenderSettings.CascadeLevel == 0)
		{
			DispatchRender(ProjectMatrix[0], CommandCollectors[0], 0.f, 100.f, RenderGraph.CreateResourceView(ShadowDepth));
		}
		else
		{
			for (uint32 CascadeIndex = 0; CascadeIndex < RenderGraph.Scene.RenderSettings.CascadeLevel; ++CascadeIndex)
			{
				DispatchRender(ProjectMatrix[CascadeIndex], CommandCollectors[CascadeIndex],
					RenderGraph.Scene.RenderSettings.CascadePercentages[CascadeIndex], RenderGraph.Scene.RenderSettings.CascadePercentages[CascadeIndex + 1],
					RenderGraph.CreateResourceView(ShadowDepth, CascadeIndex));
			}
		}
	}

	void FShadowDepthRenderer::RenderSM(const FMatrix & ProjectMatrix, FRGResourceView * ShadowDepthRTV, FDrawCommandCollector & CommandCollector)
	{
		static int ShadowDepthBias = -200;
		static float ShadowDepthBiasClamp = 0.0f;
		static float ShadowSlopDepthBias = -2.5f;

		const FDepthStencilState DefaultDepthStencilState = RenderGraph.Scene.RenderSettings.ReversedZ ? FDepthStencilState::DefaultReversedZ : FDepthStencilState::Default;

		FRasterizeState ShadowDepthRasterizeState;
		ShadowDepthRasterizeState.DepthBias = ShadowDepthBias * (RenderGraph.Scene.RenderSettings.ReversedZ ? 1 : -1);
		ShadowDepthRasterizeState.DepthBiasClamp = ShadowDepthBiasClamp / 65535.0f/*EFormat::D16*/ * (RenderGraph.Scene.RenderSettings.ReversedZ ? 1 : -1);
		ShadowDepthRasterizeState.SlopScaledDepthBias = ShadowSlopDepthBias * (RenderGraph.Scene.RenderSettings.ReversedZ ? 1 : -1);

		FViewParameter LightViewParameterData;
		LightViewParameterData.ViewMatrix = LightViewMatrix;
		LightViewParameterData.ProjectMatrix = RenderGraph.Scene.RenderSettings.ReversedZ ? ProjectMatrix.ReversedZ() : ProjectMatrix;
		LightViewParameterData.ViewProjectMatrix = LightViewParameterData.ViewMatrix * LightViewParameterData.ProjectMatrix;

		IRHIResource * LightViewParameter = RenderGraph.CreateShaderParameterT(LightViewParameterData);

		CommandCollector.CollectDrawCommands(RenderGraph, EMeshPass::Depth, RenderGraph.Scene, nullptr, LightViewParameter, nullptr, nullptr, ShadowDepthRasterizeState, DefaultDepthStencilState,
			FShadowDepthPassVS::Instance(), FShadowDepthPassVS::FShaderVariance(), FShadowDepthPassPS::Instance(), FShadowDepthPassPS::FShaderVariance());

		RenderGraph.AddPass(u8"Shadow Depth"V,
			[ShadowDepthRTV](FCompileContext & CompileContext)
			{
				CompileContext.AccessResourceView(ShadowDepthRTV, EResourceAccess::DepthWrite, EResourceLoad::Clear);
			},
			[this, ShadowDepthRTV, &CommandCollector](FRGContext & RGContext)
			{
				FViewport Viewport { 0.0f, 0.0f, (float32)ShadowDepthTextureSize, (float32)ShadowDepthTextureSize, 0.0f, 1.0f };

				RGContext.BeginPass({ }, { ShadowDepthRTV, EResourceLoad::Clear }, Viewport);
				RGContext.DispatchCommands(CommandCollector.DrawCommands);
				RGContext.EndPass();
			});
	}
}
