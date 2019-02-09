// Fill out your copyright notice in the Description page of Project Settings.

#include "Unit_Harvester.h"

AUnit_Harvester::AUnit_Harvester() {
	PrimaryActorTick.bCanEverTick = true;
	maxHealth = 200;
	currentHealth = maxHealth;
	travelSpeed = 10;
	spawnTime = 20;

	harvestingSpeed = 5;

	sightRadius = 600;
	hasTarget = false;
	hasReachedResource = false;
	maxResourseLoad = 100;
	resourseload = 0;

	resourceTarget = NULL;

	unitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HarvesterMesh"));
	unitMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/yeetHarvyDev.yeetHarvyDev")).Get());
	unitMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = unitMesh;
	unitMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	unitMesh->SetCollisionProfileName(TEXT("Trigger"));
	unitMesh->OnComponentBeginOverlap.AddDynamic(this, &AUnit_Harvester::BeginOverlap);
	unitMesh->OnComponentEndOverlap.AddDynamic(this, &AUnit_Harvester::OnOverlapEnd);
	unitMesh->SetSimulatePhysics(false);

	Controller = CreateDefaultSubobject<AUnit_HarvesterAIController>("HarvesterAI");

	collectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	collectionSphere->InitSphereRadius(sightRadius);
	collectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AUnit_Harvester::BeginOverlap);
	collectionSphere->OnComponentEndOverlap.AddDynamic(this, &AUnit_Harvester::OnOverlapEnd);
	collectionSphere->SetupAttachment(RootComponent);
}


void AUnit_Harvester::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!hasTarget || resourceTarget->RemainingResources() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Check for Targets"));
		hasReachedResource = false;
		MoveToNextLocation();
	}
	if (hasTarget) {
		if (resourceTarget->GetDistanceTo(this) <= 5) {
			resourceTarget->Harvest(harvestingSpeed);
			resourseload += harvestingSpeed;
			hasReachedResource = true;
		}
	}
}

void AUnit_Harvester::MoveToNextLocation()
{
	//Getting the units Controller
	AUnit_HarvesterAIController* myController = Cast<AUnit_HarvesterAIController>(GetController());

	//Checking if the controller was gotten correctly
	if (myController) {

		UE_LOG(LogTemp, Warning, TEXT("%d"), foundResources.Num());
		//If the harvester does not have a full load then look for another resource node to harvest from, if more than 0 have been found it then loops through them all to find the closest
		if (resourseload < maxResourseLoad) {
			if (foundResources.Num() > 0) {
				int shortestNode = 10000000;
				for (int j = 0; j < foundResources.Num(); j++) {
					if (foundResources[j]->GetDistanceTo(this) < shortestNode) {
						resourceTarget = (AResourceNode*)foundResources[j];
					}
				}
				myController->GoToActor(resourceTarget);
				hasTarget = true;
				UE_LOG(LogTemp, Warning, TEXT("Moving"));
				return;
			}
		}

		/*
		//If the harvester does have a full load then look for a refinery and travel to the closest one
		if (resourseload == maxResourseLoad) {
			if (refineryHits.Num() > 0) {
				int shortestNode = 10000000;
				FHitResult shortestResult;
				for (int j = 0; j < refineryHits.Num(); j++) {
					if (refineryHits[j].Distance < shortestNode) {
						shortestResult = refineryHits[j];
					}
				}
				myController->MoveToActor(resourceTarget, 1.0f, false);
				return;
			}
		}*/

		/*
		//Checking the area around the harvester within its sight radius
		TArray<struct FHitResult> hitResults;
		GetWorld()->SweepMultiByObjectType(hitResults, this->GetActorLocation(), this->GetActorLocation() + FVector::ForwardVector * 360, FQuat(), ECC_Pawn, FCollisionShape::MakeSphere(this->GetSightRadius()));

		//Looping through all hit results
		for (int i = 0; i < hitResults.Num(); i++) {

			UE_LOG(LogTemp, Warning, TEXT("FoundSomething"));
			//If the harvester does not have a full load then look for another resource node to harvest from, if more than 0 have been found it then loops through them all to find the closest
			if (resourseload < maxResourseLoad) {
				TArray<struct FHitResult> resourceHits;
				for (int j = 0; j < hitResults.Num(); j++) {
					if (hitResults[j].GetActor()->ActorHasTag(FName("ResourceNode"))) {
						resourceHits.Add(hitResults[j]);
					}
				}
				if (resourceHits.Num() > 0) {
					int shortestNode = 10000000;
					FHitResult shortestResult;
					for (int j = 0; j < resourceHits.Num(); j++) {
						if (resourceHits[j].Distance < shortestNode) {
							shortestResult = resourceHits[j];
						}
					}
					resourceTarget = (AResourceNode*)shortestResult.GetActor();
					myController->MoveToLocation(shortestResult.Location, 1.0f, false);
					break;
				}
			}

			//If the harvester does have a full load then look for a refinery and travel to the closest one
			if (resourseload == maxResourseLoad) {
				TArray<struct FHitResult> refineryHits;
				for (int j = 0; j < hitResults.Num(); j++) {
					if (hitResults[j].GetActor()->ActorHasTag(FName("Refinery"))) {
						refineryHits.Add(hitResults[j]);
					}
				}
				if (refineryHits.Num() > 0) {
					int shortestNode = 10000000;
					FHitResult shortestResult;
					for (int j = 0; j < refineryHits.Num(); j++) {
						if (refineryHits[j].Distance < shortestNode) {
							shortestResult = refineryHits[j];
						}
					}
					myController->MoveToLocation(shortestResult.Location, 1.0f, false);
					break;
				}
			}
		}*/
	}
}

void AUnit_Harvester::BeginPlay()
{
	Super::BeginPlay();
	MoveToNextLocation();
}

void AUnit_Harvester::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) 
{
	//if (OverlappedComponent->CurrentTag == collectionSphere->CurrentTag) {
		if (OtherActor->ActorHasTag(FName("ResourceNode"))) {
			foundResources.AddUnique((AResourceNode*)OtherActor);
		}
	//}
	/*if (OverlappedComponent == unitMesh) {
		if (resourceTarget != NULL) {
			if (OtherActor == resourceTarget) {
				hasReachedResource = true;
			}
		}
	}*/
}

void AUnit_Harvester::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappedComp == collectionSphere) {
		if (OtherActor->ActorHasTag(FName("ResourceNode"))) {
			foundResources.Remove((AResourceNode*)OtherActor);
		}
	}

}