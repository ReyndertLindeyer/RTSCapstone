// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "UNIT_Rifleman.h"
#include "UNIT_Rocketeer.h"
#include "UNIT_Engineer.h"

#include "BuildingMaster.h"
#include "Building_Construction_Yard.h"
#include "Building_PowerPlant.h"
#include "Building_Barrecks.h"
#include "Building_Refinery.h"
#include "Building_VehicleFactory.h"
#include "Building_TechCenter.h"
#include "Building_Superweapon.h"
#include "Building_Turret_Gattling.h"
#include "Building_Turret_Cannon.h"
#include "Building_Turret_Artillery.h"
#include "Building_Turret_Tesla.h"

#include "Enemy_AttackLaunchPoint.generated.h"

UCLASS()
class RTSCAPSTONE_API AEnemy_AttackLaunchPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_AttackLaunchPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		USphereComponent* radiusSphere; //Collision sphere to get a reference to all units in the area

	UFUNCTION()
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//How many of each kind of units the launch point should collect before sending them off
	UPROPERTY(EditAnywhere)
		int32 numOfBasicMelee;
	UPROPERTY(EditAnywhere)
		int32 numOfBasicRanged;
	UPROPERTY(EditAnywhere)
		int32 numOfAdvancedMelee;
	UPROPERTY(EditAnywhere)
		int32 numOfAdvancedRanged;
	UPROPERTY(EditAnywhere)
		int32 collectionRadius;

	void EnableTick();

	///////////////////////////////////////////
	//Put enemy arrays here
	///////////////////////////////////////////
	TArray<AUNIT_Rifleman*> basicArrayA;
	TArray<AUNIT_Rocketeer*> basicArrayB;
	TArray<AUNIT_Engineer*> basicArrayC;
};
