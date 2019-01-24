// Fill out your copyright notice in the Description page of Project Settings.

#include "MyRTSHUD.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"

void AMyRTSHUD::BeginPlay()
{
	Super::BeginPlay(); 
	selectionStart = false;
	grabEverything = false;
	isShift = false;
}

void AMyRTSHUD::DrawHUD() {
	Super::DrawHUD();
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