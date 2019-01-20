// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBTTask_MoveAndAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MyRTSAIController.h"
#include "MasterInfantry.h"
#include "MasterMeleeEnemy.h"

EBTNodeResult::Type UMyBTTask_MoveAndAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AMyRTSAIController* EnemyPC = Cast<AMyRTSAIController>(OwnerComp.GetAIOwner());

	AMasterMeleeEnemy *Enemy = Cast<AMasterMeleeEnemy>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(EnemyPC->EnemyKeyID));

	if (Enemy) {
		EnemyPC->MoveToActor(Enemy, 5.0f, true, true, true, 0, true);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}