// Fill out your copyright notice in the Description page of Project Settings.

#include "UNIT_Rocketeer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUNIT_Rocketeer::AUNIT_Rocketeer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent->SetWorldScale3D(FVector(0.25f));

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.5f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	UStaticMesh* Asset = MeshAsset.Object;
	BodyMesh->SetStaticMesh(Asset);
	BodyMesh->SetRelativeLocation(FVector(0.0f));
	BodyMesh->SetRelativeScale3D(FVector(2.0f));
	BodyMesh->SetCanEverAffectNavigation(false);

	SelectionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Selection Indicator"));
	SelectionIndicator->SetupAttachment(BodyMesh);
	SelectionIndicator->SetVisibility(false);
	SelectionIndicator->SetWorldLocation(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));

	PS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_RocketShooting.P_RocketShooting'")).Get();
	reactionPS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_Explosion.P_Explosion'")).Get();

	//Load our Sound Cue for the sound we created in the editor
	static ConstructorHelpers::FObjectFinder<USoundCue> fire(TEXT("/Game/Game_Assets/Sounds/Infantry_Rocket_Sounds_V1_Fix/Inf_Rocket_-_Fire_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> select(TEXT("/Game/Game_Assets/Sounds/Infantry_Rocket_Sounds_V1_Fix/Inf_Rocket_-_Select_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> order(TEXT("/Game/Game_Assets/Sounds/Infantry_Rocket_Sounds_V1_Fix/Inf_Rocket_-_Attack_Order_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> death(TEXT("/Game/Game_Assets/Sounds/Infantry_Rocket_Sounds_V1_Fix/Inf_Rocket_-_Death_Cue"));

	//Store a reference to the Cue asset
	fireCue = fire.Object;
	selectCue = select.Object;
	orderCue = order.Object;
	deathCue = death.Object;

	//Create audio component that will wrap the Cue and allow us to interact with it and it's paramiters
	audioComponentFire = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentOne"));
	audioComponentSelect = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentTwo"));
	audioComponentOrder = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentThree"));
	audioComponentDeath = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentFour"));

	//Stop sound from playing when it's created
	audioComponentFire->bAutoActivate = false;
	audioComponentSelect->bAutoActivate = false;
	audioComponentOrder->bAutoActivate = false;
	audioComponentDeath->bAutoActivate = false;

	//Attach the audio component so that it follows the unit around
	audioComponentFire->SetupAttachment(RootComponent);
	audioComponentSelect->SetupAttachment(RootComponent);
	audioComponentOrder->SetupAttachment(RootComponent);
	audioComponentDeath->SetupAttachment(RootComponent);

	currentTimer = 0.0f;
	unitState = UNIT_STATE::IDLE;

}

// Called when the game starts or when spawned
void AUNIT_Rocketeer::BeginPlay()
{
	Super::BeginPlay();
	
	if (setPlayerOwner != nullptr)
		InitializeEntity(Cast<II_Player>(setPlayerOwner), "Rocketeer", startingHealth);

	SpawnDefaultController();

}

void AUNIT_Rocketeer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (fireCue->IsValidLowLevelFast()) {
		audioComponentFire->SetSound(fireCue);
	}

	if (selectCue->IsValidLowLevelFast()) {
		audioComponentSelect->SetSound(selectCue);
	}

	if (orderCue->IsValidLowLevelFast()) {
		audioComponentOrder->SetSound(orderCue);
	}

	if (deathCue->IsValidLowLevelFast()) {
		audioComponentDeath->SetSound(deathCue);
	}
}

// Called every frame
void AUNIT_Rocketeer::Tick(float DeltaTime)
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
			

			// If there is a target, seek it.
			if (targetActor != nullptr)
				unitState = UNIT_STATE::SEEKING;

			// If there isn't a target, set a target.
			else
			{
				/// Check if entities are hostile
				for (int i = 0; i < entitiesInRange.Num(); i++)
				{
					if (Cast<II_Entity>(entitiesInRange[i])->GetEntityOwner() != GetEntityOwner() || GetEntityOwner()->teamValue != Cast<II_Entity>(entitiesInRange[i])->GetEntityOwner()->teamValue)
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
		if (targetActor == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("ENTITY MISSING"));
			unitState = UNIT_STATE::IDLE;
		}
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

				RootComponent->SetRelativeRotation((targetLocation - RootComponent->GetComponentLocation()).Rotation());

				if (currentTimer >= attackRate)
				{
					currentTimer = 0.0f;

					UE_LOG(LogTemp, Warning, TEXT("%f target health"), Cast<II_Entity>(targetActor)->GetCurrentHealth());

					AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
					projectile->InitializeProjectile(PROJECTILE_TYPE::MISSILE, targetLocation, attackDamage, 500.0f, 0.0f, 100.0f, PS, reactionPS);
					projectile->SetActorEnableCollision(false);

					audioComponentFire->Play();

					if (Cast<II_Entity>(targetActor)->GetCurrentHealth() - attackDamage <= 0)
						targetActor = nullptr;
				}
			}
		}

	}

	/// Attack Rate Timer
	// Will count down regardless of whether it's combat or not.
	// Think of it as a cooldown for an attack.
	if (currentTimer < attackRate) {
		currentTimer += DeltaTime;
	}

}

// Called to bind functionality to input
void AUNIT_Rocketeer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AUNIT_Rocketeer::SetSelection(bool state)
{
	SelectionIndicator->SetVisibility(state);
	if (state) {
		audioComponentSelect->Play();
	}
}

// This Function is no longer called
void AUNIT_Rocketeer::AttackOrder(II_Entity* target)
{
	targetActor = target->GetActor();
}

void AUNIT_Rocketeer::DestroyEntity()
{
	audioComponentDeath->Play();
	// Remove from Owner's Array
	if (GetEntityOwner() != nullptr)
	{
		if (GetEntityOwner()->GetUnits().Contains(this))
			GetEntityOwner()->GetUnits().Remove(this);

		if (GetEntityOwner()->GetBuildings().Contains(this))
			GetEntityOwner()->GetBuildings().Remove(this);
	}

	Destroy(this);
}

