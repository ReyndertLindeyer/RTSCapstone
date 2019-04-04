// Fill out your copyright notice in the Description page of Project Settings.

#include "SE_EstablishBase.h"
#include "Building_Refinery.h"
#include "Building_TechCenter.h"
#include "Building_Barrecks.h"
#include "Building_VehicleFactory.h"

#include "BuildingManagerObject.h"

// Sets default values
ASE_EstablishBase::ASE_EstablishBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASE_EstablishBase::BeginPlay()
{
	Super::BeginPlay();

	if (SetTriggerActors.Num() > 0)
		triggerActors = SetTriggerActors;
	
}

// Called every frame
void ASE_EstablishBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < triggerActors.Num(); i++)
	{
		if (!triggerActors[i]->IsValidLowLevel())
		{
			triggerActors.RemoveAt(i);
			break;
		}
	}
}

void ASE_EstablishBase::TriggerEvent(II_Player* params)
{
	if (structureNodes.Num() > 0)
	{
		// At the structureNode's position, construct the building found within the structureNode's object reference.
		for (int i = 0; i < structureNodes.Num(); i++)
		{	
			UClass* object = structureNodes[i]->structureReference;

			if (params != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("BUILDING FOUND"));
			}
			
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Nope"));
			}
				

			ABuildingMaster* building = GetWorld()->SpawnActor<ABuildingMaster>(object, structureNodes[i]->GetActorLocation(), structureNodes[i]->GetActorRotation());
			building->InitializeEntity(params, "Placeholder", 1000.0f);
			building->constructAtLocation(params);

			if (object == ABuilding_Refinery::StaticClass())
			{
				for (TObjectIterator<UBuildingManagerObject> Itr; Itr; ++Itr)
				{
					UE_LOG(LogTemp, Warning, TEXT("Refinery Placed, unlocking Factory"));
					Itr->refineryArray.Add((ABuilding_Refinery*)building);
				}
			}
				

			if (object == ABuilding_TechCenter::StaticClass())
			{
				for (TObjectIterator<UBuildingManagerObject> Itr; Itr; ++Itr)
				{
					Itr->techCenterArray.Add((ABuilding_TechCenter*)building);
				}
			}
				


			if (object == ABuilding_Barrecks::StaticClass())
				Cast<ABuilding_Barrecks>(object)->InitializeBarracks();

			if (object == ABuilding_VehicleFactory::StaticClass())
				Cast<ABuilding_VehicleFactory>(object)->InitializeWarFactory();


			//building->SetOwningEntity(params->targetPlayer);
			//building->constructAtLocation(params->targetPlayer);  // come back to this, and consider editing the function

		}

	}
}

