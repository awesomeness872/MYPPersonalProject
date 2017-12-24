// Copyright 2017 Paul Murray GPLv3

#include "RPGProject.h"
#include "MainCharacter.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //create cameracomp
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));

    //allow pawn to control rotation
    CameraComp->bUsePawnControlRotation = true;

	//initialize GunComp
	GunComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Component"));

	//setup collision type
	GetMesh()->SetCollisionProfileName(TEXT("Player"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	//load game
	LoadGame();

    //display that this class is being used
    if (GEngine){
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Using MainCharacter"));
    }

    //initialize LastSeenItem reference
    LastSeenItem = nullptr;

    //initialize inventory
    Inventory.SetNum(MAX_INVENTORY_ITEMS);

	//set GunComp to socket
	GunComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), FName("WeaponSocket"));

	//set gun type to none
	CurrentGunType = EGunType::GT_None;

	//set camera and mesh for perspective
	SetPerspective(GetPerspective());
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
	//inputs for crouching
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::CrouchPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMainCharacter::CrouchReleased);
    //inputs for weapon control
    PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacter::AimPressed);
    PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMainCharacter::AimReleased);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::FirePressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMainCharacter::FireReleased);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMainCharacter::ReloadPressed);
	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &AMainCharacter::Melee);
    //input for inventory actions
    PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AMainCharacter::PickupItem);
    PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &AMainCharacter::DropEquippedItem);
    PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AMainCharacter::HandleInventoryInput);
	//input for pause menu
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AMainCharacter::PauseMenu);
	//button to save game for testing
	PlayerInputComponent->BindKey("NumPadOne", IE_Pressed, this, &AMainCharacter::SaveGame);
}

//called when W or S is pressed, adds directional input
void AMainCharacter::MoveForward(float Value)
{
	if (bCanMove) {
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
		if (Value == 0) {
			bIsMoving = false;
		}
	}
}

//called when A or D is pressed, adds directional input
void AMainCharacter::MoveRight(float Value)
{
	if (bCanMove) {
		if (!bIsSprinting) {
			if ((Controller != NULL) && (Value != 0.0f))
			{
				bIsMoving = true;
				// find out which way is right
				const FRotator Rotation = Controller->GetControlRotation();
				const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
				// add movement in that direction
				AddMovementInput(Direction, Value);
			}
			if (Value == 0) {
				bIsMoving = false;
			}
		}
	}
}

//called when jump is pressed, starts player jump and sets jumping to true
void AMainCharacter::JumpPressed(){
	if (!bIsCrouching && !bIsAimingDownSights && bCanMove){
		if (Stamina >= .1) {
			//jump
			bPressedJump = true;

			//reduce stamina
			Stamina = Stamina - .1;
			//set value of bIsJumping
			bIsJumping = true;
		}
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
	if (!bIsCrouching) {
		//sets speed to 265
		GetCharacterMovement()->MaxWalkSpeed = 265.0f;

		//sets value of bIsAimingDownSights
		bIsAimingDownSights = true;

		//changes value fo bIsSprinting
		bIsSprinting = false;

		//prints "Walkng" when GEngine is being used
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Walking"));
		}
	}
}

//called when walk key is released, sets walk speed to 600 and prints "Running"
void AMainCharacter::WalkReleased(){
	if (!bIsCrouching) {
		//sets speed to 595
		GetCharacterMovement()->MaxWalkSpeed = 595.0f;

		//sets value of bIsAimingDownSights
		bIsAimingDownSights = false;

		//prints "Running" when GEngine is being used
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Running"));
		}
	}
}

//called when sprint key is pressed, sets walk speed to 1000 and prints "Sprinting"
void AMainCharacter::SprintPressed(){
	if (!bIsCrouching && !bIsReloading && !bIsAimingDownSights) {
		//checks if stamina is high enough
		if (Stamina >= .01) {
			//sets speed to 1000
			GetCharacterMovement()->MaxWalkSpeed = 1000.0f;

			//set value of bIsSprinting
			bIsSprinting = true;

			//prints "Sprinting" when GEngine is being used
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Sprinting"));
			}
		}
	}
}

//called when sprint key is released, sets walk speed to 600 and prints "Running"
void AMainCharacter::SprintReleased(){
	if (!bIsCrouching && !bIsAimingDownSights) {
		//sets walk speed
		GetCharacterMovement()->MaxWalkSpeed = 595.0f;

		//set value of bIsSprinting
		bIsSprinting = false;

		//prints "Running" when GEngine is being used
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Running"));
		}
	}
}

