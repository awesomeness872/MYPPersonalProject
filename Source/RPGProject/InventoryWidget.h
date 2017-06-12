// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupItem.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    //add widget into viewport and populates inventory slots
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = UI)
    void Show();

    //remove widget from viewport
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = UI)
    void Hide();

    //stores a reference in order to bind information on inventory slots
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<APickupItem*> ItemsArray;
};
