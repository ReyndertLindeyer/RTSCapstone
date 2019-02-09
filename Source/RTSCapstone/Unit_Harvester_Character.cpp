// Fill out your copyright notice in the Description page of Project Settings.

#include "Unit_Harvester_Character.h"


AUnit_Harvester_Character::AUnit_Harvester_Character() {
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

	charMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HarvesterMesh"));
	charMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/yeetHarvyDev.yeetHarvyDev")).Get());
	charMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = charMesh;
	charMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	charMesh->SetCollisionProfileName(TEXT("Trigger"));
	charMesh->OnComponentBeginOverlap.AddDynamic(this, &AUnit_Harvester_Character::BeginOverlap);
	charMesh->OnComponentEndOverlap.AddDynamic(this, &AUnit_Harvester_Character::OnOverlapEnd);
	charMesh->SetSimulatePhysics(false);

	GetCapsuleComponent()->SetupAttachment(RootComponent);
	GetCapsuleComponent()->SetCapsuleSize(0.1f, 0.2f, true);

	//AIControllerClass = AUnit_HarvesterAIController::StaticClass();

	collectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	collectionSphere->InitSphereRadius(sightRadius);
	collectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AUnit_Harvester_Character::BeginOverlap);
	collectionSphere->OnComponentEndOverlap.AddDynamic(this, &AUnit_Harvester_Character::OnOverlapEnd);
	collectionSphere->SetupAttachment(RootComponent);
}

void AUnit_Harvester_Character::BeginPlay()
{
	Super::BeginPlay();
	MoveToNextLocation();
}

void AUnit_Harvester_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!hasTarget) {
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

void AUnit_Harvester_Character::MoveToNextLocation()
{
	/*/
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
		}

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
		}
	}*/
}

void AUnit_Harvester_Character::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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


void AUnit_Harvester_Character::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OverlappedComp == collectionSphere) {
		if (OtherActor->ActorHasTag(FName("ResourceNode"))) {
			foundResources.Remove((AResourceNode*)OtherActor);
		}
	}
}