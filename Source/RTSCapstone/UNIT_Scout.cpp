// Fill out your copyright notice in the Description page of Project Settings.

#include "UNIT_Scout.h"

// Sets default values
AUNIT_Scout::AUNIT_Scout()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent->SetWorldScale3D(FVector(0.25f));

	// BODY
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BodyMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Units/Scout/Scout_V1_UNREAL_Body.Scout_V1_UNREAL_Body'"));
	UStaticMesh* bodyMesh = BodyMeshAsset.Object;
	BodyMesh->SetStaticMesh(bodyMesh);
	BodyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
	BodyMesh->SetRelativeScale3D(FVector(5.0f));
	BodyMesh->SetCanEverAffectNavigation(false);

	// TURRET
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TurretMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Units/Scout/Scout_V1_UNREAL_Gun.Scout_V1_UNREAL_Gun'"));
	UStaticMesh* turretMesh = TurretMeshAsset.Object;
	TurretMesh->SetStaticMesh(turretMesh);
	TurretMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	TurretMesh->SetRelativeScale3D(FVector(1.0f));
	TurretMesh->SetCanEverAffectNavigation(false);

	// SELECTION INDICATOR
	SelectionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Selection Indicator"));
	SelectionIndicator->SetupAttachment(BodyMesh);
	SelectionIndicator->SetVisibility(false);
	SelectionIndicator->SetWorldLocation(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));

	// PARTICLE SYSTEMS
	barrelPos = CreateDefaultSubobject<USceneComponent>(TEXT("Barrel"));
	barrelPos->SetRelativeLocation(FVector(7.25f, 0.5f, 20.75f));
	barrelPos->SetupAttachment(TurretMesh);

	PSC = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_RifleShooting.P_RifleShooting'")).Get();
	PSM = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_RocketShooting.P_RocketShooting'")).Get();
	reactionPS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_Explosion.P_Explosion'")).Get();

	currentTimer = 0.0f;
	unitState = UNIT_STATE::IDLE;

	//Load our Sound Cue for the sound we created in the editor
	static ConstructorHelpers::FObjectFinder<USoundCue> fire(TEXT("/Game/Game_Assets/Sounds/Infantry_Rifle_Sounds_V1_Fix/Inf_Rifle_-_Fire_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> select(TEXT("/Game/Game_Assets/Sounds/Basic_Tank_Sounds_V1/Basic_Tank_-_Select_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> order(TEXT("/Game/Game_Assets/Sounds/Basic_Tank_Sounds_V1/Basic_Tank_-_Attack_Order_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> death(TEXT("/Game/Game_Assets/Sounds/Basic_Tank_Sounds_V1/Basic_Tank_-_Death_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> idle(TEXT("/Game/Game_Assets/Sounds/Basic_Tank_Sounds_V1/Basic_Tank_-_Idle_LOOP_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> accel(TEXT("/Game/Game_Assets/Sounds/Basic_Tank_Sounds_V1/Basic_Tank_-_Accelerate_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> drive(TEXT("/Game/Game_Assets/Sounds/Basic_Tank_Sounds_V1/Basic_Tank_-_Drive_LOOP_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> deccel(TEXT("/Game/Game_Assets/Sounds/Basic_Tank_Sounds_V1/Basic_Tank_-_Decelerate_Cue"));

	//Store a reference to the Cue asset
	fireCue = fire.Object;
	selectCue = select.Object;
	orderCue = order.Object;
	deathCue = death.Object;
	idleCue = idle.Object;
	accelerateCue = accel.Object;
	driveCue = drive.Object;
	deccelerateCue = deccel.Object;

	//Create audio component that will wrap the Cue and allow us to interact with it and it's paramiters
	audioComponentFire = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentOne"));
	audioComponentSelect = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentTwo"));
	audioComponentOrder = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentThree"));
	audioComponentDeath = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentFour"));
	audioComponentIdle = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentFive"));
	audioComponentAccelerate = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentSix"));
	audioComponentDrive = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentSeven"));
	audioComponentDeccelerate = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentEight"));

	//Stop sound from playing when it's created
	audioComponentFire->bAutoActivate = false;
	audioComponentSelect->bAutoActivate = false;
	audioComponentOrder->bAutoActivate = false;
	audioComponentDeath->bAutoActivate = false;
	audioComponentIdle->bAutoActivate = false;
	audioComponentAccelerate->bAutoActivate = false;
	audioComponentDrive->bAutoActivate = false;
	audioComponentDeccelerate->bAutoActivate = false;

	//Attach the audio component so that it follows the unit around
	audioComponentFire->SetupAttachment(RootComponent);
	audioComponentSelect->SetupAttachment(RootComponent);
	audioComponentOrder->SetupAttachment(RootComponent);
	audioComponentDeath->SetupAttachment(RootComponent);
	audioComponentIdle->SetupAttachment(RootComponent);
	audioComponentAccelerate->SetupAttachment(RootComponent);
	audioComponentDrive->SetupAttachment(RootComponent);
	audioComponentDeccelerate->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AUNIT_Scout::BeginPlay()
{
	Super::BeginPlay();

	if (setPlayerOwner != nullptr)
		InitializeEntity(Cast<II_Player>(setPlayerOwner), "Humvee", startingHealth);

	SpawnDefaultController();
	
}

void AUNIT_Scout::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (selectCue->IsValidLowLevelFast()) {
		audioComponentSelect->SetSound(selectCue);
	}

	if (orderCue->IsValidLowLevelFast()) {
		audioComponentOrder->SetSound(orderCue);
	}

	if (deathCue->IsValidLowLevelFast()) {
		audioComponentDeath->SetSound(deathCue);
	}

	if (idleCue->IsValidLowLevelFast()) {
		audioComponentIdle->SetSound(idleCue);
	}

	if (accelerateCue->IsValidLowLevelFast()) {
		audioComponentSelect->SetSound(accelerateCue);
	}

	if (driveCue->IsValidLowLevelFast()) {
		audioComponentOrder->SetSound(driveCue);
	}

	if (deccelerateCue->IsValidLowLevelFast()) {
		audioComponentDeath->SetSound(deccelerateCue);
	}

	if (harvestCue->IsValidLowLevelFast()) {
		audioComponentFire->SetSound(fireCue);
	}
}

// Called every frame
void AUNIT_Scout::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (targetActor != nullptr)
	{

		FVector targetLocation = targetActor->GetActorLocation() - GetActorLocation();
		FRotator targetRotation = FRotationMatrix::MakeFromX(targetLocation).Rotator();
		TurretMesh->SetWorldRotation(targetRotation);
	}

	else
	{
		TurretMesh->SetWorldRotation(RootComponent->GetComponentRotation());
	}

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

	if (unitState != UNIT_STATE::MOVING)
	{
		// Detect all AActors within a Radius
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		TArray<AActor*> ignoreActors;
		TArray<AActor*> outActors;

		ignoreActors.Add(this);

		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), detectRange, objectTypes, nullptr, ignoreActors, outActors);

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
					// Check if the entity does not belong to the owner
					if (Cast<II_Entity>(entitiesInRange[i])->GetEntityOwner() != GetEntityOwner() && Cast<II_Entity>(entitiesInRange[i])->GetEntityOwner()->teamValue != GetEntityOwner()->teamValue)
					{
						UE_LOG(LogTemp, Warning, TEXT("TARGET ACQUIRED"));
						targetActor = entitiesInRange[i];
						break;
					}
				}

			}

		}
	}

	// MOVEMENT STATE
	if (unitState == UNIT_STATE::MOVING)
	{
		// Ignore Combat until unit reaches destination

		if (FVector::Dist(GetActorLocation(), targetMoveDestination) < 150.0f)
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
	if (currentTimer < attackTimer) {
		currentTimer += DeltaTime;
	}


}

// Called to bind functionality to input
void AUNIT_Scout::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUNIT_Scout::SetSelection(bool state)
{
	SelectionIndicator->SetVisibility(state);
	if (state) {
		audioComponentSelect->Play();
	}
}


// Method Unused
void AUNIT_Scout::AttackOrder(II_Entity* target)
{
	targetActor = target->GetActor();
}

void AUNIT_Scout::DestroyEntity()
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