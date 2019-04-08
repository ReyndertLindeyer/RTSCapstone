// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Components/SphereComponent.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"

#include "I_Entity.h"

#include "Projectile.generated.h"

UENUM()
enum PROJECTILE_TYPE
{
	CANNON,
	MISSILE,
};

UCLASS()
class RTSCAPSTONE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	void InitializeProjectile(PROJECTILE_TYPE type, FVector targetPosition, float damage, float travelSpeed, float travelDistance, float blastRadius, UParticleSystem* particleSystemA, UParticleSystem* particleSystemB);

	void InitializeProjectile(PROJECTILE_TYPE type, FVector targetPosition, float damage, float travelSpeed, float travelDistance, float blastRadius, UParticleSystem* particleSystemA, UParticleSystem* particleSystemB, bool shouldArc);

	bool reachedTarget;
	float countdown;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UStaticMeshComponent* staticMesh;

	PROJECTILE_TYPE projectileType;
	float projectileDamage;

	FVector targetPosition;
	FVector initialPosition;
	float travelTime;
	float travelDistance;

	float blastRadius;

	bool arcing;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* particleComp;

	UPROPERTY(EditAnywhere)
		USphereComponent* root;

	//The particle system that will be the reaction at the end, ex the explosion when the rocket connects
	UParticleSystem* reactionPS;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
