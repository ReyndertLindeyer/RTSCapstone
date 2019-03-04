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
	void AddBuilding(AActor* buildingReference);

private:
	// Variables
	TArray<AActor*> UnitList();
	TArray<AActor*> BuildingList();

	FString PlayerName;
	
	int Resources;

};