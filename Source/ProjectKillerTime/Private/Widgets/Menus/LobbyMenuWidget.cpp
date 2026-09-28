// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Menus/LobbyMenuWidget.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "ProjectKillerTimeGameState.h"
#include "ProjectKillerTimePlayerState.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Widgets/RowInfoPlayer.h"
#include "Kismet/GameplayStatics.h"

void ULobbyMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (JoinSurvivorsTeamButton) JoinSurvivorsTeamButton->OnClicked.AddDynamic(this, &ULobbyMenuWidget::OnJoinSurvivorsTeamButtonClicked);
	
	if (JoinKillerTeamButton) JoinKillerTeamButton->OnClicked.AddDynamic(this, &ULobbyMenuWidget::OnJoinKillerTeamButtonClicked);
	
	if (StartButton) StartButton->OnClicked.AddDynamic(this, &ULobbyMenuWidget::OnStartButtonClicked);
	
	if (BackButton) BackButton->OnClicked.AddDynamic(this, &ULobbyMenuWidget::OnBackButtonClicked);
}

void ULobbyMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (StartButton)
	{
		if (GetWorld()->GetNetMode() == NM_Client) StartButton->SetVisibility(ESlateVisibility::Collapsed);
		
		else StartButton->SetVisibility(ESlateVisibility::Visible);
	}
	
	AProjectKillerTimeGameState* GS = Cast<AProjectKillerTimeGameState>(GetWorld()->GetGameState());
	if (GS)
	{
		GS->OnLobbyTeamsUpdated.AddDynamic(this, &ULobbyMenuWidget::RefreshTeamLists);
	}
}

void ULobbyMenuWidget::AddPlayerToTeam(ETeamType Team)
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		AProjectKillerTimePlayerState* PS = PC->GetPlayerState<AProjectKillerTimePlayerState>();
		if (PS)
		{
			PS->Sever_SetTeam(Team);
		}
	}
}

void ULobbyMenuWidget::RefreshTeamLists()
{
	AProjectKillerTimeGameState* GS = Cast<AProjectKillerTimeGameState>(GetWorld()->GetGameState());
	
	if (!GS) return;
	
	KillerTeamList->ClearChildren();
	SurvivorsTeamList->ClearChildren();

	bool bIsKillerTaken = false;
	
	for (APlayerState* PS : GS->PlayerArray)
	{
		AProjectKillerTimePlayerState* PKTPS = Cast<AProjectKillerTimePlayerState>(PS);

		if (PKTPS)
		{
			URowInfoPlayer* RowInfoPlayer = CreateWidget<URowInfoPlayer>(this, RowInfoPlayerClass);
			RowInfoPlayer->InitRow(PKTPS);
			
			if (PKTPS->GetTeam() == ETeamType::Killer)
			{
				KillerTeamList->AddChild(RowInfoPlayer);
				
				bIsKillerTaken = true;
			}
			else if (PKTPS->GetTeam() == ETeamType::Survivor)
			{
				SurvivorsTeamList->AddChild(RowInfoPlayer);
			}
		}
	}
	
	if (JoinKillerTeamButton)
	{
		if (bIsKillerTaken) JoinKillerTeamButton->SetIsEnabled(!bIsKillerTaken);
		else JoinKillerTeamButton->SetIsEnabled(true);
	}
}

void ULobbyMenuWidget::OnJoinSurvivorsTeamButtonClicked()
{
	AddPlayerToTeam(ETeamType::Survivor);
	RefreshTeamLists();
}

void ULobbyMenuWidget::OnJoinKillerTeamButtonClicked()
{
	AddPlayerToTeam(ETeamType::Killer);
	RefreshTeamLists();
}

void ULobbyMenuWidget::OnStartButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Start Game"));
	
	GetWorld()->ServerTravel(LevelMap);
}

void ULobbyMenuWidget::OnBackButtonClicked()
{
	if (IOnlineSubsystem* OSS = Online::GetSubsystem(GetWorld()))
	{
		IOnlineSessionPtr Session = OSS->GetSessionInterface();
		
		if (Session.IsValid())
		{
			Session->OnDestroySessionCompleteDelegates.AddUObject(this, &ULobbyMenuWidget::OnDestroySessionComplete);
			
			Session->DestroySession(NAME_GameSession);
		}
	}
	
	UGameplayStatics::OpenLevel(GetWorld(), FName("Lvl_Menu"));
}

void ULobbyMenuWidget::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OSS = Online::GetSubsystem(GetWorld());
	if (OSS)
	{
		IOnlineSessionPtr Session = OSS->GetSessionInterface();
		
		if (Session.IsValid()) Session->OnDestroySessionCompleteDelegates.RemoveAll(this);
		
		UE_LOG(LogTemp, Log, TEXT("Session Destroyed"));
	}
	
	if (bWasSuccessful) UGameplayStatics::OpenLevel(GetWorld(), FName("Lvl_Menu"));
}
