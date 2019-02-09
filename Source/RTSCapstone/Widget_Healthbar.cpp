// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_Healthbar.h"

void UWidget_Healthbar::NativeConstruct()
{
	// Call the Blueprint "Event Construct" node
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("I happen"));
	// HealthBar can be nullptr if we haven't created it in the Blueprint subclass
	if (HealthBar)
	{
		UE_LOG(LogTemp, Warning, TEXT("Me too"));
		HealthBar->SetBrushFromMaterial(ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/RoboBars/Materials/MI_UmgDivisionWinXP")).Get());
	}
}