// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TaskBox.h"
#include "TagNChase.h"
#include "Net/UnrealNetwork.h"

ATaskBox::ATaskBox()
	:ServerRotationYaw(0.0f)
	, RotationSpeed(30.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneRoot);
	Mesh->SetRelativeLocation(FVector(-50.f, -50.f, 50.f));
}

void ATaskBox::BeginPlay()
{
	Super::BeginPlay();

	TN_LOG_ROLE(LogTNNet, Log, TEXT(""));
}

void ATaskBox::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ServerRotationYaw);
}

void ATaskBox::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority() == true)
	{
		AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaSeconds, 0.f));
		ServerRotationYaw = RootComponent->GetComponentRotation().Yaw;
	}
}

void ATaskBox::OnRep_ServerRotationYaw()
{
	TN_LOG_NET(LogTNNet, Log, TEXT("OnRep_ServerRotationYaw(): %f"), ServerRotationYaw);

	// 당장은 거의 효율 비슷 (Tick에서 매번 호출)
	// 주기 변경 or Tick에서 빼는 경우는 이쪽이 더 효과적
	SetActorRotation(FRotator(0.f, ServerRotationYaw, 0.f));
}
