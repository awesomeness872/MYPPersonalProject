// Copyright 2017-2018 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "DialogueStruct.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FDialogueInformation {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DialogueChunkID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<float> DialogueTimes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FText> DialogueTexts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FText> DialogueSpeaker;
};

UCLASS()
class RPGPROJECT_API UDialogueStruct : public UUserDefinedStruct
{
	GENERATED_BODY()
	
	
	
	
};
