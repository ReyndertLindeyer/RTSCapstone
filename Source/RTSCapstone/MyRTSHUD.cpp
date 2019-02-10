// Fill out your copyright notice in the Description page of Project Settings.

#include "MyRTSHUD.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"

AMyRTSHUD::AMyRTSHUD() {
	selectionStart = false;
	grabEverything = false;
	isShift = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> MyUI(TEXT("/Game/Game_Assets/Widgets/UIWidget"));
	HUDWidgetClass = MyUI.Class;
}

void AMyRTSHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AMyRTSHUD::DrawHUD() {
	Super::DrawHUD(); 
	DrawHealthBars();
	if (selectionStart) {
		if (!isShift) {
			//See if there are any units already selected
			if (foundUnits.Num() > 0) {
				for (int i = 0; i < foundUnits.Num(); i++) {
					//Code to remove the selection from existing actors
				}
			}
			//Empty the array
			foundUnits.Empty();
			//See if there are any structures already selected
			if (foundBuildings.Num() > 0) {
				for (int i = 0; i < foundBuildings.Num(); i++) {
					//Code to remove the selection from existing actors
				}
			}
			//Empty the array
			foundBuildings.Empty();
		}

		//Get the position of the mouse and use it and the starting position to draw a rectangle to denote the selection area
		mousePos = GetMousePos();
		DrawRect(FLinearColor(0, 0, 1, 0.15f), startPos.X, startPos.Y, mousePos.X - startPos.X, mousePos.Y - startPos.Y);
	}
	else if (!selectionStart && grabEverything) {
		//Select the actors themselves
		GetActorsInSelectionRectangle<AMasterUnit>(startPos, mousePos, foundUnits, false, false);
		if (foundUnits.Num() > 0) {
			for (int i = 0; i < foundUnits.Num(); i++) {
				//Code to show that the actors are selected
				foundUnits[i]->selected = true;
			}
		}
		GetActorsInSelectionRectangle<ABuildingMaster>(startPos, mousePos, foundBuildings, false, false);
		if (foundBuildings.Num() > 0) {
			for (int i = 0; i < foundBuildings.Num(); i++) {
				//Code to show that the actors are selected
			}
		}
		grabEverything = false;
	}
}

//Get the location of the mouse and then return it
FVector2D AMyRTSHUD::GetMousePos() {

	float posX = 1.0f, posY = 1.0f;

	GetOwningPlayerController()->GetMousePosition(posX, posY);

	return FVector2D(posX, posY);
}

void AMyRTSHUD::UseWidget() {
	if (HUDWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget) {

			CurrentWidget->AddToViewport();

		}

	}
}

void AMyRTSHUD::AddBuilding(ABuildingMaster * building)
{
	existingBuildings.Add(building);
}

void AMyRTSHUD::RemoveBuilding(ABuildingMaster * building)
{
	existingBuildings.Remove(building);
}

void AMyRTSHUD::DrawHealthBars()
{
	for (int32 i = 0; i < existingBuildings.Num(); i++) {
		// Select the center point of the bar as the character's location
		FVector center = existingBuildings[i]->GetActorLocation();
		// Offsets of the bar
		FVector extent = FVector(60.f, 34.f, 50.75f);

		// Project function of Canvas translates a world position to the screen position
		FVector2D center2D = FVector2D(Canvas->Project(FVector(center.X, center.Y, center.Z + extent.Z)));

		float actorExtent = 50.f;
		float healthPercentage = 0.5f;
		float yOffset = 10.f;

		healthPercentage = existingBuildings[i]->GetHealth() / existingBuildings[i]->GetMaxHealth();
		actorExtent = existingBuildings[i]->GetHeight();

		FVector pos1 = Canvas->Project(FVector(center.X, center.Y - actorExtent * 2, center.Z + extent.Z));
		FVector pos2 = Canvas->Project(FVector(center.X, center.Y + actorExtent * 2, center.Z + extent.Z));

		float barWidth = (pos2 - pos1).Size2D() * 0.2;
		float barHeight = barWidth * 0.2f;

		// Draw a background color first
		/* Background tile */
		barWidth += 2.f;
		barHeight += 2.f;

		float x = center2D.X - barWidth * 0.5f;
		float y = center2D.Y;

		FCanvasTileItem tileItem(FVector2D(x, y), FVector2D(barWidth, barHeight), FLinearColor(0.0f, 0.0f, 0.0f, 0.5f));
		tileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(tileItem);
		/* Background tile */

		// Draw the health indicator
		/* Health tile */
		barWidth -= 2.f;
		barHeight -= 2.f;

		x = center2D.X - barWidth * 0.5f;
		y = center2D.Y + 1.f;

		tileItem.Position = FVector2D(x, y);
		if(healthPercentage > 0.61)
			tileItem.SetColor(FLinearColor::Green);
		else if(healthPercentage > 0.3 && healthPercentage < 0.6)
			tileItem.SetColor(FLinearColor::Yellow);
		else
			tileItem.SetColor(FLinearColor::Red);

		tileItem.Size = FVector2D(barWidth * healthPercentage, barHeight);
		Canvas->DrawItem(tileItem);
	}
}
