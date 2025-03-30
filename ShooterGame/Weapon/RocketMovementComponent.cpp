// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketMovementComponent.h"


URocketMovementComponent::EHandleBlockingHitResult URocketMovementComponent::HandleBlockingHit(const FHitResult &Hit, float TimeTick, const FVector &MoveDelta, float &SubtickTimeRemaining) 
{
    Super::HandleBlockingHit(Hit, TimeTick, MoveDelta, SubtickTimeRemaining);
    return EHandleBlockingHitResult::AdvanceNextSubstep;
}

void URocketMovementComponent::HandleImpact(const FHitResult &Hit, float TimeSlice, const FVector &MoveDelta) 
{
    // Rockets should not stop; only explode when their collision box detects a hit.
}
