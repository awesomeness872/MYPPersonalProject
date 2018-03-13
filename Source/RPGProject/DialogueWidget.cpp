// Copyright 2017-2018 Paul Murray GPLv3

#include "RPGProject.h"
#include "MainCharacter.h"
#include "DialogueWidget.h"


void UDialogueWidget::UpdateDialogueOutputFromPlayer(FDialogueInformation NewDialogueInfo) {
	DialogueInfo = NewDialogueInfo;
	if (CurrentDialogueNumber < DialogueInfo.DialogueTimes.Num()) {
		bShouldDialogueBeVisible = true;
		GetWorld()->GetTimerManager().SetTimer(DialougeOutputVisibilityTimer, this, &UDialogueWidget::UpdateDialogueOutputTextFromPlayer, DialogueInfo.DialogueTimes[CurrentDialogueNumber], false);
	}
}

void UDialogueWidget::UpdateDialogueOutputTextFromPlayer() {
	if (CurrentDialogueNumber < DialogueInfo.DialogueTimes.Num() - 1) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::SanitizeFloat(CurrentDialogueNumber));
		CurrentDialogueNumber++;
		GetWorld()->GetTimerManager().SetTimer(DialougeOutputVisibilityTimer, this, &UDialogueWidget::UpdateDialogueOutputTextFromPlayer, DialogueInfo.DialogueTimes[CurrentDialogueNumber], false);
	}
	else {
		bShouldDialogueBeVisible = false;
	}
}
