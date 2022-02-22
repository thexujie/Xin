#pragma once

#include "Inc.h"
#include "Components/CameraComponent.h"
#include "Engine/RenderPasses/ScenePass.h"


class FWindowCamera : public UCameraComponent
{
public:
	FWindowCamera(AActor & Actor_, ptr_t WindowHandle_);

public:
	void Tick(float DeltaTime) override;

	void OnWindowUpdated();

public:
	TReferPtr<FSceneView> CreateSceneView() override;

	ptr_t WindowHandle = nullptr;

	TReferPtr<FSceneView> WindowView;
private:
	void _UpdateWindowSize();

	void Cull();
};
