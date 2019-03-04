// Fill out your copyright notice in the Description page of Project Settings.

#include "I_Entity.h"

// Add default functionality here for any II_Entity functions that are not pure virtual.

void II_Entity::InitializeEntity(II_Player* owner_, FString name_, float maxHealth_)
{
	if (!isInitialized)
	{
		name = name_;
		maxHealth = maxHealth_;
		currentHealth = maxHealth;
		owner = owner_;

		isInitialized = true;
	}
}

int II_Entity::DealDamage(float amount)
{	
	
	// Kills the target
	if (currentHealth - amount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : 0 / %f  (%f%)"), *GetName(), GetMaxHealth(), GetHealthPercentage());

		if (currentHealth - amount < 0)
		{
			float overkillAmount = -(currentHealth - amount);
			UE_LOG(LogTemp, Warning, TEXT("*OVERKILL* (%f)"), overkillAmount);
		}

		DestroyEntity();
		return 1;
	}
	
	// Doesn't kill the target
	else 
	{
		currentHealth -= amount;
		UE_LOG(LogTemp, Warning, TEXT("%s : %f / %f  (%f%)"), *GetName(), GetCurrentHealth(), GetMaxHealth(), GetHealthPercentage());
		return 0;
	}
}

FString II_Entity::GetName() 
{
	return name;
}

float II_Entity::GetCurrentHealth()
{
	return currentHealth;
}

float II_Entity::GetMaxHealth()
{
	return maxHealth;
}

// Returns a 0-1 percentage regarding the entities health ratio.
float II_Entity::GetHealthPercentage()
{
	return currentHealth / maxHealth;
}

void II_Entity::DestroyEntity()
{
	// Do Nothing
}

II_Player* II_Entity::GetEntityOwner()
{
	return owner;
}