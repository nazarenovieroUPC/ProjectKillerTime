// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestSystem/Interfaces/InteractableQuestInterface.h"
#include "QuestGiver.generated.h"

class UBoxComponent;
class UPDAQuestDefinition;

UCLASS()
class PROJECTKILLERTIME_API AQuestGiver : public AActor, public IInteractableQuestInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQuestGiver();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TObjectPtr<UPDAQuestDefinition> QuestDefinition;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void InteractQuest_Implementation(AActor* Interactor) override;
};
