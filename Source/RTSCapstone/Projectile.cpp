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

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	UStaticMesh* Asset = MeshAsset.Object;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	staticMesh->SetStaticMesh(Asset);
	staticMesh->SetRelativeLocation(FVector(0.0f));
	staticMesh->SetRelativeScale3D(FVector(0.25f));

	RootComponent = staticMesh;
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
		UE_LOG(LogTemp, Warning, TEXT("PROJECTILE AT TARGET!"));

		// Go Boom
		// Detect all AActors within a Radius
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		TArray<AActor*> ignoreActors;
		TArray<AActor*> outActors;

		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 100.0f, objectTypes, nullptr, ignoreActors, outActors);

		for (int i = 0; i < outActors.Num(); i++)
		{
			if (Cast<II_Entity>(outActors[i]))
			{
				Cast<II_Entity>(outActors[i])->DealDamage(projectileDamage);
			}
		}

		Destroy(this);

	}


}

void AProjectile::InitializeProjectile(PROJECTILE_TYPE type, FVector target, float damage, float speed, float distance)
{
	projectileType = type;
	targetPosition = target;
	travelTime = speed;
	travelDistance = distance;
	projectileDamage = damage;
}

