// Copyright 2017-2018 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemy_AIController.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AEnemy_AIController : public AAIController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		UBehaviorTree* Enemy_BehaviorTree;
public:
	AEnemy_AIController(const FObjectInitializer& ObjectInitiliazer);
	
	virtual void BeginPlay() override;
	
};
