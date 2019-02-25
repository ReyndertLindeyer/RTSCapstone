// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_AttackLaunchPoint.h"

// Sets default values
AEnemy_AttackLaunchPoint::AEnemy_AttackLaunchPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collectionRadius = 10;

	radiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("baseRadius"));
	radiusSphere->SetSphereRadius(collectionRadius);
	radiusSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	radiusSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_AttackLaunchPoint::BeginOverlap);
}

// Called when the game starts or when spawned
void AEnemy_AttackLaunchPoint::BeginPlay()
{
	Super::BeginPlay();

	///If all of the arrays are full of the correct unit then launch a wave towards the nearest player structure
	
}

void AEnemy_AttackLaunchPoint::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		///Check to see if the colliding unit is an enemy unit, what kind it is, and add it to the correct array
	}
}

// Called every frame
void AEnemy_AttackLaunchPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

