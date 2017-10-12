// Copyright 2017 Paul Murray GPLv3

#include "RPGProject.h"
#include "InteractableItem.h"


// Sets default values
AInteractableItem::AInteractableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FName AInteractableItem::GetActionText() {
	return ActionText;
}

void AInteractableItem::Activate_Implementation() {
}