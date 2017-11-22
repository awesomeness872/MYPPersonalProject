// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "PickupItem.h"
#include "SavedGame.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USavedGame : public USaveGame
{
	GENERATED_BODY()

public: 
	//should opening play
	UPROPERTY(VisibleAnywhere, Category = MC_PlayerController)
		bool bOpening;

	//player inventory
	UPROPERTY(VisibleAnywhere, Category = MainCharacter)
		TArray<APickupItem*> Inventory;

	UPROPERTY(VisibleAnywhere, Category = MainCharacter)
		float PlayerHealth = 1.0;

	UPROPERTY(VisibleAnywhere, Category = MainCharacter)
		float PlayerStamina = 1.0;

	//player location
	UPROPERTY(VisibleAnywhere, Category = MainCharacter)
		FVector PlayerLocation = FVector(-860.f, -100.f, 602.f);

	UPROPERTY(VisibleAnywhere, Category = SaveSlotData)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = SaveSlotData)
		uint32 UserIndex;

	//set properties
	USavedGame();
	

	
};