// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TaskAnimInstanceBase.h"

#include "Character/TaskCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTaskAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ATaskCharacter* TaskOwnerChar = Cast<ATaskCharacter>(OwnerCharacter);
	if (IsValid(TaskOwnerChar))
	{
		AimPitch = TaskOwnerChar->GetCurrentAimPitch();
	}
}

void UTaskAnimInstanceBase::AnimNotify_CheckMeleeAttackHit()
{
	ATaskCharacter* TaskOwnerChar = Cast<ATaskCharacter>(OwnerCharacter);
	if (IsValid(TaskOwnerChar))
	{
		TaskOwnerChar->CheckMeleeAttackHit();
	}
}