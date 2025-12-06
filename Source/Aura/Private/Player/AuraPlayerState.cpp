// Copyright Dan Wilshere


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponentBase.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	// set up GAS components directly in the enemy character class
	this->AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponentBase>("AbilitySystemComponent");
	this->AbilitySystemComponent->SetIsReplicated(true);
	// replicate gameplay effects to owning client. gameplay tags and cues are replicated to all clients.
	this->AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	this->AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	// how frequently to update clients
	// default is something like 1/2 second
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
