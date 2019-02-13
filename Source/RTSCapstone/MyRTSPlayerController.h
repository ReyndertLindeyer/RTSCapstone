// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MasterUnit.h"
#include "RifleInfantry.h"
#include "BuildingMaster.h"
#include "Building_PowerPlant.h"
#include "Building_Barrecks.h"
#include "Building_Refinery.h"
#include "BuildingManagerObject.h"
#include "MyRTSHUD.h"
#include "ProFow.h"

// Interfaces
#include "I_Entity.h"

#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "MyRTSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API AMyRTSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyRTSPlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaTime) override;

	//Pointer to the HUD
	AMyRTSHUD* HUDPtr;
	
	UPROPERTY()
		bool unlockCamera;

	UFUNCTION(BlueprintPure, Category = "UI")
		int32 GetResources();

	UFUNCTION(BlueprintPure, Category = "UI")
		int GetBuildingCost(int32 whatBuilding);

	UFUNCTION(BlueprintPure, Category = "UI")
		int GetBuildingConstructionTime(int32 whatBuilding);

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool ConstructBuilding(int32 whatBuilding);

	UFUNCTION(BlueprintCallable, Category = "UI")
		void BuildPowerPlant();

	UFUNCTION(BlueprintCallable, Category = "UI")
		void BuildRefinery();

	UFUNCTION(BlueprintCallable, Category = "UI")
		void BuildBarracks();

	UFUNCTION(BlueprintCallable, Category = "UI")
		void SubtractCost(int32 whatBuilding);

	UFUNCTION(BlueprintCallable, Category = "UI")
		void AddCost(int32 whatBuilding);

	UFUNCTION(BlueprintCallable, Category = "UI")
		int32 GetCurrentPower();

	UFUNCTION(BlueprintCallable, Category = "UI")
		int32 GetMaxPower();

	UFUNCTION(BlueprintPure, Category = "UI")
		bool IsBuilt();

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool HasBarracksSelected();

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool HasFactorySelected();

	UFUNCTION(BlueprintCallable, Category = "UI")
		void BuildUnit(int32 buildingType, int32 unitType);

	UFUNCTION(BlueprintCallable, Category = "UI")
		void ResetIsBuilt();

	UFUNCTION(BlueprintPure, Category = "UI")
		int GetTime(int32 whatBuilding);


protected:


	/// Renamed the functions for clarity.
	UFUNCTION()
		void OnLeftMousePressed();

	UFUNCTION()
		void OnLeftMouseReleased();

	UFUNCTION()
		void OnRightMousePressed();

	UFUNCTION()
		void OnRightMouseReleased();

	UFUNCTION()
		void OnMiddleMousePressed();

	UFUNCTION()
		void OnMiddleMouseReleased();

	UPROPERTY()
		TArray<ACharacter*> SelectedCharacters;

	UPROPERTY()
		AActor* SelectedStructure;
		/// TScriptInterface<II_Structure> SelectedStructure;  -- Leave this here for future reference

	
	void Shift(); //State of either shift key

	/// TArray <AMasterUnit*> selectedUnits; -- Removed and replaced with SelectedCharacters 
	/// TArray <ABuildingMaster*> selectedBuildings; -- Removed and replaced with SelectedStructure

	//ABuildingManager* buildingManager;

	UPROPERTY()
		UBuildingManagerObject* buildingManagerObject;

	bool constructingBuilding, buildingConstructed; 
	
	UPROPERTY()
		AProFow *m_fow;

	///////////////////////////////////
	// DEBUG FUNCTIONS
	///////////////////////////////////
	UFUNCTION()
		void DEBUG_DamageSelected();
};
