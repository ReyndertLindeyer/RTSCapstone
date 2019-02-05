// Fill out your copyright notice in the Description page of Project Settings.

#include "Unit_HarvesterAIController.h"

void AUnit_HarvesterAIController::Possess(APawn *InPawn) {
	Super::Possess(InPawn);
}


void AUnit_HarvesterAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) {

}