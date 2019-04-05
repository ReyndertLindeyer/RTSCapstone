// Fill out your copyright notice in the Description page of Project Settings.

#include "MyRTSPlayerController.h"
#include "Components/InputComponent.h"
#include "MyRTSAIController.h"
#include "Blueprint/UserWidget.h"
#include "MyCameraPawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "GameManager.h"

#include "Runtime/Engine/Classes/GameFramework/Character.h"


AMyRTSPlayerController::AMyRTSPlayerController() {
	bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Custom; -- Potential way to implement our own cursor?

	unlockCamera = false;

	placingBuilding = false;
	buildingConstructed = false;
	placingBuilding = false;

	updateScreen = false;

	selectedBarracks = false;
	selectedFactory = false;

	isConYardDestroyed = false;
	triggerEnd = false;

	hasMobileOutpostSelected = false;

	buildingManagerObject = CreateDefaultSubobject<UBuildingManagerObject>(TEXT("buildingManagerObject"));

	// Player Interface
	InitializePlayer(FString("Player 1"), 1);

	buildingManagerObject->SetPlayer(GetPlayerReference());
}

void AMyRTSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	

	//Assign the correct HUD to the pointer
	HUDPtr = Cast<AMyRTSHUD>(GetHUD());

	HUDPtr->SetPlayer(this);

	int32 temp1, temp2;
	GetViewportSize(temp1, temp2);
	FHitResult hit;
	GetHitResultAtScreenPosition(FVector2D(temp1 / 2, temp2 / 2), ECollisionChannel::ECC_Visibility, false, hit);

	InitResources(50000);
	ChangePower(40);

	SetHasDestroyedObjective(false);

	for (TObjectIterator<AGameManager> Itr; Itr; ++Itr)
	{
		if (Itr->spawnCYForPlayer)
			buildingManagerObject->SpawnConstructionYard(Cast<AActor>(this)->GetActorLocation());
		
		if (Itr->scriptedEventList.Num() > 0)
			if (Itr->scriptedEventList[0] != nullptr)
				scriptedEvent = Itr->scriptedEventList[0];

		//Cast<II_Player>(playerList[0])->SetBuildingDataTable(buildingDataTable);
		//Cast<II_Player>(playerList[0])->SetUnitConstructionDataTable(unitConstructionDataTable);
	}
	
	/// Disabled for debugging
	//m_fow = GetWorld()->SpawnActor<AProFow>(AProFow::StaticClass()); 

	/// Disabled for debugging
	//m_fow->revealSmoothCircle(FVector2D(hit.Location.X, hit.Location.Y), buildingManagerObject->getBuilding(0)->GetSightRadius());

	
}

void AMyRTSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (placingBuilding == true) {
		FHitResult hit;
		//GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, hit);
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
		//buildingManagerObject->MoveBuilding(FVector(hit.Location.X, hit.Location.Y, buildingManagerObject->GetBuildingToBuild()->GetActorLocation().Z));
		//GetHitResultUnderCursorByChannel(ECollisionChannel::ECC_GameTraceChannel1, false, hit);

		buildingManagerObject->MoveBuilding(FVector(hit.Location.X, hit.Location.Y, hit.Location.Z));
	}

	if (buildingCountdown > 0) {
		if (GetPower() > 0)
			buildingCountdown -= DeltaTime;
		else
			buildingCountdown -= DeltaTime / 2;
	}

	if (GetSelectedCharacters().Num() == 1) {
		if (Cast<AUNIT_MOutpost>(GetSelectedCharacters()[0])) {
			hasMobileOutpostSelected = true;
		}
	}

	if (buildingManagerObject->IsConstructionYardDestroyed()) {
		isConYardDestroyed = true;
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

	InputComponent->BindAction("DEBUG_1", IE_Pressed, this, &AMyRTSPlayerController::DEBUG_DamageSelected);
}


