// Copyright 2017-2018 Paul Murray GPLv3

#pragma once

#include "GameFramework/GameModeBase.h"
#include "RPGProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API ARPGProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
    protected:
        // Called when the game starts or when spawned
        virtual void BeginPlay() override;
	
	
};
