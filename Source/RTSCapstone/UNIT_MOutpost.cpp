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

	RootComponent->SetWorldScale3D(FVector(0.25f));

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetRelativeScale3D(FVector(3.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/yeetHarvyDev.yeetHarvyDev'"));
	UStaticMesh* Asset = MeshAsset.Object;
	BodyMesh->SetStaticMesh(Asset);
	BodyMesh->SetRelativeLocation(FVector(0.0, 0.0f, -120.0f));

	SelectionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Selection Indicator"));
	SelectionIndicator->SetupAttachment(BodyMesh);
	SelectionIndicator->SetVisibility(false);
	SelectionIndicator->SetWorldLocation(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));

	currentTimer = 0.0f;
	unitState = UNIT_STATE::IDLE;

}

// Called when the game starts or when spawned
void AUNIT_MOutpost::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDefaultController();
	InitializeEntity(nullptr, "Surveyer", startingHealth);
}

// Called every frame
void AUNIT_MOutpost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		DrawDebugSphere(GetWorld(), GetActorLocation(), detectRange, 24, FColor(0, 0, 255));
		//DrawDebugSphere(GetWorld(), GetActorLocation(), attackRange, 24, FColor(255, 0, 0));
		break;
	case UNIT_STATE::ATTACKING:
		//DrawDebugSphere(GetWorld(), GetActorLocation(), attackRange, 24, FColor(255, 0, 0));
		break;
	case UNIT_STATE::MOVING:
		DrawDebugSphere(GetWorld(), targetMoveDestination, 40.0, 3, FColor(0, 255, 0));  // How close I am to destination
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
			if (targetEntity != nullptr)
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
		if (targetEntity == nullptr)
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
		if (targetEntity == nullptr)
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
	SelectionIndicator->SetVisibility(state);
}

void AUNIT_MOutpost::AttackOrder(II_Entity* target)
{
	/*if (target->DealDamage(attackDamage) == 1)
	{
		targetEntity = nullptr;
	}*/
}

void AUNIT_MOutpost::DestroyEntity()
{
	Destroy(this);
}

