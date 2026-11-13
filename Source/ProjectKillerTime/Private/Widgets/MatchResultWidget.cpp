// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MatchResultWidget.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

void UMatchResultWidget::SetWinner(ETeamType WinningTeam)
{
	if (!TextWinner) return;

	const TCHAR* Message = (WinningTeam == ETeamType::Survivor)
		? TEXT("¡Los Survivors purificaron los templos! - Ganan los Survivors")
		: TEXT("¡Se acabó el tiempo!\nGana el Killer");

	TextWinner->SetText(FText::FromString(Message));
}

void UMatchResultWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (BackButton) BackButton->OnClicked.AddDynamic(this, &UMatchResultWidget::OnBackButtonClicked);
}

void UMatchResultWidget::OnBackButtonClicked()
{
	if (bLeaving) return;
	if (BackButton) BackButton->SetIsEnabled(false);
	
	GetWorld()->GetTimerManager().SetTimer(FallbackTimer, this, &UMatchResultWidget::GoToMenu, 2.f, false);

	UGameInstance* GI = GetGameInstance();
	UMultiplayerSessionsSubsystem* Sessions = GI ? GI->GetSubsystem<UMultiplayerSessionsSubsystem>() : nullptr;

	if (!Sessions)
	{
		GoToMenu();
		return;
	}

	Sessions->MultijugadorAlDestruirSesionCompletada.AddDynamic(this, &UMatchResultWidget::OnDestroySessionComplete);
	Sessions->DestruirSesion();
}

void UMatchResultWidget::OnDestroySessionComplete(bool bWasSuccessful)
{
	GoToMenu();
}

void UMatchResultWidget::GoToMenu()
{
	if (bLeaving) return;
	bLeaving = true;

	GetWorld()->GetTimerManager().ClearTimer(FallbackTimer);

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UMultiplayerSessionsSubsystem* Sessions = GI->GetSubsystem<UMultiplayerSessionsSubsystem>())
		{
			Sessions->MultijugadorAlDestruirSesionCompletada.RemoveAll(this);
		}
	}
	
	UGameplayStatics::OpenLevel(this, MenuLevelName);
}
