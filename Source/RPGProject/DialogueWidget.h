// Copyright 2017-2018 Paul Murray GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueStruct.h"
#include "DialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public: 
	UFUNCTION(BlueprintCallable)
		void UpdateDialogueOutputFromPlayer(FDialogueInformation NewDialogueInfo);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FDialogueInformation DialogueInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentDialogueNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bShouldDialogueBeVisible = false;

private:
	UFUNCTION()
		void UpdateDialogueOutputTextFromPlayer();

	UPROPERTY()
		FTimerHandle DialougeOutputVisibilityTimer;
};
