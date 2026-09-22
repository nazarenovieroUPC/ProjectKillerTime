// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectKillerTimeGameMode.h"

#include "ProjectKillerTimePlayerState.h"

AProjectKillerTimeGameMode::AProjectKillerTimeGameMode()
{
	bDelayedStart = true;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Match Started!"));
}

void AProjectKillerTimeGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(MatchStartDelayTimerHandle, this, &AProjectKillerTimeGameMode::StartMatch,
	                                MatchStartDelay, false);
}

void AProjectKillerTimeGameMode::StartMatch()
{
}

void AProjectKillerTimeGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
