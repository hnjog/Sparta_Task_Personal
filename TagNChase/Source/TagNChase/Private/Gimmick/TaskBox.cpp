// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TaskBox.h"
#include "TagNChase.h"

ATaskBox::ATaskBox()
{
	PrimaryActorTick.bCanEverTick = false;

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
