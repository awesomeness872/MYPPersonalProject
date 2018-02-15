// Copyright 2017-2018 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy_AIController.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	ET_Basic UMETA(DisplayName = "Basic Enemy"),
	ET_Zombie UMETA(DisplayName = "Zombie"),
};

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

	UPROPERTY(EditDefaultsOnly)
		float Health = 1;

	UPROPERTY(EditDefaultsOnly)
		float AttackDamage = .1;

	UPROPERTY(EditDefaultsOnly)
		float RateOfFire = .1;

	UFUNCTION(BlueprintCallable)
		virtual void Fire();

	UFUNCTION(BlueprintCallable)
		void FireCall();

	UFUNCTION(BlueprintCallable)
		void Dead();

	UFUNCTION(BlueprintCallable)
		void Damage(float Damage);

	UFUNCTION(BlueprintCallable)
		void SetDead(bool NewDead);

	UFUNCTION(BlueprintCallable)
		FVector GetPlayerLocation();

	UFUNCTION(BlueprintCallable)
		bool GetHasFoundPlayer();

	UFUNCTION(BlueprintCallable)
		bool GetDead();

	UFUNCTION(BlueprintCallable)
		float GetHealth();

	UFUNCTION(BlueprintCallable)
		void SetHealth(float NewHealth);

	UFUNCTION(BlueprintCallable)
		void SetAttackRange(float NewRange);

	UFUNCTION(BlueprintCallable)
		void SetAttackDamage(float NewDamage);

	UFUNCTION(BlueprintCallable)
		void SetEnemyType(EEnemyType NewType);

	UFUNCTION(BlueprintCallable)
		void Despawn();

	UPROPERTY(EditDefaultsOnly)
		EEnemyType EnemyType;

private:
	UPROPERTY(EditDefaultsOnly)
		USkeletalMeshComponent* GunComp;

	UPROPERTY()
		float AttackRaycastRange = 500.f;

	UPROPERTY()
		float PlayerRaycastRange = 100.0f;

	UPROPERTY()
		bool bIsDeadCalled;

	UPROPERTY()
		bool bHasFoundPlayer;

	UPROPERTY()
		FVector PlayerLocation;

	UPROPERTY()
		FTimerHandle DeadTimer;

	UPROPERTY()
		FTimerHandle FireTimer;
};
