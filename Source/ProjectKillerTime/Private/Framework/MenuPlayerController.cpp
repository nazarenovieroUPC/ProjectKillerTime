// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MenuPlayerController.h"

#include "Blueprint/UserWidget.h"

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!MenuWidgetClass) return;
	
	if ((MenuWidget = CreateWidget<UUserWidget>(this, MenuWidgetClass)))
	{
		MenuWidget->AddToViewport();
		
		FInputModeUIOnly InputMode;
		
		InputMode.SetWidgetToFocus(MenuWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		
		SetInputMode(InputMode);
		SetShowMouseCursor(true);
	}
}
