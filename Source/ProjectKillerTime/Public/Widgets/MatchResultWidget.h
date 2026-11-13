// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utilities/TeamType.h"
#include "MatchResultWidget.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECTKILLERTIME_API UMatchResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetWinner(ETeamType WinningTeam);

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextWinner;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MenuLevelName = "Lvl_Menu";

private:
	UFUNCTION()
	void OnBackButtonClicked();

	void OnDestroySessionComplete(bool bWasSuccessful);
	
	void GoToMenu();

	FTimerHandle FallbackTimer;
	bool bLeaving = false;
};
