// Fill out your copyright notice in the Description page of Project Settings.

#include "UNIT_Rifleman.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUNIT_Rifleman::AUNIT_Rifleman()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent->SetWorldScale3D(FVector(0.5f));
	isSelected = false;

	weight = 1;

	SetHitRadius(30);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 2.5f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	UStaticMesh* Asset = MeshAsset.Object;
	BodyMesh->SetStaticMesh(Asset);
	BodyMesh->SetRelativeLocation(FVector(0.0f));
	BodyMesh->SetRelativeScale3D(FVector(2.0f));
	BodyMesh->SetCanEverAffectNavigation(false);
	BodyMesh->bReceivesDecals = false;

	SelectionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Selection Indicator"));
	SelectionIndicator->SetupAttachment(BodyMesh);
	SelectionIndicator->SetVisibility(false);
	SelectionIndicator->SetWorldLocation(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));

	PS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_RifleShooting.P_RifleShooting'")).Get();
	reactionPS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_BulletHit.P_BulletHit'")).Get();

	//Load our Sound Cue for the sound we created in the editor
	static ConstructorHelpers::FObjectFinder<USoundCue> fire(TEXT("/Game/Game_Assets/Sounds/Infantry_Rifle_Sounds_V1_Fix/Inf_Rifle_-_Fire_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> select(TEXT("/Game/Game_Assets/Sounds/Infantry_Rifle_Sounds_V1_Fix/Inf_Rifle_-_Select_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> order(TEXT("/Game/Game_Assets/Sounds/Infantry_Rifle_Sounds_V1_Fix/Inf_Rifle_-_Attack_Order_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> death(TEXT("/Game/Game_Assets/Sounds/Infantry_Rifle_Sounds_V1_Fix/Inf_Rifle_-_Death_Cue"));

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
void AUNIT_Rifleman::BeginPlay()
{
	Super::BeginPlay();

	isDestructable = SetDestructible;

	if (setPlayerOwner != nullptr)
		InitializeEntity(Cast<II_Player>(setPlayerOwner), "Rifleman", startingHealth);

	SpawnDefaultController();

	overrideAI = false;


	bUseControllerRotationYaw = false;
	GetCharacterMovement()->SetAvoidanceEnabled(true);
	GetCharacterMovement()->AvoidanceConsiderationRadius = 200.0f;
	GetCharacterMovement()->SetRVOAvoidanceWeight(0.5f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

void AUNIT_Rifleman::PostInitializeComponents()
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
void AUNIT_Rifleman::Tick(float DeltaTime)
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
		FHitResult* rayCastOne = new FHitResult();
		FHitResult* rayCastTwo = new FHitResult();

		FVector StartTrace = BodyMesh->GetComponentLocation() + (BodyMesh->GetForwardVector() * 60);

		FVector ForwardVectorOne = BodyMesh->GetForwardVector();
		FVector ForwardVectorTwo = BodyMesh->GetForwardVector();

		ForwardVectorOne = ForwardVectorOne.RotateAngleAxis(20, FVector(0.0, 0.0, 1.0));
		ForwardVectorTwo = ForwardVectorTwo.RotateAngleAxis(-20, FVector(0.0, 0.0, 1.0));

		FVector EndTraceOne = ((ForwardVectorOne * 100) + StartTrace);
		FVector EndTraceTwo = ((ForwardVectorTwo * 100) + StartTrace);

		FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

		DrawDebugLine(GetWorld(), StartTrace, EndTraceOne, FColor(255, 0, 0), false, 1);
		if (GetWorld()->LineTraceSingleByChannel(*rayCastOne, StartTrace, EndTraceOne, ECC_Visibility, *TraceParams)) {
			if (Cast<II_Unit>(rayCastOne->GetActor())) {
				if (Cast<II_Unit>(rayCastOne->GetActor())->weight <= weight) {
					FVector push = (rayCastOne->GetActor()->GetActorLocation() - GetActorLocation());
					push = FVector(push.X / 4, push.Y / 4, push.Z) + (BodyMesh->GetRightVector() * 2);
					rayCastOne->GetActor()->SetActorLocation(rayCastOne->GetActor()->GetActorLocation() + push);
				}
				else {
					FVector push = (GetActorLocation() - rayCastOne->GetActor()->GetActorLocation());
					push = FVector(push.X / 4, push.Y / 4, push.Z) + (BodyMesh->GetRightVector() * 2);
					SetActorLocation(FVector(GetActorLocation().X + push.X, GetActorLocation().Y + push.Y, GetActorLocation().Z));
				}
			}
		}

		DrawDebugLine(GetWorld(), StartTrace, EndTraceTwo, FColor(255, 0, 0), false, 1);
		if (GetWorld()->LineTraceSingleByChannel(*rayCastTwo, StartTrace, EndTraceTwo, ECC_Visibility, *TraceParams)) {
			if (Cast<II_Unit>(rayCastTwo->GetActor())) {
				if (Cast<II_Unit>(rayCastTwo->GetActor())->weight <= weight) {
					FVector push = (rayCastTwo->GetActor()->GetActorLocation() - GetActorLocation());
					push = FVector(push.X / 4, push.Y / 4, push.Z) + (-BodyMesh->GetRightVector() * 2);
					rayCastTwo->GetActor()->SetActorLocation(rayCastTwo->GetActor()->GetActorLocation() + push);
				}
				else {
					FVector push = (GetActorLocation() - rayCastTwo->GetActor()->GetActorLocation());
					push = FVector(push.X / 4, push.Y / 4, push.Z) + (BodyMesh->GetRightVector() * 2);
					SetActorLocation(FVector(GetActorLocation().X + push.X, GetActorLocation().Y + push.Y, GetActorLocation().Z));
				}
			}
		}

		if (FVector::Dist(GetActorLocation(), targetMoveDestination) < 120.0f)
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
			if (FVector::Dist(GetActorLocation(), targetLocation) > attackRange + Cast<II_Entity>(targetActor)->GetHitRadius())
			{
				SetDestination(GetController(), moveDestination);
			}

			// Target is in range: Attack it.
			else
			{
				unitState = UNIT_STATE::ATTACKING;
				overrideAI = false;
			}
		}
	}


	// ATTACK STATE
	if (unitState == UNIT_STATE::ATTACKING)
	{
		if (targetActor == nullptr) {
			unitState = UNIT_STATE::IDLE;
		}
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

				RootComponent->SetRelativeRotation((targetLocation - RootComponent->GetComponentLocation()).Rotation());

				if (currentTimer >= attackRate)
				{
					currentTimer = 0.0f;

					AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), GetActorLocation(), GetActorRotation());
					projectile->InitializeProjectile(PROJECTILE_TYPE::CANNON, targetLocation, attackDamage, 5000.0f, 0.0f, 1.0f, PS, reactionPS);
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
void AUNIT_Rifleman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUNIT_Rifleman::ResetTarget()
{
	targetActor = nullptr;
}

void AUNIT_Rifleman::SetSelection(bool state, II_Player* inPlayer)
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

bool AUNIT_Rifleman::GetSelection() {
	return isSelected;
}

// Function is never called, but should be for hitscan classes
void AUNIT_Rifleman::AttackOrder(II_Entity* target)
{
	targetActor = target->GetActor();
}

void AUNIT_Rifleman::DestroyEntity()
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

	this->K2_DestroyActor();

	//GC
	GEngine->ForceGarbageCollection();
}


