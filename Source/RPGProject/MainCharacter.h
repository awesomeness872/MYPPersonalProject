// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "GameFramework/Character.h"
#include "PickupItem.h"
#include "MC_PlayerController.h"
#include "InventoryWidget.h"
#include "EnemyCharacter.h"
#include "InteractableItem.h"
#include "SavedGame.h"
#include "MainCharacter.generated.h"

#define MAX_INVENTORY_ITEMS 10

UCLASS()
class RPGPROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

//set of variables for storing gun ammo
private:
	//variables for MG-45
	UPROPERTY()
		float CurrentAmmo_MG45;

	UPROPERTY()
		float TotalAmmo_MG45;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    //The range of the raycast for items
    UPROPERTY(EditAnywhere)
    float ItemRaycastRange = 500.f;

	//The range of the raycast for target
	UPROPERTY(EditAnywhere)
	float ShootRaycastRange = 10000.f;

private:

    //Camera
    UPROPERTY(VisibleAnywhere)
    UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	USkeletalMeshComponent* GunComp;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	USkeletalMesh* GunMesh;

	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* FireAnimMontage;

    //functions for movement inputs
    UFUNCTION()
    void MoveForward(float Value);

    UFUNCTION()
    void MoveRight(float Value);

    UFUNCTION()
    void SprintPressed();

    UFUNCTION()
    void SprintReleased();

    UFUNCTION()
    void WalkPressed();

    UFUNCTION()
    void WalkReleased();

    UFUNCTION()
    void JumpPressed();

    UFUNCTION()
    void JumpReleased();
	
	UFUNCTION()
	void CrouchPressed();

	UFUNCTION()
	void CrouchReleased();

    //functions for using gun
	UFUNCTION()
		void FirePressed();

	UFUNCTION()
		void FireReleased();

	UFUNCTION()
		void FireSingleReleased();

    UFUNCTION()
    void Fire();

    UFUNCTION()
    void AimPressed();

    UFUNCTION()
    void AimReleased();

	UFUNCTION()
	void ReloadPressed();

	UFUNCTION()
	void ReloadReleased();

    //function for when player died
    UFUNCTION()
    void Dead();

    //raycast in front of character to find usable items
    UFUNCTION()
    void RayCast();

    //Handles the Pickup Input
    UFUNCTION()
    void PickupItem();

	UFUNCTION()
		void PauseMenu();

	//function for melee attacking
	UFUNCTION()
		void Melee();

    UPROPERTY()
    bool bIsInventoryOpen = false;

    //The actual Inventory
    UPROPERTY(VisibleAnywhere)
    TArray<APickupItem*> Inventory;

	//array for pickup status to be saved
	TArray<bool> PickupStatus;

	//array for pickup classes to be saved
	TArray<APickupItem*> PickupClass;

    //reference to currently held item
	UPROPERTY()
		APickupItem* CurrentlyEquippedItem;

	//reference to interactable item being looked at
	UPROPERTY()
		AInteractableItem* CurrentInteraction;

	//reference to last seen pickup, nullptr if none
	UPROPERTY()
		APickupItem* LastSeenItem;

    //variable telling if player is moving
    UPROPERTY()
    bool bIsMoving;

    //variable telling if player is aiming down sights
    UPROPERTY()
    bool bIsAimingDownSights;

    //variable telling if player is jumping
    UPROPERTY()
    bool bIsJumping;

    //variable telling if player is sprinting
    UPROPERTY()
    bool bIsSprinting;

    //variable for if the dead function has been called
    UPROPERTY()
    bool bDeadCalled = false;

    //variable for if player is looking at an item that can be picked up
    UPROPERTY()
    bool bIsLookingAtPickup = false;

	//variable for if player is crouching
	UPROPERTY()
	bool bIsCrouching;

	UPROPERTY()
	bool bIsReloading;

	UPROPERTY()
		bool bIsFiring;

	UPROPERTY()
		bool bIsLookingAtInteractable = false;

	UPROPERTY()
		bool bCanMove = true;

    //variable storing health
    UPROPERTY()
    float Health = 1;

    //vaiable storing stamina
    UPROPERTY()
    float Stamina = 1;

	//variable for gun information
	UPROPERTY()
		FGunInformation CurrentGun;
	
	UPROPERTY()
		float MeleeRange = 50;

	UPROPERTY()
		float MeleeDamage = .2;

	UPROPERTY()
		float MeleeCooldown = 2;

	UPROPERTY()
		bool bIsMeleeAttacking = false;

	UPROPERTY()
	UInventoryWidget* InventoryRef;

	UPROPERTY()
	FTimerHandle ReloadTimer;

	UPROPERTY()
		FTimerHandle FireTimer;

	UPROPERTY()
		EPerspective Perspective = EPerspective::P_3P;

