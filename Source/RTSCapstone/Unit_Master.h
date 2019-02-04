// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Unit_Master.generated.h"

UCLASS()
class RTSCAPSTONE_API AUnit_Master : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnit_Master();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UBehaviorTree *behavTree;

	float GetSightRadius();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	uint32 team, maxHealth, currentHealth, travelSpeed, spawnTime;

	float sightRadius;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* unitMesh;
};
