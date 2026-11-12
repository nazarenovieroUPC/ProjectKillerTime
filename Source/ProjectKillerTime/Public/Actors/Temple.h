// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "Temple.generated.h"

class USphereComponent;
class UWidgetComponent;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTempleCharge, float, ActualPercent);
UCLASS()
class PROJECTKILLERTIME_API ATemple : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATemple();
	
	//Componentes
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Mesh")
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Mesh")
	TObjectPtr<UStaticMeshComponent> TempleMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Mesh")
	TObjectPtr<UMaterialInterface> TempleMaterialCharged;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | UI")
	TObjectPtr<UWidgetComponent> WidgetProgressBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes | Distance")
	TObjectPtr<USphereComponent> VisibilitySphere;
	
	//Delegates
	UPROPERTY(BlueprintAssignable)
	FOnTempleCharge OnTempleCharge;
	
	//Temple Variables
	UPROPERTY(ReplicatedUsing = OnRep_TempleCharge)
	float TempleCharge;
	
	UPROPERTY(EditAnywhere, Category = "Temple Stats")
	float ChargeTime = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Temple Stats")
	float ChargeCount = 0.f;
	
	UPROPERTY(ReplicatedUsing = OnRep_IsActivated)
	bool bIsActivated = false;
	
	UPROPERTY(ReplicatedUsing = OnRep_IsCharging)
	bool bIsCharging = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float DistanceToViewBar = 1000.f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	//Distance Sphere
	UFUNCTION()
	void OnVisibilitySphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnVisibilitySphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	//Timer
	FTimerHandle TempleTimerHandle;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//Interact Interface
	virtual void StartedInteract_Implementation(AActor* Interactor) override;
	
	virtual void CompletedInteract_Implementation(AActor* Interactor) override;
	
	//UI
	UFUNCTION()
	void UpdateProgressBar(float ProgressPercent);
	
	UFUNCTION()
	void ToggleUIProgress(bool bShow);
	
	//Timer
	UFUNCTION()
	void OnTempleTimer();
	
	//Replicacion
	UFUNCTION()
	void OnRep_IsCharging();
	
	UFUNCTION()
	void OnRep_IsActivated();
	
	UFUNCTION()
	void OnRep_TempleCharge();
};