//called when crouch key is pressed, sets walk speed to 265 and changes value of bIsCrouched
void AMainCharacter::CrouchPressed() {
	//set walk speed
	GetCharacterMovement()->MaxWalkSpeed = 265.0f;

	//set value of bIsCrouching
	bIsCrouching = true;

	//lowers camera
	if (Perspective == EPerspective::P_3P) {
		CameraComp->AddRelativeLocation(FVector(0.0f, 0.0f, -60.0f));
	}

	//prints "Crouching"
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Crouching"));
	}
}

//called when crouch key is released, sets player to default speed and changes value of bIsCrouched
void AMainCharacter::CrouchReleased() {
	//sets speed to 595
	GetCharacterMovement()->MaxWalkSpeed = 595.0f;

	//put camera back
	if (Perspective == EPerspective::P_3P) {
		CameraComp->AddRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	}

	//set value of bIsCrouching
	bIsCrouching = false;
}

//called when aim is pressed, slows speed and zooms in
void AMainCharacter::AimPressed(){
		if (!bIsInventoryOpen && !bIsReloading && CurrentGunType!=EGunType::GT_None && bCanMove) {
			//slow character
			WalkPressed();

			if (bIsCrouching) {
				//zoom in
				CameraComp->AddRelativeLocation(FVector(-170.0f, -10.0f, 0.0f));
			}
			else {
				//zoom in
				CameraComp->AddRelativeLocation(FVector(170.0f, 10.0f, 0.0f));
			}

			//prints "Gun fired" if GEngine is being used
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Aim pressed"));
		}
	}
}

//called when aim is released, restores speed and zooms out
void AMainCharacter::AimReleased(){
	if (!bIsInventoryOpen && !bIsReloading && CurrentGunType!=EGunType::GT_None && bCanMove) {
		//restore speed
		WalkReleased();

		if (bIsCrouching) {
			//zoom out
			CameraComp->AddRelativeLocation(FVector(170.0f, 10.0f, 0.0f));
		}
		else {
			//zoom out
			CameraComp->AddRelativeLocation(FVector(-170.0f, -10.0f, 0.0f));
		}

		//prints "Gun fired" if GEngine is being used
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Aim released"));
		}
	}
}

//called when fire is pressed
void AMainCharacter::FirePressed() {
	//set bIsFiring to true 
	if (!bIsReloading && !bIsSprinting && CurrentAmmo > 0 && CurrentGunType != EGunType::GT_None && bCanMove) {
		bIsFiring = true;
		Fire();

		if (bIsAutomaticWeapon) {
			GetWorldTimerManager().SetTimer(FireTimer, this, &AMainCharacter::Fire, RateOfFire, true);
		}

		else {
			PlayAnimMontage(FireAnimMontage);
			GetWorldTimerManager().SetTimer(ReloadTimer, this, &AMainCharacter::FireSingleReleased, 0.233f, false);
		}

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Yellow, TEXT("Fire Pressed"));
		}
	}
}

//called when fire is released
void AMainCharacter::FireReleased() {
	if (!bIsReloading && !bIsSprinting && CurrentGunType != EGunType::GT_None && bCanMove) {
		if (bIsAutomaticWeapon) {
			bIsFiring = false;
		}

		GetWorldTimerManager().ClearTimer(FireTimer);

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Yellow, TEXT("Fire Released"));
		}
	}
}

//called when firing a single shot is done
void AMainCharacter::FireSingleReleased() {
	bIsFiring = false;
	
	GetWorldTimerManager().ClearTimer(ReloadTimer);

	FireReleased();
}

//fires weapon
void AMainCharacter::Fire() {
	//checks if there is enough ammo
	if (bIsFiring && CurrentAmmo > 0 && !bIsSprinting) {
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
		if (GEngine) {
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 0.5F, (uint8)'\000', 1.0F);
		}

		if (RaycastHit.GetActor() != nullptr) {
			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(RaycastHit.GetActor());
			Enemy->Damage(DamagePerRound);

			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Object Hit"));
			}
		}

		//prints "Gun fired" if GEngine is being used
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Gun fired"));
		}
	}
}

//called when melee is pressed, short raycast and deal damage on hit
void AMainCharacter::Melee() {
	if (!bIsSprinting && !bIsReloading) {
		//raycast to see if something that can be hit is in range
		//calulate start and end location
		FVector StartLocation = CameraComp->GetComponentLocation();
		FVector EndLocation = StartLocation + (CameraComp->GetForwardVector() * 500);

		FHitResult RaycastHit;

		//raycast should ignore character
		FCollisionQueryParams CQP;
		CQP.AddIgnoredActor(this);

		//raycast
		GetWorld()->LineTraceSingleByChannel(RaycastHit, StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, CQP);

		if (RaycastHit.GetActor() != nullptr) {
			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(RaycastHit.GetActor());
			Enemy->Damage(MeleeDamage);

			//set meleeattacking to true
			bIsMeleeAttacking = true;

			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Object Hit"));
			}
		}
		//set value of meleeattacking to false
		bIsMeleeAttacking = false;
	}
}

