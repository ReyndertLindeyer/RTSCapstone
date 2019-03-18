// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Components
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"

// Interface
#include "I_Unit.h"
#include "I_Entity.h"

// Meshes
#include "Components/StaticMeshComponent.h"

#include "GameFramework/Character.h"
#include "UNIT_MBT.generated.h"

UCLASS()
class RTSCAPSTONE_API AUNIT_MBT : public ACharacter, public II_Unit, public II_Entity
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

	// FUNCTIONS
public:
	// OVERRIDDEN CLASS [II_Unit] -- Handles Unit Selection
	virtual void SetSelection(bool state) override;

	// OVERRIDE CLASS [II_Unit] -- Handles Attack Orders
	virtual void AttackOrder(II_Entity* target) override;

	// OVERIDE CLASS [II_Entity] -- Handles any calls on death
	virtual void DestroyEntity() override;


private:
	float currentTimer;
	float targetTimer = 3.0f;

	UParticleSystem* PS;

	//The particle system that will be the reaction at the end, ex the explosion when the rocket connects
	UParticleSystem* reactionPS;

public:
	TArray<AActor*> entitiesInRange;

	UPROPERTY(EditAnywhere)
		float startingHealth = 650.0f;
	UPROPERTY(EditAnywhere)
		float attackRate = 2.5f;
	UPROPERTY(EditAnywhere)
		float attackDamage = 7.0f;
	UPROPERTY(EditAnywhere)
		float detectRange = 500.0f;
	UPROPERTY(EditAnywhere)
		float attackRange = 250.0f;

public:
	// Sets default values for this character's properties
	AUNIT_MBT();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
