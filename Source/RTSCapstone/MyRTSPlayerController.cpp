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
	buildingManagerObject = CreateDefaultSubobject<UBuildingManagerObject>(TEXT("buildingManagerObject"));
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
		buildingToBuild->SetActorLocation(FVector(hit.Location.X, hit.Location.Y, buildingToBuild->GetActorLocation().Z));
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
	InputComponent->BindAction("H", IE_Pressed, this, &AMyRTSPlayerController::H);
	InputComponent->BindAction("J", IE_Pressed, this, &AMyRTSPlayerController::J);
}

float AMyRTSPlayerController::GetResources()
{
	return buildingManagerObject->GetResources();
}

bool AMyRTSPlayerController::ConstructBuilding(int whatBuilding)
{
	if (!constructingBuilding) {
		FHitResult hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
		buildingToBuild = buildingManagerObject->ghostBuilding(whatBuilding, hit.Location);

		if (buildingToBuild != nullptr)
			constructingBuilding = true;
		return true;
	}
	return false;
}

int AMyRTSPlayerController::GetBuildingCost(int whatBuilding)
{
	return buildingManagerObject->GetBuildingCost((uint8)whatBuilding);
}

int AMyRTSPlayerController::GetBuildingConstructionTime(int whatBuilding)
{
	return buildingManagerObject->GetConstructionTime((uint8)whatBuilding);
}

bool AMyRTSPlayerController::BuildPowerPlant()
{
	if (!constructingBuilding) {
		FHitResult hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
		buildingToBuild = buildingManagerObject->ghostBuilding(1, hit.Location);

		if (buildingToBuild != nullptr)
			constructingBuilding = true;
		return true;
	}
	return false;
}

bool AMyRTSPlayerController::UseHUDUI()
{
	return false;
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
		if (buildingManagerObject->constructBuilding(buildingToBuild)) {
			buildingToBuild = nullptr;
			constructingBuilding = false;
		}
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

	if (constructingBuilding) {
		buildingToBuild->Destroy();
		constructingBuilding = false;
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
	if (!constructingBuilding) {
		FHitResult hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
		buildingToBuild = buildingManagerObject->ghostBuilding(3, hit.Location);

		if(buildingToBuild != nullptr)
			constructingBuilding = true;
	}
}

void AMyRTSPlayerController::H() {
	if (!constructingBuilding) {
		FHitResult hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
		buildingToBuild = buildingManagerObject->ghostBuilding(2, hit.Location);

		if (buildingToBuild != nullptr)
			constructingBuilding = true;
	}
}

void AMyRTSPlayerController::J() {
	if (!constructingBuilding) {
		FHitResult hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
		buildingToBuild = buildingManagerObject->ghostBuilding(1, hit.Location);

		if (buildingToBuild != nullptr)
			constructingBuilding = true;
	}
}