//called when reload is pressed. sets value of Reloading
void AMainCharacter::ReloadPressed() {
	if (CurrentAmmo != MaxAmmo && !GetCharacterMovement()->IsFalling() && CurrentGunType!=EGunType::GT_None, TotalAmmo > 0 && bCanMove) {
		bIsReloading = true;

		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AMainCharacter::ReloadReleased, 2.0f, false);
	}
}

//caled when reload is released
void AMainCharacter::ReloadReleased() {
	bIsReloading = false;

	//check if there is enough ammo to reload
	if (TotalAmmo >= MaxAmmo) {
		//decide how much ammo to add to current ammo
		float AmmoDiff = MaxAmmo - CurrentAmmo;

		//set clip full
		CurrentAmmo = CurrentAmmo + AmmoDiff;

		//subtract from total ammo
		TotalAmmo = TotalAmmo - AmmoDiff;
	}
	else {
		//set currrent ammo to whatever is left
		CurrentAmmo = TotalAmmo;

		//set total ammo to 0
		TotalAmmo = 0;
	}

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Reload Finished"));
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
void AMainCharacter::RayCast() {
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
	AInteractableItem* Interactable = Cast<AInteractableItem>(RaycastHit.GetActor());

	if (LastSeenItem && LastSeenItem != Pickup) {
		//if character sees a different pickup disable glowing effect on last character
		LastSeenItem->SetGlowEffect(false);
		bIsLookingAtPickup = false;
	}

	if (Pickup) {
		//enable glowing effect on current item
		LastSeenItem = Pickup;
		Pickup->SetGlowEffect(true);
		bIsLookingAtPickup = true;
	}

	//reinitalize
	else LastSeenItem = nullptr;

	if (CurrentInteraction && CurrentInteraction != Interactable) {
		bIsLookingAtInteractable = false;

	}

	if (Interactable) {
		CurrentInteraction = Interactable;
		bIsLookingAtInteractable = true;
	}

	else CurrentInteraction = nullptr;
}

//called when pickup is pressed, picks up item
void AMainCharacter::PickupItem(){
	if (LastSeenItem) {
		//find first available slot
		int32 AvailableSlot = Inventory.Find(nullptr);

		//check is item has image, if not dont add and activate use, if it does add to inventory
		if (!LastSeenItem->bIsInventoryPickup) {
			LastSeenItem->UseItem();
			LastSeenItem->SetPickedUp(true);
		}
		else if (AvailableSlot != INDEX_NONE) {
			//add item to first valid slot
			Inventory[AvailableSlot] = LastSeenItem;

			//disable item from game
			LastSeenItem->SetPickedUp(true);

			if (bIsInventoryOpen) {
				InventoryRef->Show();
			}
		}
		else {
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("Inventory full"));
			}
		}
	}

	if (CurrentInteraction) {
		//activates logic in InteractableItem
		CurrentInteraction->Activate();
	
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Item activated"));
		}
	}
}

//called when pause button is pressed
void AMainCharacter::PauseMenu() {
	//set refrenece to PC
	AMC_PlayerController* Con = Cast<AMC_PlayerController>(GetController());
	//open pause menu
	if (Con) Con->PauseGame();
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
			//raycast to determine where to place item
			FVector StartLocation = CameraComp->GetComponentLocation();

			FVector EndLocation = StartLocation + (CameraComp->GetForwardVector() * 500);

			FHitResult DropItem;

			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 0.5f, (uint8)'\000', 1.0f);
			GetWorld()->LineTraceSingleByChannel(DropItem, StartLocation, EndLocation, ECC_Visibility);

			//the location of drop
			FVector DropLocation = DropItem.Location + FVector(0.0f, 0.0f, 10.0f);

			//FVector DropLocation = GetActorLocation() + (GetActorForwardVector() *200);

			//enabling pickup
			CurrentlyEquippedItem->PickupMesh->SetEnableGravity(true);

			CurrentlyEquippedItem->SetActorEnableCollision(true);

			//check if hit was successful, if so place item at hit location if not place item in front of player view a fixed distance
			if (DropItem.bBlockingHit) {
				CurrentlyEquippedItem->SetActorLocation(DropLocation);
			}
			else {
				CurrentlyEquippedItem->SetActorLocation(EndLocation);
			}

			CurrentlyEquippedItem->PickupMesh->SetVisibility(true);


			//unreference item just placed
			Inventory[IndexOfItem] = nullptr;
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

