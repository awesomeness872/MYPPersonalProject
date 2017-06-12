// Copyright 2017 Paul Murray GPLv3

#include "RPGProject.h"
#include "MainCharacter.h"
#include "MC_PlayerController.h"

void AMC_PlayerController::BeginPlay(){

    Super::BeginPlay();

    //check if asset is assigned iin blueprint
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

    //inital value
    bIsInventoryOpen = false;
}

void AMC_PlayerController::HandleInventoryInput(){
    AMainCharacter* Char = Cast<AMainCharacter>(GetPawn());

    if (InventoryWidgetRef){
        if (bIsInventoryOpen){     
            //unpause game
            SetPause(false);

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
            //pause game
            SetPause(true);

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
