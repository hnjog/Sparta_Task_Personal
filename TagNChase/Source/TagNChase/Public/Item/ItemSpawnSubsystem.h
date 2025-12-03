// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ItemSpawnSubsystem.generated.h"

class AItemSpawnVolume;

/**
 * 
 */
UCLASS()
class TAGNCHASE_API UItemSpawnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
    void RegisterVolume(AItemSpawnVolume* Volume);
    void UnregisterVolume(AItemSpawnVolume* Volume);

    void SpawnItemInRandomVolume();

private:
    UPROPERTY()
    TArray<TWeakObjectPtr<AItemSpawnVolume>> SpawnVolumes;
};
