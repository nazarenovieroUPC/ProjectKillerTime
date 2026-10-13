// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestTrackerWidget.generated.h"

class UQuestComponent;
struct FSTQuestRuntime;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTKILLERTIME_API UQuestTrackerWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_QuestName;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_QuestDescription;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Objectives;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_QuestState;
	
	UFUNCTION()
	void OnQuestUpdated(FSTQuestRuntime QuestData);
	
private:
	UPROPERTY()
	UQuestComponent* PlayerQuestComponent;
};
