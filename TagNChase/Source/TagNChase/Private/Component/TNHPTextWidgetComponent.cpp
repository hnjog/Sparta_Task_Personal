// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TNHPTextWidgetComponent.h"

#include "UI/UW_HPText.h"

void UTNHPTextWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UUW_HPText* HPText = Cast<UUW_HPText>(GetWidget());
	if (IsValid(HPText) == true)
	{
		HPText->SetOwningActor(GetOwner());
	}
}