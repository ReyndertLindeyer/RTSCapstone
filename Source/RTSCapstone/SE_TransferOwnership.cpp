// Fill out your copyright notice in the Description page of Project Settings.

#include "SE_TransferOwnership.h"

// Sets default values
ASE_TransferOwnership::ASE_TransferOwnership()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASE_TransferOwnership::BeginPlay()
{
	Super::BeginPlay();

	if (SetTriggerActors.Num() > 0)
		triggerActors = SetTriggerActors;
	
}

// Called every frame
void ASE_TransferOwnership::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < triggerActors.Num(); i++)
	{
		if (!triggerActors[i]->IsValidLowLevel())
		{
			triggerActors.RemoveAt(i);
			break;
		}
	}

}

void ASE_TransferOwnership::TriggerEvent(II_Player* params)
{
	if (params != nullptr)
	{
		II_Player* player = Cast<II_Player>(params);

		for (int i = 0; i < inheritedStructures.Num(); i++)
		{
			Cast<II_Entity>(inheritedStructures[i])->AssignPlayer(player);
		}
		inheritedStructures.Empty();

		for (int j = 0; j < inheritedUnits.Num(); j++)
		{
			Cast<II_Entity>(inheritedUnits[j])->AssignPlayer(player);
		}
		inheritedUnits.Empty();
	}
}

