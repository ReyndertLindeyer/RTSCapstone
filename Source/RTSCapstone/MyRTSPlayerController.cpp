// Fill out your copyright notice in the Description page of Project Settings.

#include "MyRTSPlayerController.h"
#include "Components/InputComponent.h"
#include "MyRTSAIController.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AMyRTSPlayerController::AMyRTSPlayerController() {
	bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Custom; -- Potential way to implement our own cursor?

	unlockCamera = false;
	constructingBuilding = false;
	buildingConstructed = false;
	buildingManagerObject = CreateDefaultSubobject<UBuildingManagerObject>(TEXT("buildingManagerObject"));
}

void AMyRTSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Assign the correct HUD to the pointer
	HUDPtr = Cast<AMyRTSHUD>(GetHUD());

	int32 temp1, temp2;
	GetViewportSize(temp1, temp2);
	FHitResult hit;
	GetHitResultAtScreenPosition(FVector2D(temp1 / 2, temp2 / 2), ECollisionChannel::ECC_Visibility, false, hit);
	
	/// Disabled for debugging
	//buildingManagerObject->SpawnConstructionYard(hit.Location);

	/// Disabled for debugging
	//HUDPtr->AddBuilding(buildingManagerObject->getBuilding(0));

	/// Disabled for debugging
	//m_fow = GetWorld()->SpawnActor<AProFow>(AProFow::StaticClass()); 
	
	/// Disabled for debugging
	//m_fow->revealSmoothCircle(FVector2D(hit.Location.X, hit.Location.Y), buildingManagerObject->getBuilding(0)->GetSightRadius());
}

void AMyRTSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (buildingToBuild!= nullptr) {
		FHitResult hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
		buildingToBuild->SetActorLocation(FVector(hit.Location.X, hit.Location.Y, buildingToBuild->GetActorLocation().Z));
	}
	buildingManagerObject->CheckForDestroyedBuildings();
}

void AMyRTSPlayerController::SetupInputComponent() {

	Super::SetupInputComponent();

	//Jump events
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AMyRTSPlayerController::OnLeftMousePressed);
	InputComponent->BindAction("LeftClick", IE_Released, this, &AMyRTSPlayerController::OnLeftMouseReleased);

	InputComponent->BindAction("RightClick", IE_Released, this, &AMyRTSPlayerController::OnRightMousePressed);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AMyRTSPlayerController::OnRightMouseReleased);

	InputComponent->BindAction("MiddleClick", IE_Pressed, this, &AMyRTSPlayerController::OnMiddleMousePressed);
	InputComponent->BindAction("MiddleClick", IE_Released, this, &AMyRTSPlayerController::OnMiddleMouseReleased);
	
	InputComponent->BindAction("Shift", IE_Pressed, this, &AMyRTSPlayerController::Shift);
	InputComponent->BindAction("Shift", IE_Released, this, &AMyRTSPlayerController::Shift);
}

int32 AMyRTSPlayerController::GetResources()
{
	return (int32)buildingManagerObject->GetResources();
}

bool AMyRTSPlayerController::ConstructBuilding(int32 whatBuilding)
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

int32 AMyRTSPlayerController::GetBuildingCost(int32 whatBuilding)
{
	return buildingManagerObject->GetBuildingCost((uint8)whatBuilding);
}

int32 AMyRTSPlayerController::GetBuildingConstructionTime(int32 whatBuilding)
{
	return buildingManagerObject->GetConstructionTime((uint8)whatBuilding);
}

void AMyRTSPlayerController::BuildPowerPlant()
{
	if (!constructingBuilding && buildingManagerObject) {
		FHitResult hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
		buildingToBuild = buildingManagerObject->ghostBuilding(1, hit.Location); 

		if (buildingToBuild != nullptr) {
			constructingBuilding = true;
		}
	}
}

void AMyRTSPlayerController::BuildRefinery()
{
	if (!constructingBuilding && buildingManagerObject != nullptr) {
		FHitResult hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
		buildingToBuild = buildingManagerObject->ghostBuilding(2, hit.Location);

		if (buildingToBuild != nullptr) {
			constructingBuilding = true;
		}
	}
}

void AMyRTSPlayerController::BuildBarracks()
{
	if (!constructingBuilding && buildingManagerObject != nullptr) {
		FHitResult hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
		buildingToBuild = buildingManagerObject->ghostBuilding(3, hit.Location);

		if (buildingToBuild != nullptr) {
			constructingBuilding = true;
		}
	}
}

void AMyRTSPlayerController::SubtractCost(int32 whatBuilding)
{
	buildingManagerObject->SubtractCost(whatBuilding);
}

void AMyRTSPlayerController::AddCost(int32 whatBuilding)
{
	buildingManagerObject->AddCost(whatBuilding);
}

int32 AMyRTSPlayerController::GetCurrentPower()
{
	return buildingManagerObject->GetCurrentPower();
}

int32 AMyRTSPlayerController::GetMaxPower()
{
	return buildingManagerObject->GetMaxPower();
}

bool AMyRTSPlayerController::IsBuilt()
{
	return buildingConstructed;
}

bool AMyRTSPlayerController::HasBarracksSelected()
{
	return false;
}

bool AMyRTSPlayerController::HasFactorySelected()
{
	return false;
}

