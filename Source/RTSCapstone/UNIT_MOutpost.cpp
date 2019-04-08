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

	SetHitRadius(160);

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

	overrideAI = false;

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

		FHitResult* rayCastOne = new FHitResult();
		FHitResult* rayCastTwo = new FHitResult();

		FVector StartTrace = BodyMesh->GetComponentLocation() + (BodyMesh->GetForwardVector() * 400);

		FVector ForwardVectorOne = BodyMesh->GetForwardVector();
		FVector ForwardVectorTwo = BodyMesh->GetForwardVector();

		ForwardVectorOne = ForwardVectorOne.RotateAngleAxis(135, FVector(0.0, 0.0, 1.0));
		ForwardVectorTwo = ForwardVectorTwo.RotateAngleAxis(-135, FVector(0.0, 0.0, 1.0));

		FVector EndTraceOne = ((ForwardVectorOne * 180) + StartTrace);
		FVector EndTraceTwo = ((ForwardVectorTwo * 180) + StartTrace);

		FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

		DrawDebugLine(GetWorld(), StartTrace, EndTraceOne, FColor(255, 0, 0), false, 1);
		if (GetWorld()->LineTraceSingleByChannel(*rayCastOne, StartTrace, EndTraceOne, ECC_Visibility, *TraceParams)) {
			if (Cast<II_Unit>(rayCastOne->GetActor())) {
				FVector push = (rayCastOne->GetActor()->GetActorLocation() - GetActorLocation());
				push = FVector(push.X / 7, push.Y / 7, push.Z) + (BodyMesh->GetRightVector() * 2);
				rayCastOne->GetActor()->SetActorLocation(FVector(rayCastOne->GetActor()->GetActorLocation().X + push.X, rayCastOne->GetActor()->GetActorLocation().Y + push.Y, rayCastOne->GetActor()->GetActorLocation().Z));
			}
		}

		DrawDebugLine(GetWorld(), StartTrace, EndTraceTwo, FColor(255, 0, 0), false, 1);
		if (GetWorld()->LineTraceSingleByChannel(*rayCastTwo, StartTrace, EndTraceTwo, ECC_Visibility, *TraceParams)) {
			if (Cast<II_Unit>(rayCastTwo->GetActor())) {
				FVector push = (rayCastTwo->GetActor()->GetActorLocation() - GetActorLocation());
				push = FVector(push.X / 7, push.Y / 7, push.Z) + (-BodyMesh->GetRightVector() * 2);
				rayCastTwo->GetActor()->SetActorLocation(FVector(rayCastTwo->GetActor()->GetActorLocation().X + push.X, rayCastTwo->GetActor()->GetActorLocation().Y + push.Y, rayCastTwo->GetActor()->GetActorLocation().Z));
			}
		}

		if (FVector::Dist(GetActorLocation(), targetMoveDestination) < 40.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("DESTINATION REACHED"));
			unitState = UNIT_STATE::IDLE;
			overrideAI = false;
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
			//if (FVector::Dist(GetActorLocation(), targetLocation) > attackRange + Cast<II_Entity>(targetActor)->GetHitRadius())
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
			//if (FVector::Dist(GetActorLocation(), targetLocation) > attackRange + Cast<II_Entity>(targetActor)->GetHitRadius())
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

void AUNIT_MOutpost::ResetTarget()
{
	targetActor = nullptr;
}


void AUNIT_MOutpost::SetSelection(bool state, II_Player* inPlayer)
{
	isSelected = state;
	if (!selectingPlayerArray.Contains(inPlayer) && state == true) {
		selectingPlayerArray.Add(inPlayer);
	}
	else if (selectingPlayerArray.Contains(inPlayer) && state == false) {
		for (int i = 0; i < selectingPlayerArray.Num(); i++) {
			if (selectingPlayerArray[i] == inPlayer) {
				selectingPlayerArray.RemoveAt(i);
				break;
			}
		}
	}
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
		if (GetEntityOwner()->GetUnits().Contains(this))
		{
			for (int i = 0; i < GetEntityOwner()->GetUnits().Num(); i++) {
				if (GetEntityOwner()->GetUnits()[i] == this) {
					GetEntityOwner()->RemoveUnitAtIndex(i);
					break;
				}
			}
		}

		if (GetEntityOwner()->GetBuildings().Contains(this))
		{
			for (int i = 0; i < GetEntityOwner()->GetBuildings().Num(); i++) {
				if (GetEntityOwner()->GetBuildings()[i] == this) {
					GetEntityOwner()->RemoveBuildingAtIndex(i);
					break;
				}
			}
		}

		if (GetEntityOwner()->GetSelectedCharacters().Contains(this))
		{
			for (int i = 0; i < GetEntityOwner()->GetSelectedCharacters().Num(); i++) {
				if (GetEntityOwner()->GetSelectedCharacters()[i] == this) {
					GetEntityOwner()->RemoveSelectedCharacterAtIndex(i);
					break;
				}
			}
		}

		if (selectingPlayerArray.Num() > 0) {
			for (int i = 0; i < selectingPlayerArray.Num(); i++) {
				if (selectingPlayerArray[i] != GetEntityOwner()) {
					for (int j = 0; j < selectingPlayerArray[i]->GetSelectedCharacters().Num(); j++) {
						if (selectingPlayerArray[i]->GetSelectedCharacters()[j] == this) {
							selectingPlayerArray[i]->RemoveSelectedCharacterAtIndex(j);
							break;
						}
					}
				}
			}
		}
	}

	if (!UObject::IsValidLowLevel()) return;

	this->K2_DestroyActor();

	//GC
	GEngine->ForceGarbageCollection();
}