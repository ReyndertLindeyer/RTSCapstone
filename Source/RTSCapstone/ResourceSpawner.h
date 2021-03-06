// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceNode.h"
#include "Components/CapsuleComponent.h"
#include "ResourceSpawner.generated.h"

UCLASS()
class RTSCAPSTONE_API AResourceSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* root;

	UPROPERTY(EditAnywhere)
		int richness; //Distance between objects

	UPROPERTY(EditAnywhere)
		float spawnRadius;

	UPROPERTY(EditAnywhere)
		float richSpawnRadius;


};
