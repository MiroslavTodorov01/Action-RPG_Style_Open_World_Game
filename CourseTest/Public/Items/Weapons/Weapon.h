// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class COURSETEST_API AWeapon : public AItem
{
	GENERATED_BODY()	

public:

	AWeapon();

protected:

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* StartPointOfTrace;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* EndPointOfTrace;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> IgnorActors;

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere)
	float SizeOfBoxTrace;

	UPROPERTY(EditAnywhere)
	bool ActivateBoxTrace;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OverlapBeginBlade(UPrimitiveComponent* ComponentOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	void ExecuteGetHit(FHitResult& Hit);
	bool BoxTrace(FHitResult& Hit);
	bool IfActorIsTheSame(AActor* OtherActor);

public:
	
	FORCEINLINE UBoxComponent* GetWeaponBoxCollision() { return CollisionBox; };
	
	void RemoveIgnoredActorsAfterAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFieldDestruction(const FVector& ImpackPoint);

	void Equip(USceneComponent* Component, FName SocketName, AActor* NewOwner, APawn* NewInstigator);

	void SetWeaponOwner(AActor* NewOwner, APawn* NewInstigator);
};
