// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryWidget.h"
#include "MC_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AMC_PlayerController : public APlayerController
{
	GENERATED_BODY()

private:
    //inventorywidget reference
    UInventoryWidget* InventoryWidgetRef;

    //true if inventory is open, false if otherwise
    bool bIsInventoryOpen;

private:
    //inventorywidger blueprint reference
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UInventoryWidget> InventoryWidgetBP;
public:

    //properties for calling UMG HUD
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
    TSubclassOf<class UUserWidget> wUMG_HUD;

    UPROPERTY()
    UUserWidget* UMG_HUD;

    //opens or closes the inventory
    void HandleInventoryInput();

    virtual void Possess(APawn* InPawn) override;

    //override BeginPlay()
    virtual void BeginPlay() override;
	
	
};
