// Copyright 2017 Paul Murray GPLv3

#include "RPGProject.h"
#include "GunActor.h"
#include "MainCharacter.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //create cameracomp
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));

    //attach to capsule
    CameraComp->SetupAttachment(GetCapsuleComponent());

    //allow pawn to control rotation
    CameraComp->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
    //display that this class is being used
    if (GEngine){
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Using MainCharacter"));
    }

    //initialize LastSeenItem reference
    LastSeenItem = nullptr;

    //initialize inventory
    Inventory.SetNum(MAX_INVENTORY_ITEMS);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //checks if player is sprinting
    if (bIsSprinting){
        //reduces stamina
        Stamina = Stamina - 0.001;

        //checks is stamina is low
        if (Stamina <= 0){
            //stops player sprinting
            SprintReleased();
        }
    }

    //if player isn't sprinting
    else if (!bIsSprinting && Stamina <= 1) {
        //increases stamina
        Stamina = Stamina + 0.0008;
    }

    //checks if health is 0
    if (Health <= 0 && !bDeadCalled){
        //calls dead function
        Dead();
    }

    //checks if Dead has been called
    if (bDeadCalled){
        //zooms out camera
        if (CameraComp->FieldOfView <= 140){
            CameraComp->FieldOfView = CameraComp->FieldOfView + .1;
        }
    }

    //raycast every frame for item
    RayCast();
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    //inputs for moving around
    PlayerInputComponent->BindAxis("Forward", this, &AMainCharacter::MoveForward);
    PlayerInputComponent->BindAxis("Right", this, &AMainCharacter::MoveRight);
    //inputs for looking around
    PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::AddControllerPitchInput);
    //inputs for jumping
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::JumpPressed);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::JumpReleased);
    //inputs for walking
    PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &AMainCharacter::WalkPressed);
    PlayerInputComponent->BindAction("Walk", IE_Released, this, &AMainCharacter::WalkReleased);
    //inputs for sprinting
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::SprintPressed);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::SprintReleased);
    //inputs for weapon control
    PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacter::AimPressed);
    PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMainCharacter::AimReleased);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::Fire);
    //input for inventory actions
    PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AMainCharacter::PickupItem);
    PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &AMainCharacter::DropEquippedItem);
    PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AMainCharacter::HandleInventoryInput);
    //dev mode inputs
    PlayerInputComponent->BindKey("NumPadOne", IE_Pressed, this, &AMainCharacter::Num1);
    PlayerInputComponent->BindKey("NumPadTwo", IE_Pressed, this, &AMainCharacter::Num2);
    PlayerInputComponent->BindKey("NumPadThree", IE_Pressed, this, &AMainCharacter::Num3);
}

