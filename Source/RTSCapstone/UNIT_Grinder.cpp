// Fill out your copyright notice in the Description page of Project Settings.

#include "UNIT_Grinder.h"

#include "Kismet/KismetSystemLibrary.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUNIT_Grinder::AUNIT_Grinder()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComponent->SetWorldScale3D(FVector(0.25f));
	isSelected = false;

	weight = 5;

	SetHitRadius(160);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetRelativeScale3D(FVector(3.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/yeetHarvyDev.yeetHarvyDev'"));
	UStaticMesh* Asset = MeshAsset.Object;
	BodyMesh->SetStaticMesh(Asset);
	BodyMesh->SetRelativeLocation(FVector(0.0, 0.0f, -120.0f));
	BodyMesh->SetRelativeScale3D(FVector(8.0f));
	BodyMesh->SetCanEverAffectNavigation(false);
	BodyMesh->bReceivesDecals = false;

	SelectionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Selection Indicator"));
	SelectionIndicator->SetupAttachment(BodyMesh);
	SelectionIndicator->SetVisibility(false);
	SelectionIndicator->SetWorldLocation(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));

	//Dust Trail Particle System
	DustPS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_DustTrail.P_DustTrail'")).Get();
	trailParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DustPS"));
	trailParticleComp->SetupAttachment(RootComponent);
	trailParticleComp->SetRelativeLocation(FVector(-100.0, 0.0, 0.0));
	trailParticleComp->SetTemplate(DustPS);
	trailParticleComp->bAutoActivate = false;

	PSC = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_RifleShooting.P_RifleShooting'")).Get();
	PSM = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_RocketShooting.P_RocketShooting'")).Get();
	reactionPS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_Explosion.P_Explosion'")).Get();

	currentTimer = 0.0f;
	unitState = UNIT_STATE::IDLE;

	static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("Class'/Game/Game_Assets/Blueprints/Unit_Explosions/UnitExplosion.UnitExplosion_C'"));
	if (ItemBlueprint.Object) {
		ExplosionBlueprint = (UClass*)ItemBlueprint.Object;
	}
}

// Called when the game starts or when spawned
void AUNIT_Grinder::BeginPlay()
{
	Super::BeginPlay();

	if (setPlayerOwner != nullptr)
		InitializeEntity(Cast<II_Player>(setPlayerOwner), "Grinder", startingHealth);

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
void AUNIT_Grinder::Tick(float DeltaTime)
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
		if (!trailParticleComp->IsActive()) {
			trailParticleComp->Activate(true);
		}
		// if the unit encounters an enemy while moving
		if (targetActor != nullptr)
		{
			unitState = UNIT_STATE::ATTACKING;
		}

		FHitResult* rayCastOne = new FHitResult();
		FHitResult* rayCastTwo = new FHitResult();

		FVector StartTrace = RootComponent->GetComponentLocation() + (RootComponent->GetForwardVector() * 120);

		FVector ForwardVectorOne = RootComponent->GetForwardVector();
		FVector ForwardVectorTwo = RootComponent->GetForwardVector();

		ForwardVectorOne = ForwardVectorOne.RotateAngleAxis(100, FVector(0.0, 0.0, 1.0));
		ForwardVectorTwo = ForwardVectorTwo.RotateAngleAxis(-100, FVector(0.0, 0.0, 1.0));

		FVector EndTraceOne = ((ForwardVectorOne * 120) + StartTrace);
		FVector EndTraceTwo = ((ForwardVectorTwo * 120) + StartTrace);

		FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

		//DrawDebugLine(GetWorld(), StartTrace, EndTraceOne, FColor(255, 0, 0), false, 1);
		if (GetWorld()->LineTraceSingleByChannel(*rayCastOne, StartTrace, EndTraceOne, ECC_Visibility, *TraceParams)) {
			if (Cast<II_Unit>(rayCastOne->GetActor())) {
				if (Cast<II_Unit>(rayCastOne->GetActor())->weight <= weight) {
					FVector push = (rayCastOne->GetActor()->GetActorLocation() - GetActorLocation());
					push = FVector(push.X / 7, push.Y / 7, push.Z) + (RootComponent->GetRightVector() * 2);
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
					push = FVector(push.X / 7, push.Y / 7, push.Z) + (-RootComponent->GetRightVector() * 2);
					rayCastTwo->GetActor()->SetActorLocation(FVector(rayCastTwo->GetActor()->GetActorLocation().X + push.X, rayCastTwo->GetActor()->GetActorLocation().Y + push.Y, rayCastTwo->GetActor()->GetActorLocation().Z));
				}
				else {
					FVector push = (GetActorLocation() - rayCastTwo->GetActor()->GetActorLocation());
					push = FVector(push.X / 7, push.Y / 7, push.Z) + (BodyMesh->GetRightVector() * 2);
					SetActorLocation(FVector(GetActorLocation().X + push.X, GetActorLocation().Y + push.Y, GetActorLocation().Z));
				}
			}
		}


		// Search for enemies
		if (entitiesInRange.Num() > 0)
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

		if (FVector::Dist(GetActorLocation(), targetMoveDestination) < 80.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("DESTINATION REACHED"));
			unitState = UNIT_STATE::IDLE;

			trailParticleComp->Deactivate();
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
void AUNIT_Grinder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUNIT_Grinder::ResetTarget()
{
	targetActor = nullptr;
}


void AUNIT_Grinder::SetSelection(bool state, II_Player* inPlayer)
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

bool AUNIT_Grinder::GetSelection() {
	return isSelected;
}


// Method Unused
void AUNIT_Grinder::AttackOrder(II_Entity* target)
{
	targetActor = target->GetActor();
}

void AUNIT_Grinder::DestroyEntity()
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

	GetWorld()->SpawnActor<AExplosiveActor>(ExplosionBlueprint, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));


	if (!UObject::IsValidLowLevel()) return;

	this->K2_DestroyActor();

	//GC
	GEngine->ForceGarbageCollection();
}

