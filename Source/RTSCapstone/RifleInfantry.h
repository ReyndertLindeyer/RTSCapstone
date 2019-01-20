// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasterInfantry.h"
#include "RifleInfantry.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ARifleInfantry : public AMasterInfantry
{
	GENERATED_BODY()
public:
	ARifleInfantry();
	virtual void BeginPlay() override;
	
};
