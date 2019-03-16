// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingMaster.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Building_Ghost.generated.h"

UCLASS()
class RTSCAPSTONE_API ABuilding_Ghost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding_Ghost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* buildingMesh;

	bool overlapping, isInRadius;

	int32 numOfBuildingCollisions, radius;

	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetIsOverlapping();
	bool GetIsInRadius();

	UStaticMeshComponent* GetBuildingMesh();

	void SetMesh(UStaticMesh* inMesh);
};
