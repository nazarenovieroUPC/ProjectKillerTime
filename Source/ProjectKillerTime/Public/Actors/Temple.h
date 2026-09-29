// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "Temple.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTKILLERTIME_API ATemple : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATemple();
	
	UPROPERTY(EditAnywhere, Category = "Temple Stats")
	float ChargeTime = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Temple Stats")
	float MaxChargeCount = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Temple Stats")
	float ChargeCount = 0.f;
	
	UPROPERTY(ReplicatedUsing = OnRep_IsActivated)
	bool bIsActivated = false;
	
	UPROPERTY(ReplicatedUsing = OnRep_IsCharging)
	bool bIsCharging = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> TempleMesh;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> TempleMaterialCharged;
	
	FTimerHandle TempleTimerHandle;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//Interact Interface
	virtual void StartedInteract_Implementation(AActor* Interactor) override;
	
	virtual void CompletedInteract_Implementation(AActor* Interactor) override;
	
	//Overlaps
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	//Timer
	UFUNCTION()
	void OnTempleTimer();
	
	//Replicacion
	UFUNCTION()
	void OnRep_IsCharging();
	
	UFUNCTION()
	void OnRep_IsActivated();
};
