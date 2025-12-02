// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AICharacter.h"
#include "GameMode/TaskGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include "Controller/NPCController.h"

AAICharacter::AAICharacter()
{
	AIControllerClass = ANPCController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAICharacter::OnDeath()
{
	ATaskGameModeBase* GameMode = Cast<ATaskGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (HasAuthority() == true && IsValid(GameMode) == true)
	{
		GameMode->PaneltyPolice();
	}
}
