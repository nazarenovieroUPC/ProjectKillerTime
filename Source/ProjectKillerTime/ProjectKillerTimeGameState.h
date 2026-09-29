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

UCLASS()
class PROJECTKILLERTIME_API AProjectKillerTimeGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AProjectKillerTimeGameState();
	
	UPROPERTY(BlueprintAssignable)
	FOnLobbyTeamsUpdated OnLobbyTeamsUpdated;
	
protected:
	//Temples
	UPROPERTY()
	int TotalTemples = 0;
	
	UPROPERTY(ReplicatedUsing = OnRep_ActivatedTemples)
	int ActivatedTemples = 0;
	
	//Survis
	UPROPERTY()
	int SurvivorsAlive = 0;
	
	UPROPERTY()
	int SurvivorsKilled = 0;
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	//Setters
	void SetTotalTemples();
	
	void SetActivatedTemples();
	
	void SetSurvivorsAlive(int NewSurvivorsAlive);
	
	void SetSurvivorsKilled(int NewSurvivorsKilled);
	
	//Getters
	UFUNCTION()
	ETeamType GetTeam() const;
	
	//Replication
	UFUNCTION()
	void OnRep_ActivatedTemples();
};
