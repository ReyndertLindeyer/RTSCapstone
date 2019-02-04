// Fill out your copyright notice in the Description page of Project Settings.

#include "MyRTSAIController.h"


AMyRTSAIController::AMyRTSAIController() {

	PrimaryActorTick.bCanEverTick = true;

	blackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	behaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
}



void AMyRTSAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMyRTSAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ARifleInfantry* character = Cast<ARifleInfantry>(GetPawn());
}

void AMyRTSAIController::Possess(APawn *InPawn) {
	Super::Possess(InPawn);

	unit = Cast<AUnit_Master>(InPawn);

	FString Path = "/Game/Game_Assets/AI/NewBehaviorTree01";
	unit->behavTree = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, *Path));

	if (unit && unit->behavTree) {
		blackboardComp->InitializeBlackboard(*unit->behavTree->BlackboardAsset);

		EnemyKeyID = blackboardComp->GetKeyID("Target");

		RunBehaviorTree(unit->behavTree);
	}
}

//Will Check for pawns that aren't of the same type as the player
TArray<struct FHitResult> AMyRTSAIController::DetectPawns()
{
	//Will check the area around the unit using its location, the sight radius, and the collision channel Pawn. The output of found actors will be put into the HitResults TArray
	TArray<struct FHitResult> hitResults;
	GetWorld()->SweepMultiByObjectType(hitResults, GetPawn()->GetActorLocation(), GetPawn()->GetActorLocation() + FVector::ForwardVector * unit->GetSightRadius(), FQuat(), ECC_Pawn, FCollisionShape::MakeSphere(unit->GetSightRadius()));
	return hitResults;
}

//Will Check which pawn is closest and return it
void AMyRTSAIController::TargetPawn()
{
	/*if (!unit->isCombat) {
		return;
	}*/

	TArray<struct FHitResult> hitResults = DetectPawns();
	
	//Runs through all of the hit results and skips past any nullptr's and any instances of itself
	for (int i = 0; i < hitResults.Num(); i++) {
		if (hitResults[i].Actor == nullptr)
		{
			continue;
		}

		if (hitResults[i].Actor == GetPawn())
		{
			continue;
		}

		//Gets the actor of the found character
		auto targetCharacter = hitResults[i].Actor.Get();

		//If it did find something then check to see if it is on the player's side
		if (targetCharacter) {
		}

		//Code for checking if the found actor that is not on the players side is attackable

		Blackboard->SetValueAsObject(TEXT("TargetActor"), hitResults[i].Actor.Get());

		return;
	}
}

void AMyRTSAIController::MoveOrder(FVector & location)
{
	Blackboard->SetValueAsVector(TEXT("TargetLocation"), location);
}
