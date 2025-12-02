// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class TAGNCHASE_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AAICharacter();

public:
	virtual void OnDeath() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void StartRagdoll();    // 실제로 래그돌 세팅하는 함수

protected:
	UPROPERTY(ReplicatedUsing = OnRep_IsDead)
	uint8 bIsDead : 1;

	UFUNCTION()
	void OnRep_IsDead();

};
