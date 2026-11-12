// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ProgressBars/ProgressBarWidget.h"

#include "Components/ProgressBar.h"

void UProgressBarWidget::SetProgress(float Percentage)
{
	if (UIProgressBar) UIProgressBar->SetPercent(Percentage);
}
