// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MyRTSAIController.h"
#include "MasterInfantry.h"
#include "MasterMeleeEnemy.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "MyBTService_MyRTSService.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API UMyBTService_MyRTSService : public UBTService
{
	GENERATED_BODY()
	
public:
	UMyBTService_MyRTSService();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
