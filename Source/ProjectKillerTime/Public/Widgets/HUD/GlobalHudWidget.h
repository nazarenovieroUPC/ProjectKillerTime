// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GlobalHudWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTKILLERTIME_API UGlobalHudWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//Temples
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextMissingTemples;

	UFUNCTION()
	void UpdateTemples(int32 MissingTemples);
	
	//MatchTimer
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextTimer;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	int32 LastShownSeconds = -2;
	
	bool bBoundToGameState = false;
};
