// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem/Actors/QuestGiver.h"

#include "Components/BoxComponent.h"
#include "QuestSystem/Components/QuestComponent.h"


// Sets default values
AQuestGiver::AQuestGiver()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AQuestGiver::BeginPlay()
{
	Super::BeginPlay();
}

void AQuestGiver::InteractQuest_Implementation(AActor* Interactor)
{
	if (!QuestDefinition) return;
	
	if (UQuestComponent* QuestComp = Interactor->FindComponentByClass<UQuestComponent>())
	{
		QuestComp->AddQuest(QuestDefinition);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("QuestComponent not found"));
	}
}

// Called every frame
void AQuestGiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

