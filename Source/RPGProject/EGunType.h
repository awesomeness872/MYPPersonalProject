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
/*
UCLASS()
class RPGPROJECT_API UEGunType : public UUserDefinedEnum
{
	GENERATED_BODY()
	
	
	
	
};*/
