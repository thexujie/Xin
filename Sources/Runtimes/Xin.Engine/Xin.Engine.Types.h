#pragma once

#include "Xin.RHI/Xin.RHI.h"
#include "Xin.Rendering/Xin.Rendering.h"

#if XIN_WINDOWS
#ifdef _Xin_Engine_Module_
#define ENGINE_API __declspec(dllexport)
#define ENGINE_AVR __declspec(dllexport)
#else
#define ENGINE_API
#define ENGINE_VAR __declspec(dllimport)
#endif
#endif

namespace Xin::Engine
{
	using namespace Xin::RHI;
	using namespace Xin::Rendering;

	class FGameEngine;

	class UTexture;
	using UTextureRef = TReferPtr<UTexture>;

	class UMaterial;
	using UMaterialRef = TReferPtr<UMaterial>;

	class UStaticMesh;
	using UStaticMeshRef = TReferPtr<UStaticMesh>;

	class FRGResource;
	class FRGResourceView;

	using FRGTexture = FRGResource;
	using FRGBuffer = FRGResource;
	using FRGTextureRef = TReferPtr<FRGTexture>;
	using FRGBufferRef = TReferPtr<FRGBuffer>;

	class FRenderGraph;

	class FScene;
	class FActor;
	class FWorld;

	constexpr uint32 CSMCascadeMax = 4;

	enum class EBasePass
	{
		Shading = 0,
		Depth,
		Count,
	};

	enum EBasePassIndex
	{
		BPI_Shading = 0,
		BPI_Depth,
		BPI_Count,
	};
	constexpr FStringV RenderMeshPassNames[BPI_Count] = { u8"Default"V, u8"Depth"V };

	enum class EMeshPass
	{
		Base = 0,
		Depth,
		Count,
	};

	enum EMeshPassIndex
	{
		MPI_Shading = 0,
		MPI_Depth,
		MPI_Count,
	};
	constexpr TArray<FStringV, MPI_Count> MeshPassNames = { u8"Base"V, u8"Depth"V };

	enum class EPostProcessMode
	{
		Disabled = 0,
		PS,
		CS,
		Count,
	};

	enum class EFXAAMode
	{
		Disabled = 0,
		PS_Console,
		PS,
		PS_3_11,
		CS,
		Count,
	};

	enum class EBloomMode
	{
		Disabled = 0,
		PS,
		CS,
		Count
	};

	enum class EFXAALevel
	{
		Level1 = 0,
		Level2,
		Level3,
		Level4,
		Level5,
		Level6,
		Level7,
		Level8,
		Level9,
		Level10,
		Level11,
		Level12,
		Level13,
		Level14,
		Level15,
		Level16,
		Level17,
		Count,
	};

	enum class EAOMode
	{
		Disabled = 0,
		SSAO,
		HBAO,
	};

	enum class EHBAOMode
	{
		Disabled = 0,
		PS,
		CS,
		Count,
	};

	enum class EHBAOLevel
	{
		Level0 = 0,
		Level1,
		Level2,
		Count,
	};

	enum class EHBAOQuality
	{
		Quality0 = 0,
		Quality1,
		Quality2,
		Quality3,
		Count,
	};

	enum class ESSAOMode
	{
		Disabled = 0,
		PS,
		CS,
		Count,
	};

	enum class ESSAOLevel
	{
		Level0 = 0,
		Level1,
		Level2,
		Count,
	};

	enum class ESSAOQuality
	{
		Quality0 = 0,
		Quality1,
		Quality2,
		Quality3,
		Count,
	};

	enum class EBloomLevel
	{
		Level0,
		Level1,
		Count,
	};

	enum class EGussianBlurMode
	{
		PixelShader = 0,
		ComputerShader,
		ComputerShader_Basic,
	};


	class ENGINE_API IRenderingObject : public ISharedRefer
	{
	public:
	};
	using IRenderingObjectRef = TReferPtr<IRenderingObject>;

	class ENGINE_API IEngineObject : public ISharedRefer
	{
	public:
		virtual void SetLabel(FStringV Label) { this->Label = Label ; }

	public:
		FString Label;
	};
	using IEngineObjectRef = TReferPtr<IEngineObject>;

	struct FAxisSystem
	{
		FVector3 Forward = FVector3::AxisZ;
		FVector3 Upward = FVector3::AxisY;
		FVector3 Right = FVector3::AxisX;
	};

	struct FFrustum
	{
		FVector3 Position;
		FQuat Rotation;

		float32 RightSlope;
		float32 LeftSlope;
		float32 TopSlope;
		float32 BottomSlope;

		float32 NearZ;
		float32 FarZ;
	};

	struct FDirectionalLight
	{
		float32 NearZ = 0;
		float32 FarZ = 4000;
		float32 FrustumFarZ = 4000;

		FVector3 Position;
		FQuat Rotation;
	};
}