void AMyRTSPlayerController::DEBUG_DamageSelected()
{
	/*
	if (SelectedStructure != nullptr)
	{
		
		II_Entity* entity = Cast<II_Entity>(SelectedStructure);
		
		// If the Entity will survive the damage
		if (entity->GetCurrentHealth() - (entity->GetMaxHealth() / 10) > 0)
		{
			// Inflict 10% of Max Health as Damage
			entity->DealDamage(entity->GetMaxHealth() / 10);
			UE_LOG(LogTemp, Warning, TEXT("%f / %f  (%f%)"), entity->GetCurrentHealth(), entity->GetMaxHealth(), entity->GetHealthPercentage());
		}

		// Otherwise Destroy it
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroyed"));
			SelectedStructure = nullptr;
			entity->DestroyEntity();
			return;
		}
	}

	else if (GetSelectedCharacters().Num() > 0)
	{
		TArray<ACharacter*> deletionArray;

		for (int i = 0; i < GetSelectedCharacters().Num(); i++)
		{
			II_Entity* entity = Cast<II_Entity>(GetSelectedCharacters()[i]);

			// If the Entity will survive the damage
			if (entity->GetCurrentHealth() - (entity->GetMaxHealth() / 10) > 0)
			{
				// Inflict 10% of Max Health as Damage
				entity->DealDamage(entity->GetMaxHealth() / 10);
				UE_LOG(LogTemp, Warning, TEXT("%f / %f  (%f%)"), entity->GetCurrentHealth(), entity->GetMaxHealth(), entity->GetHealthPercentage());
			}

			// Otherwise Destroy it
			else
			{
				deletionArray.Add(GetSelectedCharacters()[i]);
				UE_LOG(LogTemp, Warning, TEXT("Destroyed"));
			}
		}

		if (deletionArray.Num() > 0)
		{
			for (int i = 0; i < deletionArray.Num(); i++)
			{
				GetSelectedCharacters().Remove(deletionArray[i]);
				Cast<II_Entity>(deletionArray[i])->DestroyEntity();
			}
		}
	}
	*/

	if (scriptedEvent != nullptr)
	{
		/// Leave these params for the future.  They're a fun idea, but couldn't get them
		/// 100% working for the showcase
		/*SE_PARAMS* params = new SE_PARAMS;
		params->BuildParams(Cast<II_Player>(Itr->playerList[0]));*/

		II_ScriptedEvent* s_event = Cast<II_ScriptedEvent>(scriptedEvent);

		if (s_event != nullptr)
			s_event->TriggerEvent(Cast<II_Player>(this));
		else
			UE_LOG(LogTemp, Warning, TEXT("No Scripted Event"));
	}

}


int32 AMyRTSPlayerController::GetResources()
{
	return GetResourceAmount();
}

bool AMyRTSPlayerController::GhostBuilding(int32 whatBuilding)
{
	if (!placingBuilding) {
		FHitResult hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
		
		buildingManagerObject->ghostBuilding(whatBuilding, hit.Location);

		placingBuilding = true;
		return true;
	}
	return false;
}

void AMyRTSPlayerController::ConstructBuilding(int32 whatBuilding)
{
	buildingCountdown = buildingManagerObject->GetCertainConstructionTime(whatBuilding);
	constructingBuilding = true;
}

TArray<int32> AMyRTSPlayerController::GetBuildingCost()
{
	return buildingManagerObject->GetBuildingCost();
}

TArray<int32> AMyRTSPlayerController::GetBuildingConstructionTime()
{
	return buildingManagerObject->GetConstructionTime();
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
	return GetPower();
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
	return selectedBarracks;
}

bool AMyRTSPlayerController::HasFactorySelected()
{
	return selectedFactory;
}

bool AMyRTSPlayerController::HasBuiltRefinery()
{
	return buildingManagerObject->IsRefineryBuilt();
}

bool AMyRTSPlayerController::HasBuiltTechCenter()
{
	return buildingManagerObject->IsTechCentreBuilt();
}

void AMyRTSPlayerController::BuildUnit(int32 unitType)
{
	///As this should only be called when a unit producing structure is selected the code will be simpler
	if (Cast<ABuilding_Barrecks>(SelectedStructure)) {
		//They are building something from a barrack
		ChangeResources(-Cast<ABuilding_Barrecks>(SelectedStructure)->AddToUnitQueue(unitType));
	}
	else if (Cast<ABuilding_VehicleFactory>(SelectedStructure)) {
		//They are building something from a Vehicle Factory
		ChangeResources(-Cast<ABuilding_VehicleFactory>(SelectedStructure)->AddToUnitQueue(unitType));
	}
}

