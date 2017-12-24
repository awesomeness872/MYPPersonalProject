// Copyright 2017 Paul Murray GPLv3

#include "RPGProject.h"
#include "MainCharacter.h"
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

	//set GunComp to socket
	//GunComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), FName("WeaponSocket"));
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
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::Fire() {
	if (!bIsDeadCalled) {
		//raycast to see if something that can be hit is in range
		//calulate start and end location
		FVector StartLocation = GetMesh()->GetBoneLocation(FName("thumb_03_l"));
		FVector EndLocation = StartLocation + (GunComp->GetForwardVector() * ShootRaycastRange);

		FHitResult RaycastHit;

		//raycast should ignore character
		FCollisionQueryParams CQP;
		CQP.AddIgnoredActor(this);

		//raycast
		GetWorld()->LineTraceSingleByChannel(RaycastHit, StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, CQP);

		//draw line
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 5.F, (uint8)'\000', 1.0F);

		if (RaycastHit.GetActor() != nullptr) {
			AMainCharacter* MC = Cast <AMainCharacter>(RaycastHit.GetActor());
			MC->Damage(.1f);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("Firing"));
		}
	}
}

void AEnemyCharacter::Dead() {
	//ragdoll
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	//change value of bDeadCalled
	bIsDeadCalled = true;

	GetWorld()->GetTimerManager().SetTimer(DeadTimer, this, &AEnemyCharacter::Despawn, 20.0f, false);
}

void AEnemyCharacter::Despawn() {
	Destroy();
}

void AEnemyCharacter::Damage(float Damage) {
	Health = Health - Damage;
}

FVector AEnemyCharacter::GetPlayerLocation() {
	return PlayerLocation;
}

bool AEnemyCharacter::GetHasFoundPlayer() {
	return bHasFoundPlayer;
}

float AEnemyCharacter::GetHealth() {
	return Health;
}

void AEnemyCharacter::SetHealth(float NewHealth) {
	Health = NewHealth;
}

bool AEnemyCharacter::GetDead() {
	return bIsDeadCalled;
}

void AEnemyCharacter::SetDead(bool NewDead) {
	bIsDeadCalled = NewDead;
}
