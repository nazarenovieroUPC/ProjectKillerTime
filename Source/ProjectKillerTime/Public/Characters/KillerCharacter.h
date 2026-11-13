// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKillerTimeCharacter.h"
#include "KillerCharacter.generated.h"

class UAbilityCountWidget;
class UAbilitiesComponent;

UCLASS()
class PROJECTKILLERTIME_API AKillerCharacter : public AProjectKillerTimeCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKillerCharacter();
	
	//Trap ability
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAbilitiesComponent> AbilitiesComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> AbilityWidgetClass;
	
	//Killer Variables
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float BaseVelocity = 460.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float LungeVelocity = 750.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float CooldownVelocity = 150.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool bIsAttacking = false;
	
	/** Ability Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AbilityAction;
	
	/** Attack Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AttackAction;
	
	// Attack Animations
	UPROPERTY(EditDefaultsOnly, Category = "Animaciones")
	UAnimMontage* MontageShotAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Animaciones")
	UAnimMontage* MontageLargeAttack;
	
	//Abilities Hud
	UPROPERTY()
	TObjectPtr<UAbilityCountWidget> AbilityWidget;
	
	UFUNCTION()
	void OnAbilityUpdate(int32 NewTraps);
	
	//Charge Attack
	float StartAttackTime;
	
	void StartAttack(const FInputActionValue& Value);
	void ReleaseAttack(const FInputActionValue& Value);
	
	void ApplyAttackSpeed(bool bIsLargeAttack);
	
	UFUNCTION()
	void ResetVelocity();
	
	//Timer
	FTimerHandle TimerVelocity;
	
	FTimerHandle TimerTrapCooldown;
	
	//Abilities
	void UseAbility(const FInputActionValue& Value);
    
	//RPCs
	UFUNCTION(Server, Reliable)
	void Server_DoAttack(bool bIsLargeAttack);
	
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayAnimAttack(bool bIsLargeAttack);

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
