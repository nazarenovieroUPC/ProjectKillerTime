// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem/Widgets/QuestTrackerWidget.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "QuestSystem/Components/QuestComponent.h"
#include "QuestSystem/Utilities/PDAQuestDefinition.h"
#include "QuestSystem/Utilities/STQuestRuntime.h"

void UQuestTrackerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		PlayerQuestComponent = PlayerPawn->FindComponentByClass<UQuestComponent>();

		if (PlayerQuestComponent)
		{
			PlayerQuestComponent->OnQuestStarted.AddDynamic(this, &UQuestTrackerWidget::OnQuestUpdated);
			
			PlayerQuestComponent->OnQuestProgressChanged.AddDynamic(this, &UQuestTrackerWidget::OnQuestUpdated);
			
			PlayerQuestComponent->OnQuestCompleted.AddDynamic(this, &UQuestTrackerWidget::OnQuestUpdated);
		}
	}
}

void UQuestTrackerWidget::OnQuestUpdated(FSTQuestRuntime QuestData)
{
	Text_QuestName->SetText(FText::FromName(QuestData.QuestID));
	Text_QuestDescription->SetText(QuestData.QuestDefinition->Description);
	
	FString AllObjectivesText = TEXT("");
    
	for (FSTQuestObjective& Objective : QuestData.Objectives)
	{
		FString ObjLine = FString::Printf(TEXT("- %s: %d / %d \n"), 
			*Objective.TargetID.ToString(), 
			Objective.CurrentAmount, 
			Objective.RequiredAmount);
            
		AllObjectivesText += ObjLine;
	}

	Text_Objectives->SetText(FText::FromString(AllObjectivesText));
	
	Text_QuestState->SetText(FText::FromString(QuestData.bIsCompleted ? TEXT("Completado") : TEXT("Activo")));
}

