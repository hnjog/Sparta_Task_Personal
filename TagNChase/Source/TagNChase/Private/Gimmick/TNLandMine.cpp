// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TNLandMine.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

ATNLandMine::ATNLandMine()
	:bIsExploded(false),
	NetCullDistance(1000.f)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetRootComponent());

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BoxCollision);

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->SetupAttachment(GetRootComponent());
	Particle->SetAutoActivate(false);

	SetNetCullDistanceSquared(NetCullDistance * NetCullDistance);
}

// Called when the game starts or when spawned
void ATNLandMine::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() == true)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Run on server.")), true, true, FLinearColor::Green, 5.f);
	}
	else
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (IsValid(OwnerPawn) == true)
		{
			if (OwnerPawn->IsLocallyControlled() == true)
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Run on owning client.")), true, true, FLinearColor::Green, 5.f);
			}
			else
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Run on other client.")), true, true, FLinearColor::Green, 5.f);
			}
		}
	}

	if (false == OnActorBeginOverlap.IsAlreadyBound(this, &ThisClass::OnLandMineBeginOverlap))
	{
		OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnLandMineBeginOverlap);
	}
}

void ATNLandMine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ADXLandMine::EndPlay()")), true, true, FLinearColor::Green, 5.f);

	if (true == OnActorBeginOverlap.IsAlreadyBound(this, &ThisClass::OnLandMineBeginOverlap))
	{
		OnActorBeginOverlap.RemoveDynamic(this, &ThisClass::OnLandMineBeginOverlap);
	}
}

void ATNLandMine::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsExploded);
}

void ATNLandMine::OnLandMineBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (HasAuthority() == true)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Run on server.")), true, true, FLinearColor::Green, 5.f);

		//MulticastRPCSpawnEffect();

		if (bIsExploded == false)
		{
			bIsExploded = true;
		}
	}
	else
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (IsValid(OwnerPawn) == true)
		{
			if (OwnerPawn->IsLocallyControlled() == true)
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Run on owning client.")), true, true, FLinearColor::Green, 5.f);
			}
			else
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Run on other client.")), true, true, FLinearColor::Green, 5.f);
			}
		}

		if (bIsExploded == false)
		{
			Particle->Activate(true);
		}
	}
}

void ATNLandMine::OnRep_IsExploded()
{
	if (IsValid(ExplodedMaterial) == true)
	{
		Mesh->SetMaterial(0, ExplodedMaterial);
	}
}

void ATNLandMine::MulticastRPCSpawnEffect_Implementation()
{
	
}
