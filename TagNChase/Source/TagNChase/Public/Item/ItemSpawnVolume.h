// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnVolume.generated.h"

class ATaskItem;

UCLASS()
class TAGNCHASE_API AItemSpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	AItemSpawnVolume();

public:
	FVector GetRandomPointInVolume() const;

	FORCEINLINE UClass* GetItemClass() { return ItemClass; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TObjectPtr<class UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TSubclassOf<ATaskItem> ItemClass;
};
