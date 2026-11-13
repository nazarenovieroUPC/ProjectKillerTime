// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Utilities/TeamType.h"
#include "ProjectKillerTimeGameMode.generated.h"

UCLASS(abstract)
class AProjectKillerTimeGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	/** Constructor */
	AProjectKillerTimeGameMode();
	
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<APawn> KillerPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<APawn> SurvivorPawnClass;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	
	virtual void BeginPlay() override;
	
	virtual void StartMatch() override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	FTimerHandle MatchStartDelayTimerHandle;

	FTimerHandle MatchTimerHandle;
	
	void EndMatchWithWinner(ETeamType Winner);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	float MatchStartDelay = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float MatchDuration = 600.f; // 10 minutos
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	bool IsKillerController(AController* Controller) const;
	
	void OnMatchTimeExpired();
	
	bool bMatchResolved = false;
};



