// Copyright 2017-2018 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryWidget.h"
#include "SavedGame.h"
#include "DialogueStruct.h"
#include "DialogueWidget.h"
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

	//inventorywidget reference
	UInventoryWidget* GunInventoryWidgetRef;

	//armorinventorywidget reference
	UInventoryWidget* ArmorInventoryWidgetRef;

    //true if inventory is open, false if otherwise
    bool bIsInventoryOpen = false;

	bool bIsGunInventoryOpen = false;

	bool bIsArmorInventoryOpen = false;

	bool bIsDialogueOpen = false;

private:
    //inventorywidget blueprint reference
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UInventoryWidget> InventoryWidgetBP;

	//guninventorywidget blueprint reference
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UInventoryWidget> GunInventoryWidgetBP;

	//armorinventorywidget blueprint reference
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UInventoryWidget> ArmorInventoryWidgetBP;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<class UDialogueWidget> wUMG_Dialogue;

	UPROPERTY()
		UDialogueWidget* UMG_Dialogue;

	UPROPERTY(EditDefaultsOnly)
		bool bOpening = false;

    //opens or closes the inventory
    void HandleInventoryInput();

	//opens or closes the inventory
	void HandleGunInventoryInput();

	//opens or closes the armor inventory
	void HandleArmorInventoryInput();

	//opens or closes DialogueWidget
	void ToggleDialogue(FDialogueInformation DialogueInfo);

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
