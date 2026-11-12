// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UENUM(BlueprintType)
enum class EHealthState : uint8
{
	Healthy        UMETA(DisplayName = "Healthy"),
	Injured      UMETA(DisplayName = "Injured"),
	Fallen       UMETA(DisplayName = "Fallen")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthStateChanged, EHealthState, NewHealthState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeingHealedChanged, bool, bBeingHealed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealProgressChanged, float, HealPercentage);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTKILLERTIME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "Survivor | Health")
	FOnHealthStateChanged OnHealthStateChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Survivor | Health")
	FOnBeingHealedChanged OnBeingHealedChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Survivor | Health")
	FOnHealProgressChanged OnHealProgressChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Survivor | Health")
	float MaxHealth = 100.f;
	
	UPROPERTY(ReplicatedUsing = OnRep_HealthState, BlueprintReadOnly, Category = "Survivor | Health")
	EHealthState HealthState;
	
	UPROPERTY(ReplicatedUsing = OnRep_BeingHealed, BlueprintReadOnly ,Category = "Health")
	bool bBeingHealed;
	
	UPROPERTY(ReplicatedUsing = OnRep_HealProgress, BlueprintReadOnly, Category = "Health")
	float HealProgress = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealTime = 35.f;

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	//Replication
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_HealthState();
	
	UFUNCTION()
	void OnRep_BeingHealed();
	
	UFUNCTION()
	void OnRep_HealProgress();
	
	//Timers
	FTimerHandle TimerHeal;
	
	void UpdateHeal();

public:
	UFUNCTION()
	void TakeDamage();
	
	UFUNCTION()
	void CureState();
	
	UFUNCTION()
	void StartHeal();
	
	UFUNCTION()
	void CancelHeal();
	
	//getters
	UFUNCTION()
	float GetHealthPercent() const;
};
