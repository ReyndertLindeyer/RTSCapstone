// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingManagerObject.h"

// Sets default values
UBuildingManagerObject::UBuildingManagerObject()
{
	currentPower = 20;
	maxPower = currentPower;
	resources = 5000; 
	buildingCosts.Add(100);
	buildingCosts.Add(300);
	buildingCosts.Add(200);
	buildingCosts.Add(500);
	buildingCosts.Add(1000);
	buildingConstructionTimes.Add(1);
	buildingConstructionTimes.Add(1);
	buildingConstructionTimes.Add(1);
	buildingConstructionTimes.Add(1);
	buildingConstructionTimes.Add(1);

	canBuildIndicator = CreateDefaultSubobject<UMaterial>(TEXT("GreenBuildingGhost"));
	canBuildIndicator = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Game_Assets/Materials/GreenBuildingGhost")).Get();

	cantBuildIndicator = CreateDefaultSubobject<UMaterial>(TEXT("cantBuildIndicator"));
	cantBuildIndicator = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Game_Assets/Materials/RedBuildingGhost")).Get();

	regularMaterial = CreateDefaultSubobject<UMaterial>(TEXT("regularMaterial"));
	regularMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Game_Assets/Materials/regularMaterial")).Get();
}

void UBuildingManagerObject::ghostBuilding(uint8 whatBuilding, FVector spawnLocation)
{
	UWorld* const World = this->GetWorld();
	if (World)
	{
		if (whatBuilding == 1) {
			buildingToBuild = World->SpawnActor<ABuilding_PowerPlant>(ABuilding_PowerPlant::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding == 2) {
			buildingToBuild = World->SpawnActor<ABuilding_Refinery>(ABuilding_Refinery::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding == 3) {
			buildingToBuild = World->SpawnActor<ABuilding_Barrecks>(ABuilding_Barrecks::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding == 4) {
			buildingToBuild = World->SpawnActor<ABuilding_VehicleFactory>(ABuilding_VehicleFactory::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding == 5) {
			buildingToBuild = World->SpawnActor<ABuilding_TechCenter>(ABuilding_TechCenter::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		}
		EnableAllDecals();
	}
}

void UBuildingManagerObject::MoveBuilding(FVector location)
{
	buildingToBuild->SetActorLocation(location);
	if (buildingToBuild->GetIsInRadius() && !buildingToBuild->GetIsOverlapping() && buildingToBuild->GetBuildingMesh()->GetMaterial(0) != canBuildIndicator) {
		buildingToBuild->GetBuildingMesh()->SetMaterial(0, canBuildIndicator);
	}
	else if (!buildingToBuild->GetIsInRadius() || buildingToBuild->GetIsOverlapping() && buildingToBuild->GetBuildingMesh()->GetMaterial(0) != cantBuildIndicator) {
		buildingToBuild->GetBuildingMesh()->SetMaterial(0, cantBuildIndicator);
	}
}

ABuildingMaster * UBuildingManagerObject::GetBuildingToBuild()
{
	return buildingToBuild;
}

void UBuildingManagerObject::SpawnConstructionYard(FVector spawnLocation)
{
	constructionYard = (GetWorld()->SpawnActor<ABuilding_Construction_Yard>(ABuilding_Construction_Yard::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f)));
	masterArray.Add(constructionYard);

}

bool UBuildingManagerObject::constructBuilding()
{
	if ((int32)buildingToBuild->GetCost() < resources && buildingToBuild->constructAtLocation()) {
		currentPower -= buildingToBuild->GetPowerUsage();
		if (buildingToBuild->GetPowerUsage() < 0) {
			maxPower -= buildingToBuild->GetPowerUsage();
		}

		if(buildingToBuild->IsA(ABuilding_PowerPlant::StaticClass())) {
			powerPlantArray.Add((ABuilding_PowerPlant*)buildingToBuild);
		}
		else if (buildingToBuild->IsA(ABuilding_Refinery::StaticClass())) {
			refineryArray.Add((ABuilding_Refinery*)buildingToBuild);
		}
		else if (buildingToBuild->IsA(ABuilding_Barrecks::StaticClass())) {
			barrecksArray.Add((ABuilding_Barrecks*)buildingToBuild);
		}
		else if (buildingToBuild->IsA(ABuilding_VehicleFactory::StaticClass())) {
			vehicleFactoryArray.Add((ABuilding_VehicleFactory*)buildingToBuild);
		}
		else if (buildingToBuild->IsA(ABuilding_TechCenter::StaticClass())) {
			techCenterArray.Add((ABuilding_TechCenter*)buildingToBuild);
		}

		buildingToBuild->GetBuildingMesh()->SetMaterial(0, regularMaterial);

		masterArray.Add(buildingToBuild);

		buildingToBuild = nullptr;

		DisableAllDecals();
		return true;
	}
	return false;
}

bool UBuildingManagerObject::trainInfantry(uint8 whatInfantry, ABuilding_Barrecks * whatBuilding)
{
	if (whatBuilding->GetUnitCost(whatInfantry) <= resources) {
		resources -= whatBuilding->GetUnitCost(whatInfantry);
		whatBuilding->AddToUnitQueue(whatInfantry);
		return true;
	}
	return false;
}

float UBuildingManagerObject::GetResources() {
	return (float)resources;
}


TArray<int32> UBuildingManagerObject::GetBuildingCost() 
{
	return buildingCosts;
}

TArray<int32> UBuildingManagerObject::GetConstructionTime()
{
	return buildingConstructionTimes;
}

int32 UBuildingManagerObject::GetCurrentPower()
{
	return currentPower;
}

int32 UBuildingManagerObject::GetMaxPower()
{
	return maxPower;
}

void UBuildingManagerObject::SubtractCost(int32 whatBuilding)
{
	resources -= buildingCosts[whatBuilding - 1];
}

void UBuildingManagerObject::AddCost(int32 whatBuilding)
{
	resources += buildingCosts[whatBuilding - 1];
}

void UBuildingManagerObject::EnableAllDecals()
{
	if (masterArray.Num() > 0) {
		for (int32 i = 0; i < masterArray.Num(); i++) {
			masterArray[i]->EnableBuildDecal();
		}
	}
}

void UBuildingManagerObject::DisableAllDecals()
{
	if (masterArray.Num() > 0) {
		for (int32 i = 0; i < masterArray.Num(); i++) {
			masterArray[i]->DisableBuildDecal();
		}
	}
}

void UBuildingManagerObject::CheckForDestroyedBuildings()
{
	if (masterArray.Num() > 0) {
		for (int i = 0; i < masterArray.Num(); i++) {
			if (masterArray[i]->IsDead()) {
				ABuildingMaster * temp = masterArray[i];
				masterArray.Remove(temp);
				temp->Suicide();
			}
		}
	}
}

bool UBuildingManagerObject::IsTechCentreBuilt()
{
	return false;
}

bool UBuildingManagerObject::IsRefineryBuilt()
{
	return false;
}
