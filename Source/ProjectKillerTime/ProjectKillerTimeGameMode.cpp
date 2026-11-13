// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectKillerTimeGameMode.h"

#include "EngineUtils.h"
#include "ProjectKillerTimeGameState.h"
#include "ProjectKillerTimePlayerState.h"
#include "GameFramework/PlayerStart.h"

AProjectKillerTimeGameMode::AProjectKillerTimeGameMode()
{
	bDelayedStart = true;
	
}

UClass* AProjectKillerTimeGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	const AProjectKillerTimePlayerState* PS = InController ? InController->GetPlayerState<AProjectKillerTimePlayerState>() : nullptr;

	if (!PS || PS->GetTeam() == ETeamType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("Jugador sin equipo asignado, se usa Survivor por defecto"));
	}

	const bool bIsKiller = PS && PS->GetTeam() == ETeamType::Killer;
	TSubclassOf<APawn> Chosen = bIsKiller ? KillerPawnClass : SurvivorPawnClass;

	return Chosen ? Chosen.Get() : Super::GetDefaultPawnClassForController_Implementation(InController);
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
	
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Match Started!"));
	
	if (AProjectKillerTimeGameState* GS = GetGameState<AProjectKillerTimeGameState>())
	{
		GS->StartMatchTimer(MatchDuration);
	}

	GetWorldTimerManager().SetTimer(MatchTimerHandle, this,
		&AProjectKillerTimeGameMode::OnMatchTimeExpired, MatchDuration, false);
}

void AProjectKillerTimeGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AProjectKillerTimeGameMode::EndMatchWithWinner(ETeamType Winner)
{
	if (bMatchResolved || !IsMatchInProgress()) return;
	bMatchResolved = true;

	GetWorldTimerManager().ClearTimer(MatchTimerHandle);

	if (AProjectKillerTimeGameState* GS = GetGameState<AProjectKillerTimeGameState>())
	{
		GS->SetWinningTeam(Winner);
	}

	EndMatch();
}

AActor* AProjectKillerTimeGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	const FName WantedTag = IsKillerController(Player) ? FName("Killer") : FName("Survivor");

	TArray<AActor*> Starts;
	for (TActorIterator<APlayerStart> i(GetWorld()); i; ++i)
	{
		if (i->PlayerStartTag == WantedTag) Starts.Add(*i);
	}
	
	if (Starts.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No hay PlayerStart con tag %s, se usa el default"), *WantedTag.ToString());
		return Super::ChoosePlayerStart_Implementation(Player);
	}

	return Starts[FMath::RandRange(0, Starts.Num() - 1)];
}

bool AProjectKillerTimeGameMode::IsKillerController(AController* Controller) const
{
	const AProjectKillerTimePlayerState* PS = Controller ? Controller->GetPlayerState<AProjectKillerTimePlayerState>() : nullptr;
	return PS && PS->GetTeam() == ETeamType::Killer;
}

void AProjectKillerTimeGameMode::OnMatchTimeExpired()
{
	EndMatchWithWinner(ETeamType::Killer);
}
