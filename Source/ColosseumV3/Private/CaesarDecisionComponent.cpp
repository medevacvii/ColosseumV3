// Fill out your copyright notice in the Description page of Project Settings.



#include "CaesarDecisionComponent.h"

UCaesarDecisionComponent::UCaesarDecisionComponent()
{
    // Caesar evaluates events, not every frame.
    PrimaryComponentTick.bCanEverTick = false;
}

ECaesarReaction UCaesarDecisionComponent::EvaluateArenaEvent(
    ECaesarArenaEvent ArenaEvent)
{
    // Evaluate the event.
    switch (ArenaEvent)
    {
    case ECaesarArenaEvent::Victory:
        ApprovalScore += 25.0f;
        break;

    case ECaesarArenaEvent::Cowardice:
        ApprovalScore -= 20.0f;
        break;

    case ECaesarArenaEvent::Mercy:
        ApprovalScore += 10.0f;
        break;
    }

    // Keep approval between 0 and 100.
    ApprovalScore = FMath::Clamp(ApprovalScore, 0.0f, 100.0f);

    // Determine Caesar's reaction.
    if (ApprovalScore >= 70.0f)
    {
        LastReaction = ECaesarReaction::Approve;
    }
    else if (ApprovalScore <= 30.0f)
    {
        LastReaction = ECaesarReaction::Disapprove;
    }
    else
    {
        LastReaction = ECaesarReaction::Neutral;
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Caesar AI: Approval = %.1f | Reaction = %d"),
        ApprovalScore,
        static_cast<int32>(LastReaction));

    return LastReaction;
}


