// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

// ARRAYS
//#include "I_Unit.h"
//#include "BuildingMaster.h"



#include "I_Player.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_Player : public UInterface
{
	GENERATED_BODY()
};

class RTSCAPSTONE_API II_Player
{
	GENERATED_BODY()

public:
	// Methods
	II_Player* GetPlayerReference();
	
	void InitializePlayer(FString playerName);
	FString GetPlayerName();

	void AddUnit(AActor* unitReference);
	TArray<AActor*> GetUnits();

	void AddBuilding(AActor* buildingReference);
	TArray<AActor*> GetBuildings();

	//Adds the amount given
	void ChangeResources(int amount); 
	int GetResourceAmount();

	//Adds the amount given
	void ChangePower(int amount);
	int GetPower();

private:
	// Variables
	TArray<AActor*> UnitList;
	TArray<AActor*> BuildingList;



	FString PlayerName;
	
	int Resources, power;

};
