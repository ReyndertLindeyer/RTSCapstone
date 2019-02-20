// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building_Enemy_Spawner.h"
#include "Components/SphereComponent.h"
#include "Enemy_BaseManager.generated.h"

UCLASS()
class RTSCAPSTONE_API AEnemy_BaseManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_BaseManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		USphereComponent* baseRadiusSphere; //Collision sphere to get a reference to all structures in the area

	UFUNCTION()
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY()
		TArray <ABuilding_Enemy_Spawner*> buildingsArray;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		int32 baseRadius;

};
