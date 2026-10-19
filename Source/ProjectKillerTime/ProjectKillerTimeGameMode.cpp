// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectKillerTimeGameMode.h"

#include "ProjectKillerTimePlayerState.h"

AProjectKillerTimeGameMode::AProjectKillerTimeGameMode()
{
	bDelayedStart = false;
	
}

void AProjectKillerTimeGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(MatchStartDelayTimerHandle, this, &AProjectKillerTimeGameMode::StartMatch,
	                                MatchStartDelay, false);
}

void AProjectKillerTimeGameMode::StartMatch()
{
	Super::StartMatch();
}

void AProjectKillerTimeGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
