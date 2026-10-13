// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.generated.h"


enum class EQuestObjectiveType : uint8;
class UPDAQuestDefinition;
struct FSTQuestRuntime;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUpdateDelegate, FSTQuestRuntime, QuestData)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTKILLERTIME_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UQuestComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSTQuestRuntime> ActiveQuest;
	
	UPROPERTY(BlueprintAssignable, Category = "Quests|Events")
	FOnQuestUpdateDelegate OnQuestStarted;

	UPROPERTY(BlueprintAssignable, Category = "Quests|Events")
	FOnQuestUpdateDelegate OnQuestProgressChanged;

	UPROPERTY(BlueprintAssignable, Category = "Quests|Events")
	FOnQuestUpdateDelegate OnQuestCompleted;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION()
	bool FindQuestByID(FName QuestID, FSTQuestRuntime& QuestData, int& QuestIndex);

	UFUNCTION()
	void HandleGameplayObjective(EQuestObjectiveType ObjectiveType, FName TargetID, int Amount);
	
	UFUNCTION()
	void UpdateObjectiveProgress(int QuestIndex, int ObjectiveIndex, int Amount);
	
	UFUNCTION()
	void CheckQuestCompleted(int QuestIndex);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void AddQuest(UPDAQuestDefinition* QuestDefinition);
};
