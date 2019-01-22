// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "MyRTSAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/ObjectMacros.h"
#include "BuildingMaster.generated.h"

UCLASS()
class RTSCAPSTONE_API ABuildingMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingMaster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* root;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* buildingMesh;
	
	bool selected, isBuilding;

	AMyRTSAIController* rtsAI;

	UINT32 team, maxHealth, currentHealth, powerUsage, spawnTime, cost;

	float sightRadius;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
