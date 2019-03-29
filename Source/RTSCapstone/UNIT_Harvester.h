// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Components
#include "Components/SphereComponent.h"
#include "ResourceNode.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"

#include "Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"

// Interface Elements
#include "I_Unit.h"
#include "I_Entity.h"

#include "Building_Refinery.h"

// Meshes
#include "Components/StaticMeshComponent.h"

#include "UNIT_Harvester.generated.h"

UCLASS()
class RTSCAPSTONE_API AUNIT_Harvester : public ACharacter, public II_Unit, public II_Entity
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

	virtual bool GetSelection() override;

	// OVERRIDE CLASS [II_Unit] -- Handles Attack Orders
	virtual void AttackOrder(II_Entity* target) override;

	// OVERIDE CLASS [II_Entity] -- Handles any calls on death
	virtual void DestroyEntity() override;

	void ReturnToRefinery();
	void ReturnToRefinery(ABuilding_Refinery* refinery);
	
	void TargetNode(AResourceNode* node);
	void HarvestNode(AResourceNode* node);

	void PostInitializeComponents();


private:
	float currentTimer;
	float targetTimer = 3.0f;

	bool isSelected;

	bool returning;

	//Sound Stuff
	USoundCue* selectCue;
	USoundCue* orderCue;
	USoundCue* deathCue;
	USoundCue* idleCue;
	USoundCue* accelerateCue;
	USoundCue* driveCue;
	USoundCue* deccelerateCue;
	USoundCue* harvestCue;

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
	UPROPERTY(EditAnywhere)
		UAudioComponent* audioComponentHarvest;

public:
	TArray<AActor*> entitiesInRange;
	AResourceNode* targetNode;
	ABuilding_Refinery* targetRefinery;

	float currentResources, maxResources;

	UPROPERTY(EditAnywhere)
		float startingHealth = 1200.0f;
	UPROPERTY(EditAnywhere)
		float detectRange = 4000.0f;
	UPROPERTY(EditAnywhere)
		float harvestRate = 0.25f;

public:
	// Sets default values for this character's properties
	AUNIT_Harvester();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
