// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectKillerTimePlayerState.h"

#include "ProjectKillerTimeGameState.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"

void AProjectKillerTimePlayerState::Sever_SetTeam_Implementation(ETeamType Team)
{
	if (Team == ETeamType::Killer)
	{
		
		AProjectKillerTimeGameState* GS = Cast<AProjectKillerTimeGameState>(GetWorld()->GetGameState());
	
		if (!GS) return;
		
		for (APlayerState* PS : GS->PlayerArray)
		{
			AProjectKillerTimePlayerState* PKTPS = Cast<AProjectKillerTimePlayerState>(PS);
			
			if (PKTPS && PKTPS != this && PKTPS->GetTeam() == ETeamType::Killer) return;
		}
	}
	
	
	CurrentTeam = Team;
	
	OnRep_TeamSelected();
}

void AProjectKillerTimePlayerState::OnRep_TeamSelected()
{
	AProjectKillerTimeGameState* GS = Cast<AProjectKillerTimeGameState>(GetWorld()->GetGameState());
	if (GS)
	{
		GS->OnLobbyTeamsUpdated.Broadcast();
	}
}

void AProjectKillerTimePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, CurrentTeam);
}
