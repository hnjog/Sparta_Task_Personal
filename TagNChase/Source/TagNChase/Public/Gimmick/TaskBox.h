// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TaskBox.generated.h"


class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class TAGNCHASE_API ATaskBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ATaskBox();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

};
