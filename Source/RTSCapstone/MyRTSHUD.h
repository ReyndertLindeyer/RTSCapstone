// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameFramework/Character.h"

// Interfaces
#include "I_Unit.h"
#include "I_Structure.h"

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

	UPROPERTY()
		FVector2D mouseStart;

	UPROPERTY()
		FVector2D mouseEnd;

	UPROPERTY()
		bool bStartSelecting = false;

	UPROPERTY()
		TArray<ACharacter*> FoundCharacters;

	UPROPERTY()
		TArray<ACharacter*> OldFoundCharacters;

	UFUNCTION()
		FVector2D GetMousePos2D();


	bool isShift; //Checks to see if the shift button is being held down

	// bool grabEverything; //Grabs everything in the rectangle
	

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UUserWidget> HUDWidgetClass;
	// Variable to hold the widget After Creating it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		class UUserWidget* CurrentWidget;

	void DrawUnitHealthBars(TArray<ACharacter*> SelectedUnits);
	void DrawBuildingHealthBars(AActor* SelectedBuilding);

	void SetPlayer(II_Player* owner_);

private:

	II_Player* owner;
};
