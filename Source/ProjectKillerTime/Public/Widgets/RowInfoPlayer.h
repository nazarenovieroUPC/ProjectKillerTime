// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RowInfoPlayer.generated.h"

class AProjectKillerTimePlayerState;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTKILLERTIME_API URowInfoPlayer : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextPlayerName;
	
	UFUNCTION(BlueprintCallable)
	void InitRow(AProjectKillerTimePlayerState* PS);
	
	UFUNCTION(BlueprintCallable)
	void SetPlayerName(FString PlayerName);
};
