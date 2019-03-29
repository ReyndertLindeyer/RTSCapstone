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
#include "UNIT_AvBT.generated.h"

UCLASS()
class RTSCAPSTONE_API AUNIT_AvBT : public ACharacter, public II_Unit, public II_Entity
{
	GENERATED_BODY()

		// VARIABLES & COMPONENTS
public:

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BodyMesh;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SelectionIndicator;

	UPROPERTY(EditAnywhere)
		USceneComponent* barrelPos1;
	UPROPERTY(EditAnywhere)
		USceneComponent* barrelPos2;

	UPROPERTY(EditAnywhere)
		AActor* setPlayerOwner;
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
	//Particle System Missile
	UParticleSystem* PSM;

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
	TArray<AActor*> entitiesInRange;

	
	
	UPROPERTY(EditAnywhere)
		float startingHealth = 1500.0f;
	UPROPERTY(EditAnywhere)
		float detectRange = 2000.0f;

	UPROPERTY(EditAnywhere)
		float cannonRange = 1800.0f;
	UPROPERTY(EditAnywhere)
		float cannonDamage = 26.0f;
	UPROPERTY(EditAnywhere)
		float cannonTimer = 3.0f;

	UPROPERTY(EditAnywhere)
		float missileRange = 1800.0f;
	UPROPERTY(EditAnywhere)
		float missileDamage = 22.0f;

	bool secondShot = false;

public:
	// Sets default values for this character's properties
	AUNIT_AvBT();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
