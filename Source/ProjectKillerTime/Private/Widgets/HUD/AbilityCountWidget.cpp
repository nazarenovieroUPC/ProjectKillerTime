// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/AbilityCountWidget.h"

#include "Components/TextBlock.h"

void UAbilityCountWidget::UpdateCount(int32 NewTraps)
{
	if (TextCount) TextCount->SetText(FText::AsNumber(NewTraps));
}
