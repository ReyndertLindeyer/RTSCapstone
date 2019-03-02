// Fill out your copyright notice in the Description page of Project Settings.

#include "I_Unit.h"

// Add default functionality here for any II_Unit functions that are not pure virtual.

void II_Unit::SetDestination(AController* controller, FVector destination)
{
	targetMoveDestination = destination;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, destination);
}

void II_Unit::MoveOrder(AController* controller, FVector destination)
{
	SetDestination(controller, destination);
	unitState = UNIT_STATE::MOVING;
}


void II_Unit::SetSelection(bool state)
{
	// Do Nothing
}

void II_Unit::AttackOrder(II_Entity* target)
{
	// Do nothing
}
