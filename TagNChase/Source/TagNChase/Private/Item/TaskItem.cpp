// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TaskItem.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Character/TaskCharacter.h"

ATaskItem::ATaskItem()
{
	PrimaryActorTick.bCanEverTick = false;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    SetRootComponent(Collision);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(Collision);

    Collision->OnComponentBeginOverlap.AddDynamic(this, &ATaskItem::OnItemOverlap);
}

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

void ATaskItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->IsA(ATaskCharacter::StaticClass()))
    {
        // 효과 적용
        UTNStatusComponent* StatusComp = OtherActor->GetComponentByClass<UTNStatusComponent>();
        if (IsValid(StatusComp) == false)
            return;

        switch (StatusComp->GetRole())
        {
        case ERoleType::Police:
            StatusComp->SetCurrentHP(StatusComp->GetCurrentHP() + 1.0);
            break;
        case ERoleType::Thief:
            StatusComp->ApplyProtect();
            break;
        default:
            break;
        }

        // 효과 적용 후 파괴하기
        Destroy();
    }
}