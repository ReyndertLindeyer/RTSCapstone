// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h" 
#include "BehaviorTree/BehaviorTree.h"
#include "MasterUnit.generated.h"

UCLASS()
class RTSCAPSTONE_API AMasterUnit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMasterUnit();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* root;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* charMesh;


	UPROPERTY(EditAnywhere)
		bool selected;
	UPROPERTY(EditAnywhere)
		UBehaviorTree *behavTree;

	bool isPlayer, isCombat;

	uint32 team, travelSpeed, spawnTime;

	float GetSightRadius();

	uint32 maxHealth;
	uint32 currentHealth;
	float sightRadius;
};
