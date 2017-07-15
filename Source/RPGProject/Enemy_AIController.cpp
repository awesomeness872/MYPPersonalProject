// Copyright 2017 Paul Murray GPLv3

#include "RPGProject.h"
#include "Enemy_AIController.h"


AEnemy_AIController::AEnemy_AIController(const FObjectInitializer& ObjectInitializer) {
}

void AEnemy_AIController::BeginPlay() {
	Super::BeginPlay();

	RunBehaviorTree(Enemy_BehaviorTree);
}