void AMyRTSPlayerController::BuildUnit(int32 buildingType, int32 unitType)
{
	if (buildingType == 1) {
		//They are building something from a barracks
		if (unitType == 1) {
			//Spawn a rifleman from all selected barracks
		}
		else if (unitType == 2) {
			//Spawn a rocket man from all selected barracks
		}
		else if (unitType == 3) {
			//Spawn an engineer from all selected barracks
		}
	}
	else if (buildingType == 2) {
		//They are building something from a Vehicle Factory
		if (unitType == 1) {
			//Spawn a harvester from all selected barracks
		}
		else if (unitType == 2) {
			//Spawn a humvee from all selected barracks
		}
		else if (unitType == 3) {
			//Spawn an basic tank from all selected barracks
		}
		else if (unitType == 4) {
			//Spawn a artillery tank from all selected barracks
		}
		else if (unitType == 5) {
			//Spawn an heavy tank from all selected barracks
		}
		else if (unitType == 5) {
			//Spawn an outpost creator from all selected barracks
		}
	}
}

void AMyRTSPlayerController::ResetIsBuilt()
{
	buildingConstructed = false;
}

int32 AMyRTSPlayerController::GetTime(int32 whatBuilding)
{
	return buildingManagerObject->GetConstructionTime((uint8)whatBuilding);
}

//Left mouse down to denote the start of the selection box
void AMyRTSPlayerController::OnLeftMousePressed() {	
	
	if (!constructingBuilding) 
	{
		// If there is a selected structure, deselect it
		if (SelectedStructure != nullptr)
		{
			Cast<II_Structure>(SelectedStructure)->SetSelection(false);
			SelectedStructure = nullptr;
		}

		if (HUDPtr != nullptr) 
		{
			// Begin the selection box
			HUDPtr->mouseStart = HUDPtr->GetMousePos2D();
			HUDPtr->bStartSelecting = true;

			// If isShift is false, empty the selected character list
			/// Moved this inside of the HUDPtr check.  It will cause an error
			/// if HUDPtr flagged as nullptr and this was left outside of it. 
			/// (though it should work 100% of the time regardless, just coding practice)
			if (!HUDPtr->isShift) 
			{
				SelectedCharacters.Empty();
			}
		}
	}
}

//Left mouse up to denote the end of the selection box
void AMyRTSPlayerController::OnLeftMouseReleased() {
	
	if (!constructingBuilding) 
	{
		// If there is no selected structure
		if (SelectedStructure == nullptr)
		{
			// But there is a HUD
			if (HUDPtr != nullptr) 
			{
				// Send out a Raycast
				FHitResult hit;
				GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);

				// If and actor is found and it inherits from I_Structure, select it.
				if (Cast<II_Structure>(hit.Actor))
				{
					SelectedStructure = Cast<AActor>(hit.Actor);
					Cast<II_Structure>(SelectedStructure)->SetSelection(true);
				}

				// Otherwise, if characters were found, select them.
				else 
				{
					SelectedCharacters = HUDPtr->FoundCharacters;
				}

				HUDPtr->bStartSelecting = false;
				//HUDPtr->grabEverything = true;
			}
		}

		
	}
	
	if (constructingBuilding) {
		if (buildingManagerObject->constructBuilding(buildingToBuild)) {
			HUDPtr->AddBuilding(buildingToBuild);
			m_fow->revealSmoothCircle(FVector2D(buildingToBuild->GetActorLocation().X, buildingToBuild->GetActorLocation().Y), buildingToBuild->GetSightRadius());
			buildingToBuild = nullptr;
			constructingBuilding = false;
			buildingConstructed = true;
		}
	}
}

//If the player wants to keep the already selected units selected and add to their number then they would use this
void AMyRTSPlayerController::Shift() {
	if (HUDPtr != nullptr) {
		HUDPtr->isShift = !HUDPtr->isShift;
	}
}

void AMyRTSPlayerController::OnRightMousePressed() {
	
	/// This functionality was rewritten and moved to the DrawHUD in HUDPtr.cpp
	/*if (HUDPtr->foundUnits.Num() > 0.0f) {
		for (int32 i = 0; i < HUDPtr->foundUnits.Num(); i++) {
			selectedUnits.Add(HUDPtr->foundUnits[i]);
		}
	}*/

	if (constructingBuilding) {
		buildingToBuild->Destroy();
		constructingBuilding = false;
		buildingConstructed = false;
		buildingManagerObject->DisableAllDecals();
	}

	if (SelectedCharacters.Num() > 0.0f) {
		//Cycle through all units
		for (int32 i = 0; i < SelectedCharacters.Num(); i++) {
			UE_LOG(LogTemp, Warning, TEXT("GaveOrders"));
			//Find location that the player right clicked on and store it
			FHitResult hit;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);

			//Will store the location and will have units after the first line up next to the first instead of fighting to be in the same location
			FVector MoveLocation = hit.Location + FVector(i/2 * 100, i % 2 * 100, 0);

			//Code to make the units move
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(SelectedCharacters[i]->GetController(), MoveLocation);
		}
	}

	/// This functionality was rewritten and moved to OnLeftMousePressed in this class
	/*if (HUDPtr->foundBuildings.Num() > 0.0f) {
		for (int32 i = 0; i < HUDPtr->foundBuildings.Num(); i++) {
			selectedBuildings.Add(HUDPtr->foundBuildings[i]);
		}
	}*/
}

void AMyRTSPlayerController::OnMiddleMousePressed() 
{

	unlockCamera = true;
}

void AMyRTSPlayerController::OnMiddleMouseReleased() 
{
	unlockCamera = false;
}

void AMyRTSPlayerController::OnRightMouseReleased() {
	
	/// Renamed to unlockCamera and moved to OnMiddleMousePressed in this class
	// rightClicked = !rightClicked;
}