//called when W or S is pressed, adds directional input
void AMainCharacter::MoveForward(float Value)
{
	if (bIsSprinting && Value <= 0) {
		
	}
	else if ((Controller != NULL) && (Value != 0.0f)) {
		bIsMoving = true;
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		// Limit pitch when walking or falling
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
    if (Value == 0){
        bIsMoving = false;
    }
}

//called when A or D is pressed, adds directional input
void AMainCharacter::MoveRight(float Value)
{
    if (!bIsSprinting){
        if ( (Controller != NULL) && (Value != 0.0f) )
        {
            bIsMoving = true;
            // find out which way is right
            const FRotator Rotation = Controller->GetControlRotation();
            const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
            // add movement in that direction
            AddMovementInput(Direction, Value);
        }
        if (Value == 0){
            bIsMoving = false;
        }
    }
}

//called when jump is pressed, starts player jump and sets jumping to true
void AMainCharacter::JumpPressed(){
    if (Stamina >= .1){
        //jump
        bPressedJump = true;

        //reduce stamina
        Stamina = Stamina - .1;
        //set value of bIsJumping
        bIsJumping = true;
    }
}

//called when jump is released, stops player jump and sets jumping to false
void AMainCharacter::JumpReleased(){
    //stop jump
    bPressedJump = false;

    //set value of bIsJumping
    bIsJumping = false;
}

//called when walk key is pressed, sets walk speed to 100 and prints "Walking"
void AMainCharacter::WalkPressed(){
    //sets speed to 265
    GetCharacterMovement()->MaxWalkSpeed = 265.0f;

    //sets value of bIsAimingDownSights
    bIsAimingDownSights = true;

    //changes value fo bIsSprinting
    bIsSprinting = false;

    //prints "Walkng" when GEngine is being used
    if (GEngine){
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Walking"));
    }
}

//called when walk key is released, sets walk speed to 600 and prints "Running"
void AMainCharacter::WalkReleased(){
    //sets speed to 595
    GetCharacterMovement()->MaxWalkSpeed = 595.0f;

    //sets value of bIsAimingDownSights
    bIsAimingDownSights = false;

    //prints "Running" when GEngine is being used
    if (GEngine){
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Running"));
    }
}

//called when sprint key is pressed, sets walk speed to 1000 and prints "Sprinting"
void AMainCharacter::SprintPressed(){
    //checks if stamina is high enough
    if (Stamina >= .01){
        //sets speed to 1000
        GetCharacterMovement()->MaxWalkSpeed = 1000.0f;

        //set value of bIsSprinting
        bIsSprinting = true;

        //zoom out
        CameraComp->SetRelativeLocation(FVector(-250.0f, 80.0f, 90.0f));

        //prints "Sprinting" when GEngine is being used
        if (GEngine){
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Sprinting"));
        }
    }
}

//called when sprint key is released, sets walk speed to 600 and prints "Running"
void AMainCharacter::SprintReleased(){
    //sets walk speed
    GetCharacterMovement()->MaxWalkSpeed = 595.0f;

    //set value of bIsSprinting
    bIsSprinting = false;

    //prints "Running" when GEngine is being used
    if (GEngine){
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Running"));
    }
}

//called when aim is pressed, slows speed and zooms in
void AMainCharacter::AimPressed(){
    if (!bIsInventoryOpen){
        //slow character
        WalkPressed();

        //zoom in
        CameraComp->SetRelativeLocation(FVector(-80.0f, 70.0f, 90.0f));

        //prints "Gun fired" if GEngine is being used
        if (GEngine){
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Aim pressed"));
        }
    }
}

//called when aim is released, restores speed and zooms out
void AMainCharacter::AimReleased(){
    //restore speed
    WalkReleased();

    //zoom out
    CameraComp->SetRelativeLocation(FVector(-250.0f, 80.0f, 90.0f));

    //prints "Gun fired" if GEngine is being used
    if (GEngine){
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Aim released"));
    }
}

//called when fire is pressed, fires weapon
void AMainCharacter::Fire(){
    //checks if there is enough ammo
    if (CurrentAmmo > 0){
        //reduce current ammo
        CurrentAmmo--;

		//raycast to see if something that can be hit is in range
		//calulate start and end location
		FVector StartLocation = CameraComp->GetComponentLocation();
		FVector EndLocation = StartLocation + (CameraComp->GetForwardVector() * ShootRaycastRange);

		FHitResult RaycastHit;

		//raycast should ignore character
		FCollisionQueryParams CQP;
		CQP.AddIgnoredActor(this);

		//raycast
		GetWorld()->LineTraceSingleByChannel(RaycastHit, StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, CQP);

		//draw line on raycast
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 0.5F, (uint8)'\000', 1.0F);

		if (RaycastHit.GetActor() != nullptr) {
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Object Hit"));
			}
		}

        //prints "Gun fired" if GEngine is being used
        if (GEngine){
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Gun fired"));
        }
    }
}

//called when Health is less than or equal to 0
void AMainCharacter::Dead(){
    //checks if GEngine is being used
    if (GEngine){
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Dead"));
    }

    //ragdoll
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	//disable input
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;

    //changes value of bDeadCalled
    bDeadCalled = true;
}

