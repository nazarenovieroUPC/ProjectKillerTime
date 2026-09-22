// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

//class UMenu;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECTKILLERTIME_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PlayButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OptionsButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MultiplayerMenuWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> OptionsMenuWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MP")
	FString LobbyLevel;
	
private:
	UFUNCTION()
	void OnPlayButtonClicked();
	
	UFUNCTION()
	void OnOptionsButtonClicked();
	
	UFUNCTION()
	void OnQuitButtonClicked();
};
