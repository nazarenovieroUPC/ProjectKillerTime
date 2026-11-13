// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/GlobalHudWidget.h"

#include "ProjectKillerTimeGameState.h"
#include "Components/TextBlock.h"

void UGlobalHudWidget::UpdateTemples(int32 MissingTemples)
{
	if (TextMissingTemples) TextMissingTemples->SetText(FText::AsNumber(MissingTemples));
}

void UGlobalHudWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!TextTimer || !GetWorld()) return;

	const AProjectKillerTimeGameState* GS = GetWorld()->GetGameState<AProjectKillerTimeGameState>();
	
	if (GS && !bBoundToGameState)
	{
		AProjectKillerTimeGameState* MutableGS = GetWorld()->GetGameState<AProjectKillerTimeGameState>();
		MutableGS->OnTemplesUpdated.AddDynamic(this, &UGlobalHudWidget::UpdateTemples);
		UpdateTemples(MutableGS->GetTemplesRemaining());
		bBoundToGameState = true;
	}

	const bool bRunning = GS && GS->IsMatchTimerRunning();
	const int32 Seconds = bRunning ? FMath::CeilToInt(GS->GetRemainingMatchTime()) : -1;
	
	if (Seconds == LastShownSeconds) return;
	LastShownSeconds = Seconds;

	const FString Text = (Seconds < 0)
		? TEXT("--:--")
		: FString::Printf(TEXT("%02d:%02d"), Seconds / 60, Seconds % 60);

	TextTimer->SetText(FText::FromString(Text));
}
