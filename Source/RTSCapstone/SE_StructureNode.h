// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BuildingMaster.h"

#include "SE_StructureNode.generated.h"

UCLASS()
class RTSCAPSTONE_API ASE_StructureNode : public AActor
{
	GENERATED_BODY()
	
public:		// Functionality
	ASE_StructureNode();

	virtual void Tick(float DeltaTime) override;

protected:	// Functionality
	virtual void BeginPlay() override;

public:		// Variables

	UPROPERTY(EditAnywhere)
		USceneComponent* origin;

	UPROPERTY(EditAnywhere)
		UClass* structureReference;

};
