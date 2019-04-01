// Fill out your copyright notice in the Description page of Project Settings.

#include "MyRTSHUD.h"
#include "BuildingMaster.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"

AMyRTSHUD::AMyRTSHUD() {
	//selectionStart = false;
	//grabEverything = false;
	//isShift = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> MyUI(TEXT("/Game/Game_Assets/Widgets/UIWidget"));
	HUDWidgetClass = MyUI.Class;
}

void AMyRTSHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AMyRTSHUD::DrawHUD() {
	Super::DrawHUD(); 

	if (owner->GetSelectedBuilding() != nullptr) {
		DrawBuildingHealthBars(owner->GetSelectedBuilding());
	}

	if (owner->GetSelectedCharacters().Num() > 0) {
		DrawUnitHealthBars(owner->GetSelectedCharacters());
	}
	
	if (bStartSelecting) {
		
		// If shift is held down, ignore emptying the array. 
		/// Note :: Possible issue.  Need to confirm if the array is being filled with duplicates.
		/// The intention of emptying the array was because the selection was readding them to the array.
		if (!isShift) {
			
			
			/////////////////////////////////////////////
			// Deselection -- Empties the previous selection
			/////////////////////////////////////////////

			//See if there are any units already selected
			if (FoundCharacters.Num() > 0) 
			{
				for (int i = 0; i < FoundCharacters.Num(); i++) 
				{
					//Code to remove the selection from existing actors
					
					/// Now Checks to see if the character has inheritence from the I_Unit interface
					/// Which should trigger 100% of the time, since it's a requirement to be added to the array in the first place.
					if (Cast<II_Unit>(FoundCharacters[i]))
					{
						/// Calls the inherited function from the unit.
						//Cast<II_Unit>(FoundCharacters[i])->SetSelection(false);
					}

				}
			}

			//Empty the array
			FoundCharacters.Empty();
		}

		/////////////////////////////////////////////
		// Draw Selection Box -- Like a Box.
		/////////////////////////////////////////////

		//Get the position of the mouse and use it and the starting position to draw a rectangle to denote the selection area
		mouseEnd = GetMousePos2D();

		// Selection Box
		DrawRect
		(
			FLinearColor(0, 0, 1, 0.15f),   /// Determines the color and opacity of the selection box
			mouseStart.X,					/// The Starting X position of the rectangle
			mouseStart.Y,					/// The Starting Y position of the rectangle
			mouseEnd.X - mouseStart.X,		/// The Final X position of the rectangle
			mouseEnd.Y - mouseStart.Y		/// The Final Y position of the rectangle
		);


		/////////////////////////////////////////////
		// Begin Selection -- Adds the new selection
		/////////////////////////////////////////////

		GetActorsInSelectionRectangle<ACharacter>(mouseStart, mouseEnd, FoundCharacters, false, false);

		/*
		if (FoundCharacters.Num() > 0)
		{
			for (int32 i = 0; i < FoundCharacters.Num(); i++)
			{
				if (Cast<II_Unit>(FoundCharacters[i]))
				{
						Cast<II_Unit>(FoundCharacters[i])->SetSelection(true);
				}
			}
		}
		*/
	}

	//else if (!selectionStart && grabEverything) {
	//	//Select the actors themselves
	//	GetActorsInSelectionRectangle<AMasterUnit>(startPos, mousePos, foundUnits, false, false);
	//	if (foundUnits.Num() > 0) {
	//		for (int i = 0; i < foundUnits.Num(); i++) {
	//			//Code to show that the actors are selected
	//			foundUnits[i]->selected = true;
	//		}
	//	}
	//	GetActorsInSelectionRectangle<ABuildingMaster>(startPos, mousePos, foundBuildings, false, false);
	//	if (foundBuildings.Num() > 0) {
	//		for (int i = 0; i < foundBuildings.Num(); i++) {
	//			//Code to show that the actors are selected
	//			foundBuildings[i]->SelectBuilding();
	//		}
	//	}
	//	grabEverything = false;
	//}
}

//Get the location of the mouse and then return it
FVector2D AMyRTSHUD::GetMousePos2D() {

	float posX = 1.0f, posY = 1.0f;

	GetOwningPlayerController()->GetMousePosition(posX, posY);

	return FVector2D(posX, posY);
}

