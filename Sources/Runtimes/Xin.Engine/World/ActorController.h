#pragma once

namespace Xin::Engine
{
	class ENGINE_API IActorController : public ISharedRefer
	{
	public:
		IActorController() = default;
		virtual ~IActorController() = default;

		virtual void Update(float32 DeltaTime) {}

		virtual void OnKeyDown(EKeyCode KeyCode) {}
		virtual void OnKeyUp(EKeyCode KeyCode) {}
		virtual void OnMouseDown(PointF Position, EMouseButton MouseButton) { }
		virtual void OnMouseUp(PointF Position, EMouseButton MouseButton) { }
		virtual void OnMouseMove(PointF Position) { }
		virtual void OnMouseWheel(PointF Position, Vec2F Whell) { }

	public:
		virtual void SetActor(FActor * Actor) = 0;

	public:
		FActor * Actor = nullptr;
	};

	class ENGINE_API FFirstPersionActorController : public IActorController
	{
	public:
		void Update(float32 DeltaTime) override;

		void OnKeyDown(EKeyCode KeyCode) override;
		void OnKeyUp(EKeyCode KeyCode) override;
		void OnMouseDown(PointF Position, EMouseButton MouseButton) override;
		void OnMouseUp(PointF Position, EMouseButton MouseButton) override;
		void OnMouseMove(PointF Position) override;
		void OnMouseWheel(PointF Position, Vec2F Whell) override;

		void BeginCameraMove(uint32 Move);
		void EndCameraMove(uint32 Move);
		void UpdateMoveDirection();

		void SetActor(FActor * Actor) override;

	private:
		void OnActorWorldChanged(FWorld * World);
		void OnActorTransformChanged(const FTransform & Transform);

	public:
		static constexpr uint32 MoveMask_Forward  = 0b0001;
		static constexpr uint32 MoveMask_Backward = 0b0010;
		static constexpr uint32 MoveMask_Left     = 0b0100;
		static constexpr uint32 MoveMask_Right    = 0b1000;

	public:
		float32 MoveSpeed = 500.0f;
		float32 MoveSpeedRateA = 1.0f;
		float32 MoveSpeedRateB = 1.0f;
		FVector2 RotateSpeed = { Float32PI * 0.005f, Float32PI * 0.005f };

	public:
		uint32 Moving = 0;
		FVector3 Position;
		FVector3 Direction;
		FVector3 Upward;

		uint32 Rotating = 0;
		FVector3 Rotation;
		FVector3 RotationPrev;

		PointF RotateMouseBegin = {};
		FVector3 RotateBegin {};
	};
}
