// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ConstructorHelpers.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	if (GetActorLocation() != targetPosition)
	{
		//RootComponent->SetWorldLocation(FMath::VInterpTo(GetActorLocation(), targetPosition, DeltaTime, travelTime));
		RootComponent->SetWorldLocation(FMath::VInterpConstantTo(GetActorLocation(), targetPosition, DeltaTime, travelTime));

	}

	else
	{

		countdown -= DeltaTime;

		/*
		UWorld* const World = this->GetWorld();
		UGameplayStatics::SpawnEmitterAtLocation(World, reactionPS, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f), true, EPSCPoolMethod::AutoRelease);
		*/

		if (!reachedTarget) {
			UE_LOG(LogTemp, Warning, TEXT("PROJECTILE AT TARGET!"));

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
}

