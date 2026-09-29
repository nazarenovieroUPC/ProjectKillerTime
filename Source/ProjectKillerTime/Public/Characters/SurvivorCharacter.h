// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectKillerTimeCharacter.h"
#include "Actors/Temple.h"
#include "Interfaces/SurvivorInterface.h"
#include "SurvivorCharacter.generated.h"

UCLASS()
class PROJECTKILLERTIME_API ASurvivorCharacter : public AProjectKillerTimeCharacter, public ISurvivorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivorCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Survivor | ActualTemple")
	TObjectPtr<ATemple> ActualTemple;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	/** Called to interact completed input */
	virtual void InteractStarted(const FInputActionValue& Value) override;
	
	/** Called to interact completed input */
	void InteractCompleted(const FInputActionValue& Value);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_InteractStarted(AActor* ActorToInteract);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_InteractCompleted(AActor* ActorToInteract);
	
};
