#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::Heal(float Amount)
{
	Health += Amount;

	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}

void UAttributeComponent::AddHealingPotion(int32 Amount)
{
	HealingPotions += Amount;
}

void UAttributeComponent::UseHealingPotion()
{
	Heal(HealingAmount);

	HealingPotions--;

	if (HealingPotions < 0)
	{
		HealingPotions = 0;
	}
}

void UAttributeComponent::ReceiveDamage(float Amount)
{
	Health = FMath::Clamp(Health - Amount, 0.f, MaxHealth);

	UE_LOG(LogTemp, Display, TEXT("Health: %f"), Health);
}

void UAttributeComponent::DecreasStamina(float Amount)
{
	Stamina = FMath::Clamp(Stamina - Amount, 0.f, MaxStamina);
}

float UAttributeComponent::CalculatePercentageOfHealth()
{
	return Health / MaxHealth;
}

float UAttributeComponent::CalculatePercentageOfStamina()
{
	return Stamina / MaxStamina;
}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = Stamina + StaminaRegenRate * DeltaTime;

	if (Stamina > MaxStamina)
	{
		Stamina = MaxStamina;
	}
}

void UAttributeComponent::AddGold(int32 Amount)
{
	Gold += Amount;
}

void UAttributeComponent::AddSouls(int32 Amount)
{
	Souls += Amount;
}

bool UAttributeComponent::IsDead() 
{
	return Health == 0.f;
}

bool UAttributeComponent::IfHaveEnoughStamina(float Const)
{
	return Const <= Stamina;
}

bool UAttributeComponent::IfHaveHealingPotions()
{
	return HealingPotions != 0;
}
