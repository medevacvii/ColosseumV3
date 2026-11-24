#include "Combat/GladiatorCombatComponent.h"

// Sets default values
UGladiatorCombatComponent::UGladiatorCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UGladiatorCombatComponent::BeginPlay()
{
    Super::BeginPlay();
}