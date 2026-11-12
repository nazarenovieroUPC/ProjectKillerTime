// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Temple.h"

#include "ProjectKillerTimeGameState.h"
#include "Characters/SurvivorCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/ProgressBars/ProgressBarWidget.h"


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
	
	VisibilitySphere = CreateDefaultSubobject<USphereComponent>("VisibilitySphere");
	VisibilitySphere->SetupAttachment(RootComponent);
	VisibilitySphere->SetSphereRadius(1000.f);
	
	VisibilitySphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	VisibilitySphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	VisibilitySphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	WidgetProgressBar = CreateDefaultSubobject<UWidgetComponent>("WidgetProgressBar");
	WidgetProgressBar->SetupAttachment(RootComponent);
	WidgetProgressBar->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetProgressBar->SetDrawSize(FVector2D(150.f, 20.f));
	WidgetProgressBar->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
	
	WidgetProgressBar->SetVisibility(false);
}

// Called when the game starts or when spawned
void ATemple::BeginPlay()
{
	Super::BeginPlay();
	
	OnTempleCharge.AddDynamic(this, &ATemple::UpdateProgressBar);
	
	VisibilitySphere->OnComponentBeginOverlap.AddDynamic(this, &ATemple::OnVisibilitySphereBeginOverlap);
	VisibilitySphere->OnComponentEndOverlap.AddDynamic(this, &ATemple::OnVisibilitySphereEndOverlap);
	
	if (AProjectKillerTimeGameState* GS = Cast<AProjectKillerTimeGameState>(GetWorld()->GetGameState())) GS->SetTotalTemples();
}

//Replicacion
void ATemple::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATemple, bIsActivated);
	DOREPLIFETIME(ATemple, bIsCharging);
	DOREPLIFETIME(ATemple, TempleCharge);
}

void ATemple::OnVisibilitySphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsActivated || !WidgetProgressBar) return;

	if (APawn* PlayerPawn = Cast<APawn>(OtherActor))
	{
		if (PlayerPawn->IsLocallyControlled())
		{
			WidgetProgressBar->SetVisibility(true);
			
			UpdateProgressBar(TempleCharge);
		}
	}
}

void ATemple::OnVisibilitySphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!WidgetProgressBar) return;

	if (APawn* PlayerPawn = Cast<APawn>(OtherActor))
	{
		if (PlayerPawn->IsLocallyControlled()) WidgetProgressBar->SetVisibility(false);
	}
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
	GetWorldTimerManager().SetTimer(TempleTimerHandle, this, &ATemple::OnTempleTimer, 0.05f, true);
}

void ATemple::CompletedInteract_Implementation(AActor* Interactor)
{
	IInteractInterface::CompletedInteract_Implementation(Interactor);
	
	if (bIsActivated || !HasAuthority()) return;
	
	bIsCharging = false;
	
	GetWorldTimerManager().ClearTimer(TempleTimerHandle);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Completed Interact"));
}

void ATemple::UpdateProgressBar(float ProgressPercent)
{
	if (!WidgetProgressBar || !WidgetProgressBar->IsVisible()) return;
    
	if (UProgressBarWidget* MyWidget = Cast<UProgressBarWidget>(WidgetProgressBar->GetUserWidgetObject()))
	{
		MyWidget->SetProgress(ProgressPercent);
	}
}


void ATemple::ToggleUIProgress(bool bShow)
{
	if (!WidgetProgressBar) return;
	
	if (APawn* LocalPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		float DistanceToBar = FVector::Distance(GetActorLocation(), LocalPlayer->GetActorLocation());
		
		WidgetProgressBar->SetVisibility(DistanceToBar <= DistanceToViewBar);
	}
}

//Timer
void ATemple::OnTempleTimer()
{
	ChargeCount += 0.05f;
	
	TempleCharge = ChargeCount / ChargeTime;
	OnTempleCharge.Broadcast(TempleCharge);
	
	if (HasAuthority() && ChargeCount >= ChargeTime)
	{
		bIsActivated = true;
		
		OnRep_IsActivated();
		OnRep_IsCharging();
		
		if (AProjectKillerTimeGameState* GS = Cast<AProjectKillerTimeGameState>(GetWorld()->GetGameState())) GS->SetActivatedTemples();
	}
}

//Replication
void ATemple::OnRep_IsCharging()
{
}

void ATemple::OnRep_IsActivated()
{
	if (bIsActivated)
	{
		GetWorldTimerManager().ClearTimer(TempleTimerHandle);
		ChargeCount = ChargeTime;
       
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("MaxChargeCount"));
		
		if (WidgetProgressBar)
		{
			WidgetProgressBar->SetVisibility(false);
		}
       
		if (TempleMesh && TempleMaterialCharged)
		{
			TempleMesh->SetMaterial(0, TempleMaterialCharged);
			TempleMesh->SetMaterial(3, TempleMaterialCharged);
		}
	}
}

void ATemple::OnRep_TempleCharge()
{
	OnTempleCharge.Broadcast(TempleCharge);
}
