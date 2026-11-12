// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SurvivorCharacter.h"

#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/ProgressBars/ProgressBarWidget.h"

// Sets default values
ASurvivorCharacter::ASurvivorCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	//Interact Collision (para la curación)
	InteractSphere = CreateDefaultSubobject<USphereComponent>("Interact Sphere");
	InteractSphere->SetupAttachment(RootComponent);
	
	InteractSphere->SetSphereRadius(150.f);
	
	InteractSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	
	//Heal Progress Bar
	WidgetHealBar = CreateDefaultSubobject<UWidgetComponent>("WidgetHealBar");
	WidgetHealBar->SetupAttachment(RootComponent);
	WidgetHealBar->SetVisibility(false);
	WidgetHealBar->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetHealBar->SetDrawSize(FVector2D(150.f, 20.f));
	WidgetHealBar->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
}

// Called when the game starts or when spawned
void ASurvivorCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (HealthComponent)
	{
		HealthComponent->OnHealthStateChanged.AddDynamic(this, &ASurvivorCharacter::ManageHealthChanged);
		
		HealthComponent->OnHealProgressChanged.AddDynamic(this, &ASurvivorCharacter::UpdateHealBar);
		HealthComponent->OnBeingHealedChanged.AddDynamic(this, &ASurvivorCharacter::ToggleUIHeal);
	}
	
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &ASurvivorCharacter::OnOverlapBegin);
	InteractSphere->OnComponentEndOverlap.AddDynamic(this, &ASurvivorCharacter::OnOverlapEnd);
}

//Cambios varios segun estado del jugador
void ASurvivorCharacter::ManageHealthChanged(EHealthState NewHealthState)
{
	if (NewHealthState == EHealthState::Fallen)
	{
		GetCharacterMovement()->DisableMovement();
		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetSimulatePhysics(true);
	}
	else
	{
		if (GetMesh()->IsSimulatingPhysics())
		{
			GetMesh()->SetSimulatePhysics(false);
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			
			GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
			
			GetCharacterMovement()->MaxWalkSpeed = (NewHealthState == EHealthState::Healthy) ? 600.f : 400.f;
		}
	}
	
	TObjectPtr<UMaterialInterface> NewMaterial = nullptr;
	
	switch(NewHealthState)
	{
	case EHealthState::Healthy: NewMaterial = HealthyMaterial; break;
		
	case EHealthState::Injured: NewMaterial = InjuredMaterial; break;
		
	case EHealthState::Fallen: NewMaterial = FallenMaterial; break;
	}
	
	if (NewMaterial && GetMesh()) GetMesh()->SetMaterial(0, NewMaterial);
}

AActor* ASurvivorCharacter::GetBestInteraction()
{
	if (NearbyActors.IsEmpty()) return nullptr;
	
	AActor* BestActor = nullptr;
	float BestDistance = 999999.f;
	
	for (AActor* ActualActor : NearbyActors)
	{
		if (!IsValid(ActualActor)) continue;
		
		if (ActualActor->Implements<UInteractInterface>())
		{
			float DistanceActor = FVector::DistSquared(GetActorLocation(), ActualActor->GetActorLocation());
			
			if (DistanceActor < BestDistance)
			{
				BestDistance = DistanceActor;
				BestActor = ActualActor;
			}
		}
	}
	
	return BestActor;
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

//Overlaps
void ASurvivorCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->Implements<UInteractInterface>())
	{
		NearbyActors.AddUnique(OtherActor);
	}
}

void ASurvivorCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		NearbyActors.Remove(OtherActor);
		
		if (OtherActor == ActualInteractor)
		{
			ActualInteractor = nullptr;
			Server_InteractCompleted(OtherActor);
		}
	}
}

//UI
void ASurvivorCharacter::UpdateHealBar(float HealPercent)
{
	if (!WidgetHealBar && !WidgetHealBar->IsVisible()) return;
	
	if (APawn* LocalPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		float DistanceToBar = FVector::Distance(GetActorLocation(), LocalPlayer->GetActorLocation());
		
		if (DistanceToBar <= DistanceToViewBar)
		{
			if (UProgressBarWidget* MyWidget = Cast<UProgressBarWidget>(WidgetHealBar->GetUserWidgetObject()))
			{
				MyWidget->SetProgress(HealPercent);
		
				WidgetHealBar->SetVisibility(HealPercent > 0.0f && HealPercent < 1.0f);
			}
		}
		else
		{
			WidgetHealBar->SetVisibility(false);
		}
	}
	
	
}

void ASurvivorCharacter::ToggleUIHeal(bool bShow)
{
	if (!WidgetHealBar) return;
	
	if (!bShow)
	{
		WidgetHealBar->SetVisibility(false);
	}
	else if (APawn* LocalPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		float DistanceToBar = FVector::Distance(GetActorLocation(), LocalPlayer->GetActorLocation());
		
		WidgetHealBar->SetVisibility(DistanceToBar <= DistanceToViewBar);
	}
	
}

//Interact Healing
void ASurvivorCharacter::StartedInteract_Implementation(AActor* Interactor)
{
	if (!HealthComponent || HealthComponent->HealthState == EHealthState::Healthy) return;
	
	HealthComponent->StartHeal();
}

void ASurvivorCharacter::CompletedInteract_Implementation(AActor* Interactor)
{
	IInteractInterface::CompletedInteract_Implementation(Interactor);
	
	if (!HealthComponent) return;
	
	HealthComponent->CancelHeal();
}

//Interact Actions
void ASurvivorCharacter::InteractStarted(const FInputActionValue& Value)
{
	ActualInteractor = GetBestInteraction();
	
	if (ActualInteractor) Server_InteractStarted(ActualInteractor);
	
}

void ASurvivorCharacter::InteractCompleted(const FInputActionValue& Value)
{
	if (ActualInteractor)
	{
		Server_InteractCompleted(ActualInteractor);
		ActualInteractor = nullptr;
	}
}

//RPC Implementations
bool ASurvivorCharacter::Server_InteractStarted_Validate(AActor* ActorToInteract){return true;}
void ASurvivorCharacter::Server_InteractStarted_Implementation(AActor* ActorToInteract)
{
	if (!IsValid(ActorToInteract)) return;
	
	if (ActorToInteract->Implements<UInteractInterface>()) IInteractInterface::Execute_StartedInteract(ActorToInteract, this);
}

bool ASurvivorCharacter::Server_InteractCompleted_Validate(AActor* ActorToInteract){return true;}
void ASurvivorCharacter::Server_InteractCompleted_Implementation(AActor* ActorToInteract)
{
	if (!IsValid(ActorToInteract)) return;
	
	if (ActorToInteract->Implements<UInteractInterface>()) IInteractInterface::Execute_CompletedInteract(ActorToInteract, this);
}