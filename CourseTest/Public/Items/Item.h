// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class COURSETEST_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class USoundBase* PickUpSound;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* NiagaraSpawn;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* root;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickUpEffect;

	void SpawnPickupEffect();
	void PlayPickupSound();

	UPROPERTY(BlueprintReadOnly)
	bool IfIsPickedUp;

	UFUNCTION()
	virtual void OverlapBegin(UPrimitiveComponent* ComponentOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetCollisionOnSphereComponent(ECollisionEnabled::Type CollisionType);

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformedCos();

	void SetPickUpState(bool State);

	void SetNiagaraActivate(bool State);

private:

	UPROPERTY(VisibleInstanceOnly)
	float impulsValue = 0.f;

	UPROPERTY(EditAnywhere)
	float amplitude = 3.5;

	UPROPERTY(EditAnywhere)
	float timeOfHalfAmplitude = 5.f;

	UPROPERTY(EditAnywhere)
	float RotationValue = 10.f;
};
