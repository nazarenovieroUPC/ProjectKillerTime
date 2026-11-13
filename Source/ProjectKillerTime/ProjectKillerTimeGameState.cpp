// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectKillerTimeGameState.h"

#include "ProjectKillerTimeGameMode.h"
#include "ProjectKillerTimePlayerController.h"
#include "Net/UnrealNetwork.h"

AProjectKillerTimeGameState::AProjectKillerTimeGameState()
{
	ActivatedTemples = 0;
}

void AProjectKillerTimeGameState::NotifyMatchEnded()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AProjectKillerTimePlayerController* PC = Cast<AProjectKillerTimePlayerController>(It->Get()))
		{
			PC->ShowMatchResult(WinningTeam);
		}
	}
}

void AProjectKillerTimeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AProjectKillerTimeGameState, TotalTemples);
	DOREPLIFETIME(AProjectKillerTimeGameState, ActivatedTemples);
	DOREPLIFETIME(AProjectKillerTimeGameState, MatchEndTime);
	DOREPLIFETIME(AProjectKillerTimeGameState, WinningTeam);
}

void AProjectKillerTimeGameState::StartMatchTimer(float DurationSeconds)
{
	if (!HasAuthority()) return;

	MatchEndTime = GetServerWorldTimeSeconds() + DurationSeconds;
}

float AProjectKillerTimeGameState::GetRemainingMatchTime() const
{
	return FMath::Max(0.f, MatchEndTime - GetServerWorldTimeSeconds());
}

void AProjectKillerTimeGameState::SetWinningTeam(ETeamType Winner)
{
	if (!HasAuthority() || HasWinner()) return;

	WinningTeam = Winner;
	NotifyMatchEnded();
}

void AProjectKillerTimeGameState::OnRep_WinningTeam()
{
	NotifyMatchEnded();
}

void AProjectKillerTimeGameState::SetTotalTemples()
{
	if (HasAuthority()) TotalTemples++;
}

void AProjectKillerTimeGameState::SetActivatedTemples()
{
	if (HasAuthority())
	{
		ActivatedTemples++;

		OnTemplesUpdated.Broadcast(GetTemplesRemaining());

		if (TotalTemples > 0 && ActivatedTemples >= TotalTemples)
		{
			if (AProjectKillerTimeGameMode* GM = GetWorld()->GetAuthGameMode<AProjectKillerTimeGameMode>())
			{
				GM->EndMatchWithWinner(ETeamType::Survivor);
			}
		}
	}
}

void AProjectKillerTimeGameState::SetSurvivorsAlive(int NewSurvivorsAlive)
{
}

void AProjectKillerTimeGameState::SetSurvivorsKilled(int NewSurvivorsKilled)
{
}

ETeamType AProjectKillerTimeGameState::GetTeam() const
{
	return ETeamType::None;
}

void AProjectKillerTimeGameState::OnRep_ActivatedTemples()
{
	OnTemplesUpdated.Broadcast(GetTemplesRemaining());
}

void AProjectKillerTimeGameState::OnRep_TotalTemples()
{
	OnTemplesUpdated.Broadcast(GetTemplesRemaining());
}
