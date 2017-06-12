// Copyright 2017 Paul Murray GPLv3
#include "RPGProject.h"
#include "RPGProjectGameModeBase.h"


void ARPGProjectGameModeBase::BeginPlay(){
    Super::BeginPlay();
    if (GEngine){
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Using GameModeBase"));
    }

}
