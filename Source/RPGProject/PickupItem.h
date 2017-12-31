// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EGunType.h"
#include "PickupItem.generated.h"

USTRUCT(BlueprintType)
struct FPickupInformation {
	GENERATED_BODY()

	//variable for item name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName PickupName;

	//variable for item description
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName PickupDescription;

	//variable for action menu text
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ActionText;

	//variable for it item should go to inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsInventoryPickup = true;

	//variable for if it got picked up
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPickedUp = false;

	//variable for if the pickup is a gun
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsGun = false;

	//variable for item picture
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* PickupImage;
};

UCLASS()
class RPGPROJECT_API APickupItem : public AActor
{
    GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    APickupItem();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UseItem();

	UFUNCTION()
		void UseItem_Implementation();

    UFUNCTION()
    void SetGlowEffect(bool Status);

	UPROPERTY(EditDefaultsOnly)
		FPickupInformation PickupInfo;

	UPROPERTY(EditAnywhere)
		FGunInformation GunInfo;

	//variable for item mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* PickupMesh;

	//returns struct
	UFUNCTION(BlueprintCallable)
		FPickupInformation GetPickupInfo();

	//sets PickupInfo
	UFUNCTION(BlueprintCallable)
		void SetPickupInfo(FPickupInformation NewInfo);

	//sets status of pickedup
	UFUNCTION(BlueprintCallable)
		void SetPickedUp(bool NewPickedUp);
};
