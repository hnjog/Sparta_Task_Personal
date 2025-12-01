// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/TaskGameStateBase.h"

#include "Net/UnrealNetwork.h"

void ATaskGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, AlivePlayerControllerCount);
	DOREPLIFETIME(ThisClass, MatchState);
	DOREPLIFETIME(ThisClass, TimeLimit);
}
