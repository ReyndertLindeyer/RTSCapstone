// Fill out your copyright notice in the Description page of Project Settings.

#include "UNIT_MOutpost.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUNIT_MOutpost::AUNIT_MOutpost()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent->SetWorldScale3D(FVector(0.25f));
	isSelected = false;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetRelativeScale3D(FVector(4.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Units/MOutpost/MOutpost_V1_UNREAL.MOutpost_V1_UNREAL'"));
	UStaticMesh* Asset = MeshAsset.Object;
	BodyMesh->SetStaticMesh(Asset);
	BodyMesh->SetRelativeLocation(FVector(0.0, 0.0f, -60.0f));
	BodyMesh->SetCanEverAffectNavigation(false);

	SelectionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Selection Indicator"));
	SelectionIndicator->SetupAttachment(BodyMesh);
	SelectionIndicator->SetVisibility(false);
	SelectionIndicator->SetWorldLocation(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BuildingMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Placeholder_Construction_Yard.Placeholder_Construction_Yard'"));
	BuildingAsset = BuildingMeshAsset.Object;

	buildingGhost = nullptr;

	hasRoom = false;

	currentTimer = 0.0f;
	unitState = UNIT_STATE::IDLE;
}

// Called when the game starts or when spawned
void AUNIT_MOutpost::BeginPlay()
{
	Super::BeginPlay();
	
	if (setPlayerOwner != nullptr)
		InitializeEntity(Cast<II_Player>(setPlayerOwner), "Mobile Outpost", startingHealth);

	SpawnDefaultController();

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->SetAvoidanceEnabled(true);
	GetCharacterMovement()->AvoidanceConsiderationRadius = 800.0f;
	GetCharacterMovement()->SetRVOAvoidanceWeight(1.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

// Called every frame
void AUNIT_MOutpost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	isDestructable = SetDestructible;

	// Detect all AActors within a Radius
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TArray<AActor*> ignoreActors;
	TArray<AActor*> outActors;

	ignoreActors.Add(this);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), detectRange, objectTypes, nullptr, ignoreActors, outActors);


	switch (unitState)
	{
	case UNIT_STATE::IDLE:
	case UNIT_STATE::SEEKING:
		//DrawDebugSphere(GetWorld(), GetActorLocation(), detectRange, 24, FColor(0, 0, 255));
		//DrawDebugSphere(GetWorld(), GetActorLocation(), attackRange, 24, FColor(255, 0, 0));
		break;
	case UNIT_STATE::ATTACKING:
		//DrawDebugSphere(GetWorld(), GetActorLocation(), attackRange, 24, FColor(255, 0, 0));
		break;
	case UNIT_STATE::MOVING:
		//DrawDebugSphere(GetWorld(), targetMoveDestination, 40.0, 3, FColor(0, 255, 0));  // How close I am to destination
		break;
	}


	// Narrow down all the AActors to only ones with an II_Entity script
	entitiesInRange.Empty();
	for (int i = 0; i < outActors.Num(); i++)
	{
		if (Cast<II_Entity>(outActors[i]))
		{
			if (!entitiesInRange.Contains(outActors[i]))
			{
				entitiesInRange.Add(outActors[i]);
			}
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("%i Actors In Radius"), entitiesInRange.Num());

	// IDLE STATE
	if (unitState == UNIT_STATE::IDLE)
	{
		SetDestination(GetController(), GetActorLocation());

		if (entitiesInRange.Num() > 0)
		{
			/// Check if entities are hostile

			// If there is a target, seek it.
			if (targetActor != nullptr)
				unitState = UNIT_STATE::SEEKING;

			// If there isn't a target, set a target.
			else
			{
				// Do Nothing
				
			}
				

		}
	}

	// MOVEMENT STATE
	if (unitState == UNIT_STATE::MOVING)
	{
		// Ignore Combat until unit reaches destination

		if (FVector::Dist(GetActorLocation(), targetMoveDestination) < 40.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("DESTINATION REACHED"));
			unitState = UNIT_STATE::IDLE;
		}

	}

	// SEEKING STATE
	if (unitState == UNIT_STATE::SEEKING)
	{
		if (targetActor == nullptr)
		{
			unitState = UNIT_STATE::IDLE;
		}

		else
		{
			//FVector targetLocation = Cast<AActor>(targetEntity)->GetActorLocation();
			//FVector moveDestination = targetLocation - ((GetActorLocation() - targetLocation) / 2);

			//// Target is out of range: move towards it.
			//if (FVector::Dist(GetActorLocation(), targetLocation) > attackRange)
			//{
			//	SetDestination(GetController(), moveDestination);
			//}

			//// Target is in range: Attack it.
			//else
			//{
			//	unitState = UNIT_STATE::ATTACKING;
			//}
		}
	}


	// ATTACK STATE
	if (unitState == UNIT_STATE::ATTACKING)
	{
		if (targetActor == nullptr)
			unitState = UNIT_STATE::IDLE;

		else
		{
			//FVector targetLocation = Cast<AActor>(targetEntity)->GetActorLocation();
			//FVector moveDestination = targetLocation - ((GetActorLocation() - targetLocation) / 2);

			//// Target is out of range: chase it;
			//if (FVector::Dist(GetActorLocation(), targetLocation) > attackRange)
			//{
			//	unitState = UNIT_STATE::SEEKING;
			//}

			//// Target is in range: attack it.
			//else
			//{
			//	SetDestination(GetController(), GetActorLocation());

			//	if (currentTimer >= 1.0f)
			//	{
			//		currentTimer = 0.0f;
			//		AttackOrder(targetEntity);
			//	}
			//}
		}

	}

	/// Attack Rate Timer
	// Will count down regardless of whether it's combat or not.
	// Think of it as a cooldown for an attack.
	if (currentTimer < 5.0f) {
		currentTimer += DeltaTime;
	}

}

// Called to bind functionality to input
void AUNIT_MOutpost::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AUNIT_MOutpost::SetSelection(bool state)
{
	isSelected = state;
	SelectionIndicator->SetVisibility(state);
}

bool AUNIT_MOutpost::GetSelection() {
	return isSelected;
}


// Function is Never Called
void AUNIT_MOutpost::AttackOrder(II_Entity* target)
{
	targetActor = target->GetActor();
}

bool AUNIT_MOutpost::StartGhostBuilding() {
	/*
	UE_LOG(LogTemp, Warning, TEXT("StartGhostBuilding"));
	if (buildingGhost != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("CheckGhost")); 
		buildingGhost->SetActorLocation(GetActorLocation());
		return buildingGhost->GetIsOverlapping();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("MakeGhost"));
		buildingGhost = GetWorld()->SpawnActor<ABuilding_Ghost>(ABuilding_Ghost::StaticClass(), GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		buildingGhost->SetIgnoreActor(this);
		buildingGhost->SetMesh(BuildingAsset, 6);

		return buildingGhost->GetIsOverlapping();
	}
	*/
	return false;
}

void AUNIT_MOutpost::StopGhostBuilding()
{
	/*
	UE_LOG(LogTemp, Warning, TEXT("StopGhost"));
	if (buildingGhost != nullptr) {
		buildingGhost->Destroy();
		buildingGhost = nullptr;
	}
	*/
}

void AUNIT_MOutpost::BuildGhostBuilding()
{
	/*/
	UE_LOG(LogTemp, Warning, TEXT("BuildGhost"));
	if (buildingGhost != nullptr) {
		buildingGhost->Destroy();
		buildingGhost = nullptr;
	}
	*/
	ABuildingMaster* tempBuilding = GetWorld()->SpawnActor<ABuilding_Outpost>(ABuilding_Outpost::StaticClass(), GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
	tempBuilding->InitializeEntity(GetEntityOwner(), "Outpost", 1000);
	tempBuilding->constructAtLocation(GetEntityOwner());
	DestroyEntity();
}

void AUNIT_MOutpost::DestroyEntity()
{
	// Remove from Owner's Array
	if (GetEntityOwner() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("I have died"));
		if (GetEntityOwner()->GetUnits().Contains(this))
		{
			for (int i = 0; i < GetEntityOwner()->GetUnits().Num(); i++) {
				if (GetEntityOwner()->GetUnits()[i])
					GetEntityOwner()->GetUnits().RemoveAt(i);
			}
		}

		if (GetEntityOwner()->GetBuildings().Contains(this))
		{
			for (int i = 0; i < GetEntityOwner()->GetBuildings().Num(); i++) {
				if (GetEntityOwner()->GetBuildings()[i])
					GetEntityOwner()->GetBuildings().RemoveAt(i);
			}
		}

		if (GetEntityOwner()->GetSelectedCharacters().Contains(this))
		{
			for (int i = 0; i < GetEntityOwner()->GetSelectedCharacters().Num(); i++) {
				if (GetEntityOwner()->GetSelectedCharacters()[i])
					GetEntityOwner()->GetSelectedCharacters().RemoveAt(i);
			}
		}
	}

	if (!UObject::IsValidLowLevel()) return;

	this->K2_DestroyActor();

	//GC
	GEngine->ForceGarbageCollection();
}