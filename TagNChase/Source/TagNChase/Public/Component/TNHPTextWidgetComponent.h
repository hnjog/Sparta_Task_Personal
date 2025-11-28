// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TNHPTextWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class TAGNCHASE_API UTNHPTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void InitWidget() override;
};
