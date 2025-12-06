// Copyright Dan Wilshere


#include "Character/AuraEnemy.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraEnemy::AAuraEnemy()
{
	// have the engine tick this actor every frame for interactions
	this->PrimaryActorTick.bCanEverTick = true;

	check(this->GetMesh());

	// adjusts collision presets to allow highlighting
	this->GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	// this is from the PP_Highlight material; values >= 250 are highlighted
	this->GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	this->Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	// set up GAS components directly in the enemy character class
	//   they are declared in AuraCharacterBase
	this->AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponentBase>("AbilitySystemComponent");
	this->AbilitySystemComponent->SetIsReplicated(true);
	// Only replicate minimal gameplay effect info as this is an AI character
	this->AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	this->AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraEnemy::HighlightActor()
{	
	// don't forget to change the mesh presets for collision 
	// Under Collision Presets, set to Custom
	// Under Trace Responses set Visibility to Block
	//bHighlighted = true;
	this->GetMesh()->SetRenderCustomDepth( true );
	this->Weapon->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnhighlightActor()
{
	//bHighlighted = false;
	this->GetMesh()->SetRenderCustomDepth( false );
	this->Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
	if (this->bHighlighted && this->GetWorld())
	{
		DrawDebugSphere(
			this->GetWorld(),
			this->GetActorLocation(),
			50.f,	// radius
			12,		// # segments
			FColor::Red,
			false,	// persistent
			0.1f,	// duration (s)
			0,		// priority
			1.f		// thickness
		);
	}*/
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	// set up the ability system from the GAS interface
	// because the enemy characters have a controller attached already, this is good for the GAS startup
	// since this is an AI character, the Avatar and the Owner are both "this" object
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
