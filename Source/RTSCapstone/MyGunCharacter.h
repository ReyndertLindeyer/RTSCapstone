// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MyGunCharacter.generated.h"

UCLASS()
class RTSCAPSTONE_API AMyGunCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyGunCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Mesh and root
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* root;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* charMesh;

	int maxHealth, currentHealth;

	UPROPERTY(EditAnywhere)
		bool selected;
	UPROPERTY(EditAnywhere)
		UBehaviorTree *BotBehavior;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
