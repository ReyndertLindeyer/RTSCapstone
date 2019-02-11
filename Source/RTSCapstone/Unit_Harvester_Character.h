// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasterUnit.h"
#include "ResourceNode.h"
#include "Unit_HarvesterAIController.h"
#include "Components/SphereComponent.h"
#include "Unit_Harvester_Character.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API AUnit_Harvester_Character : public AMasterUnit
{
	GENERATED_BODY()

public:
	AUnit_Harvester_Character();

	void MoveToNextLocation();

private:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	int resourseload, maxResourseLoad;

	uint32 harvestingSpeed;

	AUnit_HarvesterAIController* controller;

	bool hasTarget, hasReachedResource;

	AResourceNode* resourceTarget;

	USphereComponent* collectionSphere;

	TArray <AResourceNode*> foundResources;
};
