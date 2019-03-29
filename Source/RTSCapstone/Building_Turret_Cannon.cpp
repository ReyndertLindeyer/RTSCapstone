// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Turret_Cannon.h"

#include "Kismet/KismetSystemLibrary.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

#include "Projectile.h"



ABuilding_Turret_Cannon::ABuilding_Turret_Cannon() {
	spawnTime = 2;
	isBuilding = true;
	hasPower = true;

	ConstructorHelpers::FObjectFinderOptional<UParticleSystem> PSA(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_BuildingDust.P_BuildingDust'"));
	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MyPSC"));
	particleComp->SetTemplate(PSA.Get());
	particleComp->bAutoActivate = false;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devTurret_v1.devTurret_v1")).Get());
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);


	PS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_RocketShooting.P_RocketShooting'")).Get();
	reactionPS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_Explosion.P_Explosion'")).Get();


	currentAttackTimer = 0.0f;
}

void ABuilding_Turret_Cannon::BeginPlay()
{
	Super::BeginPlay();
	if (setPlayerOwner != nullptr) {
		InitializeStructure(Cast<II_Player>(setPlayerOwner), "Placeholder", 10.0f);
		SetName(TEXT("CannonTurret"));
		SetMaxHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("CannonTurret")), (TEXT("Context")), false)->MaxHealth);
		SetCurrentHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("CannonTurret")), (TEXT("Context")), false)->MaxHealth);
	}

	buildingMesh->SetWorldScale3D(FVector(5));
	particleComp->SetWorldLocation(this->GetActorLocation());
	particleComp->ActivateSystem();
}

void ABuilding_Turret_Cannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("TURRET IS TICKING"));

	if (constructed)
	{
		// Detect all AActors within a Radius
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		TArray<AActor*> ignoreActors;
		TArray<AActor*> outActors;

		ignoreActors.Add(this);

		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), detectRange, objectTypes, nullptr, ignoreActors, outActors);

		// Debug Turret Range
		DrawDebugSphere(GetWorld(), GetActorLocation(), detectRange, 24, FColor(0, 0, 255));


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
					// Check if the ownership of that actor is different from the ownership of this structure
					if (Cast<II_Entity>(entitiesInRange[i])->GetEntityOwner() != GetEntityOwner() || Cast<II_Entity>(entitiesInRange[i])->GetEntityOwner()->teamValue != GetEntityOwner()->teamValue)
					{
						// If there currenty isn't an actor set
						if (targetActor != this)
						{
							// Set the first actor that it can
							targetActor = entitiesInRange[0];
						}
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
				//RootComponent->SetRelativeRotation((targetActor->GetActorLocation() - RootComponent->GetComponentLocation()).Rotation());

				if (currentAttackTimer >= attackRate)
				{
					currentAttackTimer = 0.0f;

					AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
					projectile->InitializeProjectile(PROJECTILE_TYPE::CANNON, targetActor->GetActorLocation(), attackDamage, 5000.0f, 0.0f, 100.0f, PS, reactionPS);
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

void ABuilding_Turret_Cannon::SetHasPower(bool inBool)
{
	//Disable attack functions
}