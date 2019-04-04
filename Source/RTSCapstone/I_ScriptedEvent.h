// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "I_Player.h"

#include "I_ScriptedEvent.generated.h"

class SE_PARAMS
{
public:		// Functionality
	SE_PARAMS* BuildParams(II_Player* targetPlayer);

private:	// Functionality
	

public:		// Variables
	II_Player* targetPlayer;


};

UINTERFACE(MinimalAPI)
class UI_ScriptedEvent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTSCAPSTONE_API II_ScriptedEvent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void TriggerEvent(II_Player* params);
	TArray<AActor*> triggerActors;
	
};
