// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GladiatorCombatComponent.generated.h"

UCLASS(ClassGroup = (Combat), meta = (BlueprintSpawnableComponent))
class COLOSSEUMV3_API UGladiatorCombatComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UGladiatorCombatComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};