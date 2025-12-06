// Copyright Dan Wilshere


#include "Character/AuraCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraCharacter::AAuraCharacter()
{
	// instructions here for C++ camera & boom arm
	// https://dev.epicgames.com/documentation/en-us/unreal-engine/quick-start-guide-to-player-controlled-cameras-in-unreal-engine-cpp?application_version=5.5
	
	// RootComponent defined in Actor.h
	// The component that defines the transform(location, rotation, scale) of this Actor in the world, all other components must be attached to this
	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));

	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));

	// local components; capsule already exists
	TObjectPtr<UCapsuleComponent> capsule = this->GetCapsuleComponent();
	_static_mesh_component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	_spring_arm_component = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	_camera_component = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	// attach all the components; spring arm static mesh goes on the Character's existing capsule component
	_static_mesh_component->SetupAttachment(capsule);
	_spring_arm_component->SetupAttachment(_static_mesh_component);
	_camera_component->SetupAttachment(_spring_arm_component);

	// configure the camera and boom arm
	// default 50 units above the root and pitch (rotational) of -45 degrees
	_spring_arm_component->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 50.f), FRotator(-45.f, 0.f, 0.f));
	_spring_arm_component->TargetArmLength = 750.f;
	_spring_arm_component->bEnableCameraLag = true;
	_spring_arm_component->CameraLagSpeed = 3.0f;
	_spring_arm_component->bUsePawnControlRotation = 0; // 1 (enabled) by default, turning off to make top-down 3rd person perspective
	
	// keep camera static
	_spring_arm_component->bInheritPitch = false;
	_spring_arm_component->bInheritRoll = false;
	_spring_arm_component->bInheritYaw = false;

	_camera_component->bUsePawnControlRotation = 0; // 1 (enabled) by default, turn off to rotate camera with movement
	
	// set up character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f); // yaw control
	GetCharacterMovement()->bConstrainToPlane = true;	// top-down visual setting
	GetCharacterMovement()->bSnapToPlaneAtStart = true;	// top-down visual setting

	// keeps controller static
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationRoll = false;
	this->bUseControllerRotationYaw= false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	// this ensures the pawn has a controller (server side)
	// gameplay ability system needs the controller attached (possessing) the pawn

	// inherited from ACharacter
	Super::PossessedBy(NewController);
	
	// Init ability actor info for the Server
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	// inherited from APawn
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	// OnRep_PlayerState ensures the player character has a controller attached
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	// Init ability actor info for the player state
	AAuraPlayerState* player_state = GetPlayerState<AAuraPlayerState>();
	check(player_state);

	// set the character's ability system component, inherited from AAuraCharacterBase
	this->AbilitySystemComponent = player_state->GetAbilitySystemComponent();
	this->AttributeSet = player_state->GetAttributeSet();

	// in this case, the owner actor is the player state, and the avatar actor is the player object
	this->AbilitySystemComponent->InitAbilityActorInfo(player_state, this);
}
