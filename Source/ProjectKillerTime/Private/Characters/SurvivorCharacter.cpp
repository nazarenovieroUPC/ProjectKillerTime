// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SurvivorCharacter.h"

#include "EnhancedInputComponent.h"


// Sets default values
ASurvivorCharacter::ASurvivorCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASurvivorCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASurvivorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASurvivorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASurvivorCharacter::InteractStarted);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ASurvivorCharacter::InteractCompleted);
	}
}

//Interact Actions
void ASurvivorCharacter::InteractStarted(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Start Interact"));
	
	if (ActualTemple != nullptr){
		
		Server_InteractStarted(ActualTemple);
	}
}

void ASurvivorCharacter::InteractCompleted(const FInputActionValue& Value)
{
	if (ActualTemple != nullptr)
	{
		Server_InteractCompleted(ActualTemple);
	}
}

//RPC Implementations
bool ASurvivorCharacter::Server_InteractStarted_Validate(AActor* ActorToInteract){return  ActorToInteract != nullptr;}
void ASurvivorCharacter::Server_InteractStarted_Implementation(AActor* ActorToInteract)
{
	
	if (ActorToInteract->Implements<UInteractInterface>())
	{
		IInteractInterface::Execute_StartedInteract(ActorToInteract, this);
	}
}

bool ASurvivorCharacter::Server_InteractCompleted_Validate(AActor* ActorToInteract){return  ActorToInteract != nullptr;}
void ASurvivorCharacter::Server_InteractCompleted_Implementation(AActor* ActorToInteract)
{
	if (ActorToInteract->Implements<UInteractInterface>())
	{
		IInteractInterface::Execute_CompletedInteract(ActorToInteract, this);
	}
}

