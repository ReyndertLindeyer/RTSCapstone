// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Turret_Gattling.h"

#include "Kismet/KismetSystemLibrary.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

#include "Projectile.h"

ABuilding_Turret_Gattling::ABuilding_Turret_Gattling() {
	spawnTime = 2;
	isBuilding = true;
	hasPower = true;
	alternateShot = false;
	
	// Body
	static ConstructorHelpers::FObjectFinder<UStaticMesh>BodyMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Gattling_Turret/Gattling_Turret_V1_UNREAL_Base.Gattling_Turret_V1_UNREAL_Base'"));
	UStaticMesh* bodyMesh = BodyMeshAsset.Object;
	buildingMesh->SetStaticMesh(bodyMesh);
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	buildingMesh->SetRelativeScale3D(FVector(3.0f));
	buildingMesh->SetCanEverAffectNavigation(false);
	buildingMesh->SetSimulatePhysics(false);

	// Pivot
	PivotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pivot Mesh"));
	PivotMesh->SetupAttachment(buildingMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PivotMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Gattling_Turret/Gattling_Turret_V1_UNREAL_Pivot.Gattling_Turret_V1_UNREAL_Pivot'"));
	UStaticMesh* pivotMesh = PivotMeshAsset.Object;
	PivotMesh->SetStaticMesh(pivotMesh);
	PivotMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PivotMesh->SetRelativeScale3D(FVector(1.0f));
	PivotMesh->SetCanEverAffectNavigation(false);

	// Turret
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(PivotMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TurretMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Gattling_Turret/Gattling_Turret_V1_UNREAL_Head.Gattling_Turret_V1_UNREAL_Head'"));
	UStaticMesh* turretMesh = TurretMeshAsset.Object;
	TurretMesh->SetStaticMesh(turretMesh);
	TurretMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	TurretMesh->SetRelativeScale3D(FVector(1.0f));
	TurretMesh->SetCanEverAffectNavigation(false);

	// Gun 1
	RightGunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Gun Mesh"));
	RightGunMesh->SetupAttachment(TurretMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>RightGunMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Gattling_Turret/Gattling_Turret_V1_UNREAL_R-Barrel.Gattling_Turret_V1_UNREAL_R-Barrel'"));
	UStaticMesh* rightGunMesh = RightGunMeshAsset.Object;
	RightGunMesh->SetStaticMesh(rightGunMesh);
	RightGunMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	RightGunMesh->SetRelativeScale3D(FVector(1.0f));
	RightGunMesh->SetCanEverAffectNavigation(false);

	// GUN 2
	LeftGunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Gun Mesh"));
	LeftGunMesh->SetupAttachment(TurretMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>LeftGunMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Gattling_Turret/Gattling_Turret_V1_UNREAL_L-Barrel.Gattling_Turret_V1_UNREAL_L-Barrel'"));
	UStaticMesh* leftGunMesh = LeftGunMeshAsset.Object;
	LeftGunMesh->SetStaticMesh(leftGunMesh);
	LeftGunMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	LeftGunMesh->SetRelativeScale3D(FVector(1.0f));
	LeftGunMesh->SetCanEverAffectNavigation(false);

	// PARTICLE SYSTEMS
	barrelPos1 = CreateDefaultSubobject<USceneComponent>(TEXT("Left Barrel"));
	barrelPos1->SetRelativeLocation(FVector(42.0f, 16.5f, 34.5f));
	barrelPos1->SetupAttachment(LeftGunMesh);

	barrelPos2 = CreateDefaultSubobject<USceneComponent>(TEXT("Right Barrel"));
	barrelPos2->SetRelativeLocation(FVector(-42.0f, -16.5f, 34.5f));
	barrelPos2->SetupAttachment(RightGunMesh);


	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);

	PS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_RifleShooting.P_RifleShooting'")).Get();
	reactionPS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_BulletHit.P_BulletHit'")).Get();

	currentAttackTimer = 0.0f;

	static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("Class'/Game/Game_Assets/Blueprints/BarracksBlowingUp.BarracksBlowingUp_C'"));
	if (ItemBlueprint.Object) {
		ExplosionBlueprint = (UClass*)ItemBlueprint.Object;
	}
}

void ABuilding_Turret_Gattling::BeginPlay()
{
	Super::BeginPlay();
	
	buildingMesh->SetWorldScale3D(FVector(5));

}

void ABuilding_Turret_Gattling::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (setPlayerOwner != nullptr) {
		InitializeStructure(Cast<II_Player>(setPlayerOwner), "Placeholder", 10.0f);
		SetName(TEXT("GunTurret"));
		SetMaxHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("GunTurret")), (TEXT("Context")), false)->MaxHealth);
		SetCurrentHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("GunTurret")), (TEXT("Context")), false)->MaxHealth);
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

		// If there is no target, run the detection sequence.
		if (!targetActor->IsValidLowLevel())
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

		// A target actor exists
		else
		{
			// If the target is out of range, reset the targetActor reference
			if (FVector::Dist(GetActorLocation(), targetActor->GetActorLocation()) > detectRange)
				targetActor = nullptr;

			// Target is in range
			else
			{
				// Rotate towards target
				FVector Dir = (targetActor->GetActorLocation() - GetActorLocation());
				Dir.Normalize();

				PivotMesh->SetWorldRotation(FMath::Lerp(PivotMesh->GetComponentRotation(), Dir.Rotation(), 0.05f));

				// Attack Target
				if (currentAttackTimer >= attackRate)
				{
					currentAttackTimer = 0.0f;

					if (alternateShot)
					{
						AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), barrelPos1->GetComponentLocation(), FRotator(0.0f, 0.0f, 0.0f));
						projectile->InitializeProjectile(PROJECTILE_TYPE::CANNON, targetActor->GetActorLocation(), attackDamage, 5000.0f, 0.0f, 1.0f, PS, reactionPS);
						projectile->SetActorEnableCollision(false);
						alternateShot = false;
					}

					else
					{
						AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), barrelPos2->GetComponentLocation(), FRotator(0.0f, 0.0f, 0.0f));
						projectile->InitializeProjectile(PROJECTILE_TYPE::CANNON, targetActor->GetActorLocation(), attackDamage, 5000.0f, 0.0f, 1.0f, PS, reactionPS);
						projectile->SetActorEnableCollision(false);
						alternateShot = true;
					}
					

					if (Cast<II_Entity>(targetActor)->GetCurrentHealth() - attackDamage <= 0)
						targetActor = nullptr;
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

void ABuilding_Turret_Gattling::SetHasPower(bool inBool)
{
	//Disable attack functions
}

bool ABuilding_Turret_Gattling::constructAtLocation(II_Player* player)
{
	dustParticleComp->SetWorldLocation(this->GetActorLocation());
	dustParticleComp->ActivateSystem();
	tempHeight = RootComponent->GetComponentLocation().Z;
	buildingMesh->SetWorldLocation(FVector(RootComponent->GetComponentLocation().X, RootComponent->GetComponentLocation().Y, RootComponent->GetComponentLocation().Z - 325));


	return false;
}