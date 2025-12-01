// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/TaskPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "GameMode/TaskGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UW_GameResult.h"
#include "Components/TextBlock.h"
#include "Component/TNStatusComponent.h"
#include "Character/TaskCharacter.h"

void ATaskPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);

	if (IsValid(NotificationTextUIClass) == true)
	{
		UUserWidget* NotificationTextUI = CreateWidget<UUserWidget>(this, NotificationTextUIClass);
		if (IsValid(NotificationTextUI) == true)
		{
			NotificationTextUI->AddToViewport(1);

			NotificationTextUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ATaskPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void ATaskPlayerController::OnCharacterDead()
{
	ATaskGameModeBase* GameMode = Cast<ATaskGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (HasAuthority() == true && IsValid(GameMode) == true)
	{
		GameMode->OnCharacterDead(this);
	}
}

UTNStatusComponent* ATaskPlayerController::GetStatusComponent() const
{
	ATaskCharacter* TC = Cast<ATaskCharacter>(GetPawn());
	if (IsValid(TC) == false)
		return nullptr;

	return TC->GetStatus();
}

void ATaskPlayerController::ClientRPCReturnToTitle_Implementation()
{
	if (IsLocalController() == true)
	{
		// 서버의 레벨이 변경되는걸 원치 않음. 클라이언트가 이동해야하므로 if() 처리.
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Title")), true);
	}
}

void ATaskPlayerController::ClientRPCShowGameResultWidget_Implementation(int32 InRanking)
{
	if (IsLocalController() == true)
	{
		if (IsValid(GameResultUIClass) == true)
		{
			UUW_GameResult* GameResultUI = CreateWidget<UUW_GameResult>(this, GameResultUIClass);
			if (IsValid(GameResultUI) == true)
			{
				GameResultUI->AddToViewport(3);

				FString GameResultString = FString::Printf(TEXT("%s"), InRanking == 1 ? TEXT("Winner Winner!") : TEXT("Loose..."));
				GameResultUI->ResultText->SetText(FText::FromString(GameResultString));

				FString RankingString = FString::Printf(TEXT("#%02d"), InRanking);

				UTNStatusComponent* StatusComp = GetStatusComponent();
				if (IsValid(StatusComp))
				{
					RankingString += FString::Printf(TEXT(" : %s"), StatusComp->GetRole() == ERoleType::Police ? TEXT("Police") : TEXT("Thief "));
				}

				GameResultUI->RankingText->SetText(FText::FromString(RankingString));

				FInputModeUIOnly Mode;
				Mode.SetWidgetToFocus(GameResultUI->GetCachedWidget());
				SetInputMode(Mode);

				bShowMouseCursor = true;
			}
		}
	}
}