void AMyRTSPlayerController::CancelUnit()
{
	if (Cast<ABuilding_Barrecks>(SelectedStructure)) {
		//They are building something from a barrack
		ChangeResources(Cast<ABuilding_Barrecks>(SelectedStructure)->RemoveFromUnitQueue());
	}
	else if (Cast<ABuilding_VehicleFactory>(SelectedStructure)) {
		//They are building something from a Vehicle Factory
		ChangeResources(Cast<ABuilding_VehicleFactory>(SelectedStructure)->RemoveFromUnitQueue());
	}
}

float AMyRTSPlayerController::GetUnitConstructionTime()
{
	if (SelectedStructure) {
		if (Cast<ABuilding_Barrecks>(SelectedStructure)) {
			return Cast<ABuilding_Barrecks>(SelectedStructure)->StartingTime();
		}
		else if (Cast<ABuilding_VehicleFactory>(SelectedStructure)) {
			return Cast<ABuilding_VehicleFactory>(SelectedStructure)->StartingTime();
		}
	}
	return 0;
}

float AMyRTSPlayerController::GetUnitConstructionTimeLeft()
{
		if (Cast<ABuilding_Barrecks>(SelectedStructure)) {
			return Cast<ABuilding_Barrecks>(SelectedStructure)->TimeRemaining();
		}
		else if (Cast<ABuilding_VehicleFactory>(SelectedStructure)) {
			return Cast<ABuilding_VehicleFactory>(SelectedStructure)->TimeRemaining();
		}
	return 0;
}

int32 AMyRTSPlayerController::GetUnitNumber()
{
		if (Cast<ABuilding_Barrecks>(SelectedStructure)) {
			return Cast<ABuilding_Barrecks>(SelectedStructure)->GetUnitAtStartOfQueue();
		}
		else if (Cast<ABuilding_VehicleFactory>(SelectedStructure)) {
			return Cast<ABuilding_VehicleFactory>(SelectedStructure)->GetUnitAtStartOfQueue();
		}
	return 0;
}

TArray<int32> AMyRTSPlayerController::UnitQueue()
{
	///Implement returning a queue of units
	/*
	if (Cast<ABuilding_Barrecks>(SelectedStructure)) {
		return Cast<ABuilding_Barrecks>(SelectedStructure)->GetUnitAtStartOfQueue();
	}
	else if (Cast<ABuilding_VehicleFactory>(SelectedStructure)) {
		return Cast<ABuilding_VehicleFactory>(SelectedStructure)->Queue;
	}
	*/
	return TArray<int32>();
}

float AMyRTSPlayerController::GetConstructionTimeLeft()
{
	return buildingCountdown;
}

void AMyRTSPlayerController::ResetIsBuilt()
{
	buildingConstructed = false;
}

void AMyRTSPlayerController::UpdateScreenSize()
{
	updateScreen = !updateScreen;
}

//Left mouse down to denote the start of the selection box
void AMyRTSPlayerController::OnLeftMousePressed() {	
	
	if (!placingBuilding) 
	{
		// If there is a selected structure, deselect it
		if (SelectedStructure != nullptr)
		{
			SelectedStructure->SetSelection(false, this);
			SelectedStructure = nullptr;

			SetSelectedBuilding(SelectedStructure);

			selectedBarracks = false;
			selectedFactory = false;
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
				for (int i = 0; i < GetSelectedCharacters().Num(); i++) {
					Cast<II_Unit>(GetSelectedCharacters()[i])->SetSelection(false, this);
				}

				GetSelectedCharacters().Empty();
			}
		}
	}
}

