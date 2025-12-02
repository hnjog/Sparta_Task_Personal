// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AICharacter.h"
#include "GameMode/TaskGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include "Controller/NPCController.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

AAICharacter::AAICharacter()
{
	AIControllerClass = ANPCController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAICharacter::OnDeath()
{
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
    }

    USkeletalMeshComponent* MeshComp = GetMesh();
    if (MeshComp)
    {
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
        MeshComp->SetSimulatePhysics(true);
        MeshComp->SetAllBodiesSimulatePhysics(true);
        MeshComp->WakeAllRigidBodies();
    }

    DetachFromControllerPendingDestroy();

    SetLifeSpan(10.0f);

	ATaskGameModeBase* GameMode = Cast<ATaskGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (HasAuthority() == true && IsValid(GameMode) == true)
	{
		GameMode->PaneltyPolice();
	}
}
