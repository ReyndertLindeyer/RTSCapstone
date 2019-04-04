// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"

// Components
#include "Components/SphereComponent.h"

#include "Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"

// Interface
#include "I_Unit.h"
#include "I_Entity.h"

// Meshes
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

#include "GameFramework/Character.h"
#include "UNIT_Engineer.generated.h"

UCLASS()
class RTSCAPSTONE_API AUNIT_Engineer : public ACharacter, public II_Unit, public II_Entity
{
	GENERATED_BODY()

		// VARIABLES & COMPONENTS
public:

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SelectionIndicator;

	UPROPERTY(EditAnywhere)
		AActor* setPlayerOwner;

	UPROPERTY(EditAnywhere)
		bool SetDestructible = true;

	// FUNCTIONS
public:
	// OVERRIDDEN CLASS [II_Unit] -- Handles Unit Selection
	virtual void SetSelection(bool state) override;

	virtual bool GetSelection() override;

	// OVERRIDE CLASS [II_Unit] -- Handles Attack Orders
	virtual void AttackOrder(II_Entity* target) override;

	// OVERIDE CLASS [II_Entity] -- Handles any calls on death
	virtual void DestroyEntity() override;

	void PostInitializeComponents();

private:
	float currentTimer;
	float targetTimer = 3.0f;

	bool isSelected;

	//Sound Stuff
	UPROPERTY(EditAnywhere)
		USoundCue* useCue;
	UPROPERTY(EditAnywhere)
		USoundCue* selectCue;
	UPROPERTY(EditAnywhere)
		USoundCue* orderCue;
	UPROPERTY(EditAnywhere)
		USoundCue* deathCue;

	UPROPERTY(EditAnywhere)
		UAudioComponent* audioComponentUse;
	UPROPERTY(EditAnywhere)
		UAudioComponent* audioComponentSelect;
	UPROPERTY(EditAnywhere)
		UAudioComponent* audioComponentOrder;
	UPROPERTY(EditAnywhere)
		UAudioComponent* audioComponentDeath;

public:

	UPROPERTY()
		AActor* targetActor;

	TArray<AActor*> entitiesInRange;

	UPROPERTY(EditAnywhere)
		float startingHealth = 250.0f;
	UPROPERTY(EditAnywhere)
		float detectRange = 500.0f;
	UPROPERTY(EditAnywhere)
		float attackRange = 250.0f;

public:
	// Sets default values for this character's properties
	AUNIT_Engineer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void ResetTarget() override;
};
