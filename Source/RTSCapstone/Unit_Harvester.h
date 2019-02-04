// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit_Master.h"
#include "Unit_Harvester.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API AUnit_Harvester : public AUnit_Master
{
	GENERATED_BODY()

public:
	AUnit_Harvester();
	
private:
	uint32 harvestingSpeed;

	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

};