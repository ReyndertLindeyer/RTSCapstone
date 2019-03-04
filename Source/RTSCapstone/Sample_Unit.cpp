// Fill out your copyright notice in the Description page of Project Settings.

#include "Sample_Unit.h"

// Sets default values
ASample_Unit::ASample_Unit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetWorldScale3D(FVector(1.0f));
	
	SelectionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Selection Indicator"));
	SelectionIndicator->SetupAttachment(BodyMesh);
	SelectionIndicator->SetVisibility(false);
	SelectionIndicator->SetWorldLocation(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));

}

// Called when the game starts or when spawned
void ASample_Unit::BeginPlay()
{
	Super::BeginPlay();

	InitializeEntity(nullptr, "Sample_Unit", 100.0f);
	
}

// Called every frame
void ASample_Unit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASample_Unit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/// Disabled for now.  Keep for reference
//void ASample_Unit::SetDestination(FVector destination)
//{
//	UE_LOG(LogTemp, Warning, TEXT("Overridden: Setting Destination"));
//
//}

/// Disabled for now.  Keep for reference
//AController* ASample_Unit::GetUnitController()
//{
//	return GetController();
//}

void ASample_Unit::SetSelection(bool state)
{
	SelectionIndicator->SetVisibility(state);
}

void ASample_Unit::DestroyEntity()
{
	Destroy(this);
}

