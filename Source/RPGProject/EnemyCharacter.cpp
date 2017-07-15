// Copyright 2017 Paul Murray GPLv3

#include "RPGProject.h"
#include "EnemyCharacter.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Component"));

	GetMesh()->SetCollisionProfileName(TEXT("CanBeShot"));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//checks if health is 0
	if (Health <= 0 && !bIsDeadCalled) {
		//calls dead function
		Dead();
	}
	//see if player is in view
	CastForPlayer();
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::Fire() {
	//raycast to see if something that can be hit is in range
	//calulate start and end location
	FVector StartLocation = GunComp->GetComponentLocation();
	FVector EndLocation = StartLocation + (GunComp->GetRightVector() * ShootRaycastRange);

	FHitResult RaycastHit;

	//raycast should ignore character
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	//raycast
	GetWorld()->LineTraceSingleByChannel(RaycastHit, StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, CQP);

	//draw line on raycast
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 0.5F, (uint8)'\000', 1.0F);
}

void AEnemyCharacter::Dead() {
	//ragdoll
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	//changes value of bDeadCalled
	bIsDeadCalled = true;
}

void AEnemyCharacter::Damage(float Damage) {
	Health = Health - Damage;
}

void AEnemyCharacter::CastForPlyaer() {

}