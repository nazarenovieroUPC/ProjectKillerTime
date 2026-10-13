#pragma once

#include "CoreMinimal.h"
#include "STQuestObjective.generated.h"

enum class EQuestObjectiveType : uint8;

USTRUCT(BlueprintType)
struct FSTQuestObjective
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectiveID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestObjectiveType ObjectiveType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TargetID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RequiredAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentAmount;
};
