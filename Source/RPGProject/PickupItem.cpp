// Copyright 2017 Paul Murray GPLv3

#include "RPGProject.h"
#include "PickupItem.h"


// Sets default values
APickupItem::APickupItem()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //initialize proerties
    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PickupMesh"));

    //setup PickupMesh as root component
    RootComponent = PickupMesh;
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//sets glow effect around item
void APickupItem::SetGlowEffect(bool Status)
{
    PickupMesh->SetRenderCustomDepth(Status);
}

FName APickupItem::GetActionText(){
    return ActionText;
}

bool APickupItem::GetPickedUp() {
	return bPickedUp;
}
void APickupItem::UseItem_Implementation()
{
}

void APickupItem::SetPickupImage(UTexture2D* NewPickupImage) {
	PickupImage = NewPickupImage;
}

void APickupItem::SetPickupName(FName NewPickupName) {
	PickupName = NewPickupName;
}

void APickupItem::SetPickupDescription(FName NewPickupDescription) {
	PickupDescription = NewPickupDescription;
}

void APickupItem::SetActionText(FName NewActionText) {
	ActionText = NewActionText;
}

void APickupItem::SetPickedUp(bool NewPickedUp) {
	bPickedUp = NewPickedUp;
	if (NewPickedUp) {
		PickupMesh->SetEnableGravity(false);
		SetActorEnableCollision(false);
		PickupMesh->SetVisibility(false);
	}
	else {
		PickupMesh->SetEnableGravity(true);
		SetActorEnableCollision(true);
		PickupMesh->SetVisibility(true);
	}
}