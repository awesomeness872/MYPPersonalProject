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
	//initialize inventory
	Inventory.SetNum(MAX_INVENTORY_ITEMS);

	//initialize GunInventory
	GunInventory.SetNum(MAX_GUNINVENTORY_ITEMS);

	//initialize ArmorInventory
	ArmorInventory.SetNum(MAX_ARMORINVENTORY_ITEMS);

    //display that this class is being used
    if (GEngine){
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Using MainCharacter"));
    }

    //initialize LastSeenItem reference
    LastSeenItem = nullptr;

	//set GunComp to socket
	GunComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), FName("WeaponSocket"));

	//set gun type to none
	CurrentGun.Type = EGunType::GT_None;

	//load game
	LoadGame();

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
	PlayerInputComponent->BindAction("GunInventory", IE_Pressed, this, &AMainCharacter::HandleGunInventoryInput);
	PlayerInputComponent->BindAction("ArmorInventory", IE_Pressed, this, &AMainCharacter::HandleArmorInventoryInput);
	//input for pause menu
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AMainCharacter::PauseMenu);
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
			//Limit pitch when walking or falling
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
		if (!bIsInventoryOpen && !bIsReloading && CurrentGun.Type!=EGunType::GT_None && bCanMove) {
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
	if (!bIsInventoryOpen && !bIsReloading && CurrentGun.Type!=EGunType::GT_None && bCanMove) {
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
	if (!bIsReloading && !bIsSprinting && CurrentGun.CurrentAmmo > 0 && CurrentGun.Type != EGunType::GT_None && bCanMove) {
		bIsFiring = true;
		Fire();

		if (CurrentGun.bIsAutomatic) {
			GetWorldTimerManager().SetTimer(FireTimer, this, &AMainCharacter::Fire, CurrentGun.RateOfFire, true);
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
	if (!bIsReloading && !bIsSprinting && CurrentGun.Type != EGunType::GT_None && bCanMove) {
		if (CurrentGun.bIsAutomatic) {
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
	if (bIsFiring && CurrentGun.CurrentAmmo > 0 && !bIsSprinting) {
		//reduce current ammo
		CurrentGun.CurrentAmmo--;

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
			Enemy->Damage(CurrentGun.DamagePerRound);

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
		FVector EndLocation = StartLocation + (CameraComp->GetForwardVector() * MeleeRaycastRange);

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
	if (CurrentGun.CurrentAmmo != CurrentGun.MaxAmmo && !GetCharacterMovement()->IsFalling() && CurrentGun.Type!=EGunType::GT_None, CurrentGun.TotalAmmo > 0 && bCanMove) {
		bIsReloading = true;

		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AMainCharacter::ReloadReleased, 2.0f, false);
	}
}

//caled when reload is released
void AMainCharacter::ReloadReleased() {
	bIsReloading = false;

	//check if there is enough ammo to reload
	if (CurrentGun.TotalAmmo >= CurrentGun.MaxAmmo) {
		//decide how much ammo to add to current ammo
		float AmmoDiff = CurrentGun.MaxAmmo - CurrentGun.CurrentAmmo;

		//set clip full
		CurrentGun.CurrentAmmo = CurrentGun.CurrentAmmo + AmmoDiff;

		//subtract from total ammo
		CurrentGun.TotalAmmo = CurrentGun.TotalAmmo - AmmoDiff;
	}
	else {
		//set currrent ammo to whatever is left
		CurrentGun.CurrentAmmo = CurrentGun.TotalAmmo;

		//set total ammo to 0
		CurrentGun.TotalAmmo = 0;
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
		int32 AvailableGunSlot = GunInventory.Find(nullptr);
		int32 AvailableArmorSlot = ArmorInventory.Find(nullptr);

		//check is item has image, if not dont add and activate use, if it does add to inventory
		if (LastSeenItem->PickupInfo.PickupType == EPickupType::PT_UseOnPick) {
			LastSeenItem->UseItem();
			LastSeenItem->SetPickedUp(true);
		}
		//check if item is a gun
		if (LastSeenItem->PickupInfo.PickupType == EPickupType::PT_Gun) {
			if (AvailableGunSlot != INDEX_NONE) {
				GunInventory[AvailableGunSlot] = LastSeenItem;
				LastSeenItem->SetPickedUp(true);
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("Gun added"));
				}
			}

			if (bIsGunInventoryOpen) {
				GunInventoryRef->Show();
			}
		}
		//add armor to inventory
		if (LastSeenItem->PickupInfo.PickupType == EPickupType::PT_Armor) {
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Armor added"));
			if (AvailableArmorSlot != INDEX_NONE) {
				//determine how many of each armor types are in the inventory
				int BootCount = 0;
				int ChestCount = 0;
				int HelmetCount = 0;
				int PantsCount = 0;
				for (int i = 0; i < ArmorInventory.Num(); i++) {
					switch (LastSeenItem->GetArmorInfo().ArmorType) {
					case EArmorType::AT_Boots:
						BootCount++;
						break;
					case EArmorType::AT_ChestPiece:
						ChestCount++;
						break;
					case EArmorType::AT_Helmet:
						HelmetCount++;
						break;
					case EArmorType::AT_Pants:
						PantsCount++;
						break;
					default: 
						if (GEngine) {
							GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Invalid Type"));
						}
					}
				}
				//add to inventory
				switch (LastSeenItem->GetArmorInfo().ArmorType) {
				//uses slots 0-4
				case EArmorType::AT_Boots:
					if (BootCount < 5) {
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Boots Full"));
					}
					else {
						if (ArmorInventory[0] == nullptr) {
							ArmorInventory[0] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[1] == nullptr) {
							ArmorInventory[1] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[2] == nullptr) {
							ArmorInventory[2] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[3] == nullptr) {
							ArmorInventory[3] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[4] == nullptr) {
							ArmorInventory[4] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Boots Full"));
					}
					break;
				//uses slots 5-9
				case EArmorType::AT_ChestPiece:
					if (ChestCount < 5) {
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Chest Full"));
					}
					else {
						if (ArmorInventory[5] == nullptr) {
							ArmorInventory[5] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[6] == nullptr) {
							ArmorInventory[6] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[7] == nullptr) {
							ArmorInventory[7] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[8] == nullptr) {
							ArmorInventory[8] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[9] == nullptr) {
							ArmorInventory[9] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Chest Full"));
					}
					break;
					//uses slots 10-14
				case EArmorType::AT_Helmet:
					if (HelmetCount < 5) {
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Helmet Full"));
					}
					else {
						if (ArmorInventory[10] == nullptr) {
							ArmorInventory[10] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[11] == nullptr) {
							ArmorInventory[11] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[12] == nullptr) {
							ArmorInventory[12] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[13] == nullptr) {
							ArmorInventory[13] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[14] == nullptr) {
							ArmorInventory[14] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Chest Full"));
					}
					break;
					//uses slots 15-19
				case EArmorType::AT_Pants:
					if (PantsCount < 5) {
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Pants Full"));
					}
					else {
						if (ArmorInventory[15] == nullptr) {
							ArmorInventory[15] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[16] == nullptr) {
							ArmorInventory[16] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[17] == nullptr) {
							ArmorInventory[17] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[18] == nullptr) {
							ArmorInventory[18] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else if (ArmorInventory[19] == nullptr) {
							ArmorInventory[19] = LastSeenItem;
							LastSeenItem->SetPickedUp(true);
						}
						else GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Chest Full"));
					}
					break;
				default:
					if (GEngine) {
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Invalid Type"));
					}
				}
			}
			if (bIsArmorInventoryOpen) {
				//stupid hack for refreshing inventory i hope works
				HandleArmorInventoryInput();
				HandleArmorInventoryInput();
			}
		}

		else if (AvailableSlot != INDEX_NONE && LastSeenItem->PickupInfo.PickupType == EPickupType::PT_Inventory) {
			//add item to first valid slot
			Inventory[AvailableSlot] = LastSeenItem;

			//disable item from game
			LastSeenItem->SetPickedUp(true);

			if (bIsInventoryOpen) {
				InventoryRef->Show();
			}
		}
		else {
			if (GEngine && LastSeenItem->PickupInfo.PickupType == EPickupType::PT_Inventory) {
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
	if (!bIsGunInventoryOpen && !bIsArmorInventoryOpen) {
		if (bIsInventoryOpen) {
			bIsInventoryOpen = false;
		}
		else bIsInventoryOpen = true;
		AMC_PlayerController* Con = Cast<AMC_PlayerController>(GetController());
		if (Con) Con->HandleInventoryInput();
	}
}

void AMainCharacter::HandleGunInventoryInput() {
	if (!bIsInventoryOpen && !bIsArmorInventoryOpen) {
		if (bIsGunInventoryOpen) {
			bIsGunInventoryOpen = false;
		}
		else bIsGunInventoryOpen = true;
		AMC_PlayerController* Con = Cast<AMC_PlayerController>(GetController());
		if (Con) Con->HandleGunInventoryInput();
	}
}

void AMainCharacter::HandleArmorInventoryInput() {
	if (!bIsInventoryOpen && !bIsGunInventoryOpen) {
		if (bIsArmorInventoryOpen) {
			bIsArmorInventoryOpen = false;
		}
		else bIsArmorInventoryOpen = true;
		AMC_PlayerController* Con = Cast<AMC_PlayerController>(GetController());
		if (Con) Con->HandleArmorInventoryInput();
	}
}

void AMainCharacter::SetEquippedItem(UTexture2D * Texture){
    if (Texture){
        /*For this scenario we make the assumption that
        every pickup has a unique texture.
        So, in order to set the equipped item we just check every item
        inside our Inventory. Once we find an item that has the same image as the
        Texture image we're passing as a parameter we mark that item as CurrentlyEquipped.*/
        for (auto It = Inventory.CreateIterator(); It; It++){
            if ((*It) && (*It)->GetPickupInfo().PickupImage && (*It)->GetPickupInfo().PickupImage->HasSameSourceArt(Texture)){
                CurrentlyEquippedItem = *It;
                if (GEngine){
                    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("New equipped item"));
                }
                break;
            }
        }
		for (auto It = GunInventory.CreateIterator(); It; It++) {
			if ((*It) && (*It)->GetPickupInfo().PickupImage && (*It)->GetPickupInfo().PickupImage->HasSameSourceArt(Texture)) {
				CurrentlyEquippedItem = *It;
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("New equipped item"));
				}
				break;
			}
		}
		for (auto It = ArmorInventory.CreateIterator(); It; It++) {
			if ((*It) && (*It)->GetPickupInfo().PickupImage && (*It)->GetPickupInfo().PickupImage->HasSameSourceArt(Texture)) {
				CurrentlyEquippedItem = *It;
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("New equipped item"));
				}
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
        if (Inventory.Find(CurrentlyEquippedItem, IndexOfItem) || GunInventory.Find(CurrentlyEquippedItem, IndexOfItem) || ArmorInventory.Find(CurrentlyEquippedItem, IndexOfItem)) {
			//raycast to determine where to place item
			FVector StartLocation = CameraComp->GetComponentLocation();

			FVector EndLocation = StartLocation + (CameraComp->GetForwardVector() * 500);

			FHitResult DropItem;

			GetWorld()->LineTraceSingleByChannel(DropItem, StartLocation, EndLocation, ECC_Visibility);

			//the location of drop
			FVector DropLocation = DropItem.Location + FVector(0.0f, 0.0f, 10.0f);

			//enabling pickup
			CurrentlyEquippedItem->SetPickedUp(false);

			//check if hit was successful, if so place item at hit location if not place item in front of player view a fixed distance
			if (DropItem.bBlockingHit) {
				CurrentlyEquippedItem->SetActorLocation(DropLocation);
			}
			else {
				CurrentlyEquippedItem->SetActorLocation(EndLocation);
			}

			CurrentlyEquippedItem->PickupMesh->SetVisibility(true);

			//unreference item just placed
			if (CurrentlyEquippedItem->GetPickupInfo().PickupType == EPickupType::PT_Gun) {
				GunInventory[IndexOfItem] = nullptr;
			}
			else if (CurrentlyEquippedItem->GetPickupInfo().PickupType == EPickupType::PT_Armor) {
				ArmorInventory[IndexOfItem] = nullptr;
			}
			else {
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
			//unreference item just used
			Inventory[IndexOfItem] = nullptr;
		}
		if (GunInventory.Find(CurrentlyEquippedItem, IndexOfItem)) {
			//unreference item just used
			GunInventory[IndexOfItem] = nullptr;
			//set as CurrentGunActor
			CurrentGunActor = CurrentlyEquippedItem;
		}
		if (ArmorInventory.Find(CurrentlyEquippedItem, IndexOfItem)) {
			//remove from inventory
			ArmorInventory[IndexOfItem] = nullptr;
			//if boots, add old boots to inventory and set as equipped boots
			if (CurrentlyEquippedItem->GetArmorInfo().ArmorType == EArmorType::AT_Boots) {
				//if old boots equipped
				if (BootActor != nullptr) {
					//set boots as last seen item	
					LastSeenItem = BootActor;
					//re add boots
					PickupItem();
				}
				BootActor = CurrentlyEquippedItem;
			}
			else if (CurrentlyEquippedItem->GetArmorInfo().ArmorType == EArmorType::AT_ChestPiece) {
				//if old boots equipped
				if (ChestActor != nullptr) {
					//set boots as last seen item	
					LastSeenItem = ChestActor;
					//re add boots
					PickupItem();
				}
				ChestActor = CurrentlyEquippedItem;
			}
			else if (CurrentlyEquippedItem->GetArmorInfo().ArmorType == EArmorType::AT_Helmet) {
				//if old boots equipped
				if (HelmetActor != nullptr) {
					//set boots as last seen item	
					LastSeenItem = HelmetActor;
					//re add boots
					PickupItem();
				}
				HelmetActor = CurrentlyEquippedItem;
			}
			else if (CurrentlyEquippedItem->GetArmorInfo().ArmorType == EArmorType::AT_Pants) {
				//if old boots equipped
				if (PantsActor != nullptr) {
					//set boots as last seen item	
					LastSeenItem = PantsActor;
					//re add boots
					PickupItem();
				}
				PantsActor = CurrentlyEquippedItem;
			}
		}
	}
}

//called when switching weapons
void AMainCharacter::SwitchGun(FGunInformation NewGun) {
	int32 AvailableSlot = GunInventory.Find(nullptr);
	//if CurrentGunActor is set, set its guninfo to be the players and them readd to inventory
	if (CurrentGunActor != nullptr) {
		CurrentGunActor->GunInfo = CurrentGun;
		GunInventory[AvailableSlot] = CurrentGunActor;
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, TEXT("Gun returned"));
		}
	}
	//determine which gun was last held and store variables
	switch (CurrentGun.Type) {
	//if old gun is LA-34
	case EGunType::GT_LA34:
		LA_34 = CurrentGun;
		break;
	//if old gun was a MG-45
	case EGunType::GT_MG45:
		MG_45 = CurrentGun;
		break;
	//if no current gun
	default:
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, TEXT("No current gun, doesn't matter though"));
		}
	}
	//set model as gun comp
	GunComp->SetSkeletalMesh(NewGun.GunSkMesh);
	GunComp->SetRelativeTransform(NewGun.GunOffset);
	//set NewGun as CurrentGun
	CurrentGun = NewGun;
	//determine what type of gun NewGun is and add previous ammo to it
	switch (NewGun.Type) {
		//if NewGun is an LA-34
	case EGunType::GT_LA34:
		CurrentGun.CurrentAmmo += LA_34.CurrentAmmo;
		CurrentGun.TotalAmmo += LA_34.TotalAmmo;
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, TEXT("LA-34 equipped"));
		break;
		//if NewGun is an MG-45
	case EGunType::GT_MG45:
		CurrentGun.CurrentAmmo += MG_45.CurrentAmmo;
		CurrentGun.TotalAmmo += MG_45.TotalAmmo;
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, TEXT("MG-45 equipped"));
		break;
		//if NewGun is Invalid
	default:
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, TEXT("Invalid new gun"));
		}
	}
}

