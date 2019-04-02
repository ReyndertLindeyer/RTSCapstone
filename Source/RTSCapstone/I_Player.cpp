// Fill out your copyright notice in the Description page of Project Settings.

#include "I_Player.h"
#include "Building_Construction_Yard.h"




II_Player* II_Player::GetPlayerReference()
{
	return this;
}

void II_Player::AddUnit(AActor* unitReference)
{
	// Not sure why, but TArray isn't detecting any functions.
	/// I might be missing a header file that needs to be included,
	/// but I thought CoreMinimal would allow TArray functions to be called
	UnitList.Add(unitReference); 
	totalUnitsConstructed++;
}

TArray<AActor*> II_Player::GetUnits()
{
	return UnitList;
}

void II_Player::RemoveUnitAtIndex(int32 index)
{
	UnitList.RemoveAt(index);
	totalUnitsLost++;
}

void II_Player::AddBuilding(AActor* buildingReference)
{
	// Same problem occurs as the AddUnit function
	BuildingList.Add(buildingReference);
	totalBuildingsConstructed++;
}

TArray<AActor*> II_Player::GetBuildings()
{
	return BuildingList;
}

void II_Player::RemoveBuildingAtIndex(int32 index)
{
	BuildingList.RemoveAt(index);
	totalStructuresLost++;
}

void II_Player::InitializePlayer(FString playerName, int teamValue)
{
	PlayerName = playerName;
	//Setting up the variables from the datatable
	static ConstructorHelpers::FObjectFinderOptional<UDataTable> tempDataTableA(TEXT("/Game/Game_Assets/DataTables/BuildingVariables.BuildingVariables"));
	buildingDataTable = tempDataTableA.Get();
	static ConstructorHelpers::FObjectFinderOptional<UDataTable> tempDataTableB(TEXT("/Game/Game_Assets/DataTables/UnitVariables.UnitVariables"));
	unitConstructionDataTable = tempDataTableB.Get();

	hasDestroyedObjective = false;
	totalResourcesCollected = 0;
	totalBuildingsConstructed = 0;
	totalUnitsConstructed = 0;
	totalUnitsLost = 0;
	totalStructuresLost = 0;
}

FString II_Player::GetPlayerName()
{
	return PlayerName;
}

void II_Player::InitResources(int amount)
{
	Resources += amount;
}

void II_Player::ChangeResources(int amount)
{
	Resources += amount;
	totalResourcesCollected += amount;
}

int II_Player::GetResourceAmount()
{
	return Resources;
}

void II_Player::ChangePower(int amount)
{
	power += amount;
}

int II_Player::GetPower()
{
	return power;
}

UDataTable * II_Player::GetBuildingDataTable()
{
	return buildingDataTable;
}

UDataTable * II_Player::GetUnitConstructionDataTable()
{
	return unitConstructionDataTable;
}

void II_Player::SetBuildingDataTable(UDataTable * inTable)
{
	buildingDataTable = inTable;
}

void II_Player::SetUnitConstructionDataTable(UDataTable * inTable)
{
	unitConstructionDataTable = inTable;
}



TArray<ACharacter*> II_Player::GetSelectedCharacters()
{
	return SelectedCharacters;
}


void II_Player::SetSelectedCharacters(TArray<ACharacter*> inArray){
	SelectedCharacters = inArray;
}

void II_Player::RemoveSelectedCharacterAtIndex(int32 index)
{
	SelectedCharacters.RemoveAt(index);
}

void II_Player::SetSelectedBuilding(AActor* inBuilding) {
	selectedBuilding = inBuilding;
}

AActor* II_Player::GetSelectedBuilding() {
	return selectedBuilding;
}

int32 II_Player::GetTotalResourcesCollected()
{
	return totalResourcesCollected;
}

int32 II_Player::GetTotalBuildingsConstructed()
{
	return totalBuildingsConstructed;
}

int32 II_Player::GetTotalUnitsConstructed()
{
	return totalUnitsConstructed;
}

int32 II_Player::GetTotalUnitsLost()
{
	return totalUnitsLost;
}

int32 II_Player::GetTotalStructuresLost()
{
	return totalStructuresLost;
}

void II_Player::SetHasDestroyedObjective(bool inBool)
{
	hasDestroyedObjective = inBool;
}

void II_Player::SetStatistics(TArray<int32> inArray)
{
	statistics = inArray;
}

bool II_Player::GetHasDestroyedObjective()
{
	return hasDestroyedObjective;
}

TArray<int32> II_Player::GetStatistics()
{
	return statistics;
}