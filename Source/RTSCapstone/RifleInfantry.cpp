// Fill out your copyright notice in the Description page of Project Settings.

#include "RifleInfantry.h"

// Called when the game starts or when spawned
void ARifleInfantry::BeginPlay()
{
	Super::BeginPlay();
	maxHealth = 10;
	currentHealth = maxHealth;

	//The character will always point towards the direction of movement and will turn with a rate of 600 along the Y axis
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	isPlayer = true;

	sightRadius = 80.0f;
	attackRadius = 60.0f;

	AIControllerClass = AMyRTSAIController::StaticClass();
}