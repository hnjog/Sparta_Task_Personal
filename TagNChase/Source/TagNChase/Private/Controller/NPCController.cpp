// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/NPCController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


void ANPCController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (NpcBBData)
    {
        UBlackboardComponent* RawBBComp = NPCBBComp.Get();

        if (UseBlackboard(NpcBBData.Get(), RawBBComp))
        {
            NPCBBComp = RawBBComp;
        }
    }

    if (NpcBT)
    {
        RunBehaviorTree(NpcBT);
    }
}
