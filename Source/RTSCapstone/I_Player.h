// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

// ARRAYS
//#include "I_Unit.h"
//#include "BuildingMaster.h"
#include "UObject/ConstructorHelpers.h"

#include "Engine/DataTable.h"

#include "I_Player.generated.h"




USTRUCT(BlueprintType)
struct FUnitVariables : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 BuildTime;
};


USTRUCT(BlueprintType)
struct FBuildingVariables : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 BuildTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 PowerConsumption;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 MaxHealth;
};

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
	
	void InitializePlayer(FString playerName, int teamValue);
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

	UDataTable* GetBuildingDataTable();

	UDataTable* GetUnitConstructionDataTable();
	void SetBuildingDataTable(UDataTable* inTable);
	void SetUnitConstructionDataTable(UDataTable* inTable);

	int32 teamValue;

	TArray<ACharacter*> GetSelectedCharacters();
	void SetSelectedCharacters(TArray<ACharacter*> inArray);

private:
	// Variables
	TArray<AActor*> UnitList;
	TArray<AActor*> BuildingList;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDataTable* buildingDataTable;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDataTable* unitConstructionDataTable;

			TArray<ACharacter*> SelectedCharacters = TArray<ACharacter*>();

	FString PlayerName;
	
	int Resources, power;

};
