// Fill out your copyright notice in the Description page of Project Settings.

#include "Unit_HarvesterAIController.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void AUnit_HarvesterAIController::Possess(APawn *InPawn) {
	Super::Possess(InPawn);
}

void AUnit_HarvesterAIController::GoToActor(AActor * inActor)
{
	MoveToActor(inActor, 1.0f, true, true, false, 0, true);
}


void AUnit_HarvesterAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) {

}