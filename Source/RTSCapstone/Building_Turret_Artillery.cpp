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

	// Body
	static ConstructorHelpers::FObjectFinder<UStaticMesh>BodyMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Artillery_Platform/Artillery_Platform_V1_UNREAL_Base.Artillery_Platform_V1_UNREAL_Base'"));
	UStaticMesh* bodyMesh = BodyMeshAsset.Object;
	buildingMesh->SetStaticMesh(bodyMesh);
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	buildingMesh->SetRelativeScale3D(FVector(5.0f));
	buildingMesh->SetCanEverAffectNavigation(false);
	buildingMesh->SetSimulatePhysics(false);

	// Pivot
	PivotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pivot Mesh"));
	PivotMesh->SetupAttachment(buildingMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PivotMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Artillery_Platform/Artillery_Platform_V1_UNREAL_Pivot.Artillery_Platform_V1_UNREAL_Pivot'"));
	UStaticMesh* pivotMesh = PivotMeshAsset.Object;
	PivotMesh->SetStaticMesh(pivotMesh);
	PivotMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PivotMesh->SetRelativeScale3D(FVector(1.0f));
	PivotMesh->SetCanEverAffectNavigation(false);

	// Turret
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(PivotMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TurretMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Artillery_Platform/Artillery_Platform_V1_UNREAL_Cannon.Artillery_Platform_V1_UNREAL_Cannon'"));
	UStaticMesh* turretMesh = TurretMeshAsset.Object;
	TurretMesh->SetStaticMesh(turretMesh);
	TurretMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	TurretMesh->SetRelativeScale3D(FVector(1.0f));
	TurretMesh->SetCanEverAffectNavigation(false);

	// PARTICLE SYSTEMS
	barrelPos = CreateDefaultSubobject<USceneComponent>(TEXT("Barrel"));
	barrelPos->SetRelativeLocation(FVector(22.5f, -1.34f, 39.0f));
	barrelPos->SetupAttachment(TurretMesh);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);

	PS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_RocketShooting.P_RocketShooting'")).Get();
	reactionPS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_Explosion.P_Explosion'")).Get();

	currentAttackTimer = 0.0f;

	static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("Class'/Game/Game_Assets/Blueprints/BarracksBlowingUp.BarracksBlowingUp_C'"));
	if (ItemBlueprint.Object) {
		ExplosionBlueprint = (UClass*)ItemBlueprint.Object;
	}
}

void ABuilding_Turret_Artillery::BeginPlay()
{
	Super::BeginPlay();
	
	buildingMesh->SetWorldScale3D(FVector(5));

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
		if (targetActor != nullptr)
		{
			//FVector targetLocation = targetActor->GetActorLocation() - GetActorLocation();
			//FRotator targetRotation = FRotationMatrix::MakeFromX(targetLocation).Rotator();
			//PivotMesh->SetWorldRotation(targetRotation);
		}

		else
		{
			//PivotMesh->SetWorldRotation(RootComponent->GetComponentRotation());
		}

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
		if (targetActor == nullptr)
		{
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

		// A target actor exists
		else
		{
			// If the target is out of range, reset the targetActor reference
			if (FVector::Dist(GetActorLocation(), targetActor->GetActorLocation()) > detectRange)
				targetActor = nullptr;

			// Target is in range
			else
			{
				// Rotate towards the target 
				RootComponent->SetRelativeRotation((targetActor->GetActorLocation() - RootComponent->GetComponentLocation()).Rotation());

				if (currentAttackTimer >= attackRate)
				{
					currentAttackTimer = 0.0f;

					AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), barrelPos->GetComponentLocation(), FRotator(0.0f, 0.0f, 0.0f));
					projectile->InitializeProjectile(PROJECTILE_TYPE::CANNON, targetActor->GetActorLocation(), attackDamage, 4000.0f, 0.0f, 100.0f, PS, reactionPS);
					projectile->SetActorEnableCollision(false);
				}

				if (Cast<II_Entity>(targetActor)->GetCurrentHealth() - attackDamage <= 0)
					targetActor = nullptr;

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