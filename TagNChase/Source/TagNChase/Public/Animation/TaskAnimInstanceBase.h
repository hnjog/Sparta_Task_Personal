// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/BaseAnimInst.h"
#include "TaskAnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class TAGNCHASE_API UTaskAnimInstanceBase : public UBaseAnimInst
{
	GENERATED_BODY()

public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UFUNCTION()
	void AnimNotify_CheckMeleeAttackHit();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AimPitch;
};
