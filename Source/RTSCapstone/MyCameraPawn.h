// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "MyRTSPlayerController.h"
#include "MyCameraPawn.generated.h"

UCLASS()
class RTSCAPSTONE_API AMyCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Input variables
	FVector2D MovementInput;
	FVector2D CameraInput;
	FVector velocity;
	int zoom;
	float qOrE, updateScreenTime;
	bool unlocked;

	//Input functions
	void MoveX(float AxisValue);
	void MoveY(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void Zoom(float AxisValue);
	void KeyboardYaw(float AxisValue);
	void RightClick();

	UPROPERTY(EditAnywhere)
		USceneComponent* RootScene;
	UPROPERTY(EditAnywhere)
		USpringArmComponent* OurCameraSpringArm;
	UPROPERTY(EditAnywhere)
		UCameraComponent* OurCamera;
	UPROPERTY(EditAnywhere)
		USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere)
		int cameraQESpeed;
	UPROPERTY(EditAnywhere)
		float cameraSensitivity;

	AMyRTSPlayerController* rtsPC;

	FVector2D mousePos;
	int32 screenSizeX, screenSizeY;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void UpdateScreenSize();

};
