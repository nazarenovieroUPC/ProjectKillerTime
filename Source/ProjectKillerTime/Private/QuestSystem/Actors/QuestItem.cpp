// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem/Actors/QuestItem.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "QuestSystem/Framework/QuestGameState.h"


// Sets default values
AQuestItem::AQuestItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AQuestItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQuestItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuestItem::InteractQuest_Implementation(AActor* Interactor)
{
	IInteractableQuestInterface::InteractQuest_Implementation(Interactor);
	
	if (AQuestGameState* QuestGS = Cast<AQuestGameState>(UGameplayStatics::GetGameState(this)))
	{
		QuestGS->ReportGameplayObjective(ItemType, TargetID, 1);
	}
	
	Destroy();
}

