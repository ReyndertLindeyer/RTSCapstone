// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingManagerObject.h"

// Sets default values
UBuildingManagerObject::UBuildingManagerObject()
{
	power = true;

	whatBuilding = 0;

	//Setting all of the meshes for the ghost buildings
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devPowerplant_v1.devPowerplant_v1")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devRefinery_v1.devRefinery_v1")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devBarracks_v1.devBarracks_v1")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devFactory_v1.devFactory_v1")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devTechCentre.devTechCentre")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Power_Plant.Placeholder_Power_Plant")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devGattling.devGattling")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devTurret_v1.devTurret_v1")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Power_Plant.Placeholder_Power_Plant")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devTesla_v1.devTesla_v1")).Get());


	//canBuildIndicator = CreateDefaultSubobject<UMaterialInterface>(TEXT("GreenBuildingGhost"));
	canBuildIndicator = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/GreenBuildingGhost")).Get();

	//cantBuildIndicator = CreateDefaultSubobject<UMaterialInterface>(TEXT("cantBuildIndicator"));
	cantBuildIndicator = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/RedBuildingGhost")).Get();

	//regularMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("regularMaterial"));
	regularMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/regularMaterial")).Get();
}

void UBuildingManagerObject::ghostBuilding(uint8 whatBuilding_, FVector spawnLocation)
{
	UWorld* const World = this->GetWorld();
	if (World)
	{

		buildingToBuild = World->SpawnActor<ABuilding_Ghost>(ABuilding_Ghost::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		whatBuilding = whatBuilding_ - 1;
		buildingToBuild->SetMesh(buildingMeshArray[whatBuilding]);

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

ABuilding_Ghost * UBuildingManagerObject::GetBuildingToBuild()
{
	return buildingToBuild;
}

void UBuildingManagerObject::SpawnConstructionYard(FVector spawnLocation)
{
	constructionYard = GetWorld()->SpawnActor<ABuilding_Construction_Yard>(ABuilding_Construction_Yard::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
	constructionYard->InitializeEntity(thePlayer, "ConstructionYard", 20000);
	masterArray.Add(constructionYard);
	//thePlayer->AddBuilding(Cast<AActor>(constructionYard));

	thePlayer->ChangePower(20);
	maxPower = thePlayer->GetPower();

}

bool UBuildingManagerObject::constructBuilding()
{
	if (!buildingToBuild->GetIsOverlapping() && buildingToBuild->GetIsInRadius()) {

		UWorld* const World = this->GetWorld();
		ABuildingMaster* building;
		if (whatBuilding == 0) {
			building = World->SpawnActor<ABuilding_PowerPlant>(ABuilding_PowerPlant::StaticClass(), buildingToBuild->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding == 1) {
			building = World->SpawnActor<ABuilding_Refinery>(ABuilding_Refinery::StaticClass(), buildingToBuild->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding == 2) {
			building = World->SpawnActor<ABuilding_Barrecks>(ABuilding_Barrecks::StaticClass(), buildingToBuild->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding == 3) {
			building = World->SpawnActor<ABuilding_VehicleFactory>(ABuilding_VehicleFactory::StaticClass(), buildingToBuild->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding == 4) {
			building = World->SpawnActor<ABuilding_TechCenter>(ABuilding_TechCenter::StaticClass(), buildingToBuild->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding == 5) {
			building = World->SpawnActor<ABuilding_Superweapon>(ABuilding_Superweapon::StaticClass(), buildingToBuild->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding == 6) {
			building = World->SpawnActor<ABuilding_Turret_Gattling>(ABuilding_Turret_Gattling::StaticClass(), buildingToBuild->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding == 7) {
			building = World->SpawnActor<ABuilding_Turret_Cannon>(ABuilding_Turret_Cannon::StaticClass(), buildingToBuild->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuilding == 8) {
			building = World->SpawnActor<ABuilding_Turret_Artillery>(ABuilding_Turret_Artillery::StaticClass(), buildingToBuild->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else {
			building = World->SpawnActor<ABuilding_Turret_Tesla>(ABuilding_Turret_Tesla::StaticClass(), buildingToBuild->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		building->InitializeEntity(thePlayer, namesArray[whatBuilding].ToString(), buildingMaxHealth[whatBuilding]);
		building->SetOwningEntity(thePlayer);
		building->constructAtLocation(thePlayer);
		thePlayer->ChangePower(-buildingPowerConsumption[whatBuilding]);
		if (buildingPowerConsumption[whatBuilding] < 0) {
			maxPower -= buildingPowerConsumption[whatBuilding];
		}

		if (building->IsA(ABuilding_Refinery::StaticClass())) {
			refineryArray.Add((ABuilding_Refinery*)building);
			Cast<ABuilding_Refinery>(building)->InitializeRefinery();
		}

		if (building->IsA(ABuilding_TechCenter::StaticClass())) {
			techCenterArray.Add((ABuilding_TechCenter*)building);
		}

		if (building->IsA(ABuilding_Barrecks::StaticClass())) {
			Cast<ABuilding_Barrecks>(building)->InitializeBarracks();
		}

		if (building->IsA(ABuilding_VehicleFactory::StaticClass())) {
			Cast<ABuilding_VehicleFactory>(building)->InitializeWarFactory();
		}

		building->GetBuildingMesh()->SetMaterial(0, regularMaterial);

		masterArray.Add(building);

		//thePlayer->AddBuilding(Cast<AActor>(building));

		buildingToBuild->Destroy();
		buildingToBuild = nullptr;
		building = nullptr;

		if (thePlayer->GetPower() < 0) {
			power = false;
			for (int i = 0; i < masterArray.Num(); i++) {
				if (Cast<ABuilding_Barrecks>(masterArray[i])) {
					Cast<ABuilding_Barrecks>(masterArray[i])->SetHasPower(false);
				}
				else if (Cast<ABuilding_VehicleFactory>(masterArray[i])) {
					Cast<ABuilding_VehicleFactory>(masterArray[i])->SetHasPower(false);
				}
				else if (Cast<ABuilding_Superweapon>(masterArray[i])) {
					Cast<ABuilding_Superweapon>(masterArray[i])->SetHasPower(false);
				}

				if (Cast<ABuilding_Turret_Gattling>(masterArray[i])) {
					Cast<ABuilding_Turret_Gattling>(masterArray[i])->SetHasPower(false);
				}
				else if (Cast<ABuilding_Turret_Cannon>(masterArray[i])) {
					Cast<ABuilding_Turret_Cannon>(masterArray[i])->SetHasPower(false);
				}
				else if (Cast<ABuilding_Turret_Artillery>(masterArray[i])) {
					Cast<ABuilding_Turret_Artillery>(masterArray[i])->SetHasPower(false);
				}
				else if (Cast<ABuilding_Turret_Tesla>(masterArray[i])) {
					Cast<ABuilding_Turret_Tesla>(masterArray[i])->SetHasPower(false);
				}
			}
		}
		else if (thePlayer->GetPower() >= 0 && !power) {
			power = true;
			for (int i = 0; i < masterArray.Num(); i++) {
				if (Cast<ABuilding_Barrecks>(masterArray[i])) {
					Cast<ABuilding_Barrecks>(masterArray[i])->SetHasPower(true);
				}
				else if (Cast<ABuilding_VehicleFactory>(masterArray[i])) {
					Cast<ABuilding_VehicleFactory>(masterArray[i])->SetHasPower(true);
				}
				else if (Cast<ABuilding_Superweapon>(masterArray[i])) {
					Cast<ABuilding_Superweapon>(masterArray[i])->SetHasPower(false);
				}

				if (Cast<ABuilding_Turret_Gattling>(masterArray[i])) {
					Cast<ABuilding_Turret_Gattling>(masterArray[i])->SetHasPower(true);
				}
				else if (Cast<ABuilding_Turret_Cannon>(masterArray[i])) {
					Cast<ABuilding_Turret_Cannon>(masterArray[i])->SetHasPower(true);
				}
				else if (Cast<ABuilding_Turret_Artillery>(masterArray[i])) {
					Cast<ABuilding_Turret_Artillery>(masterArray[i])->SetHasPower(true);
				}
				else if (Cast<ABuilding_Turret_Tesla>(masterArray[i])) {
					Cast<ABuilding_Turret_Tesla>(masterArray[i])->SetHasPower(true);
				}
			}
		}

		DisableAllDecals();
		return true;
	}
	return false;
}

void UBuildingManagerObject::DeleteBuilding()
{
	buildingToBuild->Destroy();
	buildingToBuild = nullptr;
}

TArray<int32> UBuildingManagerObject::GetBuildingCost() 
{
	return buildingCosts;
}

TArray<int32> UBuildingManagerObject::GetConstructionTime()
{
	return buildingConstructionTimes;
}

int32 UBuildingManagerObject::GetCertainConstructionTime(int32 whatBuilding)
{
	return buildingConstructionTimes[whatBuilding-1];
}

int32 UBuildingManagerObject::GetMaxPower()
{
	return maxPower;
}

void UBuildingManagerObject::SubtractCost(int32 whatBuilding_)
{
	thePlayer->ChangeResources(-buildingCosts[whatBuilding_ - 1]);
}

void UBuildingManagerObject::AddCost(int32 whatBuilding_)
{
	thePlayer->ChangeResources(buildingCosts[whatBuilding_ - 1]);
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
	//return false; Disabled for testing
	return true;
}

void UBuildingManagerObject::SetPlayer(II_Player* inPlayer)
{
	thePlayer = inPlayer;


	static const FString ContextString(TEXT("Building Variable Context"));

	namesArray = thePlayer->GetBuildingDataTable()->GetRowNames();

	//Power Plant Variables
	FBuildingVariables* buildingVariables = thePlayer->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("PowerPlant")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);
	buildingMaxHealth.Add(buildingVariables->MaxHealth);

	//Refinery Variables
	buildingVariables = thePlayer->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("Refinery")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);
	buildingMaxHealth.Add(buildingVariables->MaxHealth);

	//Barracks Variables
	buildingVariables = thePlayer->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("Barracks")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);
	buildingMaxHealth.Add(buildingVariables->MaxHealth);

	//War Factory Variables
	buildingVariables = thePlayer->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("WarFactory")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);
	buildingMaxHealth.Add(buildingVariables->MaxHealth);

	//Tech Center Variables
	buildingVariables = thePlayer->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("TechCenter")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);
	buildingMaxHealth.Add(buildingVariables->MaxHealth);

	//Orbital Cannon Variables
	buildingVariables = thePlayer->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("OrbitalCannon")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);
	buildingMaxHealth.Add(buildingVariables->MaxHealth);

	//Gun Turret Variables
	buildingVariables = thePlayer->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("GunTurret")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);
	buildingMaxHealth.Add(buildingVariables->MaxHealth);

	//Cannon Turret Variables
	buildingVariables = thePlayer->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("CannonTurret")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);
	buildingMaxHealth.Add(buildingVariables->MaxHealth);

	//Artillery Turret Variables
	buildingVariables = thePlayer->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("ArtilleryTurret")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);
	buildingMaxHealth.Add(buildingVariables->MaxHealth);

	//Tesla Turret Variables
	buildingVariables = thePlayer->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("TeslaTurret")), ContextString, false);
	buildingCosts.Add(buildingVariables->Cost);
	buildingConstructionTimes.Add(buildingVariables->BuildTime);
	buildingPowerConsumption.Add(buildingVariables->PowerConsumption);
	buildingMaxHealth.Add(buildingVariables->MaxHealth);
}