//raycast to find usable item
void AMainCharacter::RayCast(){
    //calulate start and end location
    FVector StartLocation = CameraComp->GetComponentLocation();
    FVector EndLocation = StartLocation + (CameraComp->GetForwardVector() * ItemRaycastRange);

    FHitResult RaycastHit;

    //raycast should ignore character
    FCollisionQueryParams CQP;
    CQP.AddIgnoredActor(this);

    //raycast
    GetWorld()->LineTraceSingleByChannel(RaycastHit, StartLocation, EndLocation, ECollisionChannel::ECC_WorldDynamic, CQP);

    APickupItem* Pickup = Cast<APickupItem>(RaycastHit.GetActor());

    if (LastSeenItem && LastSeenItem != Pickup){
        //if character sees a different pickup disable glowing effect on last character
        LastSeenItem->SetGlowEffect(false);
        bIsLookingAtPickup = false;
    }

    if (Pickup){
        //enable glowing effect on current item
        LastSeenItem = Pickup;
        Pickup->SetGlowEffect(true);
        bIsLookingAtPickup = true;
    }

    //reinitalize
    else LastSeenItem = nullptr;
}

//called when pickup is pressed, picks up item
void AMainCharacter::PickupItem(){
    if (LastSeenItem){
        //find first available slot
        int32 AvailableSlot = Inventory.Find(nullptr);

        if (AvailableSlot != INDEX_NONE){
            //add item to first valid slot
            Inventory[AvailableSlot] = LastSeenItem;

            //destroy item from game
			LastSeenItem->SetActorEnableCollision(false);
			LastSeenItem->PickupMesh->SetVisibility(false);

			if (bIsInventoryOpen) {
				InventoryRef->Show();
			}
        }
        else{
            if (GEngine){
                GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("Inventory full"));
            }
        }
    }
}

//called when inventory is pressed
void AMainCharacter::HandleInventoryInput(){
    if (bIsInventoryOpen){
        bIsInventoryOpen = false;
    }
    else bIsInventoryOpen = true;
    AMC_PlayerController* Con = Cast<AMC_PlayerController>(GetController());
    if (Con) Con->HandleInventoryInput();
}

void AMainCharacter::SetEquippedItem(UTexture2D * Texture){
    if (Texture){
        /*For this scenario we make the assumption that
        every pickup has a unique texture.
        So, in order to set the equipped item we just check every item
        inside our Inventory. Once we find an item that has the same image as the
        Texture image we're passing as a parameter we mark that item as CurrentlyEquipped.*/
        for (auto It = Inventory.CreateIterator(); It; It++){
            if ((*It) && (*It)->GetPickupTexture() && (*It)->GetPickupTexture()->HasSameSourceArt(Texture)){
                CurrentlyEquippedItem = *It;
                if (GEngine){
                    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("New equipped item"));
                }
                break;
            }
        }
    }
    else if (GEngine){
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("Empty slot"));
    }
}

void AMainCharacter::DropEquippedItem(){
    if (CurrentlyEquippedItem){
        int32 IndexOfItem;
        if (Inventory.Find(CurrentlyEquippedItem, IndexOfItem)){
            //the location of drop
            FVector DropLocation = GetActorLocation() + (GetActorForwardVector() *200);

            //making a transform with default rotation and scale just setting up the location that was calculated above
            FTransform Transform;
            Transform.SetLocation(DropLocation);

            //default actor spawn parameters
            FActorSpawnParameters SpawnParams;

            //spawning our pickup
            APickupItem* PickupToSpawn = GetWorld()->SpawnActor<APickupItem>(CurrentlyEquippedItem->GetClass(), Transform, SpawnParams);

			//delete old actor
			CurrentlyEquippedItem->Destroy();

            if (PickupToSpawn){
                //unreference item just placed
                Inventory[IndexOfItem] = nullptr;
            }
        }
    }
}

