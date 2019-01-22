// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MasterUnit.h"
#include "RifleInfantry.h"
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

	bool rightClicked;

protected:

	void LeftMouseDown();
	void LeftMouseUp();

	void RightMouseUp();
	void RightMouse();

	void Shift(); //State of either shift key

	TArray <AMasterUnit*> selectedActors;
};
