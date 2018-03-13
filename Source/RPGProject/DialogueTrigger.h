// Copyright 2017-2018 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "DialogueStruct.h"
#include "DialogueTrigger.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API ADialogueTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	ADialogueTrigger();
	
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FDialogueInformation DialogueInfo;

private:
	UPROPERTY()
		bool bHasBeenActivated = false;
};
