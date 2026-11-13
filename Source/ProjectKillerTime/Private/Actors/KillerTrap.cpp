// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/KillerTrap.h"

#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"


// Sets default values
AKillerTrap::AKillerTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	TrapCollision = CreateDefaultSubobject<UBoxComponent>("TrapCollision");
	SetRootComponent(TrapCollision);
	
	TrapCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	TrapCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	TrapMesh = CreateDefaultSubobject<UStaticMeshComponent>("TrapMesh");
	TrapMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AKillerTrap::BeginPlay()
{
	Super::BeginPlay();
	
	TrapCollision->OnComponentBeginOverlap.AddDynamic(this, &AKillerTrap::OnTrapTriggered);
}

void AKillerTrap::OnTrapTriggered(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || !OtherActor) return;
	
	if (UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>())
	{
		HealthComponent->TakeDamage();
		
		Destroy();
	}
}

// Called every frame
void AKillerTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

