// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestSystem/Interfaces/InteractableQuestInterface.h"
#include "QuestItem.generated.h"

class UBoxComponent;
enum class EQuestObjectiveType : uint8;

UCLASS()
class PROJECTKILLERTIME_API AQuestItem : public AActor, public IInteractableQuestInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQuestItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Item") 
	FName TargetID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Item") 
	EQuestObjectiveType ItemType;

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
