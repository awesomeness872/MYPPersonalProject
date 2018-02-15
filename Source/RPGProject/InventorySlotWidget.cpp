// Copyright 2017-2018 Paul Murray GPLv3

#include "RPGProject.h"
#include "MainCharacter.h"
#include "InventorySlotWidget.h"

void UInventorySlotWidget::SetItemTexture(APickupItem *Item){
    //if item is calid update widget texture, if not assign a nullptr
    (Item) ? ItemImage = Item->GetPickupInfo().PickupImage : ItemImage = nullptr;
}

void UInventorySlotWidget::SetEquippedItem(){
    AMainCharacter* Char = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (Char){
        Char->SetEquippedItem(ItemImage);
    }
}

void UInventorySlotWidget::SetActionText(APickupItem *Item){
    //if item is calid update widget texture, if not assign a nullptr
    FName none;
    (Item) ? ActionText = Item->GetPickupInfo().ActionText : ActionText = none;
}
