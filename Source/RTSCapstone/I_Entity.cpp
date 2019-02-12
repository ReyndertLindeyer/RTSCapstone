// Fill out your copyright notice in the Description page of Project Settings.

#include "I_Entity.h"

// Add default functionality here for any II_Entity functions that are not pure virtual.

void II_Entity::InitializeEntity(FString name_, float maxHealth_)
{
	if (!isInitialized)
	{
		name = name_;
		maxHealth = maxHealth_;
		currentHealth = maxHealth;

		isInitialized = true;
	}
}

void II_Entity::DealDamage(float amount)
{
	currentHealth -= amount;
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