void AMyRTSHUD::DrawUnitHealthBars(TArray<ACharacter*> SelectedUnits)
{
	if (SelectedUnits.Num() > 0) {
		for (int i = 0; i < SelectedUnits.Num(); i++) {
			if (SelectedUnits[i] != nullptr) {
				if (Cast<II_Entity>(SelectedUnits[i])->GetHealthPercentage() > 0) {
					// Select the center point of the bar as the character's location
					FVector center = SelectedUnits[i]->GetActorLocation();
					// Offsets of the bar
					FVector extent = FVector(60.f, 34.f, 50.75f);

					// Project function of Canvas translates a world position to the screen position
					FVector2D center2D = FVector2D(Canvas->Project(FVector(center.X, center.Y, center.Z + extent.Z)));

					float actorExtent = 50.f;
					float healthPercentage = 0.5f;
					float yOffset = 10.f;

					healthPercentage = Cast<II_Entity>(SelectedUnits[i])->GetHealthPercentage();
					//actorExtent = 20;;

					FVector pos1 = Canvas->Project(FVector(center.X, center.Y - actorExtent * 2, center.Z + extent.Z));
					FVector pos2 = Canvas->Project(FVector(center.X, center.Y + actorExtent * 2, center.Z + extent.Z));

					float barWidth = (pos2 - pos1).Size2D() * 0.2;
					float barHeight = barWidth * 0.2f;

					// Draw a background color first
					/// Background tile ///
					barWidth += 2.f;
					barHeight += 2.f;

					float x = center2D.X - barWidth * 0.5f;
					float y = center2D.Y;

					FCanvasTileItem tileItem(FVector2D(x, y), FVector2D(barWidth, barHeight), FLinearColor(0.0f, 0.0f, 0.0f, 0.5f));
					tileItem.BlendMode = SE_BLEND_Translucent;
					Canvas->DrawItem(tileItem);
					/// Background tile ///

					// Draw the health indicator
					/// Health tile ///
					barWidth -= 2.f;
					barHeight -= 2.f;

					x = center2D.X - barWidth * 0.5f;
					y = center2D.Y + 1.f;

					tileItem.Position = FVector2D(x, y);
					if (healthPercentage > 0.6)
						tileItem.SetColor(FLinearColor::Green);
					else if (healthPercentage > 0.3 && healthPercentage <= 0.6)
						tileItem.SetColor(FLinearColor::Yellow);
					else
						tileItem.SetColor(FLinearColor::Red);

					tileItem.Size = FVector2D(barWidth * healthPercentage, barHeight);
					Canvas->DrawItem(tileItem);
				}
			}
		}
	}
}

void AMyRTSHUD::DrawBuildingHealthBars(AActor * SelectedBuilding)
{
	ABuildingMaster* tempBuilding = Cast<ABuildingMaster>(SelectedBuilding);

		// Select the center point of the bar as the character's location
		FVector center = tempBuilding->GetActorLocation();
		// Offsets of the bar
		FVector extent = FVector(60.f, 34.f, 50.75f);

		// Project function of Canvas translates a world position to the screen position
		FVector2D center2D = FVector2D(Canvas->Project(FVector(center.X, center.Y, center.Z + extent.Z)));

		float actorExtent = 50.f;
		float healthPercentage = 0.5f;
		float yOffset = 10.f;

		healthPercentage = tempBuilding->GetHealthPercentage();
		actorExtent = tempBuilding->GetHeight();

		FVector pos1 = Canvas->Project(FVector(center.X, center.Y - actorExtent * 2, center.Z + extent.Z));
		FVector pos2 = Canvas->Project(FVector(center.X, center.Y + actorExtent * 2, center.Z + extent.Z));

		float barWidth = (pos2 - pos1).Size2D() * 0.2;
		float barHeight = barWidth * 0.2f;

		// Draw a background color first
		/// Background tile ///
		barWidth += 2.f;
		barHeight += 2.f;

		float x = center2D.X - barWidth * 0.5f;
		float y = center2D.Y;

		FCanvasTileItem tileItem(FVector2D(x, y), FVector2D(barWidth, barHeight), FLinearColor(0.0f, 0.0f, 0.0f, 0.5f));
		tileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(tileItem);
		/// Background tile ///

		// Draw the health indicator
		/// Health tile ///
		barWidth -= 2.f;
		barHeight -= 2.f;

		x = center2D.X - barWidth * 0.5f;
		y = center2D.Y + 1.f;

		tileItem.Position = FVector2D(x, y);
		if (healthPercentage > 0.6)
			tileItem.SetColor(FLinearColor::Green);
		else if (healthPercentage > 0.3 && healthPercentage <= 0.6)
			tileItem.SetColor(FLinearColor::Yellow);
		else
			tileItem.SetColor(FLinearColor::Red);

		tileItem.Size = FVector2D(barWidth * healthPercentage, barHeight);
		Canvas->DrawItem(tileItem);
	
}


void AMyRTSHUD::SetPlayer(II_Player* owner_) {
	owner = owner_;
}