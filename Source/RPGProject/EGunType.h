// Copyright 2017 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EGunType.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGunType : uint8
{
	GT_None UMETA(DisplayName = "None"),
	GT_MG45	UMETA(DisplayName = "MG-45"),
};

USTRUCT(BlueprintType)
struct FGunInformation {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGunType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamagePerRound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* GunSkMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform GunOffset;
};
/*
UCLASS()
class RPGPROJECT_API UEGunType : public UUserDefinedEnum
{
	GENERATED_BODY()
	
	
	
	
};*/
