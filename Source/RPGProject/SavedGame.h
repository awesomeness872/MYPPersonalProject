// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "PickupItem.h"
#include "EnemyCharacter.h"
#include "SavedGame.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EPerspective : uint8
{
	P_1P UMETA(DisplayName = "First Person"),
	P_3P UMETA(DisplayName = "Third Person"),
};

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

	UPROPERTY(VisibleAnywhere, Category = MainCharacter)
		FGunInformation PlayerCurrentGun;

	//player location
	UPROPERTY(VisibleAnywhere, Category = MainCharacter)
		FVector PlayerLocation = FVector(-860.f, -100.f, 602.f);

	//player perspective
	UPROPERTY(VisibleAnywhere, Category = MainCharacter)
		EPerspective PlayerPerspective;

	//enemy health
	UPROPERTY(VisibleAnywhere, Category = EnemyCharacter)
		TArray<float> EnemyHealths;

	//enemy transform
	UPROPERTY(VisibleAnywhere, Category = EnemyCharacter)
		TArray<FTransform> EnemyTransform;

	//enemy dead status
	UPROPERTY(VisibleAnywhere, Category = EnemyCharacter)
		TArray<bool> EnemyDead;
	//enemy class
	UPROPERTY(VisibleAnywhere, Category = EnemyCharacter)
		TArray<AEnemyCharacter*> EnemyClass;

	//pickup status
	UPROPERTY(VisibleAnywhere, Category = PickupItem)
		TArray<bool> PickupStatus;

	UPROPERTY(VisibleAnywhere, Category = PickupItem)
		TArray<FTransform> PickupLocation;

	//pickup class
	UPROPERTY(VisibleAnywhere, Category = PickupItem)
		TArray<APickupItem*> PickupClass;
	
	UPROPERTY(VisibleAnywhere, Category = SaveSlotData)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = SaveSlotData)
		uint32 UserIndex;

	//set properties
	USavedGame();
	

	
};