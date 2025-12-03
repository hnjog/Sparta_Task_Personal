// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Component/TNStatusComponent.h"
#include "TaskItem.generated.h"

class USphereComponent;

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void UpdateVisualForLocalViewer(ERoleType InRole);

    class UTNStatusComponent* GetPlayerStatusComponent();

	UFUNCTION()
	void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnRep_ItemMesh();

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    TObjectPtr<USphereComponent> Collision;

    UPROPERTY(EditDefaultsOnly, Category = "Item|Visual")
    TObjectPtr<UStaticMesh> PoliceMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Item|Visual")
    TObjectPtr<UStaticMesh> ThiefMesh;

	UPROPERTY(ReplicatedUsing = OnRep_ItemMesh)
	TObjectPtr<UStaticMesh> ItemMesh;
};
