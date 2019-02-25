// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//How many of each kind of units the launch point should collect before sending them off
	UPROPERTY()
		int32 numOfBasicMelee;
	UPROPERTY()
		int32 numOfBasicRanged;
	UPROPERTY()
		int32 numOfAdvancedMelee;
	UPROPERTY()
		int32 numOfAdvancedRanged;
	UPROPERTY(EditAnywhere)
		int32 collectionRadius;


	///////////////////////////////////////////
	//Put enemy arrays here
	///////////////////////////////////////////
};
