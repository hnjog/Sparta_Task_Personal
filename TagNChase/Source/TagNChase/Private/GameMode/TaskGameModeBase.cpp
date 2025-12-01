// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/TaskGameModeBase.h"

#include "Controller/TaskPlayerController.h"
#include "GameState/TaskGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "TagNChase.h"

void ATaskGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ATaskGameStateBase* TaskGameState = GetGameState<ATaskGameStateBase>();
	if (IsValid(TaskGameState) == false)
	{
		return;
	}

	if (TaskGameState->MatchState != EMatchState::Waiting)
	{
		NewPlayer->SetLifeSpan(0.1f);
		return;
	}

	ATaskPlayerController* NewPlayerController = Cast<ATaskPlayerController>(NewPlayer);
	if (IsValid(NewPlayerController) == true)
	{
		AlivePlayerControllers.Add(NewPlayerController);
		NewPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
	}

}

void ATaskGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ATaskPlayerController* ExitingPlayerController = Cast<ATaskPlayerController>(Exiting);
	if (IsValid(ExitingPlayerController) == true && AlivePlayerControllers.Find(ExitingPlayerController) != INDEX_NONE)
	{
		AlivePlayerControllers.Remove(ExitingPlayerController);
		DeadPlayerControllers.Add(ExitingPlayerController);
	}
}

void ATaskGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &ThisClass::OnMainTimerElapsed, 1.f, true);
	RemainWaitingTimeForPlaying = WaitingTime;
	RemainWaitingTimeForEnding = EndingTime;
}

void ATaskGameModeBase::OnCharacterDead(ATaskPlayerController* InController)
{
	if (IsValid(InController) == false || AlivePlayerControllers.Find(InController) == INDEX_NONE)
	{
		return;
	}

	InController->ClientRPCShowGameResultWidget(AlivePlayerControllers.Num());
	AlivePlayerControllers.Remove(InController);
	DeadPlayerControllers.Add(InController);
}

void ATaskGameModeBase::PaneltyPolice()
{
	if (PoliceIndex >= AlivePlayerControllers.Num())
	{
		return;
	}

	if (IsValid(AlivePlayerControllers[PoliceIndex]) == false)
	{
		return;
	}

	AlivePlayerControllers[PoliceIndex]->PaneltyToPolice();
}

void ATaskGameModeBase::OnMainTimerElapsed()
{
	ATaskGameStateBase* TaskGameState = GetGameState<ATaskGameStateBase>();
	if (IsValid(TaskGameState) == false)
	{
		return;
	}

	switch (TaskGameState->MatchState)
	{
	case EMatchState::None:
		break;
	case EMatchState::Waiting:
	{
		FString NotificationString = FString::Printf(TEXT(""));

		if (AlivePlayerControllers.Num() < MinimumPlayerCountForPlaying)
		{
			NotificationString = FString::Printf(TEXT("Wait another players for playing."));

			RemainWaitingTimeForPlaying = WaitingTime; // 최소인원이 안된다면 대기 시간 초기화.
		}
		else
		{
			NotificationString = FString::Printf(TEXT("Wait %d seconds for playing."), RemainWaitingTimeForPlaying);

			--RemainWaitingTimeForPlaying;
		}

		if (RemainWaitingTimeForPlaying <= 0)
		{
			NotificationString = FString::Printf(TEXT(""));

			TaskGameState->MatchState = EMatchState::Playing;
			TaskGameState->StartTime();

			InitMatch();
		}

		NotifyToAllPlayer(NotificationString);

		break;
	}
	case EMatchState::Playing:
	{
		TaskGameState->AlivePlayerControllerCount = AlivePlayerControllers.Num();

		FString NotificationString = FString::Printf(TEXT("%d / %d"), TaskGameState->AlivePlayerControllerCount, TaskGameState->AlivePlayerControllerCount + DeadPlayerControllers.Num());

		NotifyToAllPlayer(NotificationString);

		if (TaskGameState->AlivePlayerControllerCount <= 1)
		{
			TaskGameState->MatchState = EMatchState::Ending;
			AlivePlayerControllers[0]->ClientRPCShowGameResultWidget(1);
		}
	}
	break;
	case EMatchState::Ending:
	{
		FString NotificationString = FString::Printf(TEXT("Waiting %d for returning to title."), RemainWaitingTimeForEnding);

		NotifyToAllPlayer(NotificationString);

		--RemainWaitingTimeForEnding;

		if (RemainWaitingTimeForEnding <= 0)
		{
			for (auto AliveController : AlivePlayerControllers)
			{
				AliveController->ClientRPCReturnToTitle();
			}
			for (auto DeadController : DeadPlayerControllers)
			{
				DeadController->ClientRPCReturnToTitle();
			}

			MainTimerHandle.Invalidate();
			GetWorldTimerManager().ClearTimer(MatchTimerHandle);
			MatchTimerHandle.Invalidate();
			FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
			UGameplayStatics::OpenLevel(this, CurrentLevelName, true, FString(TEXT("listen")));

			return;
		}
	}
	break;
	case EMatchState::End:
		break;
	default:
		break;

	}
}

