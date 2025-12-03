// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TaskItem.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Character/TaskCharacter.h"
#include "Net/UnrealNetwork.h"
#include "TagNChase.h"

ATaskItem::ATaskItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetRootComponent(Collision);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(Collision);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ATaskItem::OnItemOverlap);

	bReplicates = true;
	bAlwaysRelevant = true;
}

void ATaskItem::BeginPlay()
{
	Super::BeginPlay();

	UTNStatusComponent* StatusComponent = GetPlayerStatusComponent();
	if (IsValid(StatusComponent))
	{
		if (HasAuthority())
		{
			StatusComponent->OnRoleChanged.AddUObject(this, &ThisClass::UpdateVisualForLocalViewer);
		}
		else
		{
			UpdateVisualForLocalViewer(StatusComponent->GetRole());
		}
	}

}

void ATaskItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATaskItem, ItemMesh);
}

void ATaskItem::UpdateVisualForLocalViewer(ERoleType InRole)
{
	switch (InRole)
	{
	case ERoleType::Police:
		ItemMesh = PoliceMesh;
		break;
	case ERoleType::Thief:
		ItemMesh = ThiefMesh;
		break;
	default:
		ItemMesh = nullptr;
		break;
	}

	OnRep_ItemMesh();
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
	if (HasAuthority() == false)
		return;

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

void ATaskItem::OnRep_ItemMesh()
{
	if (MeshComp)
	{
		MeshComp->SetStaticMesh(ItemMesh);
	}
}
