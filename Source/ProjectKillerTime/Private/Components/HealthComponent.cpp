// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/HealthComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	SetIsReplicatedByDefault(true);
	
	HealthState = EHealthState::Healthy;
	
	bBeingHealed = false;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UHealthComponent, HealthState);
	DOREPLIFETIME(UHealthComponent, bBeingHealed);
	DOREPLIFETIME(UHealthComponent, HealProgress);
}

void UHealthComponent::OnRep_HealthState()
{
	OnHealthStateChanged.Broadcast(HealthState);
}

void UHealthComponent::OnRep_BeingHealed()
{
	OnBeingHealedChanged.Broadcast(bBeingHealed);
}

void UHealthComponent::OnRep_HealProgress()
{
	OnHealProgressChanged.Broadcast(GetHealthPercent());
}

void UHealthComponent::UpdateHeal()
{
	HealProgress += 0.05f;
	
	OnHealProgressChanged.Broadcast(GetHealthPercent());
	
	if (HealProgress >= HealTime)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHeal);
		bBeingHealed = false;
		HealProgress = 0.f;
		OnRep_BeingHealed();
		
		CureState();
	}
}

void UHealthComponent::StartHeal()
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || bBeingHealed) return;
	
	bBeingHealed = true;
	HealProgress = 0.f;
	
	OnBeingHealedChanged.Broadcast(bBeingHealed);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHeal, this, &UHealthComponent::UpdateHeal, 0.05f, true);
}

void UHealthComponent::CancelHeal()
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || !bBeingHealed) return;
	
	bBeingHealed = false;
	HealProgress = 0.f;
	
	OnBeingHealedChanged.Broadcast(bBeingHealed);
	
	GetWorld()->GetTimerManager().ClearTimer(TimerHeal);
}

float UHealthComponent::GetHealthPercent() const
{
	if (HealTime > 0.0f)
	{
		return HealProgress / HealTime;
	}
	return 0.0f;
}

void UHealthComponent::TakeDamage()
{
	if (!GetOwner()->HasAuthority()) return;
	
	if (HealthState == EHealthState::Healthy) HealthState = EHealthState::Injured;
	else if (HealthState == EHealthState::Injured) HealthState = EHealthState::Fallen;
	
	OnRep_HealthState();
}

void UHealthComponent::CureState()
{
	if (!GetOwner()->HasAuthority()) return;
	
	if (HealthState == EHealthState::Fallen) HealthState = EHealthState::Injured;
	else if (HealthState == EHealthState::Injured) HealthState = EHealthState::Healthy;
	
	OnRep_HealthState();
}

