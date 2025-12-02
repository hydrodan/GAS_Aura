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
	
protected:
	UPROPERTY(EditAnywhere, Category="Camera")
	TObjectPtr<class USpringArmComponent> _spring_arm_component;

	UPROPERTY(EditAnywhere, Category="Camera")
	TObjectPtr<class UCameraComponent> _camera_component;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> _static_mesh_component;
};
