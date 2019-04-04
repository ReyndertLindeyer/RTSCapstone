// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "I_ScriptedEvent.h"
#include "SE_StructureNode.h"

#include "SE_EstablishBase.generated.h"

UCLASS()
class RTSCAPSTONE_API ASE_EstablishBase : public AActor, public II_ScriptedEvent
{
	GENERATED_BODY()
	
public:		// Functionality
	ASE_EstablishBase();

	virtual void Tick(float DeltaTime) override;

	

protected:	// Functionality
	virtual void BeginPlay() override;
	virtual void TriggerEvent(II_Player* params) override;

public:		// Variables

	UPROPERTY(EditAnywhere)
		TArray<AActor*> SetTriggerActors;

	UPROPERTY(EditAnywhere)
		TArray<ASE_StructureNode*> structureNodes;

};
