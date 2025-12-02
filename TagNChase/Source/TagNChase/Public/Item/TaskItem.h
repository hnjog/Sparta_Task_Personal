// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Component/TNStatusComponent.h"
#include "TaskItem.generated.h"

UCLASS()
class TAGNCHASE_API ATaskItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATaskItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    void UpdateVisualForLocalViewer(ERoleType InRole);

    class UTNStatusComponent* GetPlayerStatusComponent();

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> MeshComp;

    UPROPERTY(EditDefaultsOnly, Category = "Item|Visual")
    TObjectPtr<UStaticMesh> PoliceMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Item|Visual")
    TObjectPtr<UStaticMesh> ThiefMesh;

};
