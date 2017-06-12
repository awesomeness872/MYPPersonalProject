// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupItem.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
    //holds reference to item texture
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UTexture2D* ItemImage;

    //holds reference to action text
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FName ActionText;

    //tells player to equip the represented item from this widget
    UFUNCTION(BlueprintCallable, Category = UI)
    void SetEquippedItem();

public:
    //sets item texture
    UFUNCTION(BlueprintCallable, Category = UI)
    void SetItemTexture(APickupItem* Item);

    //sets item texture
    UFUNCTION(BlueprintCallable, Category = UI)
    void SetActionText(APickupItem* Item);
	
};
