// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TaskGameStateBase.generated.h"

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	None,
	Waiting,
	Playing,
	Ending,
	End
};

/**
 * 
 */
UCLASS()
class TAGNCHASE_API ATaskGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void StartTime() { TimeLimit = BaseMatchTime; }
	void TimeRun() { TimeLimit--; }
	bool IsTimeOut() { return TimeLimit <= 0; }
	FORCEINLINE int32 GetMatchTime() const { return TimeLimit; }

public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 AlivePlayerControllerCount = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 TimeLimit = 0;

	UPROPERTY(EditAnywhere)
	int32 BaseMatchTime = 10;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	EMatchState MatchState = EMatchState::Waiting;
};
