// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Turret_Artillery.h"

#include "Kismet/KismetSystemLibrary.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

#include "Projectile.h"

ABuilding_Turret_Artillery::ABuilding_Turret_Artillery() {
	spawnTime = 2;
	isBuilding = true;
	hasPower = true;

	SetHitRadius(140);

	static ConstructorHelpers::FObjectFinder<USoundCue> select(TEXT("/Game/Game_Assets/Sounds/Building_Sounds_V1/Tech_Centre_-_Select_Cue"));
	selectCue = select.Object;

	//Load our Sound Cue for the sound we created in the editor
	static ConstructorHelpers::FObjectFinder<USoundCue> fire(TEXT("/Game/Game_Assets/Sounds/Basic_Tank_Sounds_V1/Basic_Tank_-_Fire_Cue"));

	//Store a reference to the Cue asset
	fireCue = fire.Object;

	//Create audio component that will wrap the Cue and allow us to interact with it and it's paramiters
	audioComponentFire = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentThree"));

	//Stop sound from playing when it's created
	audioComponentFire->bAutoActivate = false;

	//Attach the audio component so that it follows the unit around
	audioComponentFire->SetupAttachment(RootComponent);

	// Body
	static ConstructorHelpers::FObjectFinder<UStaticMesh>BodyMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Artillery_Platform/ArtilleryCannon_Cylinder001.ArtilleryCannon_Cylinder001'"));
	UStaticMesh* bodyMesh = BodyMeshAsset.Object;
	buildingMesh->SetStaticMesh(bodyMesh);
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	buildingMesh->SetRelativeScale3D(FVector(5.0f));
	buildingMesh->SetCanEverAffectNavigation(false);
	buildingMesh->SetSimulatePhysics(false);

	// Pivot
	PivotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pivot Mesh"));
	PivotMesh->SetupAttachment(buildingMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PivotMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Artillery_Platform/ArtilleryCannon_Cylinder002.ArtilleryCannon_Cylinder002'"));
	UStaticMesh* pivotMesh = PivotMeshAsset.Object;
	PivotMesh->SetStaticMesh(pivotMesh);
	PivotMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PivotMesh->SetRelativeScale3D(FVector(1.0f));
	PivotMesh->SetCanEverAffectNavigation(false);

	// PARTICLE SYSTEMS
	barrelPos = CreateDefaultSubobject<USceneComponent>(TEXT("Barrel"));
	barrelPos->SetRelativeLocation(FVector(-26.0f, 0.0f, 74.5f));
	barrelPos->SetupAttachment(PivotMesh);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);

	PS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_RifleShooting.P_RifleShooting'")).Get();
	reactionPS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_Explosion.P_Explosion'")).Get();

	currentAttackTimer = 0.0f;

	static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("Class'/Game/Game_Assets/Blueprints/BarracksBlowingUp.BarracksBlowingUp_C'"));
	if (ItemBlueprint.Object) {
		ExplosionBlueprint = (UClass*)ItemBlueprint.Object;
	}
}


void ABuilding_Turret_Artillery::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (fireCue->IsValidLowLevelFast()) {
		audioComponentFire->SetSound(fireCue);
	}
}

void ABuilding_Turret_Artillery::BeginPlay()
{
	Super::BeginPlay();
}

void ABuilding_Turret_Artillery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (setPlayerOwner != nullptr) {
		InitializeStructure(Cast<II_Player>(setPlayerOwner), "Placeholder", 10.0f);
		SetName(TEXT("ArtilleryTurret"));
		SetMaxHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("ArtilleryTurret")), (TEXT("Context")), false)->MaxHealth);
		SetCurrentHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("ArtilleryTurret")), (TEXT("Context")), false)->MaxHealth);
		setPlayerOwner = nullptr;
		return;
	}

	if (constructed)
	{
		// Detect all AActors within a Radius
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		TArray<AActor*> ignoreActors;
		TArray<AActor*> outActors;

		ignoreActors.Add(this);

		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), detectRange, objectTypes, nullptr, ignoreActors, outActors);

		// Debug Turret Range
		//DrawDebugSphere(GetWorld(), GetActorLocation(), detectRange, 24, FColor(0, 0, 255));


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

		// If there is no target, run the detection sequence.
		if (targetActor != nullptr)
		{
			// If the target is out of range, reset the targetActor reference
			if (FVector::Dist(GetActorLocation(), targetActor->GetActorLocation()) > detectRange)
				targetActor = nullptr;

			// Target is in range
			else
			{
				// Rotate towards target
				FVector Dir = -(targetActor->GetActorLocation() - GetActorLocation());
				Dir.Normalize();

				PivotMesh->SetWorldRotation(FMath::Lerp(PivotMesh->GetComponentRotation(), Dir.Rotation(), 0.05f));

				// Attack the target
				if (currentAttackTimer >= attackRate)
				{
					currentAttackTimer = 0.0f;

					AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), barrelPos->GetComponentLocation(), FRotator(0.0f, 0.0f, 0.0f));
					projectile->InitializeProjectile(PROJECTILE_TYPE::CANNON, targetActor->GetActorLocation(), attackDamage, 4000.0f, 0.0f, 100.0f, PS, reactionPS, true);
					projectile->SetActorEnableCollision(false);
					audioComponentFire->Play();
				}

				if (Cast<II_Entity>(targetActor)->GetCurrentHealth() - attackDamage <= 0)
					targetActor = nullptr;
			}
		}

		else
		{
			// Rotate back to default
			PivotMesh->SetWorldRotation(FMath::Lerp(PivotMesh->GetComponentRotation(), RootComponent->GetComponentRotation(), 0.025f));

			// If one ore more actors are detected within range
			if (entitiesInRange.Num() > 0)
			{
				// Loop through them all
				for (int i = 0; i < entitiesInRange.Num(); i++)
				{
					// Check if the entity does not belong to the owner
					if (Cast<II_Entity>(entitiesInRange[i])->GetEntityOwner() != GetEntityOwner())
					{
						targetActor = entitiesInRange[i];
						break;

					}
				}
			}
		}

		/// Attack Rate Timer
		// Will count down regardless of whether it's combat or not.
		// Think of it as a cooldown for an attack.
		if (currentAttackTimer < attackRate) {
			currentAttackTimer += DeltaTime;
		}

	}

}

void ABuilding_Turret_Artillery::SetHasPower(bool inBool)
{
	//Disable attack functions
}

bool ABuilding_Turret_Artillery::constructAtLocation(II_Player* player)
{
	dustParticleComp->SetWorldLocation(this->GetActorLocation());
	dustParticleComp->ActivateSystem();
	tempHeight = RootComponent->GetComponentLocation().Z;
	buildingMesh->SetWorldLocation(FVector(RootComponent->GetComponentLocation().X, RootComponent->GetComponentLocation().Y, RootComponent->GetComponentLocation().Z - 390));


	return false;
}