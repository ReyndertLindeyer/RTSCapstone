// Fill out your copyright notice in the Description page of Project Settings.

#include "I_Entity.h"
#include "BuildingMaster.h"
#include "I_Unit.h"

// Add default functionality here for any II_Entity functions that are not pure virtual.

void II_Entity::InitializeEntity(II_Player* owner_, FString name_, float maxHealth_)
{
	//if (!isInitialized)
	{
		name = name_;
		maxHealth = maxHealth_;
		currentHealth = maxHealth;
		owner = owner_;

		isInitialized = true;
		
		if (owner != nullptr)
		{
			if (Cast<II_Unit>(this))
			{
				UE_LOG(LogTemp, Warning, TEXT("Unit Placed in UNIT ARRAY"));
				owner->AddUnit(Cast<AActor>(this));
			}
				

			else if (Cast<ABuildingMaster>(this))
			{
				UE_LOG(LogTemp, Warning, TEXT("Building Placed in BUILDING ARRAY"));
				owner->AddBuilding(Cast<AActor>(this));
			}

			else
				UE_LOG(LogTemp, Warning, TEXT("WTF?"));
		}	
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WTF 2?"));
		}
	}
}

void II_Entity::AssignPlayer(II_Player* newOwner_)
{
	// Transfer Unit to new Owner
	if (Cast<ABuildingMaster>(this))
		newOwner_->AddBuilding(Cast<AActor>(this));

	// Transfer Structure to new Owner
	if (Cast<II_Unit>(this))
		newOwner_->AddUnit(Cast<AActor>(this));

	// Change Ownership
	owner = newOwner_;
	
}

int II_Entity::DealDamage(float amount)
{	
	//if (isDestructable)
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
	//else {

		//UE_LOG(LogTemp, Warning, TEXT("I'm imortal"));
	//}

	//return -1;
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

void II_Entity::SetCurrentHealth(float inNum)
{
	currentHealth = inNum;
}

void II_Entity::SetMaxHealth(float inNum)
{
	maxHealth = inNum;
}

void II_Entity::SetName(FString name_)
{
	name = name_;
}

void II_Entity::SetOwningEntity(II_Player * owner_)
{
	owner = owner_;
}

void II_Entity::SetHitRadius(uint32 inNum)
{
	hitRadius = inNum;
}

uint32 II_Entity::GetHitRadius()
{
	if (hitRadius) {
		return hitRadius;
	}
	return 1;
}

void II_Entity::SetIsBeingHealed(bool inBool)
{
	isBeingHealed = inBool;
}

bool II_Entity::GetIsBeingHealed()
{
	return isBeingHealed;
}

void II_Entity::DestroyEntity()
{
	// Do Nothing
}

II_Player* II_Entity::GetEntityOwner()
{
	return owner;
}

AActor* II_Entity::GetActor()
{
	return Cast<AActor>(this);
}
