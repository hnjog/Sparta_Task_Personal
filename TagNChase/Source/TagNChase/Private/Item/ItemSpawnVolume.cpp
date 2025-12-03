// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemSpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Item/ItemSpawnSubsystem.h"
#include "Item/TaskItem.h"
#include "TagNChase.h"

AItemSpawnVolume::AItemSpawnVolume()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FVector AItemSpawnVolume::GetRandomPointInVolume() const
{
	const FVector Origin = BoxComponent->GetComponentLocation();
	const FVector Extent = BoxComponent->GetScaledBoxExtent();

	FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		FNavLocation NavLoc;
		if (NavSys->ProjectPointToNavigation(RandomPoint, NavLoc, Extent))
		{
			return NavLoc.Location;
		}
	}

	return RandomPoint;
}

void AItemSpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (UItemSpawnSubsystem* Subsys = World->GetSubsystem<UItemSpawnSubsystem>())
		{
			Subsys->RegisterVolume(this);
			TN_LOG_NET(LogTNNet, Log, TEXT("Register Volume!"));
		}
	}
}

void AItemSpawnVolume::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		if (UItemSpawnSubsystem* Subsys = World->GetSubsystem<UItemSpawnSubsystem>())
		{
			Subsys->UnregisterVolume(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}