void AMainCharacter::AddAmmo(float AmmoToAdd, EGunType GunType) {
	if (GunType == CurrentGun.Type) {
		CurrentGun.TotalAmmo = CurrentGun.TotalAmmo + AmmoToAdd;
	}
	switch (GunType) {
	case EGunType::GT_None:
		break;

	case EGunType::GT_MG45:
		MG_45.TotalAmmo = MG_45.TotalAmmo + AmmoToAdd;
		break;
	case EGunType::GT_LA34:
		LA_34.TotalAmmo = LA_34.TotalAmmo + AmmoToAdd;
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
        return LastSeenItem->PickupInfo.PickupName;
    }
    else{
        FName none;
        return none;
    }
}

//called manually, if LastSeenItem is valid returns value of LastSeenIten->PickupDescription, if not returns empty
FName AMainCharacter::GetLSIDesc(){
    if (LastSeenItem){
        return LastSeenItem->PickupInfo.PickupDescription;
    }
    else{
        FName none;
        return none;
    }
}

bool AMainCharacter::GetLSIbIsInventoryPickup() {
	if (LastSeenItem) {
		return LastSeenItem->PickupInfo.PickupType == EPickupType::PT_Inventory;
	}
	else {
		return true;
	}
}

//called manually, if LastSeenItem is valid returns value of LastSeenIten->PickupImage, if not returns empty
UTexture2D* AMainCharacter::GetLSIImage(){
    if (LastSeenItem){
        return LastSeenItem->PickupInfo.PickupImage;
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

bool AMainCharacter::GetIsGunInventoryOpen() {
	return bIsGunInventoryOpen;
}

bool AMainCharacter::GetIsArmorInventoryOpen() {
	return bIsArmorInventoryOpen;
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

bool AMainCharacter::GetIsFiring() {
	return bIsFiring;
}

bool AMainCharacter::GetIsLookingAtInteractable() {
	return bIsLookingAtInteractable;
}

TArray<APickupItem*> AMainCharacter::GetInventory() {
	return Inventory;
}

TArray<APickupItem*> AMainCharacter::GetGunInventory() {
	return GunInventory;
}

TArray<APickupItem*> AMainCharacter::GetArmorInventory(){
	return ArmorInventory;
}

EPerspective AMainCharacter::GetPerspective() {
	return Perspective;
}

FGunInformation AMainCharacter::GetCurrentGun() {
	return CurrentGun;
}
void AMainCharacter::SetCurrentlyEquippedItem(APickupItem* Item) {
	CurrentlyEquippedItem = Item;
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
	Health -= Damage / DamageMitigation;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(Damage));
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, FString::SanitizeFloat(Health));
}

void AMainCharacter::SetChest(FArmorInformation ChestInfo, APickupItem* NewChestActor) {
	CurrentChest = ChestInfo;
	ChestActor = NewChestActor;
	//recalculate damage mitigation
	CalculateMitigation();
}

void AMainCharacter::SetBoots(FArmorInformation BootInfo, APickupItem* NewBootActor) {
	CurrentBoots = BootInfo;
	BootActor = NewBootActor;
	//recalculate damage mitigation
	CalculateMitigation();
}

void AMainCharacter::SetHelmet(FArmorInformation HelmetInfo, APickupItem* NewHelmetActor) {
	CurrentHelmet = HelmetInfo;
	HelmetActor = NewHelmetActor;
	//recalculate damage mitigation
	CalculateMitigation();
}

void AMainCharacter::SetPants(FArmorInformation PantsInfo, APickupItem* NewPantsActor) {
	CurrentPants = PantsInfo;
	PantsActor = NewPantsActor;
	//recalculate damage mitigation
	CalculateMitigation();
}

void AMainCharacter::CalculateMitigation() {
	DamageMitigation = CurrentChest.DamageMitigation + CurrentBoots.DamageMitigation + CurrentHelmet.DamageMitigation + CurrentPants.DamageMitigation;
}

APickupItem* AMainCharacter::GetBootActor() {
	return BootActor;
}

APickupItem* AMainCharacter::GetChestActor() {
	return ChestActor;
}

APickupItem* AMainCharacter::GetHelmetActor() {
	return HelmetActor;
}

APickupItem* AMainCharacter::GetPantsActor() {
	return PantsActor;
}
void AMainCharacter::SaveGame() {
	//let other classes know game is saving
	savingGame = true;
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
	//save player weapon information
	SaveGameInstance->PlayerCurrentGun = CurrentGun;
	SaveGameInstance->PlayerCurrentGunActor = CurrentGunActor;
	SaveGameInstance->PlayerGunInventory = GunInventory;
	//save player perspective
	SaveGameInstance->PlayerPerspective = Perspective;
	//save variables in enemy character
	for (TActorIterator<AEnemyCharacter> EnemyCharacterItr(GetWorld()); EnemyCharacterItr; ++EnemyCharacterItr) {
		AEnemyCharacter* EnemyRef = Cast<AEnemyCharacter>(*EnemyCharacterItr);
		SaveGameInstance->EnemyType.Add(EnemyRef->EnemyType);
		SaveGameInstance->EnemyHealths.Add(EnemyRef->GetHealth());
		SaveGameInstance->EnemyTransform.Add(EnemyRef->GetActorTransform());
		SaveGameInstance->EnemyDead.Add(EnemyRef->GetDead());
		SaveGameInstance->EnemyPlayerLocation.Add(EnemyRef->GetPlayerLocation());
		SaveGameInstance->EnemyHasFoundPlayer.Add(EnemyRef->GetHasFoundPlayer());
		SaveGameInstance->EnemyClass.Add(EnemyRef);
	}
	//save pickupitems in game
	for (TActorIterator<APickupItem> PickupItr(GetWorld()); PickupItr; ++PickupItr){
		APickupItem* PickupRef = Cast<APickupItem>(*PickupItr);
		SaveGameInstance->PickupInfo.Add(PickupRef->GetPickupInfo());
		SaveGameInstance->PickupMesh.Add(PickupRef->PickupMesh);
		SaveGameInstance->PickupLocation.Add(PickupRef->GetActorTransform());
		SaveGameInstance->PickupClass.Add(PickupRef);
	}	
	//save game to slot
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Saved"));
	}
	//let other classes know saving is done
	savingGame = false;
}

