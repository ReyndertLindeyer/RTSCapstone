// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "Building_Superweapon.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_Superweapon : public ABuildingMaster
{
	GENERATED_BODY()
public:
	ABuilding_Superweapon();

	UPROPERTY(EditAnywhere)
		AActor* setPlayerOwner;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetHasPower(bool inBool); //Sets the boolean for if the structure has enough power

	bool isReady;
	bool isActivated;

	float currentTimer;

	UPROPERTY(EditAnywhere)
		float chargeTime = 60.0f;
	UPROPERTY(EditAnywhere)
		float damage = 1000.0f;

	//The particle system that will be the projectile
	UParticleSystem* PS;

	//The particle system that will be the reaction at the end, ex the explosion when the rocket connects
	UParticleSystem* reactionPS;

private:
	bool hasPower;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* particleComp;
};
