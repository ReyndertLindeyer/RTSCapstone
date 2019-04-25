// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	arcing = false;

	root = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	root->InitSphereRadius(1);
	root->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = root;

	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MyPSC"));
	particleComp->SetupAttachment(RootComponent);
	particleComp->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	particleComp->bAutoActivate = false;

	countdown = 6.0f;
	reachedTarget = false;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Debug the blast radius
	//DrawDebugSphere(GetWorld(), GetActorLocation(), blastRadius, 24, FColor(0, 0, 255));

	if (GetActorLocation() != targetPosition)
	{
		//If the projectile is supposed to move in a arch
		if (!arcing) {
			RootComponent->SetWorldLocation(FMath::VInterpConstantTo(GetActorLocation(), targetPosition, DeltaTime, travelTime));
		}
		else {
			RootComponent->SetWorldLocation(FMath::VInterpConstantTo(GetActorLocation(), targetPosition, DeltaTime, travelTime));

			//Check to see how far into the shot the projectile is

			float temp1 = FVector::Dist(GetActorLocation(), targetPosition);
			float temp2 = FVector::Dist(initialPosition, targetPosition);

			temp1 = temp1 / temp2;

			//Move the projectile up or down depending on when in the arc it is and how far the shot is going in the first place
			particleComp->SetRelativeLocation(FVector(0.0, 0.0, ((temp2) * cosf(1 - (temp1 * 2))) - temp2 / 2));
		}

	}

	else
	{

		countdown -= DeltaTime;

		/*
		UWorld* const World = this->GetWorld();
		UGameplayStatics::SpawnEmitterAtLocation(World, reactionPS, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f), true, EPSCPoolMethod::AutoRelease);
		*/

		if (!reachedTarget) {
			//UE_LOG(LogTemp, Warning, TEXT("PROJECTILE AT TARGET!"));

			// Go Boom
			// Detect all AActors within a Radius
			TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
			TArray<AActor*> ignoreActors;
			TArray<AActor*> outActors;

			UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), blastRadius, objectTypes, nullptr, ignoreActors, outActors);

			for (int i = 0; i < outActors.Num(); i++)
			{
				if (Cast<II_Entity>(outActors[i]))
				{
					Cast<II_Entity>(outActors[i])->DealDamage(projectileDamage);
				}
			}

			particleComp->SetTemplate(reactionPS);
			particleComp->ActivateSystem(true);
			reachedTarget = true;
		}

	}

	if (countdown <= 0.0f) {
		Destroy(this);
	}


}

void AProjectile::InitializeProjectile(PROJECTILE_TYPE type, FVector target, float damage, float speed, float distance, float radius, UParticleSystem* particleSystemA, UParticleSystem* particleSystemB)
{
	projectileType = type;
	targetPosition = target;
	travelTime = speed;
	travelDistance = distance;
	projectileDamage = damage;
	particleComp->SetTemplate(particleSystemA);
	particleComp->ActivateSystem(true);
	reactionPS = particleSystemB;
	blastRadius = radius;
	initialPosition = GetActorLocation();
}


void AProjectile::InitializeProjectile(PROJECTILE_TYPE type, FVector target, float damage, float speed, float distance, float radius, UParticleSystem* particleSystemA, UParticleSystem* particleSystemB, bool shouldArc)
{
	projectileType = type;
	targetPosition = target;
	travelTime = speed;
	travelDistance = distance;
	projectileDamage = damage;
	particleComp->SetTemplate(particleSystemA);
	particleComp->ActivateSystem(true);
	reactionPS = particleSystemB;
	blastRadius = radius;
	initialPosition = GetActorLocation();
	arcing = shouldArc;
}

