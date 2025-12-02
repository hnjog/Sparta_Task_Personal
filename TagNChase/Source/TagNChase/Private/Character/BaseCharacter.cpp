// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "Component/TNStatusComponent.h"
#include "GameState/TaskGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	StatusComponent = CreateDefaultSubobject<UTNStatusComponent>(TEXT("StatusComponent"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	StatusComponent->OnOutOfCurrentHP.AddUObject(this, &ThisClass::OnDeath);
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("TakeDamage: %f"), DamageAmount), true, true, FLinearColor::Red, 5.f);

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ATaskGameStateBase* GameState = Cast<ATaskGameStateBase>(UGameplayStatics::GetGameState(this));
	if (IsValid(GameState) == true &&
		GameState->MatchState == EMatchState::Playing)
	{
		StatusComponent->ApplyDamage(ActualDamage);
	}
	return ActualDamage;
}
