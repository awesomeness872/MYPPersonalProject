// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

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

    //variable for item mesh
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* PickupMesh;

    //variable for item name
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName PickupName;

    //variable for item description
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName PickupDescription;

    //variable for action menu text
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName ActionText;

	//variable for it item should go to inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsInventoryPickup = true;

    UFUNCTION(BlueprintCallable)
    FName GetActionText();

    //variable for item picture
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UTexture2D* PickupImage;

    UFUNCTION()
    void SetGlowEffect(bool Status);

    //Returns the Texture of our Pickup
    FORCEINLINE UTexture2D* GetPickupTexture() { return PickupImage; }

	//set item image
	UFUNCTION(BlueprintCallable)
		void SetPickupImage(UTexture2D* NewPickupImage);

	UFUNCTION(BlueprintCallable)
		void SetPickupName(FName NewPickupName);

	UFUNCTION(BlueprintCallable)
		void SetPickupDescription(FName NewPickupDescription);

	UFUNCTION(BlueprintCallable)
		void SetActionText(FName NewActionText);
	
};
