// Copyright Dan Wilshere


#include "Character/AuraEnemy.h"
#include "DrawDebugHelpers.h"

AAuraEnemy::AAuraEnemy()
{
	// have the engine tick this actor every frame for interactions
	this->PrimaryActorTick.bCanEverTick = true;
}

void AAuraEnemy::HighlightActor()
{	
	// don't forget to change the mesh presets for collision 
	// Under Collision Presets, set to Custom
	// Under Trace Responses set Visibility to Block
	bHighlighted = true;
}

void AAuraEnemy::UnhighlightActor()
{
	bHighlighted = false;
}

void AAuraEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	}
}