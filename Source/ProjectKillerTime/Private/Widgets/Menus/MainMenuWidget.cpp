// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Widgets/Menus/MainMenuWidget.h"

#include "MultiplayerSessionOW/Public/Menu.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

class UMenu;

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (PlayButton) PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayButtonClicked);
	
	if (OptionsButton) PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionsButtonClicked);
	
	if (QuitButton)QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitButtonClicked);
}

void UMainMenuWidget::OnPlayButtonClicked()
{
	if (!MultiplayerMenuWidgetClass) return;
	
	if (UMenu* MultiPlayerMenu = CreateWidget<UMenu>(GetWorld(), MultiplayerMenuWidgetClass))
	{
		MultiPlayerMenu->AddToViewport();
		MultiPlayerMenu->ConfigurarMenu(4, "KillerTime", LobbyLevel);
		RemoveFromParent();
	}
}

void UMainMenuWidget::OnOptionsButtonClicked()
{
	if (!OptionsMenuWidgetClass) return;
	
	if (UUserWidget* OptionsMenu = CreateWidget<UUserWidget>(GetWorld(), OptionsMenuWidgetClass))
	{
		OptionsMenu->AddToViewport();
		RemoveFromParent();
	}
}

void UMainMenuWidget::OnQuitButtonClicked()
{
	if (UWorld* World = GetWorld())
	{
		APlayerController* PC = World->GetFirstPlayerController();
		UKismetSystemLibrary::QuitGame(World, PC, EQuitPreference::Quit, true);
	}
}
