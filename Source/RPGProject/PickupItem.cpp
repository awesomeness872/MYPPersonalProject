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

    PickupImage = CreateDefaultSubobject<UTexture2D>(FName("ItemTexture"));

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
