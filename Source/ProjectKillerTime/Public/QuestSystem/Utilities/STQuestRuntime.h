#pragma once

#include "CoreMinimal.h"
#include "STQuestRuntime.generated.h"

struct FSTQuestObjective;
class UPDAQuestDefinition;

USTRUCT(BlueprintType)
struct FSTQuestRuntime
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName QuestID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPDAQuestDefinition> QuestDefinition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSTQuestObjective> Objectives;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCompleted;
	
};
