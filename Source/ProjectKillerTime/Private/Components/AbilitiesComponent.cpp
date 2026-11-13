// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AbilitiesComponent.h"

#include "Actors/KillerTrap.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UAbilitiesComponent::UAbilitiesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UAbilitiesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		CurrentTraps = MaxTraps;
	}
}

void UAbilitiesComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAbilitiesComponent, CurrentTraps);
}

void UAbilitiesComponent::OnRep_CurrentTraps()
{
	OnTrapsChanged.Broadcast(CurrentTraps);
}

void UAbilitiesComponent::RechargeTrap()
{
	CurrentTraps++;
	OnTrapsChanged.Broadcast(CurrentTraps);

	if (CurrentTraps >= MaxTraps)
	{
		CurrentTraps = MaxTraps;
		GetWorld()->GetTimerManager().ClearTimer(TimerTrapRecharge);
	}
}

void UAbilitiesComponent::Server_PlaceTrap_Implementation()
{
	if (!TrapClass || CurrentTraps <= 0) return;
    
	CurrentTraps--;
	OnTrapsChanged.Broadcast(CurrentTraps);
    
	if (!GetWorld()->GetTimerManager().IsTimerActive(TimerTrapRecharge))
	{
		GetWorld()->GetTimerManager().SetTimer(TimerTrapRecharge, this, &UAbilitiesComponent::RechargeTrap, TrapRechargeTime, true);
	}
	
	FVector TrapPosition = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 100.f) - FVector(0.f, 0.f, 60.f);
	FRotator TrapRotation = GetOwner()->GetActorRotation();
	TrapRotation.Pitch = 0.f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AKillerTrap>(TrapClass, TrapPosition, TrapRotation, SpawnParams);
}

