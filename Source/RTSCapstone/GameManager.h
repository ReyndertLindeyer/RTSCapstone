// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"

#include "BuildingMaster.h"

// Interface
#include "I_Player.h"

#include "GameManager.generated.h"

UCLASS()
class RTSCAPSTONE_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

	UPROPERTY(EditAnywhere)
		TArray<AActor*> playerList;

	UPROPERTY(EditAnywhere)
		bool assignToPlayer;

	UPROPERTY(EditAnywhere)
		AActor* playerToAssign;

	UPROPERTY(EditAnywhere)
		bool spawnCYForPlayer;

	UPROPERTY(EditAnywhere)
		ABuildingMaster* objective;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//class UDataTable* buildingDataTable;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//class UDataTable* unitConstructionDataTable;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool AddPlayer(AActor* player);

};
