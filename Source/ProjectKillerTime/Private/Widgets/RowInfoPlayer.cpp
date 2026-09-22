// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/RowInfoPlayer.h"

#include "ProjectKillerTimePlayerState.h"
#include "Components/TextBlock.h"

void URowInfoPlayer::InitRow(AProjectKillerTimePlayerState* PS)
{
	if (!PS) return;
	TextPlayerName->SetText(FText::FromString(PS->GetPlayerName()));
}

void URowInfoPlayer::SetPlayerName(FString PlayerName)
{
	return;
}
