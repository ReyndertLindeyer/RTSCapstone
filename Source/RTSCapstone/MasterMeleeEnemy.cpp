// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterMeleeEnemy.h"

void AMasterMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();
	maxHealth = 10;
	currentHealth = maxHealth;

	//The character will always point towards the direction of movement and will turn with a rate of 600 along the Y axis
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	isPlayer = false;

	sightRadius = 100.0f;
	attackRadius = 1.0f;
}