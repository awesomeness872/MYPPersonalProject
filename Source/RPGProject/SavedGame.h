// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SavedGame.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USavedGame : public USaveGame
{
	GENERATED_BODY()

public: 
	UPROPERTY(VisibleAnywhere, Category = MC_PlayerController)
		bool bOpening;

	UPROPERTY(VisibleAnywhere, Category = SaveSlotData)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = SaveSlotData)
		uint32 UserIndex;

	USavedGame();
	
	
};
