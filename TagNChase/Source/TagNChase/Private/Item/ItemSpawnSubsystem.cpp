// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemSpawnSubsystem.h"
#include "Item/ItemSpawnVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Item/TaskItem.h"

void UItemSpawnSubsystem::RegisterVolume(AItemSpawnVolume* Volume)
{
	if (IsValid(Volume) == false)
		return;

	SpawnVolumes.AddUnique(Volume);
}

void UItemSpawnSubsystem::UnregisterVolume(AItemSpawnVolume* Volume)
{
	SpawnVolumes.Remove(Volume);
}

void UItemSpawnSubsystem::SpawnItemInRandomVolume()
{
    if (SpawnVolumes.Num() <= 0)
        return;

    int32 Index = FMath::RandRange(0, SpawnVolumes.Num() - 1);

    if (AItemSpawnVolume* Volume = SpawnVolumes[Index].Get())
    {
        const FVector SpawnLocation = Volume->GetRandomPointInVolume();
        FTransform Transform(FRotator::ZeroRotator, SpawnLocation);
        UClass* ItemClass = Volume->GetItemClass();

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        if (UWorld* WorldNow = GetWorld())
        {
            WorldNow->SpawnActor<ATaskItem>(ItemClass, Transform, Params);
        }
    }
}
