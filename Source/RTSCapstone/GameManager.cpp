// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"
#include "MyRTSPlayerController.h"

// Sets default values
AGameManager::AGameManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	playerList = TArray<AActor*>();

	/*
	//Setting up the variables from the datatable
	static ConstructorHelpers::FObjectFinderOptional<UDataTable> tempDataTableA(TEXT("/Game/Game_Assets/DataTables/BuildingVariables.BuildingVariables"));
	buildingDataTable = tempDataTableA.Get();
	static ConstructorHelpers::FObjectFinderOptional<UDataTable> tempDataTableB(TEXT("/Game/Game_Assets/DataTables/UnitVariables.UnitVariables"));
	unitConstructionDataTable = tempDataTableB.Get();
	*/

	assignToPlayer = false;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	playerList.SetNum(3, false);
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If the player is not intiallized
	if (playerList[0] == nullptr)
	{
		// Loop through all objects and find one that has a player controller attached to it
		for (TObjectIterator<AMyRTSPlayerController> Itr; Itr; ++Itr)
		{
			// Set the player to player 0 if one is found
			playerList[0] = Itr->GetPlayerActor();
		}

		if (playerList[0] != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Assigned"));

			if (playerToAssign != nullptr && assignToPlayer)
			{	
				II_Player* ally = Cast<II_Player>(playerToAssign);
				II_Player* player = Cast<II_Player>(playerList[0]);

				for (int i = 0; i < ally->GetBuildings().Num(); i++)
				{
					Cast<II_Entity>(ally->GetBuildings()[i])->AssignPlayer(player);
				}
				ally->GetBuildings().Empty();

				for (int j = 0; j < ally->GetUnits().Num(); j++)
				{
					Cast<II_Entity>(ally->GetUnits()[j])->AssignPlayer(player);
				}
				ally->GetUnits().Empty();

			}
		}
	}

	// Win Condition
	if (objective != nullptr)
	{
		// Do something?
	}

	else if (objective == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("WIN CONDITION CLEARED"));
	}


	// Uncomment for everyone at once
	/*for (int i = 0; i < playerList.Num(); i++)
	{
		if (playerList[i] != nullptr)
		{	
			UE_LOG(LogTemp, Warning, TEXT("%i UNITS: %i"), i, Cast<II_Player>(playerList[i])->GetUnits().Num());
		}
	}*/

	// Uncomment for just the player
	/*if (playerList[0] != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER UNITS: %i"), Cast<II_Player>(playerList[0])->GetUnits().Num());
		UE_LOG(LogTemp, Warning, TEXT("PLAYER STRUCTURES: %i"), Cast<II_Player>(playerList[0])->GetBuildings().Num());
	}*/
	
	// Uncomment for just the enemy
	//if (playerList[1] != nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("ENEMY UNITS: %i"), Cast<II_Player>(playerList[1])->GetUnits().Num());
	//	//UE_LOG(LogTemp, Warning, TEXT("ENEMY STRUCTURES: %i"), Cast<II_Player>(playerList[1])->GetBuildings().Num());
	//}
	
}

bool AGameManager::AddPlayer(AActor* player)
{
	/// For now, we're only going to have two players.  This will need to be changed if we have 3+
	playerList[0] = player;
	UE_LOG(LogTemp, Warning, TEXT("ADDING PLAYER"));

	//playerList.Add(player);

	if (playerList.Contains(player))
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER SUCCESSFULLY ADDED"));
		return true;
	}

	return false;
}

