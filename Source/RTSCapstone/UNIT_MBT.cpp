// Fill out your copyright notice in the Description page of Project Settings.

#include "UNIT_MBT.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUNIT_MBT::AUNIT_MBT()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent->SetWorldScale3D(FVector(0.25f));
	isSelected = false;

	weight = 5;

	SetHitRadius(160);

	movingStage = 0;

	// BODY
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>BodyMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Units/MBT/MBT_V1_UNREAL_Body.MBT_V1_UNREAL_Body'"));
	UStaticMesh* bodyMesh = BodyMeshAsset.Object;
	BodyMesh->SetStaticMesh(bodyMesh);
	BodyMesh->SetRelativeLocation(FVector(0.0f, -15.0f, -80.0f));
	BodyMesh->SetRelativeScale3D(FVector(4.0f));
	BodyMesh->SetCanEverAffectNavigation(false);
	BodyMesh->bReceivesDecals = false;

	// TURRET
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BodyMesh);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>TurretMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Units/MBT/MBT_V1_UNREAL_Turret.MBT_V1_UNREAL_Turret'"));
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
	barrelPos->SetRelativeLocation(FVector(63.5f, 4.25f, 26.0f));
	barrelPos->SetupAttachment(TurretMesh);
	

	PS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_RifleShooting.P_RifleShooting'")).Get();
	reactionPS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_Explosion.P_Explosion'")).Get();

	//Dust Trail Particle System
	DustPS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_DustTrail.P_DustTrail'")).Get();
	trailParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DustPS"));
	trailParticleComp->SetupAttachment(RootComponent);
	trailParticleComp->SetRelativeLocation(FVector(-100.0, 0.0, 0.0));
	trailParticleComp->SetTemplate(DustPS);
	trailParticleComp->bAutoActivate = false;

	currentTimer = 0.0f;
	unitState = UNIT_STATE::IDLE;

	//Load our Sound Cue for the sound we created in the editor
	static ConstructorHelpers::FObjectFinder<USoundCue> fire(TEXT("/Game/Game_Assets/Sounds/Basic_Tank_Sounds_V1/Basic_Tank_-_Fire_Cue"));
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
void AUNIT_MBT::BeginPlay()
{
	Super::BeginPlay();

	if (setPlayerOwner != nullptr)
		InitializeEntity(Cast<II_Player>(setPlayerOwner), "Main Battle Tank", startingHealth);

	SpawnDefaultController();

	overrideAI = false;

	//GetCapsuleComponent()->SetCapsuleRadius(160.0f);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->SetAvoidanceEnabled(true);
	GetCharacterMovement()->AvoidanceConsiderationRadius = 1200.0f;
	GetCharacterMovement()->SetRVOAvoidanceWeight(1.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

void AUNIT_MBT::PostInitializeComponents()
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

	if (fireCue->IsValidLowLevelFast()) {
		audioComponentFire->SetSound(fireCue);
	}
}

// Called every frame
void AUNIT_MBT::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	isDestructable = SetDestructible;

	if (targetActor != nullptr)
	{
		if (targetActor->IsValidLowLevel())
		{
			FVector Dir = (targetActor->GetActorLocation() - GetActorLocation());
			Dir.Normalize();

			TurretMesh->SetWorldRotation(FMath::Lerp(TurretMesh->GetComponentRotation(), Dir.Rotation(), 0.05f));
		}
	}

	else
	{
		TurretMesh->SetWorldRotation(FMath::Lerp(TurretMesh->GetComponentRotation(), RootComponent->GetComponentRotation(), 0.025f));
	}

	switch (unitState)
	{
		case UNIT_STATE::IDLE:
			//UE_LOG(LogTemp, Warning, TEXT("IDLE"));
		case UNIT_STATE::SEEKING:
			//UE_LOG(LogTemp, Warning, TEXT("SEEKING"));
			//DrawDebugSphere(GetWorld(), GetActorLocation(), detectRange, 24, FColor(0, 0, 255));
			//DrawDebugSphere(GetWorld(), GetActorLocation(), attackRange, 24, FColor(255, 0, 0));
			break;
		case UNIT_STATE::ATTACKING:
			//UE_LOG(LogTemp, Warning, TEXT("ATTACKING"));
			//DrawDebugSphere(GetWorld(), GetActorLocation(), attackRange, 24, FColor(255, 0, 0));
			break;
		case UNIT_STATE::MOVING:
			//UE_LOG(LogTemp, Warning, TEXT("MOVING"));
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

		if (movingStage == 2 && !audioComponentDrive->IsPlaying()) {
			audioComponentDeccelerate->Play();
			movingStage = 0;
		}
		if (!audioComponentIdle->IsPlaying() && !audioComponentDeccelerate->IsPlaying()) {
			audioComponentIdle->Play();
		}

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
					if (Cast<II_Entity>(entitiesInRange[i])->GetEntityOwner() != GetEntityOwner())
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
		if (!trailParticleComp->IsActive()) {
			trailParticleComp->Activate(true);
		}
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

		//DrawDebugLine(GetWorld(), StartTrace, EndTraceOne, FColor(255, 0, 0), false, 1);
		if (GetWorld()->LineTraceSingleByChannel(*rayCastOne, StartTrace, EndTraceOne, ECC_Visibility, *TraceParams)) {
			if (Cast<II_Unit>(rayCastOne->GetActor())) {
				if (Cast<II_Unit>(rayCastOne->GetActor())->weight <= weight) {
					FVector push = (rayCastOne->GetActor()->GetActorLocation() - GetActorLocation());
					push = FVector(push.X / 7, push.Y / 7, push.Z) + (BodyMesh->GetRightVector() * 2);
					rayCastOne->GetActor()->SetActorLocation(FVector(rayCastOne->GetActor()->GetActorLocation().X + push.X, rayCastOne->GetActor()->GetActorLocation().Y + push.Y, rayCastOne->GetActor()->GetActorLocation().Z));
				}
				else {
					FVector push = (GetActorLocation() - rayCastOne->GetActor()->GetActorLocation());
					push = FVector(push.X / 7, push.Y / 7, push.Z) + (BodyMesh->GetRightVector() * 2);
					SetActorLocation(FVector(GetActorLocation().X + push.X, GetActorLocation().Y + push.Y, GetActorLocation().Z));
				}
			}
		}

		//DrawDebugLine(GetWorld(), StartTrace, EndTraceTwo, FColor(255, 0, 0), false, 1);
		if (GetWorld()->LineTraceSingleByChannel(*rayCastTwo, StartTrace, EndTraceTwo, ECC_Visibility, *TraceParams)) {
			if (Cast<II_Unit>(rayCastTwo->GetActor())) {
				if (Cast<II_Unit>(rayCastTwo->GetActor())->weight <= weight) {
					FVector push = (rayCastTwo->GetActor()->GetActorLocation() - GetActorLocation());
					push = FVector(push.X / 7, push.Y / 7, push.Z) + (-BodyMesh->GetRightVector() * 2);
					rayCastTwo->GetActor()->SetActorLocation(FVector(rayCastTwo->GetActor()->GetActorLocation().X + push.X, rayCastTwo->GetActor()->GetActorLocation().Y + push.Y, rayCastTwo->GetActor()->GetActorLocation().Z));
				}
				else {
					FVector push = (GetActorLocation() - rayCastTwo->GetActor()->GetActorLocation());
					push = FVector(push.X / 7, push.Y / 7, push.Z) + (BodyMesh->GetRightVector() * 2);
					SetActorLocation(FVector(GetActorLocation().X + push.X, GetActorLocation().Y + push.Y, GetActorLocation().Z));
				}
			}
		}

		if (FVector::Dist(GetActorLocation(), targetMoveDestination) < 200.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("DESTINATION REACHED"));
			unitState = UNIT_STATE::IDLE;
			overrideAI = false;

			trailParticleComp->Deactivate();
		}

		if (movingStage == 0 && !audioComponentDeccelerate->IsPlaying()) {
			audioComponentAccelerate->Play();
			movingStage++;
		}
		if (movingStage != 0 && !audioComponentAccelerate->IsPlaying()) {
			audioComponentDrive->Play();
			movingStage++;
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
			if (FVector::Dist(GetActorLocation(), targetLocation) > attackRange + Cast<II_Entity>(targetActor)->GetHitRadius())
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
			if (FVector::Dist(GetActorLocation(), targetLocation) > attackRange + Cast<II_Entity>(targetActor)->GetHitRadius())
			{
				unitState = UNIT_STATE::SEEKING;
			}

			// Target is in range: attack it.
			else
			{
				SetDestination(GetController(), GetActorLocation());

				if (currentTimer >= attackRate)
				{
					currentTimer = 0.0f;

					AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), barrelPos->GetComponentLocation(), TurretMesh->GetComponentRotation());
					projectile->InitializeProjectile(PROJECTILE_TYPE::CANNON, targetLocation, attackDamage, 5000.0f, 0.0f, 100.0f, PS, reactionPS);
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
void AUNIT_MBT::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUNIT_MBT::ResetTarget()
{
	targetActor = nullptr;
}


void AUNIT_MBT::SetSelection(bool state, II_Player* inPlayer)
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
	if (state) {
		audioComponentSelect->Play();
	}
}

bool AUNIT_MBT::GetSelection() {
	return isSelected;
}


// Function Never Called
void AUNIT_MBT::AttackOrder(II_Entity* target)
{
	targetActor = target->GetActor();

}

void AUNIT_MBT::DestroyEntity()
{
	audioComponentDeath->Play();

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

	if (UObject::IsPendingKill()) return;

	this->K2_DestroyActor();

	//GC
	GEngine->ForceGarbageCollection();
}

