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

	uint32 team, maxHealth, currentHealth, spawnTime;

	uint32 GetPowerUsage();
	uint32 GetCost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* root;
	
	bool selected, isBuilding; //isBuilding means is the building under construction

	AMyRTSAIController* rtsAI;

	UPROPERTY(EditAnywhere)
		UMaterial* canBuildIndicator;
	UPROPERTY(EditAnywhere)
		UMaterial* cantBuildIndicator;

	float sightRadius, buildRadius;

	uint32 powerUsage, cost;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* buildingMesh;

	virtual void constructAtLocation();
};
