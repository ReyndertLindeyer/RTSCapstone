// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Widget_Healthbar.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API UWidget_Healthbar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage* HealthBar = nullptr;
};
