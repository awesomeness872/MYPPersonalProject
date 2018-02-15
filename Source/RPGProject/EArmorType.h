// Copyright 2017-2018 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EArmorType.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	AT_Helmet UMETA(DisplayName = "Helmet"),
	AT_ChestPiece UMETA(DisplayName = "Chest Piece"),
	AT_Pants UMETA(DisplayName = "Pants"),
	AT_Boots UMETA(DisplayName = "Boots"),
};

USTRUCT(BlueprintType)
struct FArmorInformation {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ArmorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamageMitigation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* ArmorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EArmorType ArmorType;
};
/*
UCLASS()
class RPGPROJECT_API UEArmorType : public UUserDefinedEnum
{
	GENERATED_BODY()
	
	
	
	
};*/
