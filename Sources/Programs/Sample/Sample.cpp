#include "PCH.h"
#include "Sample.h"
#include "RHID3D12/RHID3D12.h"
#include "Core/Platform/Win32/Win32.h"

#include <iostream>

#include "HairShaders.h"
#include "WindowCamera.h"
#include "Engine/Core/RenderSystem.h"

FSample * SampleWorld = nullptr;

void FSample::Initialize(ptr_t WindowHandleIn)
{
	SampleWorld = this;

	WindowHandle = WindowHandleIn;
	RHIFactory = RHI::D3D12::CreateRHIFactory();
	RHIFactory->Load();

	Engine = MakeShared<FRenderSystem>();
	Engine->Initialize(*RHIFactory);

	World = MakeRefer<FWorld>();

	//NumMeshRows = 10;
	//NumMeshCols = 10;

	TReferPtr<FMaterialAsset> RedMaterial = Engine->ResourceFactory.LoadResourceT<FMaterialAsset>(Str("../Content/Default.xmtl"));
	TReferPtr<FMaterialAsset> TextureMaterial = Engine->ResourceFactory.LoadResourceT<FMaterialAsset>(Str("../Content/BaseColor.xmtl"));
	TextureMaterial->SetTextureParameter(Str("tBaseColor"), MakeRefer<FTexture>(Engine.Value(),  Engine->ResourceFactory.LoadResourceT<FTextureAsset>(Str("../Content/Cat.xtex"))));
	for(int32_t RowIndex = 0; RowIndex < NumMeshRows; ++RowIndex)
	{
		for (int32_t ColIndex = 0; ColIndex < NumMeshCols; ++ColIndex)
		{
			TReferPtr<AActor> StaticMeshActor = MakeRefer<AActor>(*World);
			
			TReferPtr<UStaticMeshComponent> StaticMeshComponent = MakeRefer<UStaticMeshComponent>(*StaticMeshActor);

			StaticMeshComponent->StaticMesh = MakeRefer<FStaticMesh>(Engine.Value(), Engine->ResourceFactory.LoadResourceT<FStaticMeshAsset>(Str("../Content/Wolf.xmsh")));
			//StaticMeshComponent->Material = Engine->ResourceFactory.LoadResourceT<FMaterial>(Str("../Content/Default.xmtl"));
			StaticMeshComponent->Material = (ColIndex % 2) ? TextureMaterial : RedMaterial;
			StaticMeshComponent->Name = Format(Str("Mesh [{}][{}]"), RowIndex, ColIndex);
			
			StaticMeshActor->Components.Add(StaticMeshComponent);
			

			FVector3 Translation = {};
			Translation.X = (ColIndex - NumMeshCols / 2) * MeshSpacing.X;
			Translation.Z = RowIndex * MeshSpacing.Y + 1;
			StaticMeshActor->SetTransform(FTransform::Translate(Translation));
			
			MeshActors.Add(StaticMeshActor);
		}
	}

	{
		TReferPtr<AActor> CameraActor = MakeRefer<AActor>(*World);
		MainCamera = MakeRefer<FWindowCamera>(*CameraActor, WindowHandle);
		CameraActor->Components.Add(MainCamera);
	}
	_UpdateCamera();

	Engine->Start();
	World->BeginPlay();
	Engine->Flush();
}

void FSample::Finalize()
{
	Engine->Finalize();
}


void FSample::GameTick()
{
	Ticker.Tick();
	World->Tick(Ticker.DeltaTime);
	Engine->WaitForRender();
	Engine->Flush();
}

void FSample::OnKeyDown(uint32_t VirtualKey, uint32_t ModifyKeys)
{
	switch (VirtualKey)
	{
	default:
		break;
	}
}

void FSample::OnMouseDownL(const PointI & Point)
{
	DragPoint = Point;
	Draging = true;
	RotateLast = Rotate;
}

void FSample::OnMouseUpL(const PointI & Point)
{
	Draging = false;
}

void FSample::OnMouseMove(const PointI & Point)
{
	if (Draging)
	{
		Rotate.X = RotateLast.X -(Point.Y - DragPoint.Y) * RotateSpeed;
		Rotate.Y = RotateLast.Y -(Point.X - DragPoint.X) * RotateSpeed;
		_UpdateActors();
		Engine->Flush();
	}
}

void FSample::OnMouseWheel(const PointI & Wheel)
{
	CameraAt.Z += Wheel.Y * MoveSpeed;
	_UpdateCamera();
}

void FSample::_UpdateActors()
{
	FTransform Transform = FTransform::Identity;
	static float RotationTimeBeg = FDateTime::System();
	//Transform.Rotation = FQuat::Rotate(FVector3(0.f, 0.f, (FDateTime::System() - RotationTimeBeg) * 3.14159265f * 0.5f));
	//Transform.Rotation = FQuat::Rotate(FVector3(0.f, 0.f, (FDateTime::System() - RotationTimeBeg) * 3.14159265f * 0.5f));
	Transform.Rotation = FQuat::Rotate(Rotate);
	//Transform.Scaling = FVector3(0.5f, 0.5f, 0.5f);
	//Transform.Scaling = FVector3(10.0f, 10.0f, 10.0f);
			//Plane->SetLocalTransform(Transform);
	for (uint32_t RowIndex = 0; RowIndex < NumMeshRows; ++RowIndex)
	{
		for (uint32_t ColIndex = 0; ColIndex < NumMeshCols; ++ColIndex)
		{
			Transform.Translation.X = (ColIndex - NumMeshCols / 2) * MeshSpacing.X;
			Transform.Translation.Y = 0.0f;
			Transform.Translation.Z = RowIndex * MeshSpacing.Y + 1;
			MeshActors[RowIndex * NumMeshCols + ColIndex]->SetTransform(Transform);
		}
	}
}

void FSample::_UpdateCamera()
{
	MainCamera->ViewMatrix = FMatrix::LookAtLH(CameraAt, FVector3(), FVector3(0.f, 1.f, 0.f));
	MainCamera->ProjectionMatrix = FMatrix::PerspectiveLH(3.14159265f * 0.5f, (float)MainCamera->ViewSize.Width / MainCamera->ViewSize.Height, 0.0001f, 1000.0f);
	MainCamera->ViewProjectionMatrix = MainCamera->ViewMatrix * MainCamera->ProjectionMatrix;
	MainCamera->OnCameraViewChanged();
}

