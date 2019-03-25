// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyPlayer.h"

#include "GameManager.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"

// Sets default values
AEnemyPlayer::AEnemyPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Setting up the variables from the datatable
	/*
	static ConstructorHelpers::FObjectFinderOptional<UDataTable> tempDataTableA(TEXT("/Game/Game_Assets/DataTables/BuildingVariables.BuildingVariables"));
	SetBuildingDataTable(tempDataTableA.Get());
	static ConstructorHelpers::FObjectFinderOptional<UDataTable> tempDataTableB(TEXT("/Game/Game_Assets/DataTables/UnitVariables.UnitVariables"));
	SetUnitConstructionDataTable(tempDataTableB.Get());
	*/

	playerName = FString("Default Player");
	playerTeam = 0;

}

// Called when the game starts or when spawned
void AEnemyPlayer::BeginPlay()
{
	Super::BeginPlay();

	InitializePlayer(playerName, playerTeam);
	
}

// Called every frame
void AEnemyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

