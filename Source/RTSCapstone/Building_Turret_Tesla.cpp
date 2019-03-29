// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Turret_Tesla.h"

#include "Kismet/KismetSystemLibrary.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

ABuilding_Turret_Tesla::ABuilding_Turret_Tesla() {
	spawnTime = 2;
	isBuilding = true;
	hasPower = true;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devTesla_v1.devTesla_v1")).Get());
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);

	particleSystem = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_Lightning.P_Lightning'")).Get();

	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MyPSCA"));
	particleComp->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	particleComp->SetTemplate(particleSystem);
	particleComp->bAutoActivate = false;
	particleComp->SetupAttachment(RootComponent);

	currentAttackTimer = 0.0f;

	chain1 = false;
	chain2 = false;
}

void ABuilding_Turret_Tesla::BeginPlay()
{
	Super::BeginPlay();
	if (setPlayerOwner != nullptr) {
		InitializeStructure(Cast<II_Player>(setPlayerOwner), "Placeholder", 10.0f);
		SetName(TEXT("TeslaTurret"));
		SetMaxHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("TeslaTurret")), (TEXT("Context")), false)->MaxHealth);
		SetCurrentHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("TeslaTurret")), (TEXT("Context")), false)->MaxHealth);
	}

	buildingMesh->SetWorldScale3D(FVector(5));

}

void ABuilding_Turret_Tesla::Tick(float DeltaTime)
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
					if (Cast<II_Entity>(entitiesInRange[i])->GetEntityOwner() != GetEntityOwner())
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
			
				TArray<AActor*> chainTargetActors;

				if (currentAttackTimer >= attackRate)
				{
					
					if (chain1 && c1TargetActor != nullptr)
					{
						
						// Add Particle start to tesla and partile end to target location
						particleComp->SetBeamSourcePoint(0, targetActor->GetActorLocation(), 0);
						particleComp->SetBeamTargetPoint(0, c1TargetActor->GetActorLocation(), 0);
						particleComp->ActivateSystem(true);
						
						// Directly damage the target entity
						Cast<II_Entity>(c1TargetActor)->DealDamage(attackDamage);

						// Acquire a list of actors nearby the target that got chained to
						UKismetSystemLibrary::SphereOverlapActors(GetWorld(), c1TargetActor->GetActorLocation(), chainRange, objectTypes, nullptr, ignoreActors, chainTargetActors);

						// For each actor found
						for (int i = 0; i < chainTargetActors.Num(); i++)
						{
							// Check to see if it has an Entity script
							if (Cast<II_Entity>(chainTargetActors[i]))
							{
								// If it does, confirm that the Entity script is an enemy
								if (Cast<II_Entity>(chainTargetActors[i])->GetEntityOwner() != GetEntityOwner())
								{
									// If the Entity is an enemy, confirm that it is neither the original target, nor the first chained target
									if (chainTargetActors[i] != targetActor && chainTargetActors[i] != c1TargetActor)
									{
										// Set the second chain target
										c2TargetActor = chainTargetActors[i];
										
										// Enable the second chain sequence on next call
										chain2 = true;
										
										// Disable the first chain sequence to prevent recalling
										chain1 = false;

										//UE_LOG(LogTemp, Warning, TEXT("CHAIN 2"));

										// And break out of the loop
										break;
									}
									
								}
							}
						}

						// If a target to chain to was found, adjust the timer on the next attack cycle, otherwise reset it
						if (c2TargetActor == nullptr)
							currentAttackTimer = 0.0f;
						else
							currentAttackTimer = attackRate - 0.1f;
					}
					
					else if (chain2 && c2TargetActor != nullptr)
					{
						// Add Particle start to tesla and partile end to target location
						particleComp->SetBeamSourcePoint(0, c1TargetActor->GetActorLocation(), 0);
						particleComp->SetBeamTargetPoint(0, c2TargetActor->GetActorLocation(), 0);
						particleComp->ActivateSystem(true);
						
						// Directly damage the target entity
						Cast<II_Entity>(c2TargetActor)->DealDamage(attackDamage);

						//UE_LOG(LogTemp, Warning, TEXT("CHAIN COMPLETE"));

						// Reset the two chained actors for next attack cycle
						c1TargetActor = nullptr;
						c2TargetActor = nullptr;

						// Reset the attack timer
						currentAttackTimer = 0.0f;
					}

					else
					{
						// Add Particle start to tesla and partile end to target location
						particleComp->SetBeamSourcePoint(0, GetActorLocation(), 0);
						particleComp->SetBeamTargetPoint(0, targetActor->GetActorLocation(), 0);
						particleComp->ActivateSystem(true);
						
						// Directly damage the target entity
						Cast<II_Entity>(targetActor)->DealDamage(attackDamage);

						// Acquire a list of actors nearby the target that got hit
						UKismetSystemLibrary::SphereOverlapActors(GetWorld(), targetActor->GetActorLocation(), chainRange, objectTypes, nullptr, ignoreActors, chainTargetActors);

						// For each actor found
						for (int i = 0; i < chainTargetActors.Num(); i++)
						{
							// Check to see if it has an Entity script
							if (Cast<II_Entity>(chainTargetActors[i]))
							{
								// If it does, confirm that the Entity script is an enemy
								if (Cast<II_Entity>(chainTargetActors[i])->GetEntityOwner() != GetEntityOwner())
								{
									// If the Entity is an enemy, confirm that isn't the same as the original target
									if (chainTargetActors[i] != targetActor)
									{
										// Set the first chain target
										c1TargetActor = chainTargetActors[i];
										
										// Enable the first chain sequence on next call
										chain1 = true;

										//UE_LOG(LogTemp, Warning, TEXT("CHAIN 1"));
										
										// And break the loop
										break;
									}
								}
							}
						}

						// If a target to chain to was found, adjust the timer on the next attack cycle, otherwise reset it
						if (c1TargetActor == nullptr)
							currentAttackTimer = 0.0f;
						else
							currentAttackTimer = attackRate - 0.1f;
					}
					
					
					

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

void ABuilding_Turret_Tesla::SetHasPower(bool inBool)
{
	//Disable attack functions
}


