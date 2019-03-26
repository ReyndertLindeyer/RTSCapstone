// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Building_Turret_Tesla.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_Turret_Tesla : public ABuildingMaster
{
	GENERATED_BODY()

public:
	ABuilding_Turret_Tesla();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetHasPower(bool inBool); //Sets the boolean for if the structure has enough power

	UPROPERTY(EditAnywhere)
		AActor* setPlayerOwner;

	// AI STUFF
	AActor* targetActor;
	AActor* c1TargetActor;
	AActor* c2TargetActor;
	
	TArray<AActor*> entitiesInRange;
	float currentAttackTimer;

	UPROPERTY(EditAnywhere)
		float attackDamage = 75.0f;
	//UPROPERTY(EditAnywhere)
	//	float startingHealth = 225.0f;
	UPROPERTY(EditAnywhere)
		float attackRate = 4.0f;
	UPROPERTY(EditAnywhere)
		float detectRange = 3000.0f;
	UPROPERTY(EditAnywhere)
		float chainRange = 150.0f;
	

		bool chain1;
		bool chain2;

private:
	uint32 range;
	bool hasPower;


	UPROPERTY()
		UParticleSystemComponent* particleComp;
	UParticleSystem* particleSystem;
	
};
