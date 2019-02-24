// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "MyRTSAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/ObjectMacros.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Blueprint/UserWidget.h"

#include "Engine/DataTable.h"

#include "BuildingMaster.generated.h"


USTRUCT(BlueprintType)
struct FUnitVariables : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 BuildTime;
};

UCLASS()
class RTSCAPSTONE_API ABuildingMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingMaster();

	uint32 GetPowerUsage();
	uint32 GetCost();

	void EnableBuildDecal();
	void DisableBuildDecal();

	int32 GetHeight();
	float GetSightRadius();

	void SetSelection(bool selectionType);

	bool GetIsOverlapping();
	bool GetIsInRadius();

	bool IsSelected();

	bool IsDead();

	void Suicide();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool selected, isBuilding; //isBuilding means is the building under construction

	AMyRTSAIController* rtsAI;

	float sightRadius, buildRadius;

	uint32 powerUsage, cost;

	uint32 numOfBuildingCollisions, numOfRadiusCollisions; //Number of buildings or units that the building is colliding with during placement

	uint32 team, maxHealth, currentHealth, spawnTime;

	bool constructed, isPlaced; //Is the building constructed, and has it been placed in the world
	
	UPROPERTY(EditAnywhere)
		class UDecalComponent * selectedDecal; //Decal to show the building is selected

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* buildingMesh;

	UPROPERTY(EditAnywhere)
		USphereComponent* buildRadiusSphere;

	UPROPERTY(EditAnywhere)
		class UDecalComponent * decal; //Decal to show the buildings construction radius

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDataTable* unitConstructionDataTable;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* GetBuildingMesh();
		
	bool constructAtLocation();
	bool overlapping, isInRadius;

	UFUNCTION(BlueprintPure, Category = "UI")
		int GetHealth();
	UFUNCTION(BlueprintPure, Category = "UI")
		int GetMaxHealth();

	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);


	UFUNCTION()
		virtual void BeginRadiusOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
		virtual void OnRadiusOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
