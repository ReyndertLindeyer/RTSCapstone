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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UINT32 yield;

	bool needsRespawning; //If the resource can start its respawn procedure

	float respawnTime; //How long until the resource can respawn

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;

	void setup(UINT32 resourceYield); //Set the yield of the resource node
};