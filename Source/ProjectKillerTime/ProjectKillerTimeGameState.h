// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Utilities/TeamType.h"
#include "ProjectKillerTimeGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKILLERTIME_API AProjectKillerTimeGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	
	void SetTotalTemples(int NewTotalTemples);
	
	void SetFixedTemples(int NewFixedTemples);
	
	void SetSurvivorsAlive(int NewSurvivorsAlive);
	
	void SetSurvivorsKilled(int NewSurvivorsKilled);
	
	ETeamType GetTeam() const;
	
private:
	int TotalTemples = 0;
	
	int FixedTemples = 0;
	
	int SurvivorsAlive = 0;
	
	int SurvivorsKilled = 0;
};
