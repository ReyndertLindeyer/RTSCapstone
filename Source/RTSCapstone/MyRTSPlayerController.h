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
#include "BuildingManager.h"
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

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wUI;
	// Variable to hold the widget After Creating it.
	UUserWidget* MyUI;

	bool rightClicked;

protected:

	void LeftMouseDown();
	void LeftMouseUp();

	void RightMouseUp();
	void RightMouse();

	void Shift(); //State of either shift key

	void G();
	void H();
	void J();

	TArray <AMasterUnit*> selectedUnits;
	TArray <ABuildingMaster*> selectedBuildings;

	//ABuildingManager* buildingManager;

	//UBuildingManagerObject* buildingManagerObject;

	bool constructingBuilding;
};
