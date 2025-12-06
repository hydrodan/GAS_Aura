// Copyright Dan Wilshere

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h" // this must be last

// integer literal for edge highlighting, 250 is red
#define CUSTOM_DEPTH_RED 250

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:

	AAuraEnemy();

	/** Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;
	/** End Enemy Interface */

	virtual void Tick(float DeltaTime) override;
	
	//UPROPERTY(BlueprintReadOnly)
	//bool bHighlighted = false;

protected:
	virtual void BeginPlay() override;
};
