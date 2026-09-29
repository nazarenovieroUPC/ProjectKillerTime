// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Temple.h"

#include "Characters/SurvivorCharacter.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ATemple::ATemple()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	
	bIsActivated = false;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;
	BoxCollision->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	
	TempleMesh = CreateDefaultSubobject<UStaticMeshComponent>("TempleMesh");
	TempleMesh->SetupAttachment(BoxCollision);
}

// Called when the game starts or when spawned
void ATemple::BeginPlay()
{
	Super::BeginPlay();
	
	if (BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATemple::OnOverlapBegin);
		BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ATemple::OnOverlapEnd);
	}
}

//Replicacion
void ATemple::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATemple, bIsActivated);
	DOREPLIFETIME(ATemple, bIsCharging);
}

// Called every frame
void ATemple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Interact Implementation
void ATemple::StartedInteract_Implementation(AActor* Interactor)
{
	IInteractInterface::StartedInteract_Implementation(Interactor);
	
	if (bIsActivated || !HasAuthority()) return;
	
	bIsCharging = true;
	
	OnRep_IsCharging();
}

void ATemple::CompletedInteract_Implementation(AActor* Interactor)
{
	IInteractInterface::CompletedInteract_Implementation(Interactor);
	
	if (bIsActivated) return;
	
	bIsCharging = false;
	
	GetWorldTimerManager().ClearTimer(TempleTimerHandle);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Completed Interact"));
}

//Overlap box collision
void ATemple::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	ASurvivorCharacter* Survivor = Cast<ASurvivorCharacter>(OtherActor);
	
	if (!Survivor) return;
	
	Survivor->ActualTemple = this;
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Can Interact"));
	
}

void ATemple::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		ASurvivorCharacter* Survivor = Cast<ASurvivorCharacter>(OtherActor);
		if (Survivor)
		{
			if(Survivor->ActualTemple == this)
			{
				Survivor->ActualTemple = nullptr;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Can't Interact"));
			}
		}
	}
}

//Timer
void ATemple::OnTempleTimer()
{
		
	ChargeCount += 0.05f / ChargeTime;
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("ChargeCount: %f"), ChargeCount));
	
	if (HasAuthority() && ChargeCount >= MaxChargeCount)
	{
		bIsActivated = true;
		
		OnRep_IsActivated();
	}
}

//Replication
void ATemple::OnRep_IsCharging()
{
	if (bIsCharging) GetWorldTimerManager().SetTimer(TempleTimerHandle, this, &ATemple::OnTempleTimer, 0.5, true);
	
}

void ATemple::OnRep_IsActivated()
{
	if (bIsActivated)
	{
		GetWorldTimerManager().ClearTimer(TempleTimerHandle);
		
		ChargeCount = MaxChargeCount;
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("MaxChargeCount"));
		
		if (TempleMesh && TempleMaterialCharged)
		{
			TempleMesh->SetMaterial(0, TempleMaterialCharged);
			TempleMesh->SetMaterial(3, TempleMaterialCharged);
		}
	}
}
