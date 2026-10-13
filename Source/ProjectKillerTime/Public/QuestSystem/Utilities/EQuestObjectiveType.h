#pragma once

#include "CoreMinimal.h"
#include "EQuestObjectiveType.generated.h"

UENUM(BlueprintType)
enum class EQuestObjectiveType : uint8
{
	Kill,
	Collect,
	Interact,
	ReachLocation
};

