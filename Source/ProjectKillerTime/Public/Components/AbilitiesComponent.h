// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitiesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrapsChanged, int32, NewTrapCount);
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTKILLERTIME_API UAbilitiesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilitiesComponent();

	UPROPERTY(BlueprintAssignable, Category = "Killer | Traps")
	FOnTrapsChanged OnTrapsChanged;

	UPROPERTY(EditDefaultsOnly, Category = "Killer | Traps")
	TSubclassOf<class AKillerTrap> TrapClass;

	UPROPERTY(EditDefaultsOnly, Category = "Killer | Traps")
	int32 MaxTraps = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Killer | Traps")
	float TrapRechargeTime = 10.0f;
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentTraps)
	int32 CurrentTraps;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CurrentTraps();

	FTimerHandle TimerTrapRecharge;
	void RechargeTrap();

public:

	UFUNCTION(Server, Reliable)
	void Server_PlaceTrap();
};
