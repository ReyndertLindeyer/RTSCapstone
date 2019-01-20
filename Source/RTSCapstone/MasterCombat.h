// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasterUnit.h"
#include "MasterCombat.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API AMasterCombat : public AMasterUnit
{
	GENERATED_BODY()
	
public:
	float attackRadius;

	AMasterCombat();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
