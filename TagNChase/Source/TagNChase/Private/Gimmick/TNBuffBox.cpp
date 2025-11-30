// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TNBuffBox.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Character/TaskCharacter.h"

// Sets default values
ATNBuffBox::ATNBuffBox()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(TriggerBox);
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));
	ParticleEffect->SetupAttachment(TriggerBox);
	ParticleEffect->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ATNBuffBox::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
}

void ATNBuffBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ParticleEffect->Activate(true);
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	ParticleEffect->OnSystemFinished.AddDynamic(this, &ThisClass::OnEffectFinished);

	ATaskCharacter* OverlappingCharacter = Cast<ATaskCharacter>(OtherActor);
	if (IsValid(OverlappingCharacter) == true)
	{
		OverlappingCharacter->TakeBuff(50.f);
	}
}

void ATNBuffBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	if (HasAuthority() == true)
	{
		Destroy();
	}
}
