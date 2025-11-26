// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TaskBox.h"
#include "TagNChase.h"
#include "Net/UnrealNetwork.h"

ATaskBox::ATaskBox()
	:ServerRotationYaw(0.0f)
	, RotationSpeed(30.0f)
	, AccDeltaSecondSinceReplicated(0.0f)
	, NetCullDistance(1000.f)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	const static float BoxActorNetUpdateFrequency = 1.f;
	SetNetUpdateFrequency(BoxActorNetUpdateFrequency);
	// 1초에 1번씩 액터 레플리케이션 시도. 즉, 서버 성능이 아무리 좋아도 1초에 1번씩만 레플리케이션함.
	NetUpdatePeriod = 1 / GetNetUpdateFrequency();

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneRoot);
	Mesh->SetRelativeLocation(FVector(-50.f, -50.f, 50.f));

	SetNetCullDistanceSquared(NetCullDistance * NetCullDistance);
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
	else
	{
		if (NetUpdatePeriod < KINDA_SMALL_NUMBER)
		{
			return;
		}

		AccDeltaSecondSinceReplicated += DeltaSeconds;
		const float LerpRatio = FMath::Clamp(AccDeltaSecondSinceReplicated / NetUpdatePeriod, 0.f, 1.f);

		const float NextServerRotationYaw = ServerRotationYaw + RotationSpeed * NetUpdatePeriod;

		const float EstimatedClientRotationYaw = FMath::Lerp(ServerRotationYaw, NextServerRotationYaw, LerpRatio);
		SetActorRotation(FRotator(0.f, EstimatedClientRotationYaw, 0.f));
	}

	// NetCullDistanceSquared를 시각화 하기 위한 디버그 드로잉
	DrawDebugSphere(GetWorld(), GetActorLocation(), NetCullDistance / 2.f, 16, FColor::Green, false, -1.f);
}

bool ATaskBox::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	bool bIsNetRelevant = Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);

	if (false == bIsNetRelevant)
	{
		TN_LOG_NET(LogTNNet, Log, TEXT("%s is not relevant for(%s, %s)"), *GetName(), *RealViewer->GetName(), *ViewTarget->GetName());
	}

	return bIsNetRelevant;
}

void ATaskBox::OnRep_ServerRotationYaw()
{
	TN_LOG_NET(LogTNNet, Log, TEXT("OnRep_ServerRotationYaw(): %f"), ServerRotationYaw);

	// 당장은 거의 효율 비슷 (Tick에서 매번 호출)
	// 주기 변경 or Tick에서 빼는 경우는 이쪽이 더 효과적
	SetActorRotation(FRotator(0.f, ServerRotationYaw, 0.f));

	AccDeltaSecondSinceReplicated = 0.f;
}