//called when switching weapons
void AMainCharacter::SwitchGun(EGunType NewGun, float NewMaxAmmo, float NewDamagePerRound, float NewRateOfFire, bool NewbIsAutomatic, USkeletalMesh* NewGunSKMesh, FVector NewGunLocation, FRotator NewGunRotation, FVector NewGunScale){
	switch (CurrentGunType) {
		case EGunType::GT_None:
			break;

		case EGunType::GT_MG45:
			CurrentAmmo_MG45 = CurrentAmmo;

			TotalAmmo_MG45 = TotalAmmo;

			break;

		default :
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, TEXT("Invalid current gun"));
			}
	}

	//check what kind of gun is getting equipped and store its ammo values
	switch (NewGun) {
		case EGunType::GT_None:
			CurrentAmmo = 0;

			TotalAmmo = 0;

			CurrentGunType = EGunType::GT_None;
			break;

		case EGunType::GT_MG45:
			CurrentAmmo = CurrentAmmo_MG45;

			TotalAmmo = TotalAmmo_MG45;

			CurrentGunType = EGunType::GT_MG45;
			break; 

		default:
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, TEXT("Invalid new gun"));
			}
	}

	//change variables based on new variables
	MaxAmmo = NewMaxAmmo;
	DamagePerRound = NewDamagePerRound;
	RateOfFire = NewRateOfFire;
	bIsAutomaticWeapon = NewbIsAutomatic;
	
	//change gun mesh
	GunComp->SetSkeletalMesh(NewGunSKMesh);

	//set location, rotation, and scale
	GunComp->SetRelativeLocationAndRotation(NewGunLocation, NewGunRotation);
	GunComp->SetRelativeScale3D(NewGunScale);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Cyan, TEXT("Gun Switched"));
	}
}

