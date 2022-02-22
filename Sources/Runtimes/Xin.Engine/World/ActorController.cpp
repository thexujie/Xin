#include "PCH.h"
#include "ActorController.h"
#include "World/Actor.h"
#include "World/World.h"

namespace Xin::Engine
{
	void FFirstPersionActorController::Update(float32 DeltaTime)
	{
		bool Changed = false;
		if (Moving)
		{
			Position = Actor->Transform.Translation;
			Direction = Actor->World->AxisSystem.Forward * Actor->Transform.Rotation;
			Upward = Actor->World->AxisSystem.Upward * Actor->Transform.Rotation;

			FVector3 MoveDirection = FVector3::Zero;
			if (Moving & MoveMask_Forward)
				MoveDirection += Direction;
			if (Moving & MoveMask_Backward)
				MoveDirection -= Direction;
			if (Moving & MoveMask_Left)
				MoveDirection -= Upward ^ Direction;
			if (Moving & MoveMask_Right)
				MoveDirection += Upward ^ Direction;

			Position += MoveDirection * DeltaTime * MoveSpeed * MoveSpeedRateA * MoveSpeedRateB;
			Changed = true;
		}

		if (Rotation != RotationPrev)
		{
			RotationPrev = Rotation;
			Changed = true;
		}

		if (Changed)
		{
			Changed = false;
			FMatrix RotateTransform = FMatrix::RotatePitchYawRoll(Rotation);
			Direction = RotateTransform.TransformNormal(Actor->World->AxisSystem.Forward);
			Upward = RotateTransform.TransformNormal(Actor->World->AxisSystem.Upward);

			FTransform Transform { FQuat::Rotate(Rotation), Position, FVector3::One };
			if (Actor)
				Actor->SetWorldTransform(Transform);
		}
		IActorController::Update(DeltaTime);
	}

	void FFirstPersionActorController::OnKeyDown(EKeyCode KeyCode)
	{
		switch (KeyCode)
		{
		case EKeyCode::W:
			BeginCameraMove(MoveMask_Forward);
			break;
		case EKeyCode::A:
			BeginCameraMove(MoveMask_Left);
			break;
		case EKeyCode::S:
			BeginCameraMove(MoveMask_Backward);
			break;
		case EKeyCode::D:
			BeginCameraMove(MoveMask_Right);
			break;
		case EKeyCode::Shift:
			MoveSpeedRateA = 5;
			break;
		case EKeyCode::Ctrl:
			MoveSpeedRateB = 0.1f;
			break;
		default:
			break;
		}
	}

	void FFirstPersionActorController::OnKeyUp(EKeyCode KeyCode)
	{
		switch (KeyCode)
		{
		case EKeyCode::W:
			EndCameraMove(MoveMask_Forward);
			break;
		case EKeyCode::A:
			EndCameraMove(MoveMask_Left);
			break;
		case EKeyCode::S:
			EndCameraMove(MoveMask_Backward);
			break;
		case EKeyCode::D:
			EndCameraMove(MoveMask_Right);
			break;
		case EKeyCode::Shift:
			MoveSpeedRateA = 1;
			break;
		case EKeyCode::Ctrl:
			MoveSpeedRateB = 1;
			break;
		default:
			break;
		}
	}

	void FFirstPersionActorController::OnMouseDown(PointF MousePosition, EMouseButton MouseButton)
	{
		if (MouseButton != EMouseButton::Right)
			return;

		Rotating = true;
		RotateMouseBegin = MousePosition;
		Position = Actor->Transform.Translation;
		Direction = Actor->World->AxisSystem.Forward * Actor->Transform.Rotation;
		Upward = Actor->World->AxisSystem.Upward * Actor->Transform.Rotation;
		RotateBegin = RotationPrev = Rotation = Actor->Transform.Rotation.Angles();
	}

	void FFirstPersionActorController::OnMouseUp(PointF MousePosition, EMouseButton MouseButton)
	{
		if (MouseButton != EMouseButton::Right)
			return;
		Rotating = false;
	}

	void FFirstPersionActorController::OnMouseMove(PointF MousePosition)
	{
		if (!Rotating)
			return;

		PointF Offset = MousePosition - RotateMouseBegin;
		if (Offset.X == 0 && Offset.Y == 0)
			return;

		//RotateMouseBegin = Point;

		Rotation.Pitch = RotateBegin.Pitch + Offset.Y * RotateSpeed.Y;
		Rotation.Yaw = RotateBegin.Yaw + Offset.X * RotateSpeed.X;
		Rotation.Pitch = Clamp(Rotation.Pitch, -0.4999f * Float32PI, 0.4999f * Float32PI);
	}

	void FFirstPersionActorController::OnMouseWheel(PointF Position, Vec2F Whell)
	{

	}

	void FFirstPersionActorController::BeginCameraMove(uint32 Move)
	{
		Moving |= Move;
		UpdateMoveDirection();
	}

	void FFirstPersionActorController::EndCameraMove(uint32 Move)
	{
		Moving &= ~Move;
		UpdateMoveDirection();
	}

	void FFirstPersionActorController::UpdateMoveDirection()
	{
	}

	void FFirstPersionActorController::SetActor(FActor * Actor_)
	{
		Actor = Actor_;
		Actor->OnWorldChanged += { this, & FFirstPersionActorController::OnActorWorldChanged };
		Actor->OnTransformChanged += { this, &FFirstPersionActorController::OnActorTransformChanged };
		OnActorWorldChanged(Actor->World);
	}

	void FFirstPersionActorController::OnActorWorldChanged(FWorld * World)
	{
		if (Actor->World)
		{
			Position = Actor->Transform.Translation;
			Direction = Actor->World->AxisSystem.Forward * Actor->Transform.Rotation;
			Upward = Actor->World->AxisSystem.Upward * Actor->Transform.Rotation;
			RotationPrev = Rotation = Actor->Transform.Rotation.Angles();
		}
	}

	void FFirstPersionActorController::OnActorTransformChanged(const FTransform & Transform)
	{
		if (Actor->World)
		{
			Position = Actor->Transform.Translation;
			Direction = Actor->World->AxisSystem.Forward * Actor->Transform.Rotation;
			Upward = Actor->World->AxisSystem.Upward * Actor->Transform.Rotation;
			RotationPrev = Rotation = Actor->Transform.Rotation.Angles();
		}
	}
}
