// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityCountWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTKILLERTIME_API UAbilityCountWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextCount;
	
	UFUNCTION()
	void UpdateCount(int32 NewTraps);
};
