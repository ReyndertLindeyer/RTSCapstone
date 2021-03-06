// Fill out your copyright notice in the Description page of Project Settings.

#include "I_Unit.h"

// Add default functionality here for any II_Unit functions that are not pure virtual.

void II_Unit::SetDestination(AController* controller, FVector destination)
{
	Cast<AAIController>(controller)->MoveToLocation(destination, 40, false, true, false, true, 0, true);
}

void II_Unit::MoveOrder(AController* controller, FVector destination)
{
	targetMoveDestination = destination;
	SetDestination(controller, destination);
	unitState = UNIT_STATE::MOVING;
	ResetTarget();
	overrideAI = true;
}

void II_Unit::ResetTarget() 
{
	// Virtual Function
}

void II_Unit::SetSelection(bool state, II_Player* inPlayer)
{
	// Do Nothing
}

bool II_Unit::GetSelection()
{
	return false;
}

void II_Unit::AttackOrder(II_Entity* target)
{
	// Do nothing
}
