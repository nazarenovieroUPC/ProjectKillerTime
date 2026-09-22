// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MenuGameMode.h"

#include "ProjectKillerTimePlayerState.h"
#include "Framework/MenuPlayerController.h"

AMenuGameMode::AMenuGameMode()
{
	PlayerControllerClass = AMenuPlayerController::StaticClass();
	DefaultPawnClass = nullptr;
}

void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMenuGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
}