//Left mouse up to denote the end of the selection box
void AMyRTSPlayerController::OnLeftMouseReleased() {
	
	if (!placingBuilding) 
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
				if (Cast<ABuildingMaster>(hit.Actor))
				{
					SelectedStructure = Cast<ABuildingMaster>(hit.Actor);
					SelectedStructure->SetSelection(true, this);

					
					if (Cast<ABuilding_Barrecks>(SelectedStructure) && Cast<II_Entity>(SelectedStructure)->GetEntityOwner() == this) {
						selectedBarracks = true;
					}
					if (Cast<ABuilding_VehicleFactory>(SelectedStructure) && Cast<II_Entity>(SelectedStructure)->GetEntityOwner() == this) {
						selectedFactory = true;
					}

					SetSelectedBuilding(SelectedStructure);
					

					/// Debugging
					II_Entity* entity = Cast<II_Entity>(SelectedStructure);
					if (entity->GetEntityOwner() != nullptr)
					{
						UE_LOG(LogTemp, Warning, TEXT("%s (%s) : %f / %f  (%f%)"), *entity->GetName(), *entity->GetEntityOwner()->GetPlayerName(), entity->GetCurrentHealth(), entity->GetMaxHealth(), entity->GetHealthPercentage());
					}

					else
					{
						UE_LOG(LogTemp, Warning, TEXT("%s (none) : %f / %f  (%f%)"), *entity->GetName(), entity->GetCurrentHealth(), entity->GetMaxHealth(), entity->GetHealthPercentage());
					}
					/// End Debug

				}

				// Otherwise, if characters were found, select them.
				else 
				{
					SetSelectedCharacters(HUDPtr->FoundCharacters);

					/// Debugging
					for (int i = 0; i < GetSelectedCharacters().Num(); i++)
					{
						II_Entity* entity = Cast<II_Entity>(GetSelectedCharacters()[i]);
						if (entity->GetEntityOwner() != nullptr)
						{
							UE_LOG(LogTemp, Warning, TEXT("%s (%s) : %f / %f  (%f%)"), *entity->GetName(), *entity->GetEntityOwner()->GetPlayerName(), entity->GetCurrentHealth(), entity->GetMaxHealth(), entity->GetHealthPercentage());
						}

						else
						{
							UE_LOG(LogTemp, Warning, TEXT("%s (none) : %f / %f  (%f%)"), *entity->GetName(), entity->GetCurrentHealth(), entity->GetMaxHealth(), entity->GetHealthPercentage());
						}
					}
					/// End Debug
				}

				HUDPtr->bStartSelecting = false;

				for (int i = 0; i < GetSelectedCharacters().Num(); i++) {
					Cast<II_Unit>(GetSelectedCharacters()[i])->SetSelection(true, this);
				}
				//HUDPtr->grabEverything = true;
			}
		}

		
	}
	
	if (placingBuilding) {
		if (buildingManagerObject->constructBuilding()) {
			///HUDPtr->AddBuilding(buildingToBuild);
			///m_fow->revealSmoothCircle(FVector2D(buildingToBuild->GetActorLocation().X, buildingToBuild->GetActorLocation().Y), buildingToBuild->GetSightRadius());
			placingBuilding = false;
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

	if (placingBuilding) {
		buildingManagerObject->DeleteBuilding();
		placingBuilding = false;
		buildingConstructed = false;
		buildingManagerObject->DisableAllDecals();
	}

	if (GetSelectedCharacters().Num() > 0.0f) {
		//Cycle through all units
		for (int32 i = 0; i < GetSelectedCharacters().Num(); i++) {
			if (!(i < GetSelectedCharacters().Num())) {
				break;
			}


			if (GetSelectedCharacters()[i] != nullptr) {

				/// Disable Commands for Units that are not your own
				if (Cast<II_Entity>(GetSelectedCharacters()[i])->GetEntityOwner() == this)
				{
					UE_LOG(LogTemp, Warning, TEXT("GaveOrders"));
					//Find location that the player right clicked on and store it
					FHitResult hit;
					GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);

					//Will store the location and will have units after the first line up next to the first instead of fighting to be in the same location
					FVector MoveLocation = hit.Location + FVector(i / 2 * 500, i % 2 * 500, 0);

					// If an entity is hit
					if (Cast<II_Entity>(hit.Actor))
					{
						// If the Selected unit is a harvester and thie hit actor is a refinery
						if (Cast<AUNIT_Harvester>(GetSelectedCharacters()[i]))
						{
							if (Cast<ABuilding_Refinery>(hit.Actor))
							{
								// If the the harvester and the refinery share the same owner
								if (Cast<II_Entity>(hit.Actor)->GetEntityOwner() == Cast<II_Player>(this))
								{
									// Return to the selected refinery
									Cast<AUNIT_Harvester>(GetSelectedCharacters()[i])->ReturnToRefinery(Cast<ABuilding_Refinery>(hit.Actor));
								}
							}

							else if (Cast<AResourceNode>(hit.Actor))
							{
								Cast<AUNIT_Harvester>(GetSelectedCharacters()[i])->TargetNode(Cast<AResourceNode>(hit.Actor));
							}

						}

						else if (Cast<II_Entity>(hit.Actor)->GetEntityOwner() != Cast<II_Player>(this))
						{
							UE_LOG(LogTemp, Warning, TEXT("Enemy Entity Hit"));
							Cast<II_Unit>(GetSelectedCharacters()[i])->AttackOrder(Cast<II_Entity>(hit.Actor));
						}
					}

					else
					{
						//Code to make the units move	
						Cast<II_Unit>(GetSelectedCharacters()[i])->MoveOrder(GetSelectedCharacters()[i]->GetController(), MoveLocation);
					}


				}

				/// Keep this here for if we need to command enemy units (DEBUG)
				//UE_LOG(LogTemp, Warning, TEXT("GaveOrders"));
				////Find location that the player right clicked on and store it
				//FHitResult hit;
				//GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);

				////Will store the location and will have units after the first line up next to the first instead of fighting to be in the same location
				//FVector MoveLocation = hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);

				////Code to make the units move
				//Cast<II_Unit>(GetSelectedCharacters()[i])->MoveOrder(GetSelectedCharacters()[i]->GetController(), MoveLocation);


			}
		}

		/// This functionality was rewritten and moved to OnLeftMousePressed in this class
		/*if (HUDPtr->foundBuildings.Num() > 0.0f) {
			for (int32 i = 0; i < HUDPtr->foundBuildings.Num(); i++) {
				selectedBuildings.Add(HUDPtr->foundBuildings[i]);
			}
		}*/
	}
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

	//Check to see if there is a selected structure, if there is check to see if it is a unit producing structure, if it is then set the location as the structure's waypoint
	if (SelectedStructure) {
		if (Cast<ABuilding_Barrecks>(SelectedStructure)) {
			FHitResult hit;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
			Cast<ABuilding_Barrecks>(SelectedStructure)->SetWaypoint(hit.Location);

		}
		else if (Cast<ABuilding_VehicleFactory>(SelectedStructure)) {
			FHitResult hit;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
			Cast<ABuilding_VehicleFactory>(SelectedStructure)->SetWaypoint(hit.Location);
		}
	}
}