//called when item is used from inventory, removes from inventory and world
void AMainCharacter::ItemUsed() {
	if (CurrentlyEquippedItem) {
		int32 IndexOfItem;
		if (Inventory.Find(CurrentlyEquippedItem, IndexOfItem)) {
			//destroy item in world
			CurrentlyEquippedItem->Destroy();

			//unreference item just used
			Inventory[IndexOfItem] = nullptr;
		}
	}
}

//called manually, returns  value of bIsAimingDownSights
bool AMainCharacter::GetIsAimingDownSights(){
    return bIsAimingDownSights;
}

//called manually, returns value of bIsJumping
bool AMainCharacter::GetIsJumping(){
    return bIsJumping;
}

//called manually, returns value of bDeadCalled
bool AMainCharacter::GetDeadCalled(){
    return bDeadCalled;
}

//called manually, returns value of CurrentAmmo
float AMainCharacter::GetCurrentAmmo(){
    return CurrentAmmo;
}

//called manually, returns value of TotalAmmo
float AMainCharacter::GetTotalAmmo(){
    return MaxAmmo;
}

//called manually, returns value of Health
float AMainCharacter::GetHealth(){
    return Health;
}

//called manually, returns value of Stamina
float AMainCharacter::GetStamina(){
    return Stamina;
}

//called manually, returns value of bIsLookingAtPickup
bool AMainCharacter::GetIsLookingAtPickup(){
    return bIsLookingAtPickup;
}

//called manually, if LastSeenItem is valid returns value of LastSeenIten->PickupName, if not returns empty
FName AMainCharacter::GetLSIName(){
    if (LastSeenItem){
        return LastSeenItem->PickupName;
    }
    else{
        FName none;
        return none;
    }
}

//called manually, if LastSeenItem is valid returns value of LastSeenIten->PickupDescription, if not returns empty
FName AMainCharacter::GetLSIDesc(){
    if (LastSeenItem){
        return LastSeenItem->PickupDescription;
    }
    else{
        FName none;
        return none;
    }
}

//called manually, if LastSeenItem is valid returns value of LastSeenIten->PickupImage, if not returns empty
UTexture2D* AMainCharacter::GetLSIImage(){
    if (LastSeenItem){
        return LastSeenItem->PickupImage;
    }
    else {
        return nullptr;
    }
}

APickupItem* AMainCharacter::GetLSI() {
	if (LastSeenItem) {
		return LastSeenItem;
	}
	else {
		return nullptr;
	}
}

bool AMainCharacter::GetIsInventoryOpen() {
	return bIsInventoryOpen;
}

bool AMainCharacter::GetIsAutomaticWeapon() {
	return bIsAutomaticWeapon;
}

void AMainCharacter::SetCurrentlyEquippedItem(APickupItem* Item) {
	CurrentlyEquippedItem = Item;
}

void AMainCharacter::SetMaxAmmo(float NewMaxAmmo) {
	MaxAmmo = NewMaxAmmo;
}

void AMainCharacter::SetGunMesh(UStaticMesh* NewGunMesh) {
	GunMesh = NewGunMesh;
}

void  AMainCharacter::SetDamagePerRound(float NewDPR) {
	DamagePerRound = NewDPR;

}

void  AMainCharacter::SetRateOfFire(float NewRateOfFire) {
	RateOfFire = NewRateOfFire;
}

void AMainCharacter::SetIsAutomaticWeapon(bool bIsAutomatic) {
	bIsAutomaticWeapon = bIsAutomatic;
}

//called when numpad 1 is pressed
void AMainCharacter::Num1(){
    //set current ammo equal to total ammo
    CurrentAmmo = MaxAmmo;
}

//called when numpad 2 is pressed
void AMainCharacter::Num2(){
    //refil stamina
    Stamina = 1;
}

//called when numpad 3 is pressed
void AMainCharacter::Num3(){
    //set health to 0
    Health = 0;
}
