// Fill out your copyright notice in the Description page of Project Settings.

#include "MyRTSPlayerController.h"
#include "Components/InputComponent.h"
#include "MyRTSAIController.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AMyRTSPlayerController::AMyRTSPlayerController() {
	bShowMouseCursor = true;
	rightClicked = false;
	constructingBuilding = false;
}

void AMyRTSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Assign the correct HUD to the pointer
	HUDPtr = Cast<AMyRTSHUD>(GetHUD());


}

void AMyRTSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (buildingToBuild!= nullptr) {
		FHitResult hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
		buildingToBuild->SetActorLocation(hit.Location);
	}
}

void AMyRTSPlayerController::SetupInputComponent() {

	Super::SetupInputComponent();

	//Jump events
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AMyRTSPlayerController::LeftMouseDown);
	InputComponent->BindAction("LeftClick", IE_Released, this, &AMyRTSPlayerController::LeftMouseUp);

	InputComponent->BindAction("RightClick", IE_Released, this, &AMyRTSPlayerController::RightMouseUp);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AMyRTSPlayerController::RightMouse);
	InputComponent->BindAction("RightClick", IE_Released, this, &AMyRTSPlayerController::RightMouse);

	InputComponent->BindAction("Shift", IE_Pressed, this, &AMyRTSPlayerController::Shift);
	InputComponent->BindAction("Shift", IE_Released, this, &AMyRTSPlayerController::Shift);

	InputComponent->BindAction("G", IE_Pressed, this, &AMyRTSPlayerController::G);
}

//Left mouse down to denote the start of the selection box
void AMyRTSPlayerController::LeftMouseDown() {
	if (!constructingBuilding) {
		if (HUDPtr != nullptr) {
			HUDPtr->startPos = HUDPtr->GetMousePos();
			HUDPtr->selectionStart = true;
		}
		if (!HUDPtr->isShift) {
			selectedUnits.Empty();
		}
	}
}

//Left mouse up to denote the end of the selection box
void AMyRTSPlayerController::LeftMouseUp() {
	if (!constructingBuilding) {
		if (HUDPtr != nullptr) {
			HUDPtr->selectionStart = false;
			HUDPtr->grabEverything = true;
		}
	}
	if (constructingBuilding) {
		buildingToBuild->constructAtLocation();
		buildingToBuild = nullptr;
		constructingBuilding = false;
	}
}

//If the player wants to keep the already selected units selected and add to their number then they would use this
void AMyRTSPlayerController::Shift() {
	if (HUDPtr != nullptr) {
		HUDPtr->isShift = !HUDPtr->isShift;
	}
}

void AMyRTSPlayerController::RightMouseUp() {
	if (HUDPtr->foundUnits.Num() > 0.0f) {
		for (int i = 0; i < HUDPtr->foundUnits.Num(); i++) {
			selectedUnits.Add(HUDPtr->foundUnits[i]);
		}
	}
	if (selectedUnits.Num() > 0.0f) {
		//Cycle through all units
		for (int i = 0; i < selectedUnits.Num(); i++) {
			UE_LOG(LogTemp, Warning, TEXT("GaveOrders"));
			//Find location that the player right clicked on and store it
			FHitResult hit;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);

			//Will store the location and will have units after the first line up next to the first instead of fighting to be in the same location
			FVector MoveLocation = hit.Location + FVector(i/2 * 100, i % 2 * 100, 0);

			//Code to make the units move
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(selectedUnits[i]->GetController(), MoveLocation);
		}
	}
	if (HUDPtr->foundBuildings.Num() > 0.0f) {
		for (int i = 0; i < HUDPtr->foundBuildings.Num(); i++) {
			selectedBuildings.Add(HUDPtr->foundBuildings[i]);
		}
	}
}

void AMyRTSPlayerController::RightMouse() {
	rightClicked = !rightClicked;
}

void AMyRTSPlayerController::G() {
	FHitResult hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
	buildingToBuild = GetWorld()->SpawnActor<ABuilding_Construction_Yard>(ABuilding_Construction_Yard::StaticClass(), hit.Location, FRotator(0.0f, 0.0f, 0.0f));
	constructingBuilding = true;
}