// Fill out your copyright notice in the Description page of Project Settings.

#include "UNIT_Roomba.h"

#include "Kismet/KismetSystemLibrary.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUNIT_Roomba::AUNIT_Roomba()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComponent->SetWorldScale3D(FVector(0.25f));
	isSelected = false;

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

	PSC = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_RifleShooting.P_RifleShooting'")).Get();
	PSM = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_RocketShooting.P_RocketShooting'")).Get();
	reactionPS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_Explosion.P_Explosion'")).Get();

	currentTimer = 0.0f;
	unitState = UNIT_STATE::IDLE;
}

// Called when the game starts or when spawned
void AUNIT_Roomba::BeginPlay()
{
	Super::BeginPlay();


	if (setPlayerOwner != nullptr)
		InitializeEntity(Cast<II_Player>(setPlayerOwner), "Mr. Stabby", startingHealth);

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
void AUNIT_Roomba::Tick(float DeltaTime)
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
		DrawDebugSphere(GetWorld(), GetActorLocation(), detectRange, 24, FColor(0, 0, 255));
		DrawDebugSphere(GetWorld(), GetActorLocation(), attackRange, 24, FColor(255, 0, 0));
		break;
	case UNIT_STATE::ATTACKING:
		DrawDebugSphere(GetWorld(), GetActorLocation(), attackRange, 24, FColor(255, 0, 0));
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
			if (targetActor != nullptr)
				unitState = UNIT_STATE::SEEKING;

			// If there isn't a target, set a target.
			else
			{
				/// Check if entities are hostile
				for (int i = 0; i < entitiesInRange.Num(); i++)
				{
					if (Cast<II_Entity>(entitiesInRange[i])->GetEntityOwner() != GetEntityOwner() && Cast<II_Entity>(entitiesInRange[i])->GetEntityOwner()->teamValue != GetEntityOwner()->teamValue)
					{
						targetActor = entitiesInRange[0];
					}
				}

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
			FVector targetLocation = targetActor->GetActorLocation();
			FVector moveDestination = targetLocation - ((GetActorLocation() - targetLocation) / 2);

			// Target is out of range: move towards it.
			if (FVector::Dist(GetActorLocation(), targetLocation) > attackRange)
			{
				SetDestination(GetController(), moveDestination);
			}

			// Target is in range: Attack it.
			else
			{
				unitState = UNIT_STATE::ATTACKING;
			}
		}
	}


	// ATTACK STATE
	if (unitState == UNIT_STATE::ATTACKING)
	{
		if (targetActor == nullptr)
			unitState = UNIT_STATE::IDLE;

		else
		{
			FVector targetLocation = targetActor->GetActorLocation();
			FVector moveDestination = targetLocation - ((GetActorLocation() - targetLocation) / 2);

			// Target is out of range: chase it;
			if (FVector::Dist(GetActorLocation(), targetLocation) > attackRange)
			{
				unitState = UNIT_STATE::SEEKING;
			}

			// Target is in range: attack it.
			else
			{
				SetDestination(GetController(), GetActorLocation());

				if (currentTimer >= attackTimer)
				{
					
					Cast<II_Entity>(targetActor)->DealDamage(attackDamage);

					if (Cast<II_Entity>(targetActor)->GetCurrentHealth() - attackDamage <= 0)
						targetActor = nullptr;

				}
			}
		}

	}

	/// Attack Rate Timer
	// Will count down regardless of whether it's combat or not.
	// Think of it as a cooldown for an attack.
	if (currentTimer < attackTimer) {
		currentTimer += DeltaTime;
	}


}

// Called to bind functionality to input
void AUNIT_Roomba::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUNIT_Roomba::ResetTarget()
{
	targetActor = nullptr;
}

void AUNIT_Roomba::SetSelection(bool state)
{
	isSelected = state;
	SelectionIndicator->SetVisibility(state);
}

bool AUNIT_Roomba::GetSelection() {
	return isSelected;
}


// Method Unused
void AUNIT_Roomba::AttackOrder(II_Entity* target)
{
	targetActor = target->GetActor();
}

void AUNIT_Roomba::DestroyEntity()
{
	// Remove from Owner's Array
	if (GetEntityOwner() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("I have died"));
		if (GetEntityOwner()->GetUnits().Contains(this))
		{
			for (int i = 0; i < GetEntityOwner()->GetUnits().Num(); i++) {
				if (GetEntityOwner()->GetUnits()[i] == this)
					GetEntityOwner()->RemoveUnitAtIndex(i);
			}
		}

		if (GetEntityOwner()->GetBuildings().Contains(this))
		{
			for (int i = 0; i < GetEntityOwner()->GetBuildings().Num(); i++) {
				if (GetEntityOwner()->GetBuildings()[i] == this)
					GetEntityOwner()->RemoveBuildingAtIndex(i);
			}
		}

		if (GetEntityOwner()->GetSelectedCharacters().Contains(this))
		{
			for (int i = 0; i < GetEntityOwner()->GetSelectedCharacters().Num(); i++) {
				if (GetEntityOwner()->GetSelectedCharacters()[i] == this)
					GetEntityOwner()->RemoveSelectedCharacterAtIndex(i);
			}
		}
	}


	if (!UObject::IsValidLowLevel()) return;

	this->K2_DestroyActor();

	//GC
	GEngine->ForceGarbageCollection();
}

