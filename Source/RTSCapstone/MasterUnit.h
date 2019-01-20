// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyRTSAIController.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	//The area that the units will look around in for danger
	UBoxComponent* dangerDetection;

	uint32 maxHealth;
	uint32 currentHealth;
	float sightRadius;
};
