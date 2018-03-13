// Copyright 2017-2018 Paul Murray GPLv3

#include "RPGProject.h"
#include "MC_PlayerController.h"
#include "DialogueTrigger.h"

ADialogueTrigger::ADialogueTrigger() {
	//Register Events
	OnActorBeginOverlap.AddDynamic(this, &ADialogueTrigger::OnOverlapBegin);
}

void ADialogueTrigger::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) {
	if (OtherActor && (OtherActor != this) && !bHasBeenActivated) {
		AMC_PlayerController* Con = Cast<AMC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		Con->ToggleDialogue(DialogueInfo);
		bHasBeenActivated = true;
	}
}