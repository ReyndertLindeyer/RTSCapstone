// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingManagerObject.h"

// Sets default values
UBuildingManagerObject::UBuildingManagerObject()
{
	power = true;

	whatBuilding = 0;

	//Setting all of the meshes for the ghost buildings
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/PowerPlant_Model/PowerPlant.PowerPlant")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Refinery_Model/Resource_Refinery.Resource_Refinery")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Barracks_Model/Barracks.Barracks")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/VehicleFactory_Model/Vehicle_factory.Vehicle_factory")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devTechCentre.devTechCentre")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/OrbitalCannon_Model/Orbital_Cannon.Orbital_Cannon")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Gattling_Turret/Gattling_Turret_V1_UNREAL.Gattling_Turret_V1_UNREAL")).Get()); 
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/AT_Cannon/AT_Cannon_V1_UNREAL.AT_Cannon_V1_UNREAL")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Artillery_Platform/ArtilleryCannon.ArtilleryCannon")).Get());
	buildingMeshArray.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Tesla_Tower/TeslaTower_v1.TeslaTower_v1")).Get());
	
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
		if (whatBuilding > 5)
			buildingToBuild->SetMesh(buildingMeshArray[whatBuilding], 4);
		else if (whatBuilding == 3)
			buildingToBuild->SetMesh(buildingMeshArray[whatBuilding], 10);
		else if(whatBuilding == 4)
			buildingToBuild->SetMesh(buildingMeshArray[whatBuilding], 5);
		else 
			buildingToBuild->SetMesh(buildingMeshArray[whatBuilding], 2);

		if (whatBuilding_ == 1) {
			buildingToBuild->GetBuildingMesh()->SetMaterial(1, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(2, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(3, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(4, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(5, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(6, canBuildIndicator);
		}

		if (whatBuilding_ == 2) {
			buildingToBuild->GetBuildingMesh()->SetMaterial(1, canBuildIndicator);
		}

		if (whatBuilding_ == 3) {
			buildingToBuild->GetBuildingMesh()->SetMaterial(1, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(2, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(3, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(4, canBuildIndicator);
		}

		if (whatBuilding_ == 4) {
			buildingToBuild->GetBuildingMesh()->SetMaterial(1, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(2, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(3, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(4, canBuildIndicator);
		}

		EnableAllDecals();

		buildingToBuild->thePlayer = thePlayer;
	}
}

void UBuildingManagerObject::MultiGhostBuilding(uint8 whatBuilding_, FVector spawnLocation)
{
	UWorld* const World = this->GetWorld();
	if (World)
	{
		ABuilding_Ghost* temp = World->SpawnActor<ABuilding_Ghost>(ABuilding_Ghost::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		if ((whatBuilding_ - 1) > 5)
			temp->SetMesh(buildingMeshArray[(whatBuilding_ - 1)], 4);
		else if ((whatBuilding_ - 1) == 3 || (whatBuilding_ - 1) == 4)
			temp->SetMesh(buildingMeshArray[(whatBuilding_ - 1)], 5);
		else
			temp->SetMesh(buildingMeshArray[(whatBuilding_ - 1)], 2);

		if (whatBuilding_ == 1) {
			temp->GetBuildingMesh()->SetMaterial(1, canBuildIndicator);
			temp->GetBuildingMesh()->SetMaterial(2, canBuildIndicator);
			temp->GetBuildingMesh()->SetMaterial(3, canBuildIndicator);
			temp->GetBuildingMesh()->SetMaterial(4, canBuildIndicator);
			temp->GetBuildingMesh()->SetMaterial(5, canBuildIndicator);
			temp->GetBuildingMesh()->SetMaterial(6, canBuildIndicator);
		}

		if (whatBuilding_ == 2) {
			temp->GetBuildingMesh()->SetMaterial(1, canBuildIndicator);
		}

		if (whatBuilding_ == 3) {
			temp->GetBuildingMesh()->SetMaterial(1, canBuildIndicator);
			temp->GetBuildingMesh()->SetMaterial(2, canBuildIndicator);
			temp->GetBuildingMesh()->SetMaterial(3, canBuildIndicator);
			temp->GetBuildingMesh()->SetMaterial(4, canBuildIndicator);
		}

		temp->GetBuildingMesh()->SetMaterial(0, canBuildIndicator);

		whatBuildingArray.Add(whatBuilding_ - 1);
		ghostBuildingArray.Add(temp);
	}
}

void UBuildingManagerObject::MoveBuilding(FVector location)
{
	buildingToBuild->SetActorLocation(location);

	///Temp Code
	/*
	bool isInRadius = true;

	TArray<AActor*> tempArray = thePlayer->GetBuildings();

	for (int i = 0; i < tempArray.Num(); i++) {
			//float distance = (outActors[i]->GetRootComponent()->GetComponentLocation() - RootComponent->GetComponentLocation()).Size();
			//UE_LOG(LogTemp, Warning, TEXT("ghost is x: %d y: %d z: %d"), RootComponent->GetComponentLocation().X, RootComponent->GetComponentLocation().Y, RootComponent->GetComponentLocation().Z);
			//UE_LOG(LogTemp, Warning, TEXT("other is x: %d y: %d z: %d"), outActors[i]->GetRootComponent()->GetComponentLocation().X, outActors[i]->GetRootComponent()->GetComponentLocation().Y, outActors[i]->GetRootComponent()->GetComponentLocation().Z);
			//UE_LOG(LogTemp, Warning, TEXT("%d"), FVector::Dist(tempArray[i]->GetActorLocation(), location));

			//if (FVector::Distance(this->GetActorLocation(), outActors[i]->GetActorLocation()) < Cast<ABuildingMaster>(outActors[i])->GetConstructionRadius() * 2) {
			if (FVector::Dist(tempArray[i]->GetActorLocation(), location) < Cast<ABuildingMaster>(tempArray[i])->GetConstructionRadius() * 2) {
				isInRadius = true;
				break;
			}
			else {
				isInRadius = false;
			}
	}

	buildingToBuild->SetIsInRadius(isInRadius);
	*/

	if (buildingToBuild->GetIsInRadius() && !buildingToBuild->GetIsOverlapping() && buildingToBuild->GetBuildingMesh()->GetMaterial(0) != canBuildIndicator) {
		buildingToBuild->GetBuildingMesh()->SetMaterial(0, canBuildIndicator);

		if (whatBuilding == 0) {
			buildingToBuild->GetBuildingMesh()->SetMaterial(1, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(2, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(3, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(4, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(5, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(6, canBuildIndicator);
		}

		if (whatBuilding == 1) {
			buildingToBuild->GetBuildingMesh()->SetMaterial(1, canBuildIndicator);
		}

		if (whatBuilding == 2) {
			buildingToBuild->GetBuildingMesh()->SetMaterial(1, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(2, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(3, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(4, canBuildIndicator);
		}

		if (whatBuilding == 3) {
			buildingToBuild->GetBuildingMesh()->SetMaterial(1, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(2, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(3, canBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(4, canBuildIndicator);
		}
	}
	else if (!buildingToBuild->GetIsInRadius() || buildingToBuild->GetIsOverlapping() && buildingToBuild->GetBuildingMesh()->GetMaterial(0) != cantBuildIndicator) {
		buildingToBuild->GetBuildingMesh()->SetMaterial(0, cantBuildIndicator);

		if (whatBuilding == 0) {
			buildingToBuild->GetBuildingMesh()->SetMaterial(1, cantBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(2, cantBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(3, cantBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(4, cantBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(5, cantBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(6, cantBuildIndicator);
		}

		if (whatBuilding == 1) {
			buildingToBuild->GetBuildingMesh()->SetMaterial(1, cantBuildIndicator);
		}

		if (whatBuilding == 2) {
			buildingToBuild->GetBuildingMesh()->SetMaterial(1, cantBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(2, cantBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(3, cantBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(4, cantBuildIndicator);
		}

		if (whatBuilding == 3) {
			buildingToBuild->GetBuildingMesh()->SetMaterial(1, cantBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(2, cantBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(3, cantBuildIndicator);
			buildingToBuild->GetBuildingMesh()->SetMaterial(4, cantBuildIndicator);
		}
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
		thePlayer->ChangePower(-buildingPowerConsumption[whatBuilding]);
		if (buildingPowerConsumption[whatBuilding] < 0) {
			maxPower -= buildingPowerConsumption[whatBuilding];
		}

		if (building->IsA(ABuilding_Refinery::StaticClass())) {
			refineryArray.Add((ABuilding_Refinery*)building);
			//Cast<ABuilding_Refinery>(building)->InitializeRefinery();
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
		building->constructAtLocation(thePlayer);

		building->GetBuildingMesh()->SetMaterial(0, regularMaterial);

		masterArray.Add(building);

		//thePlayer->AddBuilding(Cast<AActor>(building));

		buildingToBuild->Destroy();
		buildingToBuild = nullptr;
		building = nullptr;

		if (thePlayer->GetPower() < 0) {
			power = false;
			for (int i = 0; i < thePlayer->GetBuildings().Num(); i++) {
				if (Cast<ABuilding_Barrecks>(thePlayer->GetBuildings()[i])) {
					Cast<ABuilding_Barrecks>(thePlayer->GetBuildings()[i])->SetHasPower(false);
				}
				else if (Cast<ABuilding_VehicleFactory>(thePlayer->GetBuildings()[i])) {
					Cast<ABuilding_VehicleFactory>(thePlayer->GetBuildings()[i])->SetHasPower(false);
				}
				else if (Cast<ABuilding_Superweapon>(thePlayer->GetBuildings()[i])) {
					Cast<ABuilding_Superweapon>(thePlayer->GetBuildings()[i])->SetHasPower(false);
				}

				if (Cast<ABuilding_Turret_Gattling>(thePlayer->GetBuildings()[i])) {
					Cast<ABuilding_Turret_Gattling>(thePlayer->GetBuildings()[i])->SetHasPower(false);
				}
				else if (Cast<ABuilding_Turret_Cannon>(thePlayer->GetBuildings()[i])) {
					Cast<ABuilding_Turret_Cannon>(thePlayer->GetBuildings()[i])->SetHasPower(false);
				}
				else if (Cast<ABuilding_Turret_Artillery>(thePlayer->GetBuildings()[i])) {
					Cast<ABuilding_Turret_Artillery>(thePlayer->GetBuildings()[i])->SetHasPower(false);
				}
				else if (Cast<ABuilding_Turret_Tesla>(thePlayer->GetBuildings()[i])) {
					Cast<ABuilding_Turret_Tesla>(thePlayer->GetBuildings()[i])->SetHasPower(false);
				}
			}
		}
		else if (thePlayer->GetPower() >= 0 && !power) {
			power = true;
			for (int i = 0; i < thePlayer->GetBuildings().Num(); i++) {
				if (Cast<ABuilding_Barrecks>(thePlayer->GetBuildings()[i])) {
					Cast<ABuilding_Barrecks>(thePlayer->GetBuildings()[i])->SetHasPower(true);
				}
				else if (Cast<ABuilding_VehicleFactory>(thePlayer->GetBuildings()[i])) {
					Cast<ABuilding_VehicleFactory>(thePlayer->GetBuildings()[i])->SetHasPower(true);
				}
				else if (Cast<ABuilding_Superweapon>(thePlayer->GetBuildings()[i])) {
					Cast<ABuilding_Superweapon>(thePlayer->GetBuildings()[i])->SetHasPower(false);
				}

				if (Cast<ABuilding_Turret_Gattling>(thePlayer->GetBuildings()[i])) {
					Cast<ABuilding_Turret_Gattling>(thePlayer->GetBuildings()[i])->SetHasPower(true);
				}
				else if (Cast<ABuilding_Turret_Cannon>(thePlayer->GetBuildings()[i])) {
					Cast<ABuilding_Turret_Cannon>(thePlayer->GetBuildings()[i])->SetHasPower(true);
				}
				else if (Cast<ABuilding_Turret_Artillery>(thePlayer->GetBuildings()[i])) {
					Cast<ABuilding_Turret_Artillery>(thePlayer->GetBuildings()[i])->SetHasPower(true);
				}
				else if (Cast<ABuilding_Turret_Tesla>(thePlayer->GetBuildings()[i])) {
					Cast<ABuilding_Turret_Tesla>(thePlayer->GetBuildings()[i])->SetHasPower(true);
				}
			}
		}

		DisableAllDecals();
		return true;
	}
	return false;
}

void UBuildingManagerObject::MultiConstructBuilding()
{
	UWorld* const World = this->GetWorld();
	ABuildingMaster* building;
	for (int i = 0; i < whatBuildingArray.Num(); i++) {
		building = nullptr;
		if (whatBuildingArray[i] == 0) {
			building = World->SpawnActor<ABuilding_PowerPlant>(ABuilding_PowerPlant::StaticClass(), ghostBuildingArray[i]->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuildingArray[i] == 1) {
			building = World->SpawnActor<ABuilding_Refinery>(ABuilding_Refinery::StaticClass(), ghostBuildingArray[i]->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuildingArray[i] == 2) {
			building = World->SpawnActor<ABuilding_Barrecks>(ABuilding_Barrecks::StaticClass(), ghostBuildingArray[i]->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuildingArray[i] == 3) {
			building = World->SpawnActor<ABuilding_VehicleFactory>(ABuilding_VehicleFactory::StaticClass(), ghostBuildingArray[i]->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuildingArray[i] == 4) {
			building = World->SpawnActor<ABuilding_TechCenter>(ABuilding_TechCenter::StaticClass(), ghostBuildingArray[i]->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuildingArray[i] == 5) {
			building = World->SpawnActor<ABuilding_Superweapon>(ABuilding_Superweapon::StaticClass(), ghostBuildingArray[i]->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuildingArray[i] == 6) {
			building = World->SpawnActor<ABuilding_Turret_Gattling>(ABuilding_Turret_Gattling::StaticClass(), ghostBuildingArray[i]->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuildingArray[i] == 7) {
			building = World->SpawnActor<ABuilding_Turret_Cannon>(ABuilding_Turret_Cannon::StaticClass(), ghostBuildingArray[i]->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (whatBuildingArray[i] == 8) {
			building = World->SpawnActor<ABuilding_Turret_Artillery>(ABuilding_Turret_Artillery::StaticClass(), ghostBuildingArray[i]->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		else {
			building = World->SpawnActor<ABuilding_Turret_Tesla>(ABuilding_Turret_Tesla::StaticClass(), ghostBuildingArray[i]->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
		}
		building->InitializeEntity(thePlayer, namesArray[whatBuildingArray[i]].ToString(), buildingMaxHealth[whatBuildingArray[i]]);
		building->SetOwningEntity(thePlayer);

		building->constructAtLocation(thePlayer);

		building->GetBuildingMesh()->SetMaterial(0, regularMaterial);

		masterArray.Add(building);

		//thePlayer->AddBuilding(Cast<AActor>(building));

		ghostBuildingArray[i]->Destroy();
		ghostBuildingArray[i] = nullptr;
		building = nullptr;
	}
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
	if (thePlayer->GetBuildings().Num() > 0) {
		for (int32 i = 0; i < thePlayer->GetBuildings().Num(); i++) {
			Cast<ABuildingMaster>(thePlayer->GetBuildings()[i])->EnableBuildDecal();
		}
	}
}

void UBuildingManagerObject::DisableAllDecals()
{
    if (thePlayer->GetBuildings().Num() > 0) {
        for (int32 i = 0; i < thePlayer->GetBuildings().Num(); i++) {
            Cast<ABuildingMaster>(thePlayer->GetBuildings()[i])->DisableBuildDecal();
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
	return false;
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


bool UBuildingManagerObject::IsConstructionYardDestroyed() {
	if (constructionYard != nullptr) {
		return false;
	}
	else {
		return true;
	}
}


void UBuildingManagerObject::ConstructOutpost(FVector spawnLocation)
{
	ABuildingMaster* tempBuilding = GetWorld()->SpawnActor<ABuilding_Outpost>(ABuilding_Outpost::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
	tempBuilding->SetActorScale3D(FVector(5));

	tempBuilding->InitializeEntity(thePlayer, "Outpost", 1000);

	tempBuilding->SetOwningEntity(thePlayer);

	tempBuilding->constructAtLocation(thePlayer);

	tempBuilding->GetBuildingMesh()->SetMaterial(0, regularMaterial);

	masterArray.Add(tempBuilding);
}
