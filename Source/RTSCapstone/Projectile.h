// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"

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

	void InitializeProjectile(PROJECTILE_TYPE type, FVector targetPosition, float damage, float travelSpeed, float travelDistance, UParticleSystem* particleSystem);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UStaticMeshComponent* staticMesh;

	PROJECTILE_TYPE projectileType;
	float projectileDamage;

	FVector targetPosition;
	float travelTime;
	float travelDistance;

	UPROPERTY()
		UParticleSystemComponent* particleComp;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
