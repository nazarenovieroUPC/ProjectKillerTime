#pragma once
#include "CoreMinimal.h"
#include "TeamType.generated.h"

//Enum de los diferentes equipos
UENUM(BlueprintType)
enum class ETeamType : uint8
{
	None UMETA(DisplayName = "None"),
	Survivor UMETA(DisplayName = "Survivor"),
	Killer UMETA(DisplayName = "Killer"),
};