public:	 
    UFUNCTION(BlueprintCallable)
    bool GetIsAimingDownSights();

    UFUNCTION(BlueprintCallable)
    bool GetIsJumping();

    UFUNCTION(BlueprintCallable)
    bool GetDeadCalled();

    UFUNCTION(BlueprintCallable)
    bool GetIsLookingAtPickup();

    //variable storing health
    UFUNCTION(BlueprintCallable)
    float GetHealth();

    //vaiable storing stamina
    UFUNCTION(BlueprintCallable)
    float GetStamina();

	UFUNCTION(BlueprintCallable)
		FName GetInteractionAction();

    UFUNCTION(BlueprintCallable)
    FName GetLSIName();

    UFUNCTION(BlueprintCallable)
    FName GetLSIDesc();

	UFUNCTION(BlueprintCallable)
		bool GetLSIbIsInventoryPickup();

    UFUNCTION(BlueprintCallable)
    UTexture2D* GetLSIImage();

	UFUNCTION(BlueprintCallable)
	APickupItem* GetLSI();

	UFUNCTION(BlueprintCallable)
		AInteractableItem* GetCurrentInteraction();

	UFUNCTION(BlueprintCallable)
	bool GetIsInventoryOpen();

	UFUNCTION(BlueprintCallable)
	bool GetIsCrouching();

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetGunComp();

	UFUNCTION(BlueprintCallable)
	bool GetIsReloading();

	UFUNCTION(BlueprintCallable)
		bool GetIsFiring();

	UFUNCTION(BlueprintCallable)
		bool GetIsLookingAtInteractable();

	UFUNCTION(BlueprintCallable)
		bool GetCanMove();

	UFUNCTION(BlueprintCallable)
		TArray<APickupItem*> GetInventory();

	UFUNCTION(BlueprintCallable)
		EPerspective GetPerspective();

	UFUNCTION(BlueprintCallable)
		FGunInformation GetCurrentGun();

    //sets a new equipped item based on texture
    UFUNCTION(BlueprintCallable)
    void SetEquippedItem(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable)
	void SetCurrentlyEquippedItem(APickupItem* Item);

	UFUNCTION(BlueprintCallable)
		void SetCanMove(bool canMove);

	UFUNCTION(BlueprintCallable)
	void AddAmmo(float AmmoToAdd, EGunType GunType);

	UFUNCTION(BlueprintCallable)
	void DropEquippedItem();

	UFUNCTION(BlueprintCallable)
	void HandleInventoryInput();

	UFUNCTION(BlueprintCallable)
	void ItemUsed();

	UFUNCTION(BlueprintCallable)
		void SwitchGun(FGunInformation NewGun);

	UFUNCTION(BlueprintCallable)
		void SetPerspective(EPerspective NewPerspective);

	UFUNCTION(BlueprintCallable)
		void Damage(float Damage);

	UFUNCTION(BlueprintCallable)
		void SaveGame();

	UFUNCTION(BlueprintCallable)
		void LoadGame();
};
