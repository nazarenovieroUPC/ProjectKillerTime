// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillerTrap.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTKILLERTIME_API AKillerTrap : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKillerTrap();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* TrapCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* TrapMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTrapTriggered(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
