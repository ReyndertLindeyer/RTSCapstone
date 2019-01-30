// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "MyRTSAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
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
	UPROPERTY(EditAnywhere)
		UMaterial* regularMaterial;


	UCapsuleComponent* canPlace; //Temp capsule collider that will be used to see if the player is constructing the building close enough to another one

	float sightRadius, buildRadius;

	uint32 powerUsage, cost;

	uint32 numOfCollisions; //Number of buildings or units that the building is colliding with during placement

	bool constructed, isPlaced; //Is the building constructed, and has it been placed in the world

	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* buildingMesh;

	virtual void constructAtLocation();
	bool overlapping;
};
