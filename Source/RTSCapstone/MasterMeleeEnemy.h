// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasterCombat.h"
#include "MasterMeleeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API AMasterMeleeEnemy : public AMasterCombat
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
};
