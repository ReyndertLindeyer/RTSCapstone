// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MasterUnit.h"
#include "BuildingMaster.h"
#include "Blueprint/UserWidget.h"
#include "Logging/LogMacros.h"
#include "MyRTSHUD.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API AMyRTSHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AMyRTSHUD();

	virtual void BeginPlay() override;

	virtual void DrawHUD() override; //Override the basic HUD

	FVector2D startPos, mousePos; //Where the player pressed down the left mouse button and the updated location of the mouse cursor

	FVector2D GetMousePos();

	bool selectionStart;

	bool grabEverything; //Grabs everything in the rectangle

	bool isShift; //Checks to see if the shift button is being held down

	TArray <AMasterUnit*> foundUnits;
	TArray <ABuildingMaster*> foundBuildings;

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UUserWidget> HUDWidgetClass;
	// Variable to hold the widget After Creating it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		class UUserWidget* CurrentWidget;

	void AddBuilding(ABuildingMaster* building);
	void RemoveBuilding(ABuildingMaster* building);
	void DrawHealthBars();

private:
	TArray <ABuildingMaster*> existingBuildings;
};
