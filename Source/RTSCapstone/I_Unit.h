// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "AIController.h"
#include "I_Entity.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "I_Unit.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_Unit : public UInterface
{
	GENERATED_BODY()
};

UENUM()
enum class UNIT_STATE {
	IDLE		UMETA(DisplayName = "Idle"),
	MOVING		UMETA(DisplayName = "Moving"),
	SEEKING		UMETA(DisplayName = "Seeking"),
	ATTACKING	UMETA(DisplayName = "Attacking")
};


class RTSCAPSTONE_API II_Unit
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	/// Disabled for now.  Keep for reference.  
	/// Right now we have movement that doesn't need this.  However, when we implement more complex movement, we might need it.
	void SetDestination(AController* controller, FVector destination);	// Allows AI to Decision Making 
	void MoveOrder(AController* controller, FVector destination);		// Overrides AI Decision Making

	virtual void SetSelection(bool state);
	virtual void AttackOrder(II_Entity* target);
	
	II_Entity* targetEntity;
	FVector targetMoveDestination;

	UNIT_STATE unitState;

	
};