// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "I_Player.h"

#include "I_Entity.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_Entity : public UInterface
{
	GENERATED_BODY()
};

class RTSCAPSTONE_API II_Entity
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/// Once we implement ownership, we'll be adding an additional argument to this function.
	void InitializeEntity(II_Player* owner_, FString name, float maxHealth);

	int DealDamage(float amount);

	FString GetName();
	float GetCurrentHealth();
	float GetMaxHealth();
	float GetHealthPercentage();
	
	virtual void DestroyEntity();

	II_Player* GetEntityOwner();


private:
	bool isInitialized = false;
	FString name;
	float currentHealth, maxHealth = 100.0f;
	II_Player* owner;
};