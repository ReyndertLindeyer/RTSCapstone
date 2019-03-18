// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Interface
#include "I_Player.h"

#include "EnemyPlayer.generated.h"

UCLASS()
class RTSCAPSTONE_API AEnemyPlayer : public AActor, public II_Player
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
