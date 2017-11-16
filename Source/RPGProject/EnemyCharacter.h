// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy_AIController.h"
#include "EGunType.h"
#include "SavedGame.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class RPGPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void Fire();

	UFUNCTION(BlueprintCallable)
		void Dead();

	UFUNCTION(BlueprintCallable)
		void Damage(float Damage);

	UFUNCTION(BlueprintCallable)
		FVector GetPlayerLocation();

	UFUNCTION(BlueprintCallable)
		bool GetHasFoundPlayer();

	//variable storing current ammo amount
	UPROPERTY(EditDefaultsOnly)
		float CurrentAmmo = 0;

	//variable storing max ammo in clip
	UPROPERTY(EditDefaultsOnly)
		float MaxAmmo = 0;

	//variableEditDefaultsOnly storing total ammo
	UPROPERTY()
		float TotalAmmo = 0;

	UPROPERTY(EditDefaultsOnly)
		float DamagePerRound;

	UPROPERTY(EditDefaultsOnly)
		float RateOfFire;

	UPROPERTY(EditDefaultsOnly)
		bool bIsAutomaticWeapon = false;

	UPROPERTY(EditDefaultsOnly)
		EGunType CurrentGunType;

private:
	UPROPERTY(EditDefaultsOnly)
		USkeletalMeshComponent* GunComp;

	UPROPERTY(EditAnywhere)
		UBehaviorTree* ChaseTree;

	UPROPERTY()
		float Health = 1;

	UPROPERTY()
		float ShootRaycastRange = 500.f;

	UPROPERTY()
		float PlayerRaycastRange = 100.0f;

	UPROPERTY()
		bool bIsDeadCalled;

	UPROPERTY()
		bool bHasFoundPlayer;

	UPROPERTY()
		FVector PlayerLocation;
};
