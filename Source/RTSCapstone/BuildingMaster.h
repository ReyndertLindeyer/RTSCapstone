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

// INTERFACE
#include "I_Entity.h"
#include "I_Player.h"

#include "Engine/StaticMesh.h"

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
class RTSCAPSTONE_API ABuildingMaster : public AActor, public II_Entity
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingMaster();

	void EnableBuildDecal();
	void DisableBuildDecal();

	int32 GetHeight();
	float GetConstructionRadius();
	float GetSightRadius();

	void SetSelection(bool selectionType);

	bool IsSelected();

	bool IsDead();

	void Suicide();

	// Entity Interface
	void DestroyEntity() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called in the constructAtLocation() method.
	// Functionality needs to be implemented on each building
	void InitializeStructure(II_Player* player, FString name, float health);

	bool selected, isBuilding; //isBuilding means is the building under construction

	//Whether or not the building's radius should be on
	bool buildRadiusActive;

	float sightRadius, buildRadius;

	uint32 team, spawnTime;

	bool constructed; //Is the building constructed, and has it been placed in the world
	
	UPROPERTY(EditAnywhere)
		class UDecalComponent * selectedDecal; //Decal to show the building is selected

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* buildingMesh;

	UPROPERTY(EditAnywhere)
		class UDecalComponent * decal; //Decal to show the buildings construction radius

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDataTable* unitConstructionDataTable;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* GetBuildingMesh();
		
	bool constructAtLocation(II_Player* player);
};
