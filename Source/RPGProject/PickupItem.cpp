// Copyright 2017-2018 Paul Murray GPLv3

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
}

void APickupItem::UseItem_Implementation() {}

FPickupInformation APickupItem::GetPickupInfo() {
	return PickupInfo;
}

FArmorInformation APickupItem::GetArmorInfo() {
	return ArmorInfo;
}

FGunInformation APickupItem::GetGunInfo() {
	return GunInfo;
}

void APickupItem::SetPickupInfo(FPickupInformation NewInfo) {
	PickupInfo = NewInfo;
}

void APickupItem::SetPickedUp(bool NewPickedUp) {
	PickupInfo.bPickedUp = NewPickedUp;
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