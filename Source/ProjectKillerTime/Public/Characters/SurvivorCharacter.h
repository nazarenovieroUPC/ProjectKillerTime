// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKillerTimeCharacter.h"
#include "Actors/Temple.h"
#include "Interfaces/SurvivorInterface.h"
#include "SurvivorCharacter.generated.h"

class UWidgetComponent;
class USphereComponent;
enum class EHealthState : uint8;
class UHealthComponent;

UCLASS()
class PROJECTKILLERTIME_API ASurvivorCharacter : public AProjectKillerTimeCharacter, public ISurvivorInterface, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivorCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* InteractSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> WidgetHealBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly ,Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent;
	
	//MI
	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	TObjectPtr<UMaterialInterface> HealthyMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	TObjectPtr<UMaterialInterface> InjuredMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	TObjectPtr<UMaterialInterface> FallenMaterial;
	
	//Interact Variables
	UPROPERTY()
	TArray<AActor*> NearbyActors;
	
	UPROPERTY()
	TObjectPtr<AActor> ActualInteractor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float DistanceToViewBar = 1000.f;
	
	//Velocity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalkSpeed")
	float HealthyVelocity = 575.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalkSpeed")
	float InjuredVelocity = 500.f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void ManageHealthChanged(EHealthState NewHealthState);
	
	UFUNCTION()
	AActor* GetBestInteraction();
	
	//Inputs
	/** Called to interact completed input */
	virtual void InteractStarted(const FInputActionValue& Value) override;
	
	/** Called to interact completed input */
	void InteractCompleted(const FInputActionValue& Value);
	
	//Server Functions
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_InteractStarted(AActor* ActorToInteract);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_InteractCompleted(AActor* ActorToInteract);

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Overlaps Funcs
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	//UI Funcs
	UFUNCTION()
	void UpdateHealBar(float HealPercent);
	
	UFUNCTION()
	void ToggleUIHeal(bool bShow);
	
	//Interact Interface
	virtual void StartedInteract_Implementation(AActor* Interactor) override;
	
	virtual void CompletedInteract_Implementation(AActor* Interactor) override;
};
