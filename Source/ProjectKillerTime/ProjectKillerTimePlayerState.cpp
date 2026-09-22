// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectKillerTimePlayerState.h"

#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"

void AProjectKillerTimePlayerState::SetTeam(ETeamType Team)
{
	if (Team == ETeamType::Survivor)
	{
		CurrentTeam = Team;
		return;
	}
	
	if (Team == ETeamType::Killer)
	{
		UWorld* World = GetWorld();
		if (World && World->GetGameState())
		{
			
			for (APlayerState* PS : World->GetGameState()->PlayerArray)
			{
				AProjectKillerTimePlayerState* PKTPS = Cast<AProjectKillerTimePlayerState>(PS);
				if (PKTPS)
				{
					if (PKTPS && PKTPS != this)
					{
						if (PKTPS->GetTeam() == ETeamType::Killer) return;
					}
				}
			}
		}
	}
	
	CurrentTeam = Team;
	
}

void AProjectKillerTimePlayerState::OnRep_TeamSelected()
{
	
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Team Changed!"));
	UE_LOG(LogTemp, Warning, TEXT("Team changed on client"))
}

void AProjectKillerTimePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, CurrentTeam);
}
