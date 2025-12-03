// Copyright Dan Wilshere


#include "AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// verify we have the input mapping context
	check(this->AuraContext);

	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(subsys);

	// zero priority
	subsys->AddMappingContext(this->AuraContext, 0);

	// some defaults
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// this allows us to use input from keyboard & mouse
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// known to be the Enhanced version, because it's set in Project Settings > Input > Default Input Component Class
	UEnhancedInputComponent* eic = CastChecked<UEnhancedInputComponent>(this->InputComponent);

	// bind input move action to our input component
	eic->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& Input)
{
	// controller will be positioned to the front of the character
	const FVector2D input_axis_vector = Input.Get<FVector2D>();
	const FRotator rotation = GetControlRotation();
	const FRotator yaw_rotation(0.f, rotation.Yaw, 0.f);

	// normalized unit axis vectors
	const FVector forward_dir = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::X);
	const FVector right_dir = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::Y);

	// get the pawn controller by the player
	// this gets called every frame, including before the pawn is instantied, so we're doing a passive check here
	if (APawn* controlled_pawn = GetPawn<APawn>())
	{
		controlled_pawn->AddMovementInput(forward_dir, input_axis_vector.Y);
		controlled_pawn->AddMovementInput(right_dir, input_axis_vector.X);
	}
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	this->CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult cursor_hit;
	this->GetHitResultUnderCursor(ECC_Visibility, false, cursor_hit);
	if (!cursor_hit.bBlockingHit) return;

	_last_actor = _this_actor;
	// because we're using TScriptInterface, we don't have to use 'Cast<IEnemyInterface>(cursor_hit.GetActor())';
	// documented https://www.udemy.com/course/unreal-engine-5-gas-top-down-rpg/learn/lecture/39782698#overview
	// at minute 6
	_this_actor = cursor_hit.GetActor();

	/**
	* Line trace from cursor, several scenarios are possible
	* A. LastActor is null && ThisActor is null
	*   - do nothing
	* B. LastActor is null && ThisActor is valid
	*   - this is an enemy interace; hovering over this actor for the first time; highlight this actor
	* C. LastActor is valid && ThisActor is null
	*   - unhighlight last actor
	* D. both are valid, but LastActor != ThisActor
	*   - change highlight (unhighlight LastActor and highlight ThisActor)
	* E. both are valid and same actor
	*   - still hovering over the same enemy; do nothing
	*/
	if (_last_actor  == nullptr )
	{
		if (_this_actor != nullptr)
		{
			// Case B
			_this_actor->HighlightActor();
		}
		else
		{
			// Case A - do nothing
		}
	}
	else
	{
		// last actor is valid
		if (_this_actor == nullptr)
		{
			// Case C
			_last_actor->UnhighlightActor();
		}
		else if(_last_actor != _this_actor)
		{
			// Case D
			_last_actor->UnhighlightActor();
			_this_actor->HighlightActor();
		}
		else
		{
			// Case E; do nothing
		}
	}
}
