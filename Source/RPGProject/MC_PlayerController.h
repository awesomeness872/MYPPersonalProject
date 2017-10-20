// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryWidget.h"
#include "SavedGame.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget> wUMG_OpeningPlayer;

	UPROPERTY()
		UUserWidget* UMG_OpeningPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget> wUMG_PauseMenu;

	UPROPERTY()
		UUserWidget* UMG_PauseMenu;

	UPROPERTY(EditDefaultsOnly)
		bool bOpening = true;

    //opens or closes the inventory
    void HandleInventoryInput();

    virtual void Possess(APawn* InPawn) override;

    //override BeginPlay()
    virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void PauseGame();

	UFUNCTION(BlueprintCallable)
		void PlayOpening();

	UFUNCTION(BlueprintCallable)
		bool GetbOpening();

	UFUNCTION(BlueprintCallable)
		void SetOpening(bool Opening);
};