void AMainCharacter::AddAmmo(float AmmoToAdd, EGunType GunType) {
	if (GunType == CurrentGunType) {
		TotalAmmo = TotalAmmo + AmmoToAdd;
	}
	switch (GunType) {
	case EGunType::GT_None:
		break;

	case EGunType::GT_MG45:
		TotalAmmo_MG45 = TotalAmmo_MG45 + AmmoToAdd;

		break;
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
    return TotalAmmo;
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

FName AMainCharacter::GetInteractionAction() {
	if (CurrentInteraction) {
		return CurrentInteraction->GetActionText();
	}
	else {
		FName none;
		return none;
	}
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

bool AMainCharacter::GetLSIbIsInventoryPickup() {
	if (LastSeenItem) {
		return LastSeenItem->bIsInventoryPickup;
	}
	else {
		return true;
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

AInteractableItem* AMainCharacter::GetCurrentInteraction() {
	if (CurrentInteraction) {
		return CurrentInteraction;
	}

	else return nullptr;
}

bool AMainCharacter::GetIsInventoryOpen() {
	return bIsInventoryOpen;
}

bool AMainCharacter::GetIsAutomaticWeapon() {
	return bIsAutomaticWeapon;
}

USkeletalMeshComponent* AMainCharacter::GetGunComp() {
	return GunComp;
}

bool AMainCharacter::GetIsCrouching() {
	return bIsCrouching;
}

bool AMainCharacter::GetIsReloading() {
	return bIsReloading;
}

bool AMainCharacter::GetCanMove() {
	return bCanMove;
}

EGunType AMainCharacter::GetCurrentGunType() {
	return CurrentGunType;
}

bool AMainCharacter::GetIsFiring() {
	return bIsFiring;
}

bool AMainCharacter::GetIsLookingAtInteractable() {
	return bIsLookingAtInteractable;
}

TArray<APickupItem*> AMainCharacter::GetInventory() {
	return Inventory;
}

EPerspective AMainCharacter::GetPerspective() {
	return Perspective;
}

void AMainCharacter::SetCurrentlyEquippedItem(APickupItem* Item) {
	CurrentlyEquippedItem = Item;
}

void AMainCharacter::SetMaxAmmo(float NewMaxAmmo) {
	MaxAmmo = NewMaxAmmo;
}

void AMainCharacter::SetGunMesh(USkeletalMesh* NewGunMesh) {
	GunComp->SetSkeletalMesh(NewGunMesh);
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

void AMainCharacter::SetCanMove(bool CanMove) {
	bCanMove = CanMove;
}

void AMainCharacter::SetPerspective(EPerspective NewPerspective) {
	Perspective = NewPerspective;

	if (NewPerspective == EPerspective::P_1P) {
		//attach camera to head
		CameraComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), FName("FirstPersonCamera"));

	}

	if (NewPerspective == EPerspective::P_3P) {
		//detach camera from head and set to thirdperson position
		CameraComp->DetachFromComponent(FDetachmentTransformRules(FDetachmentTransformRules::KeepRelativeTransform));
		CameraComp->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
		CameraComp->SetRelativeLocation(FVector(-250.0f, 80.0f, 90.0f));
	}
}

void AMainCharacter::Damage(float Damage) {
	Health = Health - Damage;
}

void AMainCharacter::SaveGame() {
	//get reference to save game
	USavedGame* SaveGameInstance = Cast<USavedGame>(UGameplayStatics::CreateSaveGameObject(USavedGame::StaticClass()));
	//set reference to playercontroller
	AMC_PlayerController* Con = Cast<AMC_PlayerController>(GetController());
	//save value of bOpening
	SaveGameInstance->bOpening = Con->bOpening;
	//save inventory
	SaveGameInstance->Inventory = Inventory;
	//save player location 
	SaveGameInstance->PlayerLocation = GetActorLocation();
	//save player health
	SaveGameInstance->PlayerHealth = Health;
	//save player stamina
	SaveGameInstance->PlayerStamina = Stamina;
	//save variables in enemy character
	for (TActorIterator<AEnemyCharacter> EnemyCharacterItr(GetWorld()); EnemyCharacterItr; ++EnemyCharacterItr) {
		AEnemyCharacter* EnemyRef = Cast<AEnemyCharacter>(*EnemyCharacterItr);
		SaveGameInstance->EnemyHealths.Add(EnemyRef->GetHealth());
		SaveGameInstance->EnemyTransform.Add(EnemyRef->GetActorTransform());
		SaveGameInstance->EnemyDead.Add(EnemyRef->GetDead());
		SaveGameInstance->EnemyClass.Add(EnemyRef);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Saved Enemy"));
	}
	//save pickupitems in game
	for (TActorIterator<APickupItem> PickupItr(GetWorld()); PickupItr; ++PickupItr){
		APickupItem* PickupRef = Cast<APickupItem>(*PickupItr);
		SaveGameInstance->PickupStatus.Add(PickupRef->GetPickedUp());
		SaveGameInstance->PickupClass.Add(PickupRef);
	}	
	//save game to slot
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Saving..."));
	}
}

void AMainCharacter::LoadGame() {
	//get reference to save game
	USavedGame* LoadGameInstance = Cast<USavedGame>(UGameplayStatics::CreateSaveGameObject(USavedGame::StaticClass()));
	//load game
	LoadGameInstance = Cast<USavedGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	//check if saved game exists
	if (LoadGameInstance) {
		//set value of bOpening in player controller
		AMC_PlayerController* Con = Cast<AMC_PlayerController>(GetController());
		Con->SetOpening(LoadGameInstance->bOpening);
		//load inventory
		Inventory = LoadGameInstance->Inventory;
		//load player postition
		SetActorLocation(LoadGameInstance->PlayerLocation);
		//load player health
		Health = LoadGameInstance->PlayerHealth;
		//load player stamina 
		Stamina = LoadGameInstance->PlayerStamina;
		//load variables for enemies
		for (int i = 0; i < LoadGameInstance->EnemyClass.Num(); i++) {
			AEnemyCharacter* EnemyRef = Cast<AEnemyCharacter>(LoadGameInstance->EnemyClass[i]);
			EnemyRef->SetHealth(LoadGameInstance->EnemyHealths[i]);
			EnemyRef->SetActorLocationAndRotation(LoadGameInstance->EnemyTransform[i].GetLocation(), LoadGameInstance->EnemyTransform[i].GetRotation());
			//if enemy is saved as dead, delete from game
			if (LoadGameInstance->EnemyDead[i]) {
				EnemyRef->Despawn();
			}
			FString HealthText = FString::SanitizeFloat(LoadGameInstance->EnemyHealths[i]);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, *HealthText);
		}
		//load variables for pickups
		for (int i = 0; i < LoadGameInstance->PickupClass.Num(); i++) {
			APickupItem* PickupRef = Cast<APickupItem>(LoadGameInstance->PickupClass[i]);
			PickupRef->SetPickedUp(LoadGameInstance->PickupStatus[i]);
		}
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Loaded"));
		}
	}
}