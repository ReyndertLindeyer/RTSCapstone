// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

#include "Building_Turret_Gattling.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_Turret_Gattling : public ABuildingMaster
{
	GENERATED_BODY()

public:
	ABuilding_Turret_Gattling();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual bool constructAtLocation(II_Player* player) override;

	void SetHasPower(bool inBool); //Sets the boolean for if the structure has enough power

	//The particle system that will be the projectile
	UParticleSystem* PS;

	//The particle system that will be the reaction at the end, ex the explosion when the rocket connects
	UParticleSystem* reactionPS;

	UPROPERTY(EditAnywhere)
		AActor* setPlayerOwner;

	// AI STUFF

	UPROPERTY()
		AActor* targetActor;
	
	UPROPERTY()
		TArray<AActor*> entitiesInRange;
	
	float currentAttackTimer;

	UPROPERTY(EditAnywhere)
		float attackDamage = 11.0f;
	//UPROPERTY(EditAnywhere)
	//	float startingHealth = 225.0f;
	UPROPERTY(EditAnywhere)
		float attackRate = 0.25f;
	UPROPERTY(EditAnywhere)
		float detectRange = 2000.0f;

	UStaticMeshComponent* TurretMesh;
	UStaticMeshComponent* PivotMesh;
	UStaticMeshComponent* LeftGunMesh;
	UStaticMeshComponent* RightGunMesh;

	UPROPERTY(EditAnywhere)
		USceneComponent* barrelPos1;
	UPROPERTY(EditAnywhere)
		USceneComponent* barrelPos2;

private:
	uint32 range;
	bool hasPower;
	bool alternateShot;
};
