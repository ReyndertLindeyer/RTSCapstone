// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "I_ScriptedEvent.h"
#include "I_Entity.h"


#include "SE_TransferOwnership.generated.h"

UCLASS()
class RTSCAPSTONE_API ASE_TransferOwnership : public AActor, public II_ScriptedEvent
{
	GENERATED_BODY()
	
public:		// Functionality
	
	ASE_TransferOwnership();

	virtual void Tick(float DeltaTime) override;

	

protected:	// Functionality
	
	virtual void BeginPlay() override;
	virtual void TriggerEvent(II_Player* targetPlayer) override;

public:		// Variables	
	
	UPROPERTY(EditAnywhere)
		TArray<AActor*> SetTriggerActors;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> inheritedUnits;
	
	UPROPERTY(EditAnywhere)
		TArray<AActor*> inheritedStructures;

	

};
