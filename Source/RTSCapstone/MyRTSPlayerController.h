// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BuildingMaster.h"
#include "Building_PowerPlant.h"
#include "Building_Barrecks.h"
#include "UNIT_MOutpost.h"
#include "BuildingManagerObject.h"
#include "MyRTSHUD.h"
//#include "ProFow.h"

// Interfaces
#include "I_Entity.h"
#include "I_Player.h"

#include "I_ScriptedEvent.h"



#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "MyRTSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API AMyRTSPlayerController : public APlayerController, public II_Player
{
	GENERATED_BODY()
	
public:
	AMyRTSPlayerController();

	UPROPERTY(EditAnywhere)
		AActor* scriptedEvent;

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaTime) override;

	//Pointer to the HUD
	AMyRTSHUD* HUDPtr;
	
	UPROPERTY()
		bool unlockCamera;
	
	UPROPERTY()
		bool updateScreen;

	UFUNCTION(BlueprintPure, Category = "UI")
		int32 GetResources();

	UFUNCTION(BlueprintCallable, Category = "UI")
		TArray<int32> GetBuildingCost();

	UFUNCTION(BlueprintCallable, Category = "UI")
		TArray<int32> GetBuildingConstructionTime();

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GhostBuilding(int32 whatBuilding);

	UFUNCTION(BlueprintCallable, Category = "UI")
		void ConstructBuilding(int32 whatBuilding);

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
		bool HasBuiltRefinery(); //Has the player built a refinery, if so they will be able to construct advanced buildings

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool HasBuiltTechCenter(); //Has the player built a Tech Center, if so they will be able to construct advanced Units

	UFUNCTION(BlueprintCallable, Category = "UI")
		void BuildUnit(int32 unitType);

	UFUNCTION(BlueprintCallable, Category = "UI")
		void CancelUnit();

	UFUNCTION(BlueprintPure, Category = "UI")
		float GetUnitConstructionTime();

	UFUNCTION(BlueprintPure, Category = "UI")
		float GetUnitConstructionTimeLeft();

	UFUNCTION(BlueprintCallable, Category = "UI")
		int32 GetUnitNumber(); //Get what unit is being built

	UFUNCTION(BlueprintCallable, Category = "UI")
		TArray <int32> UnitQueue();

	UFUNCTION(BlueprintCallable, Category = "UI")
		float GetConstructionTimeLeft();

	UFUNCTION(BlueprintCallable, Category = "UI")
		void ResetIsBuilt();

	UFUNCTION(BlueprintCallable, Category = "UI")
		void UpdateScreenSize();

	//Functions for the Outpost
	UFUNCTION(BlueprintCallable, Category = "UI")
		bool HasMOutpostSelected();
	UFUNCTION(BlueprintCallable, Category = "UI")
		bool StartGhostOutpost();
	UFUNCTION(BlueprintCallable, Category = "UI")
		void StopGhostOutpost();
	UFUNCTION(BlueprintCallable, Category = "UI")
		void BuildGhostOutpost();

	UFUNCTION(BlueprintCallable, Category = "UI")
		void MoveUnitsToLocation(TArray<ACharacter*> unitsToMove, FVector MoveLocation);

	UFUNCTION()
		AActor* GetPlayerActor();



	UFUNCTION(BlueprintCallable, Category = "Demo")
		void AutoGhostBuilding(int32 whatBuilding, FVector location);

	UFUNCTION(BlueprintCallable, Category = "Demo")
		void AutoConstructBuilding();

	UFUNCTION(BlueprintCallable, Category = "Demo")
		void AutoDestroyBuilding(TArray<ABuildingMaster*> inList);


	UFUNCTION(BlueprintCallable, Category = "EndGame")
		bool HasDestroyedObjective();

	UFUNCTION(BlueprintCallable, Category = "EndGame")
		bool ConstructionYardDestroyed();

	void SetTriggerEnd();
	bool ConYardGone();

	UFUNCTION(BlueprintCallable, Category = "EndGame")
		TArray<int32> Statistics();

protected:

	// DEBUGGING
	bool spawnCY = false;

	bool hasMobileOutpostSelected;

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
		ABuildingMaster* SelectedStructure;
		/// TScriptInterface<II_Structure> SelectedStructure;  -- Leave this here for future reference

	
	void Shift(); //State of either shift key

	/// TArray <AMasterUnit*> selectedUnits; -- Removed and replaced with SelectedCharacters 
	/// TArray <ABuildingMaster*> selectedBuildings; -- Removed and replaced with SelectedStructure

	UPROPERTY()
		UBuildingManagerObject* buildingManagerObject;

	bool constructingBuilding, placingBuilding, buildingConstructed, selectedBarracks, selectedFactory, isConYardDestroyed, triggerEnd;

	float buildingCountdown; //Countdown to building completion

	//UPROPERTY()
	//	AProFow *m_fow;

	///////////////////////////////////
	// DEBUG FUNCTIONS
	///////////////////////////////////
	UFUNCTION()
		void DEBUG_DamageSelected();


};
