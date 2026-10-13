// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STQuestObjective.h"
#include "Engine/DataAsset.h"
#include "PDAQuestDefinition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKILLERTIME_API UPDAQuestDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName QuestID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText QuestName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSTQuestObjective> Objectives;
};
