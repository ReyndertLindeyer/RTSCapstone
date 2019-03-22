// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building_Enemy_Spawner.h"
#include "Enemy_AttackLaunchPoint.h"
#include "ResourceSpawner.h"
#include "Components/SphereComponent.h"
#include "Enemy_BaseManager.generated.h"

UCLASS()
class RTSCAPSTONE_API AEnemy_BaseManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_BaseManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		UStaticMeshComponent* baseSphere; //Collision sphere to get a reference to all structures in the area
	UPROPERTY()
		USphereComponent* baseRadiusSphere; //Collision sphere to get a reference to all structures in the area

	UPROPERTY()
		TArray<UStaticMesh*> baseMeshes;

	UPROPERTY()
		TArray <float> counterArray;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		bool isStartingArea; //If this is next to the starting area then the level creator should tick it to true

	void ActivateManager(); //Activate all of the buildings that the manager controls

	UPROPERTY(EditAnywhere)
		TArray <AEnemy_BaseManager*> adjacentManagerArray; //To be filled by level creator so that the ActivateManager classes of the adjacent areas can be called

	UPROPERTY(EditAnywhere)
		TArray <ABuilding_Enemy_Spawner*> buildingsArray; //List of all buildings in the area

	UPROPERTY(EditAnywhere)
		AEnemy_AttackLaunchPoint* launchPoint; //The area that the waves of enemies are going to be launched from

};
