// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"

// Components
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"

#include "Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"

// Interface
#include "I_Unit.h"
#include "I_Entity.h"

// Meshes
#include "Components/StaticMeshComponent.h"

#include "GameFramework/Character.h"
#include "UNIT_Scout.generated.h"

UCLASS()
class RTSCAPSTONE_API AUNIT_Scout : public ACharacter, public II_Unit, public II_Entity
{
	GENERATED_BODY()

		// VARIABLES & COMPONENTS
public:

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BodyMesh;
	
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere)
		USceneComponent* barrelPos;
	
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

	//Particle System Cannon
	UParticleSystem* PSC;

	//The particle system that will be the reaction at the end, ex the explosion when the rocket connects
	UParticleSystem* reactionPS;

	//Sound Stuff
	USoundCue* fireCue;
	USoundCue* selectCue;
	USoundCue* orderCue;
	USoundCue* deathCue;
	USoundCue* idleCue;
	USoundCue* accelerateCue;
	USoundCue* driveCue;
	USoundCue* deccelerateCue;
	USoundCue* harvestCue;

	UPROPERTY(EditAnywhere)
		UAudioComponent* audioComponentFire;
	UPROPERTY(EditAnywhere)
		UAudioComponent* audioComponentSelect;
	UPROPERTY(EditAnywhere)
		UAudioComponent* audioComponentOrder;
	UPROPERTY(EditAnywhere)
		UAudioComponent* audioComponentDeath;
	UPROPERTY(EditAnywhere)
		UAudioComponent* audioComponentIdle;
	UPROPERTY(EditAnywhere)
		UAudioComponent* audioComponentAccelerate;
	UPROPERTY(EditAnywhere)
		UAudioComponent* audioComponentDrive;
	UPROPERTY(EditAnywhere)
		UAudioComponent* audioComponentDeccelerate;

public:

	UPROPERTY()
		AActor* targetActor;

	UPROPERTY()
		TArray<AActor*> entitiesInRange;

	UPROPERTY(EditAnywhere)
		float startingHealth = 700.0f;
	UPROPERTY(EditAnywhere)
		float detectRange = 1500.0f;

	UPROPERTY(EditAnywhere)
		float attackRange = 1300.0f;
	UPROPERTY(EditAnywhere)
		float attackDamage = 15.0f;
	UPROPERTY(EditAnywhere)
		float attackTimer = 0.5f;

	//######################################################

public:
	// Sets default values for this character's properties
	AUNIT_Scout();

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
