// Copyright 2017 Paul Murray GPLv3

#include "RPGProject.h"
#include "EC_Zombie.h"

//called when game begins, changes varibles to fit class
void AEC_Zombie::BeginPlay() {
	Super::BeginPlay();

	//set attack range to melee range
	SetAttackRange(MeleeRange);

	//set damage to melee damage
	SetAttackDamage(MeleeDamage);

	//set enemy type to zombie
	SetEnemyType(EEnemyType::ET_Zombie);
}

