// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProjectKillerTimeGameMode.generated.h"

UCLASS(abstract)
class AProjectKillerTimeGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	AProjectKillerTimeGameMode();
	
	virtual void BeginPlay() override;
	
	virtual void StartMatch() override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	FTimerHandle MatchStartDelayTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	float MatchStartDelay;
	
};



