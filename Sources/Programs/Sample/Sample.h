#pragma once

#include "Inc.h"
#include "WindowCamera.h"


class FSample
{
public:
	FSample() = default;
	~FSample() = default;

	void Initialize(ptr_t WindowHandleIn);
	void Finalize();

	void GameTick();

public:
	void OnKeyDown(uint32_t VirtualKey, uint32_t ModifyKeys);
	void OnMouseDownL(const PointI & Point);
	void OnMouseUpL(const PointI & Point);
	void OnMouseMove(const PointI & Point);
	void OnMouseWheel(const PointI & Wheel);

private:
	void _UpdateActors();
	void _UpdateCamera();
	
private:
	ptr_t WindowHandle = nullptr;

	TSharedPtr<FRHIFactory> RHIFactory;
	TSharedPtr<FRenderSystem> Engine;

	TReferPtr<FWorld> World;

	intx_t NumMeshRows = 100;
	intx_t NumMeshCols = 100;
	Vec2F MeshSpacing = { 1, 1 };
	TVector<AActorPtr> MeshActors;

	TReferPtr<FMaterialAsset> Material;
	TReferPtr<FWindowCamera> MainCamera;

	FVector3 CameraAt = FVector3(0.f, 1.f, -1.f);
	// Intersect
	PointI DragPoint;
	bool Draging = false;
	FVector3 RotateLast = {};
	FVector3 Rotate = {};
	float RotateSpeed = 0.01f;
	float MoveSpeed = 0.01f;

	FTicker Ticker;
};