void ATaskGameModeBase::NotifyToAllPlayer(const FString& NotificationString)
{
	for (auto AlivePlayerController : AlivePlayerControllers)
	{
		AlivePlayerController->NotificationText = FText::FromString(NotificationString);
	}

	for (auto DeadPlayerController : DeadPlayerControllers)
	{
		DeadPlayerController->NotificationText = FText::FromString(NotificationString);
	}
}

void ATaskGameModeBase::InitMatch()
{
	AlivePlayerControllers.RemoveAll([](ATaskPlayerController* PC)
		{
			return PC == nullptr;
		});

	if (AlivePlayerControllers.Num() != 2)
	{
		TN_LOG_NET(LogTNNet, Warning, TEXT("InitMatch: Player count is not 2 (Current: %d)"), AlivePlayerControllers.Num());
		return;
	}

	PoliceIndex = FMath::RandRange(0, 1);
	const int32 ThiefIndex = 1 - PoliceIndex;

	ATaskPlayerController* PolicePC = AlivePlayerControllers[PoliceIndex];
	ATaskPlayerController* ThiefPC = AlivePlayerControllers[ThiefIndex];

	AssignRoleToController(PolicePC, ERoleType::Police);
	AssignRoleToController(ThiefPC, ERoleType::Thief);

	TN_LOG_NET(LogTNNet, Log, TEXT("InitMatch: Police = %s, Thief = %s"),
		*GetNameSafe(PolicePC),
		*GetNameSafe(ThiefPC));

	GetWorldTimerManager().SetTimer(
		MatchTimerHandle,
		FTimerDelegate::CreateLambda([this]()
			{
				if (!IsValid(this))
					return;

				ATaskGameStateBase* TGS = GetGameState<ATaskGameStateBase>();

				if (!IsValid(TGS) ||
					TGS->MatchState != EMatchState::Playing)
				{
					GetWorldTimerManager().ClearTimer(MatchTimerHandle);
					return;
				}

				TGS->TimeRun();

				TN_LOG_NET(LogTNNet, Log, TEXT("Match Time : %d"), TGS->GetMatchTime());
				if (TGS->IsTimeOut())
				{
					MatchTimeOut();
					GetWorldTimerManager().ClearTimer(MatchTimerHandle);
				}
			}),
		1.f,
		true
	);
}


void ATaskGameModeBase::AssignRoleToController(ATaskPlayerController* PC, ERoleType NewRole)
{
	if (PC == nullptr)
	{
		return;
	}

	if (APawn* Pawn = PC->GetPawn())
	{
		if (UTNStatusComponent* Status = Pawn->FindComponentByClass<UTNStatusComponent>())
		{
			Status->SetRole(NewRole);
		}
	}
}

void ATaskGameModeBase::MatchTimeOut()
{
	// 지금은 PoliceIndex 를 저장하지만, 나중에는 플레이어 컨트롤러로 체크하는게 더 좋을듯
	if (PoliceIndex >= AlivePlayerControllers.Num())
	{
		return;
	}

	if (IsValid(AlivePlayerControllers[PoliceIndex]) == false)
	{
		return;
	}

	AlivePlayerControllers[PoliceIndex]->OnCharacterDead();
}
