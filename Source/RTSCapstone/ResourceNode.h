// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ResourceNode.generated.h"

UCLASS()
class RTSCAPSTONE_API AResourceNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceNode();

	void setup(UINT32 resourceYield, bool canRespawn); //Set the yield of the resource node

	float Harvest();

	float RemainingResources();

	bool isOccupied;

private:

	bool respawnable;

	float yield;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool needsRespawning; //If the resource can start its respawn procedure

	float respawnTime; //How long until the resource can respawn

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;
};
