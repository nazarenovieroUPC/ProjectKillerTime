// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem/Framework/QuestGameState.h"

AQuestGameState::AQuestGameState()
{
}

void AQuestGameState::ReportGameplayObjective(EQuestObjectiveType ObjectiveType, FName TargetID, int Amount)
{
	OnGameplayObjectiveEvent.Broadcast(ObjectiveType, TargetID, Amount);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Gameplay Objective Reported"));
}
