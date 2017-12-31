// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "EC_Zombie.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AEC_Zombie : public AEnemyCharacter
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//variable for attack range
	UPROPERTY(EditDefaultsOnly)
		float MeleeRange = 100;

	//variable for attack damage
	UPROPERTY(EditDefaultsOnly)
		float MeleeDamage = .2;
};
