// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AICharacter.h"
#include "GameMode/TaskGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include "Controller/NPCController.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

AAICharacter::AAICharacter()
{
	AIControllerClass = ANPCController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAICharacter::OnDeath()
{
    if (!HasAuthority())
        return;

    if (bIsDead)
        return;

    bIsDead = true;

    StartRagdoll();

    ATaskGameModeBase* TGM = Cast<ATaskGameModeBase>(UGameplayStatics::GetGameMode(this));
    if (IsValid(TGM))
    {
        TGM->PaneltyPolice();
    }
}

void AAICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, bIsDead);
}

void AAICharacter::StartRagdoll()
{
    USkeletalMeshComponent* MeshComp = GetMesh();
    if (!MeshComp)
        return;

    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        AICon->StopMovement();

        if (UBrainComponent* Brain = AICon->GetBrainComponent())
        {
            Brain->StopLogic(TEXT("Dead"));
        }
    }

    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->DisableMovement();
        MoveComp->StopMovementImmediately();
    }

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
    MeshComp->SetAllBodiesSimulatePhysics(true);
    MeshComp->SetSimulatePhysics(true);
    MeshComp->WakeAllRigidBodies();

    DetachFromControllerPendingDestroy();

    if (HasAuthority())
    {
        SetLifeSpan(10.0f);
        TearOff();
    }
}

void AAICharacter::OnRep_IsDead()
{
    if (bIsDead)
    {
        StartRagdoll();
    }
}
