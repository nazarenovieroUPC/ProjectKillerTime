// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyMenuWidget.generated.h"

class URowInfoPlayer;
class UButton;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class PROJECTKILLERTIME_API ULobbyMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> SurvivorsTeamList;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> KillerTeamList;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> JoinSurvivorsTeamButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> JoinKillerTeamButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MP")
	TSubclassOf<URowInfoPlayer> RowInfoPlayerClass;
	
	UFUNCTION(BlueprintCallable)
	void AddPlayerToTeam(ETeamType Team);
	
	UFUNCTION(BlueprintCallable)
	void RefreshTeamLists();
	
private:
	UFUNCTION()
	void OnJoinSurvivorsTeamButtonClicked();
	
	UFUNCTION()
	void OnJoinKillerTeamButtonClicked();
	
	UFUNCTION()
	void OnBackButtonClicked();
	
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
};
