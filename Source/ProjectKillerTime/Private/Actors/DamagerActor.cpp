// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/DamagerActor.h"

#include "Characters/SurvivorCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"


// Sets default values
ADamagerActor::ADamagerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ZonaDmg = CreateDefaultSubobject<UBoxComponent>(TEXT("ZonaDmg"));
	RootComponent = ZonaDmg;
	ZonaDmg->SetBoxExtent(FVector(100.f, 100.f, 50.f));
	
	ZonaDmg->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ZonaDmg->SetCollisionResponseToAllChannels(ECR_Ignore);
	ZonaDmg->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void ADamagerActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (ZonaDmg)
	{
		ZonaDmg->OnComponentBeginOverlap.AddDynamic(this, &ADamagerActor::OnOverlapBegin);
	}
}

// Called every frame
void ADamagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADamagerActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// REGLA MULTIJUGADOR: Las trampas y el daño solo los calcula el Servidor.
	// Si un Cliente local detecta el overlap, lo ignoramos para evitar desincronización o hacks.
	if (!HasAuthority()) return;

	if (OtherActor && OtherActor != this)
	{
		
		if (UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>())
		{
			HealthComponent->TakeDamage();
			
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, TEXT("Trampa: ¡Daño aplicado al Survivor!"));
			
			Destroy();
		}
	}
}

