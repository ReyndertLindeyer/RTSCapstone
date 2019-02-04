// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Unit_Master.h"
#include "MasterUnit.h"
#include "MyRTSAIController.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API AMyRTSAIController : public AAIController
{
	GENERATED_BODY()

public:

	AMyRTSAIController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual void Possess(APawn *InPawn) override;

	uint8 EnemyKeyID;

	AUnit_Master* unit;

	//Transient means set to zero initially
	UPROPERTY(transient)
		UBlackboardComponent *blackboardComp;

	UPROPERTY(transient)
		UBehaviorTreeComponent *behaviorTree;

	UFUNCTION()
		TArray<struct FHitResult> DetectPawns();
	UFUNCTION()
		void TargetPawn();
	UFUNCTION()
		void MoveOrder(FVector& location);


};
