// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TaskCharacter.h"

// Sets default values
ATaskCharacter::ATaskCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATaskCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATaskCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATaskCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

