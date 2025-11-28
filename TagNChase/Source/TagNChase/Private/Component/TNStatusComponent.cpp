// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TNStatusComponent.h"

// Sets default values for this component's properties
UTNStatusComponent::UTNStatusComponent()
	: CurrentHP(100.f)
	, MaxHP(100.f)
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UTNStatusComponent::ApplyDamage(float InDamage)
{
	const float PreviousHP = CurrentHP;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, PreviousHP);

	SetCurrentHP(PreviousHP - ActualDamage);

	return ActualDamage;
}

void UTNStatusComponent::SetCurrentHP(float InCurrentHP)
{
	CurrentHP = InCurrentHP;
	if (CurrentHP <= KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.f;
		OnOutOfCurrentHP.Broadcast();
	}
	OnCurrentHPChanged.Broadcast(CurrentHP);
}

void UTNStatusComponent::SetMaxHP(float InMaxHP)
{
	MaxHP = InMaxHP;

	if (MaxHP < KINDA_SMALL_NUMBER)
	{
		MaxHP = 0.1f;
	}

	OnMaxHPChanged.Broadcast(MaxHP);
}
