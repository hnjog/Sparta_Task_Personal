// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TaskItem.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"

// Sets default values
ATaskItem::ATaskItem()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
}

// Called when the game starts or when spawned
void ATaskItem::BeginPlay()
{
	Super::BeginPlay();
	
    // 서버 아니라면
    if (GetNetMode() != NM_DedicatedServer)
    {
        UTNStatusComponent* StatusComponent = GetPlayerStatusComponent();

        if (IsValid(StatusComponent) == false)
        {
            StatusComponent->OnRoleChanged.AddUObject(this, &ThisClass::UpdateVisualForLocalViewer);
            UpdateVisualForLocalViewer(StatusComponent->GetRole());
        }
    }
}

void ATaskItem::UpdateVisualForLocalViewer(ERoleType InRole)
{
    UStaticMesh* NewMesh = nullptr;

    switch (InRole)
    {
    case ERoleType::Police:
        NewMesh = PoliceMesh;
        break;
    case ERoleType::Thief:
        NewMesh = ThiefMesh;
        break;
    default:
        break;
    }

    MeshComp->SetStaticMesh(NewMesh);
}

UTNStatusComponent* ATaskItem::GetPlayerStatusComponent()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (IsValid(PC) == false)
    {
        return nullptr;
    }

    ACharacter* Char = Cast<ACharacter>(PC->GetPawn());
    if (IsValid(Char) == false)
    {
        return nullptr;
    }

    UTNStatusComponent* StatusComponent = Char->GetComponentByClass<UTNStatusComponent>();

    if (IsValid(StatusComponent) == false)
    {
        return nullptr;
    }
    return StatusComponent;
}
