// Copyright Dan Wilshere


#include "AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

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