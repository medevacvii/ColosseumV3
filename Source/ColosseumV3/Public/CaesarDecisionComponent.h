// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CaesarDecisionComponent.generated.h"

// Events Caesar can observe.
UENUM(BlueprintType)
enum class ECaesarArenaEvent : uint8
{
    Victory,
    Cowardice,
    Mercy
};

// Decisions Caesar can make.
UENUM(BlueprintType)
enum class ECaesarReaction : uint8
{
    Neutral,
    Approve,
    Disapprove
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COLOSSEUMV3_API UCaesarDecisionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCaesarDecisionComponent();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Caesar AI",
        meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float ApprovalScore = 50.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Caesar AI")
    ECaesarReaction LastReaction = ECaesarReaction::Neutral;

    UFUNCTION(BlueprintCallable, Category = "Caesar AI")
    ECaesarReaction EvaluateArenaEvent(ECaesarArenaEvent ArenaEvent);
};