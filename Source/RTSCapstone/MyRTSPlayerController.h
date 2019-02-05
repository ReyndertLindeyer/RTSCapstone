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

	UPROPERTY(EditAnywhere)
		ABuildingMaster* buildingToBuild;
	
	bool rightClicked;

	UFUNCTION(BlueprintPure, Category = "UI")
		float GetResources();
	
	UFUNCTION(BlueprintPure, Category = "UI")
		bool ConstructBuilding(int whatBuilding);

	UFUNCTION(BlueprintPure, Category = "UI")
		int GetBuildingCost(int whatBuilding);

	UFUNCTION(BlueprintPure, Category = "UI")
		int GetBuildingConstructionTime(int whatBuilding);

	UFUNCTION(BlueprintCallable, Category = "UI")
		void BuildPowerPlant();

	UFUNCTION(BlueprintCallable, Category = "UI")
		void BuildRefinery();

	UFUNCTION(BlueprintCallable, Category = "UI")
		void BuildBarracks();

	UFUNCTION(BlueprintCallable, Category = "UI")
		void UseHUDUI();

	UFUNCTION(BlueprintCallable, Category = "UI")
		void SubtractCost(int whatBuilding);

	UFUNCTION(BlueprintCallable, Category = "UI")
		void AddCost(int whatBuilding);

	UFUNCTION(BlueprintPure, Category = "UI")
		bool IsBuilt();

	UFUNCTION(BlueprintCallable, Category = "UI")
		void ResetIsBuilt();

	UFUNCTION(BlueprintPure, Category = "UI")
		int GetTime(int whatBuilding);


protected:

	void LeftMouseDown();
	void LeftMouseUp();

	void RightMouseUp();
	void RightMouse();

	void Shift(); //State of either shift key

	TArray <AMasterUnit*> selectedUnits;
	TArray <ABuildingMaster*> selectedBuildings;

	//ABuildingManager* buildingManager;

	UBuildingManagerObject* buildingManagerObject;

	bool constructingBuilding, buildingConstructed;
};
