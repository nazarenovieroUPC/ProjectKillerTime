// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectKillerTimePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "ProjectKillerTime.h"
#include "Widgets/MatchResultWidget.h"
#include "Widgets/HUD/GlobalHudWidget.h"
#include "Widgets/Input/SVirtualJoystick.h"

void AProjectKillerTimePlayerController::ShowMatchResult(ETeamType WinningTeam)
{
	if (!IsLocalPlayerController() || MatchResultWidget || !MatchResultClass) return;

	MatchResultWidget = CreateWidget<UMatchResultWidget>(this, MatchResultClass);
	if (!MatchResultWidget) return;

	MatchResultWidget->SetWinner(WinningTeam);
	MatchResultWidget->AddToViewport(100);
	
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MatchResultWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
	bShowMouseCursor = true;
}

void AProjectKillerTimePlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	
	bShowMouseCursor = false;
	
	if (IsLocalPlayerController() && GlobalHudClass)
	{
		GlobalHud = CreateWidget<UGlobalHudWidget>(this, GlobalHudClass);
		if (GlobalHud) GlobalHud->AddToViewport();
	}
	
	// only spawn touch controls on local player controllers
	if (IsLocalPlayerController() && ShouldUseTouchControls())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogProjectKillerTime, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AProjectKillerTimePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

bool AProjectKillerTimePlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}
