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

	void Heal(float Amount);
	void AddHealingPotion(int32 Amount);
	void UseHealingPotion();
	void ReceiveDamage(float Amount);
	void DecreasStamina(float Amount);
	float CalculatePercentageOfHealth();
	float CalculatePercentageOfStamina();
	void RegenStamina(float DeltaTime);
	void AddGold(int32 Amount);
	void AddSouls(int32 Amount);
	bool IsDead();
	bool IfHaveEnoughStamina(float Const);
	bool IfHaveHealingPotions();

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

	UPROPERTY(EditAnywhere)
	float StaminaRegenRate;

	UPROPERTY(EditAnywhere)
	float HealingAmount;

	UPROPERTY()
	int32 Gold = 0;

	UPROPERTY()
	int32 Souls = 0;

	UPROPERTY()
	int32 HealingPotions = 0;

	UPROPERTY(EditAnywhere)
	float DodgeStaminaCost = 15.f;

public:
	FORCEINLINE float GetHealth() const { return Health; };
	FORCEINLINE int32 GetHealingPotions() const { return HealingPotions; };
	FORCEINLINE float GetStamina() const { return Stamina; };
	FORCEINLINE float GetStaminaRegenRate() const { return StaminaRegenRate; };
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; };
	FORCEINLINE int32 GetGold() const { return Gold; };
	FORCEINLINE int32 GetSouls() const { return Souls; };
	FORCEINLINE float GetDodgeStaminaConst() const { return DodgeStaminaCost; };
};
