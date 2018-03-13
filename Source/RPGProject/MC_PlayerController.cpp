// Copyright 2017-2018 Paul Murray GPLv3

#include "RPGProject.h"
#include "MainCharacter.h"
#include "MC_PlayerController.h"

void AMC_PlayerController::BeginPlay(){

    Super::BeginPlay();

    //check if asset is assigned in blueprint
    if (wUMG_HUD){
        //create widget and store it
        UMG_HUD = CreateWidget<UUserWidget>(this, wUMG_HUD);

        //make sure pointer holds widget
        if (UMG_HUD){
            //add to viewport
            UMG_HUD->AddToViewport();
        }
    }
}

void AMC_PlayerController::Possess(APawn *InPawn){
    Super::Possess(InPawn);

    if (InventoryWidgetBP){
        //create inventory widget based on blueprint reference
        InventoryWidgetRef = CreateWidget<UInventoryWidget>(this, InventoryWidgetBP);
    }
	if (GunInventoryWidgetBP) {
		GunInventoryWidgetRef = CreateWidget<UInventoryWidget>(this, GunInventoryWidgetBP);
	}
	if (ArmorInventoryWidgetBP) {
		ArmorInventoryWidgetRef = CreateWidget<UInventoryWidget>(this, ArmorInventoryWidgetBP);
	}
}

void AMC_PlayerController::HandleInventoryInput(){
    AMainCharacter* Char = Cast<AMainCharacter>(GetPawn());

    if (InventoryWidgetRef){
        if (bIsInventoryOpen){    
            //mark the inventory as closed
            bIsInventoryOpen = false;

            //hide cursor
            bShowMouseCursor = false;

            //tell game that we want to register inputs for our game and not for our UI
            FInputModeGameOnly InputMode;
            SetInputMode(InputMode);

            //remove from viewport
            InventoryWidgetRef->RemoveFromViewport();
        }
        else {
            //mark inventory as open
            bIsInventoryOpen = true;

            //repopulate ItemsArray
            InventoryWidgetRef->ItemsArray = Char->GetInventory();

            //show inventory
            InventoryWidgetRef->Show();

            //show cursor
            bShowMouseCursor = true;

            //tell game that we want register inputs in game and UI
            FInputModeGameAndUI InputMode;
            SetInputMode(InputMode);
        }
    }
}

void AMC_PlayerController::HandleGunInventoryInput() {
	AMainCharacter* Char = Cast<AMainCharacter>(GetPawn());

	if (GunInventoryWidgetRef) {
		if (bIsGunInventoryOpen) {
			//mark the inventory as closed
			bIsGunInventoryOpen = false;

			//hide cursor
			bShowMouseCursor = false;

			//tell game that we want to register inputs for our game and not for our UI
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);

			//remove from viewport
			GunInventoryWidgetRef->RemoveFromViewport();
		}
		else {
			//mark inventory as open
			bIsGunInventoryOpen = true;

			//repopulate ItemsArray
			GunInventoryWidgetRef->ItemsArray = Char->GetGunInventory();

			//show inventory
			GunInventoryWidgetRef->Show();

			//show cursor
			bShowMouseCursor = true;

			//tell game that we want register inputs in game and UI
			FInputModeGameAndUI InputMode;
			SetInputMode(InputMode);
		}
	}
}

void AMC_PlayerController::HandleArmorInventoryInput() {
	AMainCharacter* Char = Cast<AMainCharacter>(GetPawn());

	if (ArmorInventoryWidgetRef) {
		if (bIsArmorInventoryOpen) {
			//mark the inventory as closed
			bIsArmorInventoryOpen = false;

			//hide cursor
			bShowMouseCursor = false;

			//tell game that we want to register inputs for our game and not for our UI
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);

			//remove from viewport
			ArmorInventoryWidgetRef->RemoveFromViewport();
		}
		else {
			//mark inventory as open
			bIsArmorInventoryOpen = true;

			//repopulate ItemsArray
			ArmorInventoryWidgetRef->ItemsArray = Char->GetArmorInventory();

			//show inventory
			ArmorInventoryWidgetRef->Show();

			//show cursor
			bShowMouseCursor = true;

			//tell game that we want register inputs in game and UI
			FInputModeGameAndUI InputMode;
			SetInputMode(InputMode);
		}
	}
}

void AMC_PlayerController::ToggleDialogue(FDialogueInformation DialogueInfo) {
	//check if set in blueprint
	if (wUMG_Dialogue) {
		//create widget
		UMG_Dialogue = CreateWidget<UDialogueWidget>(this, wUMG_Dialogue);
		//check if holding widget
		if (UMG_Dialogue) {
			//check if dialogue is open, if not -> open it and set dialogue information, if open -> close it
			if (bIsDialogueOpen == false) {
				UMG_Dialogue->AddToViewport();
				UMG_Dialogue->UpdateDialogueOutputFromPlayer(DialogueInfo);
			}
			else {
				UMG_Dialogue->RemoveFromViewport();
			}
		}
	}
}

void AMC_PlayerController::PauseGame() {
	//check if asset is assigned in blueprint
	if (wUMG_PauseMenu) {
		//create widget and store it
		UMG_PauseMenu = CreateWidget<UUserWidget>(this, wUMG_PauseMenu);
		//make sure pointer holds widget
		if (UMG_PauseMenu) {
			//add to viewport
			UMG_PauseMenu->AddToViewport();
			//show cursor
			bShowMouseCursor = true;
			//tell game that we want register inputs in UI
			FInputModeUIOnly InputMode;
			SetInputMode(InputMode);
			//pause game
			SetPause(true);
		}
	}
}

void AMC_PlayerController::PlayOpening() {
	if (bOpening) {
		if (wUMG_OpeningPlayer) {
			//create widget and store as variable
			UMG_OpeningPlayer = CreateWidget<UUserWidget>(this, wUMG_OpeningPlayer);
			//check is pointer holds widget
			if (UMG_OpeningPlayer) {
				//add to viewport
				UMG_OpeningPlayer->AddToViewport();
			}
		}
		bOpening = false;
	}
}

bool AMC_PlayerController::GetbOpening() {
	return bOpening;
}

void AMC_PlayerController::SetOpening(bool Opening) {
	bOpening = Opening;
}