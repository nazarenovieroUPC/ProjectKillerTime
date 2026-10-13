// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem/Components/QuestComponent.h"

#include "QuestSystem/Framework/QuestGameState.h"
#include "QuestSystem/Utilities/PDAQuestDefinition.h"
#include "QuestSystem/Utilities/STQuestRuntime.h"


// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	if (AQuestGameState* QuestGameState = Cast<AQuestGameState>(GetWorld()->GetAuthGameMode()->GetGameState))
	{
		QuestGameState->OnGameplayObjectiveEvent.AddDynamic(this, &UQuestComponent::HandleGameplayObjective);
	}
}

bool UQuestComponent::FindQuestByID(FName QuestID, FSTQuestRuntime& QuestData, int& QuestIndex)
{
	for (int i = 0; i < ActiveQuest.Num(); ++i)
	{
		if (ActiveQuest[i].QuestID == QuestID)
		{
			QuestData = ActiveQuest[i];
			QuestIndex = i;
			return true;
		}		
	}
	
	return false;
}

void UQuestComponent::HandleGameplayObjective(EQuestObjectiveType ObjectiveType, FName TargetID, int Amount)
{
	for (int i = 0; i < ActiveQuest.Num(); ++i)
	{
		if (!ActiveQuest[i].bIsActive || ActiveQuest[i].bIsCompleted) return;
		
		for (int j = 0; j < ActiveQuest[i].QuestDefinition->Objectives.Num(); ++j)
		{
			if (ActiveQuest[i].Objectives[j].ObjectiveType == ObjectiveType && ActiveQuest[i].Objectives[j].TargetID == TargetID)
			{
				UpdateObjectiveProgress(i, j, Amount)
			}
		}
	}
}

void UQuestComponent::UpdateObjectiveProgress(int QuestIndex, int ObjectiveIndex, int Amount)
{
	if (!ActiveQuest.IsValidIndex(QuestIndex)) return;
	if (!ActiveQuest[QuestIndex].QuestDefinition->Objectives.IsValidIndex(ObjectiveIndex)) return;
	
	FSTQuestObjective& CurrentObjective = ActiveQuest[QuestIndex].Objectives[ObjectiveIndex];
	
	int NewAmount = CurrentObjective.CurrentAmount + Amount;
	
	CurrentObjective.CurrentAmount = FMath::Clamp(NewAmount, 0, CurrentObjective.RequiredAmount);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("QuestUpdated")))
}

void UQuestComponent::CheckQuestCompleted(int QuestIndex)
{
	FSTQuestRuntime& TargetQuest = ActiveQuest[QuestIndex];
	
	for (const FSTQuestObjective& Objective : TargetQuest.Objectives)
	{
		if (Objective.CurrentAmount < Objective.RequiredAmount) return;
	}
	
	TargetQuest.bIsCompleted = true;
	TargetQuest.bIsActive = false;
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Quest Completed")));
}

void UQuestComponent::AddQuest(UPDAQuestDefinition* QuestDefinition)
{
	if (!QuestDefinition) return;
	FName AddedQuestID = QuestDefinition->QuestID;
	
	FSTQuestRuntime DummyQuestData;
	int32 ExistingIndex = INDEX_NONE;
	if (FindQuestByID(AddedQuestID, DummyQuestData, ExistingIndex))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Quest %s already exists"), *AddedQuestID.ToString()));
		return;
	}
	
	FSTQuestRuntime NewQuestData;
	
	NewQuestData.QuestID = AddedQuestID;
	NewQuestData.QuestDefinition = QuestDefinition;
	NewQuestData.bIsActive = true;
	NewQuestData.bIsCompleted = false;
	
	ActiveQuest.Add(NewQuestData);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Added Quest %s"), *AddedQuestID.ToString()));
}

// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

