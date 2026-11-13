// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/KillerCharacter.h"

#include "EnhancedInputComponent.h"
#include "ProjectKillerTime.h"
#include "Blueprint/UserWidget.h"
#include "Components/AbilitiesComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/HUD/AbilityCountWidget.h"


// Sets default values
AKillerCharacter::AKillerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	AbilitiesComponent = CreateDefaultSubobject<UAbilitiesComponent>("AbilitiesComponent");
}

// Called when the game starts or when spawned
void AKillerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = BaseVelocity;
	
	if (GetMesh() && IsLocallyControlled()) GetMesh()->SetOwnerNoSee(true);
}

void AKillerCharacter::OnAbilityUpdate(int32 NewTraps)
{
	if (AbilityWidget) AbilityWidget->UpdateCount(NewTraps);
}

void AKillerCharacter::StartAttack(const FInputActionValue& Value)
{
	if (bIsAttacking) return;
	
	StartAttackTime = GetWorld()->GetTimeSeconds();
}

void AKillerCharacter::ReleaseAttack(const FInputActionValue& Value)
{
	if (bIsAttacking) return;
	
	float PressedTime = GetWorld()->GetTimeSeconds() - StartAttackTime;
	bool bIsCharged = false;
	
	bIsAttacking = true;
	
	ApplyAttackSpeed(bIsCharged);
    
	Server_DoAttack(bIsCharged);
}

void AKillerCharacter::ApplyAttackSpeed(bool bIsLargeAttack)
{
	if (!GetCharacterMovement()) return;

	GetWorldTimerManager().ClearTimer(TimerVelocity);

	GetCharacterMovement()->MaxWalkSpeed = bIsLargeAttack ? LungeVelocity : CooldownVelocity;

	GetWorldTimerManager().SetTimer(TimerVelocity, this, &AKillerCharacter::ResetVelocity,
									bIsLargeAttack ? 3.f : 2.f, false);
}

void AKillerCharacter::UseAbility(const FInputActionValue& Value)
{
	if (AbilitiesComponent) AbilitiesComponent->Server_PlaceTrap();
}

void AKillerCharacter::ResetVelocity()
{
	if (GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = BaseVelocity;
	
	bIsAttacking = false;
}

void AKillerCharacter::Server_DoAttack_Implementation(bool bIsLargeAttack)
{
	bIsAttacking = true;
	
	Multicast_PlayAnimAttack(bIsLargeAttack);
	
	ApplyAttackSpeed(bIsLargeAttack);
	
	float AttackRange = bIsLargeAttack ? 350.f : 150.f;

	FVector StartHit = GetActorLocation();
	FVector FinHit = StartHit + (GetActorForwardVector() * AttackRange);
	
	TArray<FHitResult> Impacts;
	UKismetSystemLibrary::BoxTraceMulti(this, StartHit, FinHit, FVector(50.f, 50.f, 50.f), 
		GetActorRotation(), UEngineTypes::ConvertToTraceType(ECC_Pawn), false, 
		TArray<AActor*>(), EDrawDebugTrace::ForDuration, Impacts, true);

	for (FHitResult& Hit : Impacts)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor != this)
		{
			if (UHealthComponent* HealthComponent = HitActor->FindComponentByClass<UHealthComponent>())
			{
				HealthComponent->TakeDamage();
				break;
			}
		}
	}
}

void AKillerCharacter::Multicast_PlayAnimAttack_Implementation(bool bIsLargeAttack)
{
	UAnimMontage* MontageToPlay = bIsLargeAttack ? MontageLargeAttack : MontageShotAttack;

	if (MontageToPlay) PlayAnimMontage(MontageToPlay);
}

// Called to bind functionality to input
void AKillerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (AbilityWidgetClass)
	{
		AbilityWidget = CreateWidget<UAbilityCountWidget>(GetWorld(), AbilityWidgetClass);
		
		if (AbilityWidget) AbilityWidget->AddToViewport();
	}
	
	if (AbilitiesComponent)
	{
		AbilitiesComponent->OnTrapsChanged.AddDynamic(this, &AKillerCharacter::OnAbilityUpdate);
		OnAbilityUpdate(AbilitiesComponent->CurrentTraps);
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AKillerCharacter::StartAttack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AKillerCharacter::ReleaseAttack);
		
		EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &AKillerCharacter::UseAbility);
	}else
	{
		UE_LOG(LogProjectKillerTime, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}