// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Utilities/TeamType.h"
#include "ProjectKillerTimePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKILLERTIME_API AProjectKillerTimePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(ReplicatedUsing = OnRep_TeamSelected)
	ETeamType CurrentTeam = ETeamType::None;
	
	void SetTeam(ETeamType Team);
	
	ETeamType GetTeam() const{return CurrentTeam;}
	
	UFUNCTION()
	void OnRep_TeamSelected();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
