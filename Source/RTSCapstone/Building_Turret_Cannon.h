// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "Building_Turret_Cannon.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_Turret_Cannon : public ABuildingMaster
{
	GENERATED_BODY()

public:
	ABuilding_Turret_Cannon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetHasPower(bool inBool); //Sets the boolean for if the structure has enough power

	//The particle system that will be the projectile
	UParticleSystem* PS;

	//The particle system that will be the reaction at the end, ex the explosion when the rocket connects
	UParticleSystem* reactionPS;

	UPROPERTY(EditAnywhere)
		AActor* setPlayerOwner;

	// AI STUFF
	AActor* targetActor;
	TArray<AActor*> entitiesInRange;
	float currentAttackTimer;

	UPROPERTY(EditAnywhere)
		float attackDamage = 25.0f;
	//UPROPERTY(EditAnywhere)
	//	float startingHealth = 225.0f;
	UPROPERTY(EditAnywhere)
		float attackRate = 2.25f;
	UPROPERTY(EditAnywhere)
		float detectRange = 500.0f;
	
private:
	uint32 range;
	bool hasPower;
};
