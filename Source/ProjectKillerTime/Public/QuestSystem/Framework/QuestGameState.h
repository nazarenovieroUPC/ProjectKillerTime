// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "QuestGameState.generated.h"

enum class EQuestObjectiveType : uint8;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnGameplayObjectiveEvent, 
	EQuestObjectiveType, ObjectiveType, 
	FName, TargetID, 
	int, Amount
);

UCLASS()
class PROJECTKILLERTIME_API AQuestGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AQuestGameState();
	
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnGameplayObjectiveEvent OnGameplayObjectiveEvent;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void ReportGameplayObjective(EQuestObjectiveType ObjectiveType, FName TargetID, int Amount);
};