bool AMyRTSPlayerController::HasMOutpostSelected()
{
	return hasMobileOutpostSelected;
}

bool AMyRTSPlayerController::StartGhostOutpost()
{
	return Cast<AUNIT_MOutpost>(GetSelectedCharacters()[0])->StartGhostBuilding();
}

void AMyRTSPlayerController::StopGhostOutpost()
{
	Cast<AUNIT_MOutpost>(GetSelectedCharacters()[0])->StopGhostBuilding();
}

void AMyRTSPlayerController::BuildGhostOutpost()
{
	Cast<AUNIT_MOutpost>(GetSelectedCharacters()[0])->BuildGhostBuilding();
}

void AMyRTSPlayerController::MoveUnitsToLocation(TArray<ACharacter*> unitsToMove, FVector MoveLocation)
{
	for (int i = 0; i < unitsToMove.Num(); i++) {
		//if (Cast<II_Unit*>(unitsToMove[i])) {
			Cast<II_Unit>(unitsToMove[i])->MoveOrder(unitsToMove[i]->GetController(), MoveLocation);
		//}
	}
}

AActor* AMyRTSPlayerController::GetPlayerActor()
{
	return Cast<AActor>(this);
}

void AMyRTSPlayerController::AutoGhostBuilding(int32 whatBuilding, FVector location)
{
	buildingManagerObject->MultiGhostBuilding(whatBuilding, location);
}

void AMyRTSPlayerController::AutoConstructBuilding()
{
	buildingManagerObject->MultiConstructBuilding();
}

void AMyRTSPlayerController::AutoDestroyBuilding(TArray<ABuildingMaster*> inList) {
	for (int i = 0; i < inList.Num(); i++) {
		inList[i]->DealDamage(inList[i]->GetMaxHealth());
	}
	buildingManagerObject->ghostBuildingArray.Empty();
	buildingManagerObject->whatBuildingArray.Empty();
}

bool AMyRTSPlayerController::HasDestroyedObjective()
{
	return GetHasDestroyedObjective();
}

bool AMyRTSPlayerController::ConstructionYardDestroyed()
{
	return triggerEnd;
}


void AMyRTSPlayerController::SetTriggerEnd() {
	triggerEnd = true;
}

bool AMyRTSPlayerController::ConYardGone() {
	return isConYardDestroyed;
}

TArray<int32> AMyRTSPlayerController::Statistics()
{
	return GetStatistics();
}
