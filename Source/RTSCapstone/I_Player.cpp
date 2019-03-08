// Fill out your copyright notice in the Description page of Project Settings.

#include "I_Player.h"




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
}

TArray<AActor*> II_Player::GetUnits()
{
	return UnitList;
}

void II_Player::AddBuilding(AActor* buildingReference)
{
	// Same problem occurs as the AddUnit function
	BuildingList.Add(buildingReference);
}

TArray<AActor*> II_Player::GetBuildings()
{
	return BuildingList;
}

void II_Player::InitializePlayer(FString playerName)
{
	PlayerName = playerName;
}

FString II_Player::GetPlayerName()
{
	return PlayerName;
}

void II_Player::AddResources(int amount)
{
	Resources += amount;
}

int II_Player::GetResources()
{
	return Resources;
}





