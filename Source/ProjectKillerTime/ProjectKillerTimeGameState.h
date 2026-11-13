// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Utilities/TeamType.h"
#include "ProjectKillerTimeGameState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyTeamsUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTemplesUpdated, int32, TemplesRemaining);

UCLASS()
class PROJECTKILLERTIME_API AProjectKillerTimeGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AProjectKillerTimeGameState();
	
	UPROPERTY(BlueprintAssignable)
	FOnLobbyTeamsUpdated OnLobbyTeamsUpdated;
	
	UPROPERTY(BlueprintAssignable, Category = "Eventos")
	FOnTemplesUpdated OnTemplesUpdated;

	
protected:
	//Temples
	UPROPERTY(ReplicatedUsing = OnRep_TotalTemples)
	int32 TotalTemples = 0;
	
	UPROPERTY(ReplicatedUsing = OnRep_ActivatedTemples)
	int32 ActivatedTemples = 0;
	
	//MatchTime
	UPROPERTY(Replicated)
	float MatchEndTime = 0.f;
	
	//Survis
	UPROPERTY()
	int SurvivorsAlive = 0;
	
	UPROPERTY()
	int SurvivorsKilled = 0;
	
	//Winner
	UPROPERTY(ReplicatedUsing = OnRep_WinningTeam)
	ETeamType WinningTeam = ETeamType::None;

	void NotifyMatchEnded();
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	//Temporizador
	void StartMatchTimer(float DurationSeconds);

	UFUNCTION(BlueprintPure, Category = "Match")
	bool IsMatchTimerRunning() const { return MatchEndTime > 0.f && !HasWinner(); }
	
	//Match Winners
	void SetWinningTeam(ETeamType Winner);

	UFUNCTION(BlueprintPure, Category = "Match")
	bool HasWinner() const { return WinningTeam != ETeamType::None; }

	UFUNCTION()
	void OnRep_WinningTeam();
	
	//Setters
	void SetTotalTemples();
	
	void SetActivatedTemples();
	
	void SetSurvivorsAlive(int NewSurvivorsAlive);
	
	void SetSurvivorsKilled(int NewSurvivorsKilled);
	
	//Getters
	int32 GetTemplesRemaining() const { return TotalTemples - ActivatedTemples; }
	
	UFUNCTION(BlueprintPure, Category = "Match")
	float GetRemainingMatchTime() const;
	
	UFUNCTION(BlueprintPure, Category = "Match")
	ETeamType GetWinningTeam() const { return WinningTeam; }
	
	UFUNCTION()
	ETeamType GetTeam() const;
	
	//Replication
	UFUNCTION()
	void OnRep_ActivatedTemples();
	
	UFUNCTION()
	void OnRep_TotalTemples();
};
