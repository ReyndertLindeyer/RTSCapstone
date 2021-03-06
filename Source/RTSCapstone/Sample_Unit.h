// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Interface
#include "I_Unit.h"
#include "I_Entity.h"

// Meshes
#include "Components/StaticMeshComponent.h"

#include "Sample_Unit.generated.h"

UCLASS()
class RTSCAPSTONE_API ASample_Unit : public ACharacter, public II_Unit, public II_Entity
{
	GENERATED_BODY()

// VARIABLES
public:

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SelectionIndicator;

	UPROPERTY(EditAnywhere)
		bool SetDestructible = true;

// FUNCTIONS
public: 	

	// Sets default values for this character's properties
	ASample_Unit();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	// OVERRIDDEN CLASS [II_Unit] -- Handles Unit Movement
	/// Disabled for now.  Keep for reference.
	//virtual void SetDestination(FVector destination) override;

	// OVERRIDDEN CLASS [II_Unit] -- Getter for AI Controller
	/// Disabled for now.  Keep for reference
	//virtual AController* GetUnitController() override;

	// OVERRIDDEN CLASS [II_Unit] -- Handles Unit Selection

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void DestroyEntity() override;

};
