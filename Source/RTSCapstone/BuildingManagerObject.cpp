// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingManagerObject.h"

// Sets default values
UBuildingManagerObject::UBuildingManagerObject()
{
	currentPower = 20;
	maxPower = currentPower;
	resources = 5000; 

	whatBuilding = 0;

	static ConstructorHelpers::FObjectFinderOptional<UDataTable> tempDataTable(TEXT("/Game/Game_Assets/DataTables/BuildingVariables.BuildingVariables"));
	buildingDataTable = tempDataTable.Get();

	static const FString ContextString(TEXT("Building Variable Context"));

	//Power Plant Variables
	FBuildingVariables* buildingVariables = buildingDataTable->FindRow<FBuildingVariables>(FName(TEXT("PowerPlant")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);

	//Refinery Variables
	buildingVariables = buildingDataTable->FindRow<FBuildingVariables>(FName(TEXT("Refinery")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);

	//Barracks Variables
	buildingVariables = buildingDataTable->FindRow<FBuildingVariables>(FName(TEXT("Barracks")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);

	//War Factory Variables
	buildingVariables = buildingDataTable->FindRow<FBuildingVariables>(FName(TEXT("WarFactory")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);

	//Tech Center Variables
	buildingVariables = buildingDataTable->FindRow<FBuildingVariables>(FName(TEXT("TechCenter")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);

	canBuildIndicator = CreateDefaultSubobject<UMaterial>(TEXT("GreenBuildingGhost"));
	canBuildIndicator = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Game_Assets/Materials/GreenBuildingGhost")).Get();

	cantBuildIndicator = CreateDefaultSubobject<UMaterial>(TEXT("cantBuildIndicator"));
	cantBuildIndicator = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Game_Assets/Materials/RedBuildingGhost")).Get();

	regularMaterial = CreateDefaultSubobject<UMaterial>(TEXT("regularMaterial"));
	regularMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Game_Assets/Materials/regularMaterial")).Get();
}

void UBuildingManagerObject::ghostBuilding(uint8 whatBuilding_, FVector spawnLocation)
{
	UWorld* const World = this->GetWorld();
	if (World)
	{
		if (whatBuilding_ == 1) {
			buildingToBuild = World->SpawnActor<ABuilding_PowerPlant>(ABuilding_PowerPlant::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding_ == 2) {
			buildingToBuild = World->SpawnActor<ABuilding_Refinery>(ABuilding_Refinery::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding_ == 3) {
			buildingToBuild = World->SpawnActor<ABuilding_Barrecks>(ABuilding_Barrecks::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding_ == 4) {
			buildingToBuild = World->SpawnActor<ABuilding_VehicleFactory>(ABuilding_VehicleFactory::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding_ == 5) {
			buildingToBuild = World->SpawnActor<ABuilding_TechCenter>(ABuilding_TechCenter::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		}
		whatBuilding = whatBuilding_ - 1;
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

bool UBuildingManagerObject::constructBuilding(II_Player* player)
{
	if (buildingToBuild->constructAtLocation(player)) {
			currentPower -= buildingPowerConsumption[whatBuilding];
		if (buildingPowerConsumption[whatBuilding] < 0) {
			maxPower -= buildingPowerConsumption[whatBuilding];
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

void UBuildingManagerObject::SubtractCost(int32 whatBuilding_)
{
	resources -= buildingCosts[whatBuilding_ - 1];
}

void UBuildingManagerObject::SubtractResourceAmount(int32 amount)
{
	resources -= amount;
}

void UBuildingManagerObject::AddCost(int32 whatBuilding_)
{
	resources += buildingCosts[whatBuilding_ - 1];
}

void UBuildingManagerObject::AddResourceAmount(int32 amount)
{
	resources += amount;
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
	if (techCenterArray.Num() > 0) {
		return true;
	}
	return false;
}

bool UBuildingManagerObject::IsRefineryBuilt()
{
	if (refineryArray.Num() > 0) {
		return true;
	}
	return false;
}