void AMainCharacter::LoadGame() {
	//get reference to save game
	USavedGame* LoadGameInstance = Cast<USavedGame>(UGameplayStatics::CreateSaveGameObject(USavedGame::StaticClass()));
	//load game
	LoadGameInstance = Cast<USavedGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	//check if saved game exists
	if (LoadGameInstance) {
		//set variable to loet other classes know it's loading
		loadingGame = true;
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
		//load player gun information
		SwitchGun(LoadGameInstance->PlayerCurrentGun);
		CurrentGunActor = LoadGameInstance->PlayerCurrentGunActor;
		GunInventory = LoadGameInstance->PlayerGunInventory;
		//load player perspective
		SetPerspective(LoadGameInstance->PlayerPerspective);
		//load variables for enemies
		for (int i = 0; i < LoadGameInstance->EnemyClass.Num(); i++) {
			if (LoadGameInstance->EnemyClass[i] != nullptr) {
				AEnemyCharacter* EnemyRef = Cast<AEnemyCharacter>(LoadGameInstance->EnemyClass[i]);
				EnemyRef->EnemyType = LoadGameInstance->EnemyType[i];
				EnemyRef->SetHealth(LoadGameInstance->EnemyHealths[i]);
				EnemyRef->SetActorLocationAndRotation(LoadGameInstance->EnemyTransform[i].GetLocation(), LoadGameInstance->EnemyTransform[i].GetRotation());
				//if enemy is saved as dead, delete from game
				if (LoadGameInstance->EnemyDead[i]) {
					EnemyRef->Despawn();
				}
			}
		}
		//load variables for pickups
		for (int i = 0; i < LoadGameInstance->PickupClass.Num(); i++) {
			if (LoadGameInstance->PickupClass[i] != nullptr) {
				APickupItem* PickupRef = Cast<APickupItem>(LoadGameInstance->PickupClass[i]);
				PickupRef->SetPickupInfo(LoadGameInstance->PickupInfo[i]);
				PickupRef->PickupMesh = LoadGameInstance->PickupMesh[i];
				PickupRef->SetPickedUp(LoadGameInstance->PickupInfo[i].bPickedUp);
				PickupRef->SetActorTransform(LoadGameInstance->PickupLocation[i]);
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, LoadGameInstance->PickupClass[i]->GetName());
			}
		}
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Loaded"));
		}
		//set variables to let other classes know loading is done
		loadingGame = false;
	}
}