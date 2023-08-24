// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COURSETEST_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UAttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ReceiveDamage(float Amount);
	float CalculatePercentageOfHealth();
	float CalculatePercentageOfStamina();
	int32 AddGold(int32 Amount);
	int32 AddSouls(int32 Amount);
	bool IsDead();
	
	FORCEINLINE float GetHealth() { return Health; };
protected:
	
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere)
	float Health;

	UPROPERTY(EditAnywhere)
	float MaxHealth;

	UPROPERTY(EditAnywhere)
	float Stamina;

	UPROPERTY(EditAnywhere)
	float MaxStamina;

	UPROPERTY()
	int32 Gold = 0;

	UPROPERTY()
	int32 Souls = 0;
};
