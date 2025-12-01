// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Component/TNStatusComponent.h"
#include "TaskGameModeBase.generated.h"

class ATaskPlayerController;

/**
 * 
 */
UCLASS()
class TAGNCHASE_API ATaskGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void BeginPlay() override;

public:
	void OnCharacterDead(ATaskPlayerController* InController);

private:
	UFUNCTION()
	void OnMainTimerElapsed();

	void NotifyToAllPlayer(const FString& NotificationString);

	void InitMatch();

	void AssignRoleToController(ATaskPlayerController* PC, ERoleType NewRole);

	void MatchTimeOut();

public:
	FTimerHandle MainTimerHandle;
	FTimerHandle MatchTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 WaitingTime = 15;

	int32 RemainWaitingTimeForPlaying = 15;

	int32 MinimumPlayerCountForPlaying = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 EndingTime = 15;

	int32 RemainWaitingTimeForEnding = 15;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<ATaskPlayerController>> AlivePlayerControllers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<ATaskPlayerController>> DeadPlayerControllers;

	int32 PoliceIndex;
};
