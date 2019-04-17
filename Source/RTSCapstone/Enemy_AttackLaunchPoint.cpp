// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_AttackLaunchPoint.h"

// Sets default values
AEnemy_AttackLaunchPoint::AEnemy_AttackLaunchPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	collectionRadius = 800;

	radiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("baseRadius"));
	radiusSphere->SetSphereRadius(collectionRadius);
	radiusSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called when the game starts or when spawned
void AEnemy_AttackLaunchPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_AttackLaunchPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Detect all AActors within a Radius
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TArray<AActor*> ignoreActors;
	TArray<AActor*> outActors;

	ignoreActors.Add(this);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 800, objectTypes, nullptr, ignoreActors, outActors);

	basicArrayA.Empty();
	basicArrayB.Empty();
	basicArrayC.Empty();
	basicArrayD.Empty();

	for (int i = 0; i < outActors.Num(); i++)
	{
		if (Cast<II_Entity>(outActors[i]))
		{
			///Check to see if the colliding unit is an enemy unit, what kind it is, and add it to the correct array
			if (Cast<AUNIT_Grinder>(outActors[i])) {
				basicArrayA.Add(Cast<AUNIT_Grinder>(outActors[i]));
			}
			if (Cast<AUNIT_Gattling>(outActors[i])) {
				basicArrayB.Add(Cast<AUNIT_Gattling>(outActors[i]));
			}
			if (Cast<AUNIT_Roomba>(outActors[i])) {
				basicArrayC.Add(Cast<AUNIT_Roomba>(outActors[i]));
			}
			if (Cast<AUNIT_Prism>(outActors[i])) {
				basicArrayD.Add(Cast<AUNIT_Prism>(outActors[i]));
			}
		}
	}

	///If all of the arrays are full of the correct unit then launch a wave towards the nearest player structure
	if (basicArrayA.Num() >= numOfBasicMelee && basicArrayB.Num() >= numOfBasicRanged && basicArrayC.Num() >= numOfAdvancedMelee && basicArrayD.Num() >= numOfAdvancedRanged) { //Add more variables for more units
		int32 closestBuilding = 100000;
		AActor* tempActor = nullptr;
		TArray<AActor*> tempArray;

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_PowerPlant::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}				
			}
		}

		tempArray.Empty();

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_Refinery::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}
			}
		}

		tempArray.Empty();

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_Barrecks::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}
			}
		}

		tempArray.Empty();

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_VehicleFactory::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}
			}
		}

		tempArray.Empty();

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_TechCenter::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}
			}
		}

		tempArray.Empty();

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_Superweapon::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}
			}
		}

		tempArray.Empty();

		if (tempActor != nullptr) {
			for (int i = 0; i < basicArrayA.Num(); i++) {
				basicArrayA[i]->MoveOrder(basicArrayA[i]->GetController(), tempActor->GetActorLocation());
			}
			for (int i = 0; i < basicArrayB.Num(); i++) {
				basicArrayB[i]->MoveOrder(basicArrayB[i]->GetController(), tempActor->GetActorLocation());
			}
			for (int i = 0; i < basicArrayC.Num(); i++) {
				basicArrayC[i]->MoveOrder(basicArrayC[i]->GetController(), tempActor->GetActorLocation());
			}
		}

	}
}

void AEnemy_AttackLaunchPoint::EnableTick()
{
	PrimaryActorTick.bCanEverTick = true;
}

