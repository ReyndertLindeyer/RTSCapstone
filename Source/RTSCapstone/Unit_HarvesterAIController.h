// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Unit_HarvesterAIController.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API AUnit_HarvesterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	void MoveOrder(FVector& location);

	virtual void Possess(APawn *InPawn) override;

	void GoToActor(AActor* inActor);
	
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

};
