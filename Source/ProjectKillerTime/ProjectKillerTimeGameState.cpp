// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectKillerTimeGameState.h"

#include "Net/UnrealNetwork.h"

AProjectKillerTimeGameState::AProjectKillerTimeGameState()
{
	ActivatedTemples = 0;
}

void AProjectKillerTimeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AProjectKillerTimeGameState, ActivatedTemples);
}

void AProjectKillerTimeGameState::SetTotalTemples()
{
	if (HasAuthority())
	{
		TotalTemples++;
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver, FString::Printf(TEXT("Total de Templos: %d"), TotalTemples));
	}
}

void AProjectKillerTimeGameState::SetActivatedTemples()
{
	if (HasAuthority())
	{
		ActivatedTemples++;
		
		OnRep_ActivatedTemples();
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver, FString::Printf(TEXT("Templos Purificados: %d"), ActivatedTemples));
}
