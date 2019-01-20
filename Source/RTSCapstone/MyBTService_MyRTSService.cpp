// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBTService_MyRTSService.h"


UMyBTService_MyRTSService::UMyBTService_MyRTSService() {
	//Makes a new instance for each AI
	bCreateNodeInstance = true;
}

void UMyBTService_MyRTSService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	//Get the AI controller of the tree that is passed in
	AMyRTSAIController *controller = Cast<AMyRTSAIController>(OwnerComp.GetAIOwner());

	if (controller) {
		controller->TargetPawn();
		/*
		//Find the enemy if they are a player controller
		AMasterMeleeEnemy *Enemy = Cast<AMasterMeleeEnemy>(GetWorld()->GetFirstPlayerController()->GetPawn());

		//If an enemy has been found set the key ID in the blueprint
		if(Enemy){
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(controller->EnemyKeyID, Enemy);
		}
		*/
	}
}