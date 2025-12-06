// Copyright Dan Wilshere

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	
	//~ Begin APawn Interface.
	// Gameplay ability overrides
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	//~ End APawn Interface.

protected:

	UPROPERTY(EditAnywhere, Category="Camera")
	TObjectPtr<class USpringArmComponent> _spring_arm_component;

	UPROPERTY(EditAnywhere, Category="Camera")
	TObjectPtr<class UCameraComponent> _camera_component;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _static_mesh_component;

private:
	// helper function to set up Ability System Component; used by both client & server side
	void InitAbilityActorInfo();
};
