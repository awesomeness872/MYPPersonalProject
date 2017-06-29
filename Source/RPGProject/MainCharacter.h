// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "GameFramework/Character.h"
#include "PickupItem.h"
#include "MC_PlayerController.h"
#include "InventoryWidget.h"
#include "MainCharacter.generated.h"

#define MAX_INVENTORY_ITEMS 10

UCLASS()
class RPGPROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
    void Fire();

    UFUNCTION()
    void AimPressed();

    UFUNCTION()
    void AimReleased();

	UFUNCTION()
	void Reload();

    //function for when player died
    UFUNCTION()
    void Dead();

    //raycast in front of character to find usable items
    UFUNCTION()
    void RayCast();

    //reference to last seen pickup, nullptr if none
    UPROPERTY()
    APickupItem* LastSeenItem;

    //Handles the Pickup Input
    UFUNCTION()
    void PickupItem();

    UPROPERTY()
    bool bIsInventoryOpen = false;

    //The actual Inventory
    UPROPERTY(VisibleAnywhere)
    TArray<APickupItem*> Inventory;

    //reference to currently held item
    APickupItem* CurrentlyEquippedItem;

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

    //variable storing health
    UPROPERTY()
    float Health = 1;

    //vaiable storing stamina
    UPROPERTY()
    float Stamina = 1;

    //variable storing current ammo amount
    UPROPERTY()
    float CurrentAmmo = 25;

	//variable storing max ammo in clip
	UPROPERTY()
	float MaxAmmo = 25;

    //variable storing total ammo
    UPROPERTY()
    float TotalAmmo = 25;

	UPROPERTY()
	float DamagePerRound;

	UPROPERTY()
	float RateOfFire;

	UPROPERTY()
	bool bIsAutomaticWeapon = true;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	UInventoryWidget* InventoryRef;

    //functions for dev mode
    //function called when numpad 1 is pressed
    UFUNCTION()
    void Num1();

    //function called when numpad 2 is pressed
    UFUNCTION()
    void Num2();

    //function called when numpad 3 is pressed
    UFUNCTION()
    void Num3();

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

    //variable storing current ammo amount
    UFUNCTION(BlueprintCallable)
    float GetCurrentAmmo();

    //variable storing total ammo
    UFUNCTION(BlueprintCallable)
    float GetTotalAmmo();

    UFUNCTION(BlueprintCallable)
    FName GetLSIName();

    UFUNCTION(BlueprintCallable)
    FName GetLSIDesc();

    UFUNCTION(BlueprintCallable)
    UTexture2D* GetLSIImage();

	UFUNCTION(BlueprintCallable)
	APickupItem* GetLSI();

	UFUNCTION(BlueprintCallable)
	bool GetIsInventoryOpen();

	UFUNCTION(BlueprintCallable)
	bool GetIsAutomaticWeapon();

	UFUNCTION(BlueprintCallable)
	bool GetIsCrouching();

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetGunComp();

    //sets a new equipped item based on texture
    UFUNCTION(BlueprintCallable)
    void SetEquippedItem(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable)
	void SetCurrentlyEquippedItem(APickupItem* Item);

    UFUNCTION(BlueprintCallable)
    TArray<APickupItem*> GetInventory() { return Inventory; }

    UFUNCTION(BlueprintCallable)
    void DropEquippedItem();

	UFUNCTION(BlueprintCallable)
	void HandleInventoryInput();

	UFUNCTION(BlueprintCallable)
	void ItemUsed();

	UFUNCTION(BlueprintCallable)
	void SetMaxAmmo(float NewMaxAmmo);

	UFUNCTION(BlueprintCallable)
	void SetGunMesh(USkeletalMesh* NewGunMesh);

	UFUNCTION(BlueprintCallable)
	void SetDamagePerRound(float NewDPR);

	UFUNCTION(BlueprintCallable)
	void SetRateOfFire(float NewRateOfFire);

	UFUNCTION(BlueprintCallable)
	void SetIsAutomaticWeapon(bool bIsAutomatic);

};
